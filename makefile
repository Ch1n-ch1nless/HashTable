CC=g++
CFLAGS=-Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal\
       -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs \
       -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions\
	   -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security\
	   -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor\
	   -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing\
	   -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla\
	   -D_DEBUG -mavx2 -mfma -mavx -msse4.2


TEXT_SRC_DIR = ./TextLib/source/
TEXT_OBJ_DIR = ./TextLib/object/

HASH_SRC_DIR = ./HashTable/source/
HASH_OBJ_DIR = ./HashTable/object/

LIST_SRC_DIR = ./List/source/
LIST_OBJ_DIR = ./List/object/

TEXT_SRC = $(wildcard $(TEXT_SRC_DIR)*.cpp)
TEXT_OBJ = $(patsubst $(TEXT_SRC_DIR)%.cpp, $(TEXT_OBJ_DIR)%.o, $(TEXT_SRC))

HASH_SRC = $(wildcard $(HASH_SRC_DIR)*.cpp)
HASH_OBJ = $(patsubst $(HASH_SRC_DIR)%.cpp, $(HASH_OBJ_DIR)%.o, $(HASH_SRC))

LIST_SRC = $(wildcard $(LIST_SRC_DIR)*.cpp)
LIST_OBJ = $(patsubst $(LIST_SRC_DIR)%.cpp, $(LIST_OBJ_DIR)%.o, $(LIST_SRC))

all: prepare_file

prepare_file: $(TEXT_OBJ) $(LIST_OBJ) $(HASH_OBJ) main.o
	$(CC) $(TEXT_OBJ) $(LIST_OBJ) $(HASH_OBJ) main.o -o hash_table.out

$(TEXT_OBJ_DIR)%.o : $(TEXT_SRC_DIR)%.cpp
	$(CC) $(CFLAGS) $(OPT_LEVEL) -c $< -o $@

$(LIST_OBJ_DIR)%.o : $(LIST_SRC_DIR)%.cpp
	$(CC) $(CFLAGS) $(OPT_LEVEL) -c $< -o $@

$(HASH_OBJ_DIR)%.o : $(HASH_SRC_DIR)%.cpp
	$(CC) $(CFLAGS) $(OPT_LEVEL) -c $< -o $@

main.o : main.cpp
	$(CC) $(CFLAGS) $(OPT_LEVEL) -c $< -o $@

clean:
	rm $(TEXT_OBJ) $(LIST_OBJ) $(HASH_OBJ) main.o