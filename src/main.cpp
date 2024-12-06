#include <array>
#include <cmath>
#include <format>
#include <iostream>

#include "monte_carlo.h"
#include "timer.h"

template<size_t N>
static void estimate(MonteCarlo& mc, std::array<int, N> sampleCounts)
{
	for (auto samples : sampleCounts)
	{
		ScopeTimer timer;
		auto [mean, variance] = mc.estimate(samples);
		std::cout << std::format("{:>10} Samples \tEstimate: {:<10} \tVariance: {:<10}\t", samples, mean, variance);
	}
}

auto main() -> int
{
	std::cout << "Monte Carlo Sampling\n";
	std::cout << "------------------------\n";

	auto f = [](float x) -> float
		{
			return 5.0f * std::pow(x, 4);
		};

	auto sampleCounts = std::array{ 100, 10'000, 1'000'000 };

	std::cout << "\nConstant PDF\n";
	{
		auto constant_pdf = [](float x) -> float
			{
				return 1.0f;
			};

		MonteCarlo mc(f, constant_pdf);
		estimate(mc, sampleCounts);
	}

	std::cout << "\nLinear PDF\n";
	{
		auto linear_pdf = [](float x) -> float
			{
				return 2.0f * x;
			};

		MonteCarlo mc(f, linear_pdf);
		estimate(mc, sampleCounts);
	}

	std::cout << "\nQuadratic PDF\n";
	{
		auto quadratic_pdf = [](float x) -> float
			{
				return 3.0f * x * x;
			};

		MonteCarlo mc(f, quadratic_pdf);
		estimate(mc, sampleCounts);
	}

	std::cout << "\nBad PDF\n";
	{
		auto bad_pdf = [](float x) -> float
			{
				return -2.0f * x + 2.0f;
			};

		MonteCarlo mc(f, bad_pdf);
		estimate(mc, sampleCounts);
	}

	std::cout << "\nPerfect PDF\n";
	{
		auto perfect_pdf = f;
		MonteCarlo mc(f, perfect_pdf);
		estimate(mc, sampleCounts);
	}

	std::cin.get();
	return 0;
}
