#include <benchmark/benchmark.h>
#include <random>
#include "tmatrix.h"
#include "tmatrix_for_cmp.h"  // Неоптимизированная матрица

template <typename MatrixType>
MatrixType CreateRandomMatrix(size_t rows, size_t cols) {
    MatrixType m(rows, cols);
    std::mt19937 gen(423226337); // Фиксированный сид
    std::uniform_real_distribution<double> dis(-20.0, 20.0);

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            m[i][j] = dis(gen);
        }
    }
    return m;
}

static void BM_UnoptimizedMatrixMultiplication(benchmark::State& state) {
    size_t m = state.range(0);
    size_t k = state.range(1);
    size_t n = state.range(2);

    auto m1 = CreateRandomMatrix<TDynamicMatrixForCMP<double>>(m, k);
    auto m2 = CreateRandomMatrix<TDynamicMatrixForCMP<double>>(k, n);

    for (auto _ : state) {
        auto res = m1 * m2;
        benchmark::DoNotOptimize(res);
    }
}
BENCHMARK(BM_UnoptimizedMatrixMultiplication)->Args({ 256, 256, 256 })->Args({ 256, 384, 512 })->Args({ 512, 512, 512 })
        ->Args({ 1024, 512, 768 })->Args({ 1024, 1024, 1024 })->Args({ 2048, 1024, 2048 })
        ->Unit(benchmark::kMillisecond)->MinWarmUpTime(1.0)->Repetitions(7)->DisplayAggregatesOnly(true);

static void BM_OptimizedMatrixMultiplication(benchmark::State& state) {
    size_t m = state.range(0);
    size_t k = state.range(1);
    size_t n = state.range(2);

    auto m1 = CreateRandomMatrix<TDynamicMatrix<double>>(m, k);
    auto m2 = CreateRandomMatrix<TDynamicMatrix<double>>(k, n);

    for (auto _ : state) {
        auto res = m1 * m2;
        benchmark::DoNotOptimize(res);
    }
}
BENCHMARK(BM_OptimizedMatrixMultiplication)->Args({ 256, 256, 256 })->Args({ 256, 384, 512 })->Args({ 512, 512, 512 })
        ->Args({ 1024, 512, 768 })->Args({ 1024, 1024, 1024 })->Args({ 2048, 1024, 2048 })
        ->Unit(benchmark::kMillisecond)->MinWarmUpTime(1.0)->Repetitions(7)->DisplayAggregatesOnly(true);

BENCHMARK_MAIN();
