set(TEST_SOURCES
    src/simple4GB.cpp
)
gc64_hmcsim_add_test(simple4GBstall "${TEST_SOURCES}")
unset(TEST_SOURCES)
add_test(NAME simple4GBstall COMMAND simple4GBstall)
