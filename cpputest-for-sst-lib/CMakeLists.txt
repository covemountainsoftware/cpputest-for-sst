include_directories(include)
include_directories(${CMS_SST_SRC_DIR})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_definitions(-DCPPUTEST_FOR_SST_LIB_VERSION=\"${cpputest-for-sst-lib_VERSION}\")
add_library(cpputest-for-sst-lib
        src/sst_support.cpp
        src/cms_cpputest_sst_ctrl.cpp
        src/cpputestMain.cpp
        src/cms_dbc_assert_support.cpp
)
add_subdirectory(tests)

target_compile_options(cpputest-for-sst-lib PRIVATE -Wall -Wextra -Werror -Wpedantic)

target_include_directories(cpputest-for-sst-lib PUBLIC
        ${CMS_SST_INCLUDE_DIR}
        include
)
