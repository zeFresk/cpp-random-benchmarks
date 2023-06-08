#include <benchmark/benchmark.h>

#include <include/pcg_random.hpp>

template <typename G>
static void pcg_cpp(benchmark::State& state) {
	G gen;
	for (auto _:state) {
		auto v = gen();
		benchmark::DoNotOptimize(v);
		benchmark::ClobberMemory();
	}
	state.SetItemsProcessed(state.iterations());
}

BENCHMARK(pcg_cpp<pcg32>);
BENCHMARK(pcg_cpp<pcg32_fast>);
BENCHMARK(pcg_cpp<pcg32_oneseq>);
BENCHMARK(pcg_cpp<pcg64>);
BENCHMARK(pcg_cpp<pcg64_fast>);
BENCHMARK(pcg_cpp<pcg64_oneseq>);
