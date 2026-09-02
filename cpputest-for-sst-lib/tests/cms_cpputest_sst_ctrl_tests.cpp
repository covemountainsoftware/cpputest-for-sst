#include "sst.hpp"
#include <chrono>
#include <string>
#include "cms_cpputest_sst_ctrl.hpp"
#include "cms_dummy_active_object.hpp"

//cpputest header include must always be last
#include "cms_cpputest.hpp"
#include "CppUTest/TestHarness.h"

using namespace cms::test;

TEST_GROUP(sst_ctrl_tests)
{
    std::array<SST::Evt const*, 10> events;

    void setup() final
    {
        events.fill(nullptr);
        sst_ctrl::Setup();
    }

    void teardown() final
    {
        sst_ctrl::Teardown();
    }
};

TEST(sst_ctrl_tests,
     provides_for_ability_to_move_time_forward_firing_active_object_timers_as_expected)
{
    enum Signals { SIG_1 = 200, SIG_2 };

    using namespace std::chrono_literals;

    std::size_t sigOneCount = 0;
    std::size_t sigTwoCount = 0;

    // a 'dummy' active object is needed to verify
    // that QF timers are actually firing.
    auto dummy = std::unique_ptr<DefaultDummyActiveObject>(
        new DefaultDummyActiveObject());

    dummy->SetPostedEventHandler([&](SST::Evt const* e)
    {
        if (e->sig == SIG_1)
        {
            sigOneCount++;
        }
        else if (e->sig == SIG_2)
        {
            sigTwoCount++;
        }
        else
        {
            CMS_TEST_EXIT;
        }
    });

    dummy->start(sst_ctrl::UNIT_UNDER_TEST_PRIORITY,
                 events.data(), events.size(),
                 nullptr);

    SST::TimeEvt singleShotTimer(SIG_1, dummy.get());
    singleShotTimer.arm(1000, 0);

    SST::TimeEvt repeatingTimer(SIG_2, dummy.get());
    repeatingTimer.arm(2000, 2000);

    // Now move time forward 6 seconds.
    // Expect one SIG_1 and three hits on SIG_2
    cms::test::sst_ctrl::MoveTimeForward(6s);

    CHECK_EQUAL(1, sigOneCount);
    CHECK_EQUAL(3, sigTwoCount);
}

TEST(sst_ctrl_tests, provides_lib_version)
{
    auto version = sst_ctrl::GetVersion();
    CHECK_TRUE(version != nullptr);

    std::string version_str(version);
    CHECK_FALSE(version_str.empty());
}