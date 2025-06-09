set(TEST_SOURCES
    src/simple8GB.cpp
)
gc64_hmcsim_add_test(simpleconfig8GB "${TEST_SOURCES}")
unset(TEST_SOURCES)
