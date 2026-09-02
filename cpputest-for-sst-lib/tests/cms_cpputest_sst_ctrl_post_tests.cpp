#include "cms_cpputest_sst_ctrl.hpp"
#include "sst.hpp"
#include "cms_dummy_active_object.hpp"

//cpputest header include must always be last
#include "CppUTest/TestHarness.h"

using namespace cms::test;

TEST_GROUP(sst_ctrl_post_tests)
{
    DefaultDummyActiveObjectUniquePtr dummy = nullptr;
    std::array<SST::Evt const*, 10> events;

    void setup() final
    {
        sst_ctrl::Setup();
        dummy = std::make_unique<DefaultDummyActiveObject>();
        dummy->start(sst_ctrl::UNIT_UNDER_TEST_PRIORITY,
                     events.data(), events.size(),
                     nullptr);
    }

    void teardown() final
    {
        sst_ctrl::Teardown();
    }
};

TEST(sst_ctrl_post_tests,
     provides_a_post_and_process_helper_func_with_trivial_signal_enum)
{
    enum TestEnum : SST::Signal { TEST1_SIG = 100 };
    SST::Signal capturedSig = 0xFFFF;
    dummy->SetPostedEventHandler([&](const SST::Evt* e)
    {
        capturedSig = e->sig;
    });
    sst_ctrl::PostAndProcess<TEST1_SIG>(dummy.get());
    CHECK_EQUAL(TEST1_SIG, capturedSig);
}

TEST(sst_ctrl_post_tests,
     provides_a_post_and_process_helper_func)
{
    static constexpr SST::Signal TEST2_SIG = 111;
    static constexpr SST::Evt testEvent{TEST2_SIG};
    SST::Signal capturedSig = -1;
    dummy->SetPostedEventHandler([&](const SST::Evt* e)
    {
        capturedSig = e->sig;
    });
    sst_ctrl::PostAndProcess(&testEvent, dummy.get());
    CHECK_EQUAL(TEST2_SIG, capturedSig);
}
