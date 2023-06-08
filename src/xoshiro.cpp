#include <benchmark/benchmark.h>

#include <xoshiro.h>

using namespace Xoshiro;

template <typename G>
static void xoshiro(benchmark::State& state) {
	G gen;
	for (auto _:state) {
		auto v = gen();
		benchmark::DoNotOptimize(v);
		benchmark::ClobberMemory();
	}
	state.SetItemsProcessed(state.iterations());
}

BENCHMARK(xoshiro<Xoshiro128PP>);
BENCHMARK(xoshiro<Xoshiro256PP>);

