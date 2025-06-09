set(TEST_SOURCES src/dyn_cmc.c src/execute_test.c src/shiftamt.c)
gc64_hmcsim_add_test(dyn_cmc "${TEST_SOURCES}")
unset(TEST_SOURCES)
add_dependencies(dyn_cmc dynamic_cmctemplate)
get_target_property(dynamic_cmctemplate_BINARY_DIR dynamic_cmctemplate
                    BINARY_DIR)
add_test(NAME dyn_cmc_smalltest_4GB
         COMMAND dyn_cmc -L ${dynamic_cmctemplate_BINARY_DIR}/.. -b 16 -c 4 -d
                 20 -l 4 -m 64 -n 1 -q 64 -v 32 -x 128)
add_test(NAME dyn_cmc_smalltest_8GB
         COMMAND dyn_cmc -L ${dynamic_cmctemplate_BINARY_DIR}/.. -b 16 -c 8 -d
                 20 -l 8 -m 64 -n 1 -q 64 -v 64 -x 128)
unset(dynamic_cmctemplate_BINARY_DIR)
