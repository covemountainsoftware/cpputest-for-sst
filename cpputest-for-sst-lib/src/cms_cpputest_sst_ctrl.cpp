#include "cms_cpputest_sst_ctrl.hpp"
#include "sst.hpp"
#include "dbc_assert.h"
#include <array>

DBC_MODULE_NAME("cms-sst-ctrl")

static SST::ReadySet task_readySet;
static std::array<SST::Task*, SST_PORT_MAX_TASK + 1U> task_registry;
static uint32_t l_ticksPerSecond = 0;
static SST::TimeEvt* timeEvt_head = nullptr;

//most of the code in the SST namespace is a copy of SST0, with the exception
//of the 'run()' method and the init() method
namespace SST
{
    // SST kernel facilities -----------------------------------------------------
    void init()
    {
        task_readySet = 0;
        task_registry.fill(nullptr);
        timeEvt_head = nullptr;
    }

    //............................................................................
    // ReSharper disable once CppDFAConstantFunctionResult
    int Task::run()
    {
        //this method should not be called in a unit testing environment
        DBC_REQUIRE(__LINE__, true == false);
        return 0;
    }

    // SST Task facilities -------------------------------------------------------
    void Task::start(
        TaskPrio prio,
        Evt const** qBuf, QCtr qLen,
        Evt const* const ie)
    {
        //! @pre
        // - the priority must be in range
        // - the queue storage must be provided
        // - the queue length must not be zero
        // - the priority must not be in use
        //
        DBC_REQUIRE(__LINE__,
                    (0U < prio) && (prio <= SST_PORT_MAX_TASK)
                    && (qBuf != nullptr) && (qLen > 0U)
                    && (task_registry[prio] == nullptr));

        m_prio = prio;
        m_qBuf = qBuf;
        m_end = qLen - 1U;
        m_head = 0U;
        m_tail = 0U;
        m_nUsed = 0U;

        task_registry[prio] = this;

        // initialize this task with the initialization event
        init(ie); // virtual call
        // TBD: implement event recycling
    }

    //............................................................................
    void Task::post(Evt const* const e) noexcept
    {
        //! @pre the queue must be sized adequately and cannot overflow
        DBC_REQUIRE(__LINE__, m_nUsed <= m_end);

        SST_PORT_CRIT_STAT
        SST_PORT_CRIT_ENTRY();
        m_qBuf[m_head] = e; // insert event into the queue
        // need to wrap the head?
        if (m_head == 0U)
        {
            m_head = m_end; // wrap around
        }
        else
        {
            --m_head;
        }
        ++m_nUsed;
        task_readySet |= (1U << (m_prio - 1U));
        SST_PORT_CRIT_EXIT();
    }

    //............................................................................
    TimeEvt::TimeEvt(Signal sig, Task* task) : Evt()
    {
        this->sig = sig;
        m_task = task;
        m_ctr = 0U;
        m_interval = 0U;

        // insert this time event into the linked-list
        m_next = timeEvt_head;
        timeEvt_head = this;
    }

    //............................................................................
    void TimeEvt::arm(TCtr ctr, TCtr interval)
    {
        SST_PORT_CRIT_STAT
        SST_PORT_CRIT_ENTRY();
        m_ctr = ctr;
        m_interval = interval;
        SST_PORT_CRIT_EXIT();
    }

    //............................................................................
    bool TimeEvt::disarm()
    {
        SST_PORT_CRIT_STAT
        SST_PORT_CRIT_ENTRY();
        bool status = (m_ctr != 0U);
        m_ctr = 0U;
        m_interval = 0U;
        SST_PORT_CRIT_EXIT();
        return status;
    }

    //............................................................................
    void TimeEvt::tick()
    {
        for (TimeEvt* t = timeEvt_head; t != nullptr; t = t->m_next)
        {
            SST_PORT_CRIT_STAT
            SST_PORT_CRIT_ENTRY();
            if (t->m_ctr == 0U)
            {
                // disarmed? (most frequent case)
                SST_PORT_CRIT_EXIT();
            }
            else if (t->m_ctr == 1U)
            {
                // expiring?
                t->m_ctr = t->m_interval;
                SST_PORT_CRIT_EXIT();

                t->m_task->post(t);
            }
            else
            {
                // timing out
                --t->m_ctr;
                SST_PORT_CRIT_EXIT();
            }
        }
    }

    // copy of, and modified version of,
    // the event loop of the SST0 kernel, i.e. Task::run(),
    // for unit testing purposes.
    void Task::runUntilNoReadyActiveObjects()
    {
        for (;;)
        {
            if (task_readySet != 0U)
            {
                // any SST tasks ready to run?
                std::uint_fast8_t const p = SST_LOG2(task_readySet);
                Task* const task = task_registry[p];
                SST_PORT_INT_ENABLE();

                // the task must have some events in the queue
                DBC_ASSERT(100, task->m_nUsed > 0U);

                // get the event out of the queue
                // NOTE: no critical section because task->m_tail is accessed
                // only from this task
                //
                Evt const* e = task->m_qBuf[task->m_tail];
                if (task->m_tail == 0U)
                {
                    /* need to wrap the tail? */
                    task->m_tail = task->m_end; /* wrap around */
                }
                else
                {
                    --task->m_tail;
                }
                SST_PORT_INT_DISABLE();
                if ((--task->m_nUsed) == 0U)
                {
                    /* no more events in the queue? */
                    task_readySet &= ~(1U << (p - 1U));
                }
                SST_PORT_INT_ENABLE();

                // dispatch the received event to this task
                task->dispatch(e); // virtual call
                // TBD: implement event recycling
            }
            else
            {
                // no SST tasks are ready to run --> idle
                return;
            }
        }
    }
}

void cms::test::sst_ctrl::Setup(const uint32_t ticksPerSecond)
{
    DBC_ASSERT(__LINE__, l_ticksPerSecond == 0);

    l_ticksPerSecond = ticksPerSecond;
    task_readySet = 0;
    task_registry.fill(nullptr);
    timeEvt_head = nullptr;

    SST::init();
}

void cms::test::sst_ctrl::Teardown()
{
    task_readySet = 0;
    task_registry.fill(nullptr);
    l_ticksPerSecond = 0;
    timeEvt_head = nullptr;
}

void cms::test::sst_ctrl::ProcessEvents()
{
    SST::Task::runUntilNoReadyActiveObjects();
}

void cms::test::sst_ctrl::MoveTimeForward(const std::chrono::milliseconds& duration)
{
    DBC_ASSERT(__LINE__, l_ticksPerSecond != 0);
    DBC_ASSERT(__LINE__, duration.count() >= 0);

    const double millisecondsPerTick = 1000.0 / l_ticksPerSecond;

    using LoopCounter_t = uint64_t;

    constexpr LoopCounter_t ONCE = 1;
    // if called, ensure at least one tick is processed
    LoopCounter_t ticks = std::max(
        ONCE, static_cast<LoopCounter_t>(duration.count() / millisecondsPerTick));

    for (LoopCounter_t i = 0; i < ticks; ++i)
    {
        SST::TimeEvt::tick();
        ProcessEvents();
    }
}

const char* cms::test::sst_ctrl::GetVersion()
{
    return CPPUTEST_FOR_SST_LIB_VERSION;
}
