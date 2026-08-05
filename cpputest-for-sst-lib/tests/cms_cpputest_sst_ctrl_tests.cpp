#include "sst.hpp"
#include <chrono>
#include <string>
#include "cms_cpputest_sst_ctrl.hpp"

//cpputest header include must always be last
#include "CppUTest/TestHarness.h"

using namespace cms::test;

TEST_GROUP(sst_ctrl_tests)
{
    void setup() final
    {
        sst_ctrl::Setup();
    }

    void teardown() final
    {
        cms::test::sst_ctrl::Teardown();
    }
};

TEST(sst_ctrl_tests,
     provides_for_ability_to_move_time_forward_firing_active_object_timers_as_expected)
{
#if 0 //TODO
    using namespace std::chrono_literals;

    enum Signals { SIG_1 = Q_USER_SIG, SIG_2 };
    qf_ctrl::Setup(10, 1000);

    int sigOneCount = 0;
    int sigTwoCount = 0;

    // a 'dummy' active object is needed to verify
    // that QF timers are actually firing.
    auto dummy = std::unique_ptr<DefaultDummyActiveObject>(
      new DefaultDummyActiveObject());
    dummy->dummyStart();
    dummy->SetPostedEventHandler([&](QEvt const* e) {
        if (e->sig == SIG_1) {
            sigOneCount++;
        }
        else if (e->sig == SIG_2) {
            sigTwoCount++;
        }
        else {
            CMS_TEST_EXIT;
        }
    });

    QTimeEvt singleShotTimer;
    QTimeEvt_ctorX(&singleShotTimer, dummy->getQActive(), SIG_1, 0U);
    QTimeEvt_armX(&singleShotTimer, 1000, 0);

    QTimeEvt repeatingTimer;
    QTimeEvt_ctorX(&repeatingTimer, dummy->getQActive(), SIG_2, 0U);
    QTimeEvt_armX(&repeatingTimer, 2000, 2000);

    // Now move time forward 6 seconds.
    // Expect one SIG_1 and three hits on SIG_2
    qf_ctrl::MoveTimeForward(6s);

    CHECK_EQUAL(1, sigOneCount);
    CHECK_EQUAL(3, sigTwoCount);
#endif
}

TEST(sst_ctrl_tests, provides_lib_version)
{
    auto version = sst_ctrl::GetVersion();
    CHECK_TRUE(version != nullptr);

    std::string version_str(version);
    CHECK_FALSE(version_str.empty());
}