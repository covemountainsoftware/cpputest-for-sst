#include "dbc_assert.h"
#include "cms_assert_mock_support.hpp"

//cpputest header include must always be last
#include "CppUTest/TestHarness.h"

DBC_MODULE_NAME("cms-dbc-assert-tests")

TEST_GROUP(sst_dbc_assert_tests)
{
    void setup() final
    {
    }

    void teardown() final
    {
        mock().clear();
    }
};

TEST(sst_dbc_assert_tests, DBC_Assert_results_in_expected_mock_hit_and_proper_test_exit)
{
    cms::test::MockExpectDbcAssert();
    DBC_ASSERT(__LINE__, true == false);
    mock().checkExpectations();
}

TEST(sst_dbc_assert_tests,
     DBC_Assert_with_id_results_in_expected_mock_hit_and_proper_test_exit)
{
    constexpr int TEST_ID = 1234;

    cms::test::MockExpectDbcAssert(DBC_module_name_, TEST_ID);
    DBC_ASSERT(TEST_ID, true == false);
    mock().checkExpectations();
}
