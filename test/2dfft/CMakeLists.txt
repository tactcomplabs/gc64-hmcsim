set(TEST_SOURCES
    src/2dfft.c
)
gc64_hmcsim_add_test(2dfft "${TEST_SOURCES}")
unset(TEST_SOURCES)
add_test(NAME 2dfft COMMAND 2dfft)
