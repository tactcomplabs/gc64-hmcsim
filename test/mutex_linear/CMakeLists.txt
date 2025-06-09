set(TEST_SOURCES
    src/execute_test.c
    src/mutex_linear.c
    src/shiftamt.c
)
gc64_hmcsim_add_test(mutex_linear "${TEST_SOURCES}")
unset(TEST_SOURCES)
