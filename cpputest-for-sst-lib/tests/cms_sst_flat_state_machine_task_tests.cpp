#include "cms_cpputest_sst_ctrl.hpp"
#include "sst.hpp"
#include "cms_sst_flat_state_machine_task.hpp"
#include <array>

//cpputest header include must always be last
#include "CppUTestExt/MockSupport.h"
#include "CppUTest/TestHarness.h"

using namespace cms::test;

namespace
{
    /**
     * A trivial test class to indirectly test our SstFlatStateMachineTask,
     * using CppUTest mock calls to trace the behavior.
     */
    class TestDerived : public cms::SstFlatStateMachineTask
    {
    public:
        enum Signals
        {
            DO_PING = cms::SM_BEGIN_USER_SIGNALS,
            DO_PONG
        };

        TestDerived() = default;
        ~TestDerived() override = default;

    protected:
        StateRtn InitialPseudoState(const SST::Evt*) override
        {
            mock("TestDerived").actualCall("InitialPseudoState");
            return TransitionTo(&TestDerived::StateOfPinging);
        }

    private:
        StateRtn StateOfPinging(const SST::Evt* e)
        {
            switch (e->sig)
            {
            case cms::SM_ENTER_SIG:
                mock("TestDerived").actualCall("PingingEnter");
                return Handled();
            case cms::SM_EXIT_SIG:
                mock("TestDerived").actualCall("PingingExit");
                return Handled();
            case DO_PING:
                return Handled();
            case DO_PONG:
                return TransitionTo(&TestDerived::StateOfPonging);
            default:
                mock("TestDerived").actualCall("PingingDefault");
                return Handled();
            }
        }

        StateRtn StateOfPonging(const SST::Evt* e)
        {
            switch (e->sig)
            {
            case cms::SM_ENTER_SIG:
                mock("TestDerived").actualCall("PongingEnter");
                return Handled();
            case cms::SM_EXIT_SIG:
                mock("TestDerived").actualCall("PongingExit");
                return Handled();
            case DO_PONG:
                return Handled();
            case DO_PING:
                return TransitionTo(&TestDerived::StateOfPinging);
            default:
                mock("TestDerived").actualCall("PongingDefault");
                return Handled();
            }
        }
    };
}

TEST_GROUP(cms_sst_flat_state_machine_task_tests)
{
    std::unique_ptr<TestDerived> underTest = nullptr;
    std::array<SST::Evt const*, 10> events;

    void setup() final
    {
        sst_ctrl::Setup();

        underTest = std::make_unique<TestDerived>();
    }

    void teardown() final
    {
        mock().clear();
        sst_ctrl::Teardown();
    }

    void startUnitUnderTestTaskUntilPinging()
    {
        CHECK_TRUE(underTest != nullptr);

        mock("TestDerived").expectOneCall("InitialPseudoState");
        mock("TestDerived").expectOneCall("PingingEnter");

        underTest->start(sst_ctrl::UNIT_UNDER_TEST_PRIORITY,
                         events.data(), events.size(),
                         nullptr);
        sst_ctrl::ProcessEvents();
        mock().checkExpectations();
    }

    void startUntilPonging()
    {
        startUnitUnderTestTaskUntilPinging();
        static constexpr SST::Evt DoPong{TestDerived::DO_PONG};

        mock("TestDerived").expectOneCall("PingingExit");
        mock("TestDerived").expectOneCall("PongingEnter");
        sst_ctrl::PostAndProcess(&DoPong, underTest.get());
        sst_ctrl::ProcessEvents();
        mock().checkExpectations();
    }
};

TEST(cms_sst_flat_state_machine_task_tests,
     can_create_without_crash)
{
    CHECK_TRUE(underTest != nullptr);
}

TEST(cms_sst_flat_state_machine_task_tests,
     can_start_with_initial_expected_transition)
{
    startUnitUnderTestTaskUntilPinging();
}

TEST(cms_sst_flat_state_machine_task_tests,
     can_transition_from_pinging_to_ponging)
{
    startUntilPonging();
}

TEST(cms_sst_flat_state_machine_task_tests,
     can_transition_from_ponging_to_pinging)
{
    startUntilPonging();
    static constexpr SST::Evt DoPing{TestDerived::DO_PING};

    mock("TestDerived").expectOneCall("PongingExit");
    mock("TestDerived").expectOneCall("PingingEnter");
    sst_ctrl::PostAndProcess(&DoPing, underTest.get());
    mock().checkExpectations();
}

TEST(cms_sst_flat_state_machine_task_tests,
     ping_when_already_pinging_is_silent)
{
    startUnitUnderTestTaskUntilPinging();
    static constexpr SST::Evt DoPing{TestDerived::DO_PING};
    sst_ctrl::PostAndProcess(&DoPing, underTest.get());
    mock().checkExpectations();
}

TEST(cms_sst_flat_state_machine_task_tests,
     pong_when_already_ponging_is_silent)
{
    startUntilPonging();
    static constexpr SST::Evt DoPong{TestDerived::DO_PONG};
    sst_ctrl::PostAndProcess(&DoPong, underTest.get());
    mock().checkExpectations();
}
