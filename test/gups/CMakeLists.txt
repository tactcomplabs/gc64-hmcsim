set(TEST_SOURCES
    src/execute_test.c
    src/gups.c
    src/shiftamt.c
)
gc64_hmcsim_add_test(gups "${TEST_SOURCES}")
unset(TEST_SOURCES)
