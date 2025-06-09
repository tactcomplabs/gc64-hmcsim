set(TEST_SOURCES
    src/execute_test.c
    src/readmat.c
    src/shiftamt.c
    src/spmv.c
)
gc64_hmcsim_add_test(spmv "${TEST_SOURCES}")
unset(TEST_SOURCES)
add_test(NAME spmv COMMAND spmv)
