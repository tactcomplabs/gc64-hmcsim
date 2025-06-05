set(TEST_SOURCES
    src/simple.c
)
gc64_hmcsim_add_test(simple "${TEST_SOURCES}")
unset(TEST_SOURCES)
add_test(NAME simple COMMAND simple)
