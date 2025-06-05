set(TEST_SOURCES
    src/sst_init.c
)
gc64_hmcsim_add_test(sst_init "${TEST_SOURCES}")
unset(TEST_SOURCES)
add_test(NAME sst_init COMMAND sst_init)
