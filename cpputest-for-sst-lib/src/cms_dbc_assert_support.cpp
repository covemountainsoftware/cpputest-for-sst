#include "dbc_assert.h"
#include <cstdio>
#include <cassert>

//cpputest headers must always be last
#include "cms_cpputest.hpp"
#include "cms_assert_mock_support.hpp"

static bool m_printAssertMeta = false;

void cms::test::AssertOutputEnable()
{
    m_printAssertMeta = true;
}

void cms::test::AssertOutputDisable()
{
    m_printAssertMeta = false;
}

void DBC_fault_handler(char const* module, const int id)
{
    if (m_printAssertMeta)
    {
        fprintf(stderr, "%s : %s %d\n", __FUNCTION__, module, id);
    }

    // The TEST_EXIT macro used below is throwing an exception.
    // However, if any methods are marked as 'noexcept'
    // this will not be useful if trying to test
    // any assert that happens in the context of a 'noexcept'.
    //
    // Per https://en.cppreference.com/w/cpp/language/noexcept_spec:
    //   "Non-throwing functions are permitted to call potentially-throwing
    //    functions. Whenever an exception is thrown and the search for a
    //    handler encounters the outermost block of a non-throwing function,
    //    the function std::terminate ... is called ..."
    //
    mock(cms::test::DBC_MOCK_NAME)
        .actualCall(cms::test::DBC_HANDLER_FUNC_NAME)
        .withParameter("module", module)
        .withParameter("id", id);

    CMS_TEST_EXIT;
}

