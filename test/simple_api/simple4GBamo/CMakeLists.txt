set(TEST_SOURCES
    src/simple4GB.cpp
)
gc64_hmcsim_add_test(simple4GBamo "${TEST_SOURCES}")
unset(TEST_SOURCES)
add_test(NAME simple4GB COMMAND simple4GB)
