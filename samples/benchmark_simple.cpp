#include <benchmark/benchmark.h>

#include <chrono>
#include <thread>

// Простой бенчмарк: пустая функция (проверяет overhead)
static void BM_EmptyFunction(benchmark::State& state) {
    for (auto _ : state) {
        // Ничего не делаем
    }
}
BENCHMARK(BM_EmptyFunction);

// Бенчмарк: маленькая задержка (проверяет точность измерений)
static void BM_SmallDelay(benchmark::State& state) {
    for (auto _ : state) {
        // Симулируем маленькую работу
        volatile int x = 0;
        for (int i = 0; i < 100; ++i) {
            x += i;
        }
        benchmark::DoNotOptimize(x);
    }
}
BENCHMARK(BM_SmallDelay);

// Бенчмарк: с параметром (размер работы)
static void BM_WorkWithSize(benchmark::State& state) {
    int size = state.range(0);

    for (auto _ : state) {
        volatile int sum = 0;
        for (int i = 0; i < size; ++i) {
            sum += i;
        }
        benchmark::DoNotOptimize(sum);
    }
    state.SetItemsProcessed(state.iterations() * size);
}
BENCHMARK(BM_WorkWithSize)->Arg(100)->Arg(1000)->Arg(10000);

// Бенчмарк: с pause/resume (симуляция подготовки данных)
static void BM_WithPause(benchmark::State& state) {
    for (auto _ : state) {
        // Пауза для "подготовки данных" (не измеряется)
        state.PauseTiming();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        state.ResumeTiming();

        // Измеряемая работа
        volatile int x = 0;
        for (int i = 0; i < 1000; ++i) {
            x += i;
        }
        benchmark::DoNotOptimize(x);
    }
}
BENCHMARK(BM_WithPause);

// Обязательно: main функция
BENCHMARK_MAIN();