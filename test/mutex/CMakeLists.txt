set(TEST_SOURCES
    src/execute_test.c
    src/mutex.c
    src/shiftamt.c
)
gc64_hmcsim_add_test(mutex "${TEST_SOURCES}")
unset(TEST_SOURCES)
