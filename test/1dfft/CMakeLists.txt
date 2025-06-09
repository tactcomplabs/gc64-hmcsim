set(TEST_SOURCES
    src/1dfft.c
)
gc64_hmcsim_add_test(1dfft "${TEST_SOURCES}")
unset(TEST_SOURCES)
add_test(NAME 1dfft COMMAND 1dfft)
