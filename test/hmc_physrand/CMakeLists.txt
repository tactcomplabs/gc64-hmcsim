set(TEST_SOURCES
    src/execute_test.c
    src/genrand.c
    src/physrand_main.c
    src/shiftamt.c
)
gc64_hmcsim_add_test(hmc_physrand "${TEST_SOURCES}")
unset(TEST_SOURCES)
