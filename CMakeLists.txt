cmake_minimum_required(VERSION 3.23)
project(gc64-hmcsim LANGUAGES C CXX)

set(HMC_MAJOR_VERSION 3 CACHE STRING "")
set(HMC_MINOR_VERSION 0 CACHE STRING "")
set(HMC_PRODUCT_REVISION 2 CACHE STRING "")
set(HMC_PROTOCOL_REVISION 2 CACHE STRING "")
set(HMC_PHY_REVISION 2 CACHE STRING "")
set(HMC_PHY_SPEED 0 CACHE STRING "physical link speed")
option(HMC_ALLOC_MEM "perform memory allocation" OFF)

add_compile_options(-Wall)

add_library(hmcsim)
target_compile_definitions(hmcsim PRIVATE
    HMC_MAJOR_VERSION=${HMC_MAJOR_VERSION}
    HMC_MINOR_VERSION=${HMC_MINOR_VERSION}
    HMC_PRODUCT_REVISION=${HMC_PRODUCT_REVISION}
    HMC_PROTOCOL_REVISION=${HMC_PROTOCOL_REVISION}
    HMC_PHY_REVISION=${HMC_PHY_REVISION}
    HMC_PHY_SPEED=${HMC_PHY_SPEED}
)
if(HMC_ALLOC_MEM)
    target_compile_definitions(hmcsim PRIVATE HMC_ALLOC_MEM)
endif()
target_compile_definitions(hmcsim PRIVATE $<$<CONFIG:Debug>:HMC_DEBUG>)

add_subdirectory(include)
add_subdirectory(cmc)
add_subdirectory(src)
add_subdirectory(tools)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
    if(BUILD_TESTING)
        add_subdirectory(test)
    endif()
endif()
