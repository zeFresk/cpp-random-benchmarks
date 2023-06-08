#include <benchmark/benchmark.h>
#include <random>

static void c_rand(benchmark::State& state) {
	std::srand(0);
	for (auto _:state) {
		auto v = std::rand();
		benchmark::DoNotOptimize(v);
	}
	state.SetItemsProcessed(state.iterations());
}

BENCHMARK(c_rand);
