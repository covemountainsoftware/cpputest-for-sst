
set(TEST_APP_NAME cpputest-for-qpc-lib-tests)
set(TEST_SOURCES
        cms_cpputest_sst_ctrl_post_tests.cpp
)

# this include expects TEST_SOURCES and TEST_APP_NAME to be
# defined, and creates the cpputest based test executable target
include(${CMS_CMAKE_DIR}/cpputestCMake.cmake)

target_link_libraries(${TEST_APP_NAME} cpputest-for-sst-lib ${CPPUTEST_LDFLAGS})