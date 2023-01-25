
#include <benchmark/benchmark.h>

#include <numeric>

static void sorting(benchmark::State& state)
{
    const auto count = (size_t)state.range(0);

    std::vector<int32_t> buffer(count);
    std::iota(std::begin(buffer), std::end(buffer), 1);

    for (auto _ : state)
    {
        state.PauseTiming();
        std::reverse(std::begin(buffer), std::end(buffer));
        state.ResumeTiming();

        std::sort(std::begin(buffer), std::end(buffer));
        benchmark::DoNotOptimize(buffer.size());
    }
}
BENCHMARK(sorting)->Args({ 10 })->Args({ 100 });

BENCHMARK_MAIN();
