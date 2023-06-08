#include <benchmark/benchmark.h>

#include <cstdint>

#include <pcg32.h>
#include <pcg32_8.h>

static auto wjakob_pcg32(benchmark::State& state) {
	pcg32 gen{};
	for (auto _:state) {
		auto v = gen.nextUInt();
		benchmark::DoNotOptimize(v);
		benchmark::ClobberMemory();
	}
	state.SetItemsProcessed(state.iterations());
}

static auto wjakob_pcg32_8(benchmark::State& state) {
	pcg32_8 gen{};
	std::uint32_t out[8];
	for (auto _:state) {
		gen.nextUInt(out);
		benchmark::DoNotOptimize(out);
		benchmark::ClobberMemory();
	}
	state.SetItemsProcessed(state.iterations() * 8);
}

BENCHMARK(wjakob_pcg32);
BENCHMARK(wjakob_pcg32_8);

