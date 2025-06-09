macro(gc64_hmcsim_add_cmc name)
    add_library(${name})
    target_sources(${name} PRIVATE cmc.c)
    target_include_directories(${name}
        PRIVATE "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
        PUBLIC "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
    install(TARGETS ${name} DESTINATION cmc)
endmacro()

add_subdirectory(amo_popcount)
add_subdirectory(dynamic_template)
add_subdirectory(fullempty)
add_subdirectory(mutex)
add_subdirectory(template)
