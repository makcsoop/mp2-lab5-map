CC = g++
CFLAGS = -Wall -Wextra -Werror -std=c++23 

BENCH_FLAGS = -Wall -Wextra -std=c++23 -O3 -march=native

src = src
samples = samples
test = test
GTEST_DIR = googletest/googletest

BENCHMARK_DIR = benchmark
BENCHMARK_BUILD = $(BENCHMARK_DIR)/build
BENCHMARK_LIB = $(BENCHMARK_BUILD)/src/libbenchmark.a

$(BENCHMARK_LIB):
	@echo "📦 Building Google Benchmark..."
	@mkdir -p $(BENCHMARK_BUILD)
	@cd $(BENCHMARK_BUILD) && \
		cmake .. -DCMAKE_BUILD_TYPE=Release -DBENCHMARK_ENABLE_TESTING=OFF && \
		make benchmark


benchmark_quick: $(BENCHMARK_LIB)
	$(CC) $(BENCH_FLAGS) \
		$(src)/map.cpp \
		$(src)/HashTable.cpp \
		$(samples)/benchmark_all.cpp \
		-Iinclude \
		-I$(BENCHMARK_DIR)/include \
		$(BENCHMARK_LIB) \
		-pthread \
		-o benchmark_all
	./benchmark_all --benchmark_filter="/(8|64|512)$$" --benchmark_min_time=0.05s

benchmark_insert: $(BENCHMARK_LIB)
	$(CC) $(BENCH_FLAGS) \
		$(src)/map.cpp \
		$(src)/HashTable.cpp \
		$(samples)/benchmark_all.cpp \
		-Iinclude \
		-I$(BENCHMARK_DIR)/include \
		$(BENCHMARK_LIB) \
		-pthread \
		-o benchmark_all
	./benchmark_all --benchmark_filter="Insert" --benchmark_min_time=0.1s

benchmark_stl: $(BENCHMARK_LIB)
	$(CC) $(BENCH_FLAGS) \
		$(src)/map.cpp \
		$(src)/HashTable.cpp \
		$(samples)/benchmark_all.cpp \
		-Iinclude \
		-I$(BENCHMARK_DIR)/include \
		$(BENCHMARK_LIB) \
		-pthread \
		-o benchmark_all
	./benchmark_all --benchmark_filter="Std" --benchmark_min_time=0.1s

benchmark_save: $(BENCHMARK_LIB)
	$(CC) $(BENCH_FLAGS) \
		$(src)/map.cpp \
		$(src)/HashTable.cpp \
		$(samples)/benchmark_all.cpp \
		-Iinclude \
		-I$(BENCHMARK_DIR)/include \
		$(BENCHMARK_LIB) \
		-pthread \
		-o benchmark_all
	./benchmark_all --benchmark_min_time=0.1s --benchmark_format=json --benchmark_out=results.json

clean_benchmark:
	rm -rf $(BENCHMARK_BUILD)
	rm -f benchmark_all results.json

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

test_hash:
	clang-format -i $(src)/*.cpp include/* $(samples)/*.cpp
	$(CC) $(CFLAGS) \
		$(src)/map.cpp \
		$(src)/HashTable.cpp \
		$(test)/test_hashTable.cpp \
		$(GTEST_DIR)/src/gtest-all.cc \
		$(GTEST_DIR)/src/gtest_main.cc \
		-Iinclude \
		-I$(GTEST_DIR) \
		-I$(GTEST_DIR)/include \
		-pthread


map:
	clang-format -i $(src)/*.cpp include/* $(samples)/*.cpp $(test)/*
	$(CC) $(CFLAGS) $(src)/map.cpp $(src)/tree.cpp $(samples)/main_map.cpp -Iinclude -o map

tree: clean
	clang-format -i $(src)/*.cpp include/* $(samples)/*.cpp $(test)/*
	$(CC) $(CFLAGS) $(src)/map.cpp $(src)/tree.cpp $(samples)/main_tree.cpp -Iinclude -o tree

hash: clean
	clang-format -i $(src)/*.cpp include/* $(samples)/*.cpp $(test)/*
	$(CC) $(CFLAGS) $(src)/map.cpp $(samples)/main_hashTable.cpp -Iinclude -o hash

dheap: clean
	clang-format -i $(src)/*.cpp include/* $(samples)/*.cpp $(test)/*
	$(CC) $(CFLAGS) $(samples)/main_D-heap.cpp -Iinclude -o heap

arif: clean
	clang-format -i $(src)/*.cpp include/* $(samples)/*.cpp $(test)/*
	g++ test/test_arithmetic.cpp  -I./include src/*.cpp -I./gtest gtest/gtest-all.cc -std=c++17 test/test_main.cpp

map_rebuild:
	rm -f map test_map a.out
	clang-format -i $(src)/*.cpp include/* $(samples)/*.cpp $(test)/*
	$(CC) $(CFLAGS) $(src)/map.cpp $(src)/tree.cpp $(samples)/main_map.cpp -Iinclude -o map

clean:
	rm -f map test_map a.out tree hash heap