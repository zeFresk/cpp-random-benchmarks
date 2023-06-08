#include <benchmark/benchmark.h>

#include <random>

#include "burtle.h"

static auto burtleburtle_ranval(benchmark::State& state) {
	ranctx ctx;
	raninit(&ctx, std::random_device{}());
	for (auto _:state) {
		auto v = ranval(&ctx);
		benchmark::DoNotOptimize(v);
		benchmark::ClobberMemory();
	}
	state.SetItemsProcessed(state.iterations());
}

BENCHMARK(burtleburtle_ranval);
