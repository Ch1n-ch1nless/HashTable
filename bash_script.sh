#!/bin/bash

make clean
make OPT_LEVEL=-O3
./hash_table.out 0 Results/const_hash.csv
./hash_table.out 1 Results/first_ascii_code_hash.csv
./hash_table.out 2 Results/strlen_hash.csv
./hash_table.out 3 Results/sum_codes_hash.csv
./hash_table.out 4 Results/ror_calc_hash.csv
./hash_table.out 5 Results/rol_calc_hash.csv
./hash_table.out 6 Results/crc32_hash.csv