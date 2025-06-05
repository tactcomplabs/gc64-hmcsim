set(TEST_SOURCES
    src/simple4GB.cpp
)
gc64_hmcsim_add_test(simpleconfig4GB "${TEST_SOURCES}")
unset(TEST_SOURCES)
