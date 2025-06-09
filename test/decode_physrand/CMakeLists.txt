set(TEST_SOURCES
    src/execute_test.c src/genrand.c src/physrand_main.c src/shiftamt.c)
gc64_hmcsim_add_test(decode_physrand "${TEST_SOURCES}")
unset(TEST_SOURCES)
add_test(NAME decode_physrand_4link_2GB_bcopy
         COMMAND decode_physrand -b 8 -c 2 -l 4 -m 64 -n 1 -q 64 -x 128 -d 20
                 -v 32 -N 33554432 -R 50 -W 50 -S 65656)
add_test(NAME decode_physrand_4link_4GB_bcopy
         COMMAND decode_physrand -b 16 -c 4 -l 4 -m 64 -n 1 -q 64 -x 128 -d 20
                 -v 32 -N 33554432 -R 50 -W 50 -S 65656)
add_test(NAME decode_physrand_4link_4GB
         COMMAND decode_physrand -b 16 -c 4 -l 4 -m 64 -n 1 -q 64 -x 128 -d 20
                 -v 32 -N 1024 -R 50 -W 50 -S 65656)
add_test(NAME decode_physrand_8link_4GB_bcopy
         COMMAND decode_physrand -b 8 -c 4 -l 8 -m 64 -n 1 -q 64 -x 128 -d 20
                 -v 32 -N 33554432 -R 50 -W 50 -S 65656)
add_test(NAME decode_physrand_8link_8GB_bcopy
         COMMAND decode_physrand -b 16 -c 8 -l 8 -m 64 -n 1 -q 64 -x 128 -d 20
                 -v 32 -N 33554432 -R 50 -W 50 -S 65656)
add_test(NAME decode_physrand_smalltest
         COMMAND decode_physrand -b 8 -c 2 -l 4 -m 64 -n 1 -q 64 -x 128 -d 20
                 -v 32 -N 10 -R 50 -W 50 -S 65656)
