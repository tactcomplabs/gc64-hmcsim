set(TEST_SOURCES src/execute_test.c src/fe_tree.c src/shiftamt.c)
gc64_hmcsim_add_test(fe_tree "${TEST_SOURCES}")
unset(TEST_SOURCES)
add_dependencies(fe_tree readxx readef readff writexe writexf incff)
get_target_property(readxx_BINARY_DIR readxx BINARY_DIR)
# This assumes that all the required libraries are within the same
# subdirectory.
set(fullempty_BINARY_DIR ${readxx_BINARY_DIR}/..)
add_test(NAME fe_tree_smalltest
    COMMAND fe_tree -L ${fullempty_BINARY_DIR}
    -b 16 -c 4 -d 20 -l 4 -m 64 -n 1 -q 64 -v 32 -x 128 -T 10)
# TODO 4link_4GB, 8link_8GB
unset(readxx_BINARY_DIR)
unset(fullempty_BINARY_DIR)
