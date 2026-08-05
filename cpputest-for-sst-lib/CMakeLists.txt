include_directories(include)
include_directories(${CMS_QPC_SRC_DIR})
include_directories(${CMS_CPPUTEST_QP_PORT_TOP_DIR}/include)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_definitions(-DCPPUTEST_FOR_QPC_LIB_VERSION=\"${cpputest-for-qpc-lib_VERSION}\")

add_library(cpputest-for-sst-lib
        src/sst_support.cpp
        src/cms_cpputest_sst_ctrl.cpp
        src/cpputestMain.cpp
)
add_subdirectory(tests)

target_compile_options(cpputest-for-sst-lib PRIVATE -Wall -Wextra -Werror -Wpedantic)

target_include_directories(cpputest-for-sst-lib PUBLIC
        ${CMS_SST_INCLUDE_DIR}
        include
)
