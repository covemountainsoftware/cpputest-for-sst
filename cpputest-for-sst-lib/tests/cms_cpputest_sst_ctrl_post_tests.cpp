#include "cms_cpputest_sst_ctrl.hpp"
#include "sst.hpp"

//cpputest header include must always be last
#include "CppUTest/TestHarness.h"

using namespace cms::test;

TEST_GROUP(sst_ctrl_post_tests)
{
    void setup() final
    {
        sst_ctrl::Setup();
    }

    void teardown() final
    {
        sst_ctrl::Teardown();
    }
};

TEST(sst_ctrl_post_tests,
     provides_a_post_and_process_helper_func_with_trivial_signal_enum)
{
   //TODO
}

TEST(sst_ctrl_post_tests,
     provides_a_post_and_process_helper_func)
{
    //TODO
}
