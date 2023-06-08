#include <benchmark/benchmark.h>

#include <random>

template <class G>
static void std_gen(benchmark::State& state) {
	G gen;
	for (auto _:state) {
		auto v = gen();
		benchmark::DoNotOptimize(v);
		benchmark::ClobberMemory();
	}
	state.SetItemsProcessed(state.iterations());
}

BENCHMARK(std_gen<std::mt19937>);
BENCHMARK(std_gen<std::mt19937_64>);
BENCHMARK(std_gen<std::minstd_rand>);
BENCHMARK(std_gen<std::minstd_rand0>);
BENCHMARK(std_gen<std::ranlux24_base>);
BENCHMARK(std_gen<std::ranlux48_base>);
BENCHMARK(std_gen<std::ranlux24>);
BENCHMARK(std_gen<std::ranlux48>);
BENCHMARK(std_gen<std::knuth_b>);
