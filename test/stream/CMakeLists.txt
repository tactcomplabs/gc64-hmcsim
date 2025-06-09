set(TEST_SOURCES
    src/execute_test.c
    src/stream.c
    src/shiftamt.c
)
gc64_hmcsim_add_test(stream "${TEST_SOURCES}")
unset(TEST_SOURCES)
