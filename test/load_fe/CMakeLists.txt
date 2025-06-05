set(TEST_SOURCES
    src/load_fe.c
)
gc64_hmcsim_add_test(load_fe "${TEST_SOURCES}")
unset(TEST_SOURCES)
add_test(NAME load_fe COMMAND load_fe)
