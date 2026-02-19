CC = g++
CFLAGS = -Wall -Wextra -Werror -std=c++23 -fsanitize=address

src = src
samples = samples
test = test
GTEST_DIR = googletest/googletest

all: clean test_map map

test_map:
	clang-format -i $(src)/*.cpp include/* $(samples)/*.cpp
	$(CC) $(CFLAGS) \
		$(src)/map.cpp \
		$(src)/tree.cpp \
		$(test)/test_map.cpp \
		$(GTEST_DIR)/src/gtest-all.cc \
		$(GTEST_DIR)/src/gtest_main.cc \
		-Iinclude \
		-I$(GTEST_DIR) \
		-I$(GTEST_DIR)/include \
		-pthread
		# -o test_map

test_tree:
	clang-format -i $(src)/*.cpp include/* $(samples)/*.cpp
	$(CC) $(CFLAGS) \
		$(src)/map.cpp \
		$(src)/tree.cpp \
		$(test)/test_tree.cpp \
		$(GTEST_DIR)/src/gtest-all.cc \
		$(GTEST_DIR)/src/gtest_main.cc \
		-Iinclude \
		-I$(GTEST_DIR) \
		-I$(GTEST_DIR)/include \
		-pthread
		# -o test_map

map:
	clang-format -i $(src)/*.cpp include/* $(samples)/*.cpp $(test)/*
	$(CC) $(CFLAGS) $(src)/map.cpp $(src)/tree.cpp $(samples)/main_map.cpp -Iinclude -o map

tree: clean
	clang-format -i $(src)/*.cpp include/* $(samples)/*.cpp $(test)/*
	$(CC) $(CFLAGS) $(src)/map.cpp $(src)/tree.cpp $(samples)/main_tree.cpp -Iinclude -o tree

map_rebuild:
	rm -f map test_map a.out
	clang-format -i $(src)/*.cpp include/* $(samples)/*.cpp $(test)/*
	$(CC) $(CFLAGS) $(src)/map.cpp $(src)/tree.cpp $(samples)/main_map.cpp -Iinclude -o map

clean:
	rm -f map test_map a.out tree