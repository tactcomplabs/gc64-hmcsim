set(TEST_SOURCES
    src/simple8GB.cpp
)
gc64_hmcsim_add_test(simple8GBamo "${TEST_SOURCES}")
unset(TEST_SOURCES)
add_test(NAME simple8GBamo COMMAND simple8GBamo)
