cmake_minimum_required(VERSION 3.16)
project(cpputest-for-sst-lib VERSION 0.9.0)
include(FetchContent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

set(CMS_EXTERNALS_TOP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/externals)
set(CMS_CMAKE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cpputest-for-sst-lib/cmake CACHE INTERNAL "")

if(NOT DEFINED CMS_SST_TOP_DIR)
    set(CMS_SST_TOP_DIR ${CMS_EXTERNALS_TOP_DIR}/Super-Simple-Tasker)
    FetchContent_Declare(sst
            GIT_REPOSITORY https://github.com/QuantumLeaps/Super-Simple-Tasker.git
            GIT_TAG        41d0d219a2c037aa1354c9140ed899fb5b020bc3 #2.3.4
            SOURCE_DIR     ${CMS_SST_TOP_DIR}
    )
    message("Fetching Super-Simple-Tasker git repository")
    FetchContent_MakeAvailable(sst)
endif(NOT DEFINED CMS_SST_TOP_DIR)

include(${CMS_CMAKE_DIR}/sstCMakeSupport.cmake)
add_subdirectory(cpputest-for-sst-lib)
