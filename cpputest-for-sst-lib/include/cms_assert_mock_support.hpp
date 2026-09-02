/// @brief Support methods to help with unit testing for DBC Asserts.

#ifndef CMS_TEST_ASSERT_MOCK_SUPPORT_HPP
#define CMS_TEST_ASSERT_MOCK_SUPPORT_HPP

#include "CppUTestExt/MockSupport.h"

namespace cms::test
{

    static constexpr const char* DBC_MOCK_NAME = "DBC";
    static constexpr const char* DBC_HANDLER_FUNC_NAME = "DBC_fault_handler";

    void AssertOutputEnable();
    void AssertOutputDisable();

    inline void MockExpectDbcAssert()
    {
        mock(DBC_MOCK_NAME)
            .expectOneCall(DBC_HANDLER_FUNC_NAME)
            .ignoreOtherParameters();
    }

    inline void MockExpectDbcAssert(const char* module, const int id)
    {
        mock(DBC_MOCK_NAME)
            .expectOneCall(DBC_HANDLER_FUNC_NAME)
            .withParameter("module", module)
            .withParameter("id", id);
    }
}

#endif   // CMS_TEST_ASSERT_MOCK_SUPPORT_HPP
