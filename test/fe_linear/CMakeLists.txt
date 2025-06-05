set(TEST_SOURCES src/execute_test.c src/fe_linear.c src/shiftamt.c)
gc64_hmcsim_add_test(fe_linear "${TEST_SOURCES}")
unset(TEST_SOURCES)
add_dependencies(fe_linear readef writexe)
get_target_property(readef_BINARY_DIR readef BINARY_DIR)
# This assumes that all the required libraries are within the same
# subdirectory.
set(fullempty_BINARY_DIR ${readef_BINARY_DIR}/..)
add_test(NAME fe_linear_smalltest
    COMMAND fe_linear -L ${fullempty_BINARY_DIR}
    -b 16 -c 4 -d 20 -l 4 -m 64 -n 1 -q 64 -v 32 -x 128 -T 2)
# TODO 4link_4GB, 8link_8GB
unset(readef_BINARY_DIR)
unset(fullempty_BINARY_DIR)
