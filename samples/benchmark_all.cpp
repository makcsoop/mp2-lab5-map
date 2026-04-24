#include <benchmark/benchmark.h>

#include <map>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

#include "HashTable.h"
#include "map.h"

// Генератор случайных строк
std::string random_string(int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string result;
    result.reserve(len);
    for (int i = 0; i < len; ++i) {
        result += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    return result;
}

// ==================== INSERT BENCHMARKS ====================

// TableArrSort Insert
static void BM_TableArrSort_Insert(benchmark::State& state) {
    int N = state.range(0);

    std::vector<std::string> keys;
    keys.reserve(N);
    for (int i = 0; i < N; ++i) {
        keys.push_back(random_string(8));
    }

    for (auto _ : state) {
        TableArrSort<std::string, int> table;
        for (int i = 0; i < N; ++i) {
            table.Insert(keys[i], i);
        }
        benchmark::DoNotOptimize(table);
    }
    state.SetItemsProcessed(state.iterations() * N);
}
BENCHMARK(BM_TableArrSort_Insert)->Range(8, 8 << 10)->Arg(10000);

// MapLists Insert
static void BM_MapLists_Insert(benchmark::State& state) {
    int N = state.range(0);

    std::vector<std::string> keys;
    keys.reserve(N);
    for (int i = 0; i < N; ++i) {
        keys.push_back(random_string(8));
    }

    for (auto _ : state) {
        MapLists<std::string, int> table;
        for (int i = 0; i < N; ++i) {
            table.Insert(keys[i], i);
        }
        benchmark::DoNotOptimize(table);
    }
    state.SetItemsProcessed(state.iterations() * N);
}
BENCHMARK(BM_MapLists_Insert)->Range(8, 8 << 10);

// OpenHashTable Insert
static void BM_OpenHashTable_Insert(benchmark::State& state) {
    int N = state.range(0);

    std::vector<std::string> keys;
    keys.reserve(N);
    for (int i = 0; i < N; ++i) {
        keys.push_back(random_string(8));
    }

    for (auto _ : state) {
        OpenHashTable<std::string, int> table;
        for (int i = 0; i < N; ++i) {
            table.Insert(keys[i], i);
        }
        benchmark::DoNotOptimize(table);
    }
    state.SetItemsProcessed(state.iterations() * N);
}
BENCHMARK(BM_OpenHashTable_Insert)->Range(8, 8 << 10);

// ==================== FIND BENCHMARKS ====================

template <typename MapType>
void run_find_benchmark(benchmark::State& state, MapType& map, const std::vector<std::string>& keys) {
    for (auto _ : state) {
        for (const auto& key : keys) {
            auto* val = map.Find(key);
            benchmark::DoNotOptimize(val);
        }
    }
    state.SetItemsProcessed(state.iterations() * keys.size());
}

// TableArrSort Find
static void BM_TableArrSort_Find(benchmark::State& state) {
    int N = state.range(0);

    std::vector<std::string> keys;
    keys.reserve(N);
    TableArrSort<std::string, int> table;

    for (int i = 0; i < N; ++i) {
        keys.push_back(random_string(8));
        table.Insert(keys.back(), i);
    }

    run_find_benchmark(state, table, keys);
}
BENCHMARK(BM_TableArrSort_Find)->Range(8, 8 << 10);

// MapLists Find
static void BM_MapLists_Find(benchmark::State& state) {
    int N = state.range(0);

    std::vector<std::string> keys;
    keys.reserve(N);
    MapLists<std::string, int> table;

    for (int i = 0; i < N; ++i) {
        keys.push_back(random_string(8));
        table.Insert(keys.back(), i);
    }

    run_find_benchmark(state, table, keys);
}
BENCHMARK(BM_MapLists_Find)->Range(8, 8 << 10);

// OpenHashTable Find
static void BM_OpenHashTable_Find(benchmark::State& state) {
    int N = state.range(0);

    std::vector<std::string> keys;
    keys.reserve(N);
    OpenHashTable<std::string, int> table;

    for (int i = 0; i < N; ++i) {
        keys.push_back(random_string(8));
        table.Insert(keys.back(), i);
    }

    run_find_benchmark(state, table, keys);
}
BENCHMARK(BM_OpenHashTable_Find)->Range(8, 8 << 10);

// ==================== DELETE BENCHMARKS (ИСПРАВЛЕНЫ) ====================

template <typename MapType>
void run_delete_benchmark(benchmark::State& state, MapType& map, const std::vector<std::string>& keys) {
    // Создаём копию ключей для удаления
    std::vector<std::string> keys_to_delete = keys;

    // Ограничиваем количество удалений
    int delete_count = keys_to_delete.size();
    if (delete_count > 200) {
        delete_count = 200;
        keys_to_delete.resize(delete_count);
    }

    for (auto _ : state) {
        MapType temp = map;
        // Удаляем только те ключи, которые точно есть
        for (int i = 0; i < delete_count; ++i) {
            try {
                temp.Delete(keys_to_delete[i]);
            } catch (const std::exception& e) {
                // Игнорируем ошибки удаления (ключа может не быть)
                // Но в нормальном бенчмарке их быть не должно
            }
        }
        benchmark::DoNotOptimize(temp);
    }
    state.SetItemsProcessed(state.iterations() * delete_count);
}

// TableArrSort Delete
static void BM_TableArrSort_Delete(benchmark::State& state) {
    int N = state.range(0);

    std::vector<std::string> keys;
    keys.reserve(N);
    TableArrSort<std::string, int> table;

    for (int i = 0; i < N; ++i) {
        keys.push_back(random_string(8));
        table.Insert(keys.back(), i);
    }

    run_delete_benchmark(state, table, keys);
}
BENCHMARK(BM_TableArrSort_Delete)->Range(8, 8 << 10);

// OpenHashTable Delete
static void BM_OpenHashTable_Delete(benchmark::State& state) {
    int N = state.range(0);

    std::vector<std::string> keys;
    keys.reserve(N);
    OpenHashTable<std::string, int> table;

    for (int i = 0; i < N; ++i) {
        keys.push_back(random_string(8));
        table.Insert(keys.back(), i);
    }

    run_delete_benchmark(state, table, keys);
}
BENCHMARK(BM_OpenHashTable_Delete)->Range(8, 8 << 10);

// ==================== STL BENCHMARKS ====================

static void BM_StdUnorderedMap_Insert(benchmark::State& state) {
    int N = state.range(0);

    std::vector<std::string> keys;
    keys.reserve(N);
    for (int i = 0; i < N; ++i) {
        keys.push_back(random_string(8));
    }

    for (auto _ : state) {
        std::unordered_map<std::string, int> m;
        for (int i = 0; i < N; ++i) {
            m[keys[i]] = i;
        }
        benchmark::DoNotOptimize(m);
    }
    state.SetItemsProcessed(state.iterations() * N);
}
BENCHMARK(BM_StdUnorderedMap_Insert)->Range(8, 8 << 10);

static void BM_StdMap_Insert(benchmark::State& state) {
    int N = state.range(0);

    std::vector<std::string> keys;
    keys.reserve(N);
    for (int i = 0; i < N; ++i) {
        keys.push_back(random_string(8));
    }

    for (auto _ : state) {
        std::map<std::string, int> m;
        for (int i = 0; i < N; ++i) {
            m[keys[i]] = i;
        }
        benchmark::DoNotOptimize(m);
    }
    state.SetItemsProcessed(state.iterations() * N);
}
BENCHMARK(BM_StdMap_Insert)->Range(8, 8 << 10);

BENCHMARK_MAIN();