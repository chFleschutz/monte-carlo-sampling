#include <cmath>
#include <format>
#include <functional>
#include <iostream>
#include <random>
#include <array>

#include "scope_timer.h"

class MonteCarlo
{
public:
	MonteCarlo(std::function<float(float)> f, std::function<float(float)> pdf)
		: m_f(f), m_pdf(pdf)
	{
	}

	auto estimate(int samples) -> std::pair<float, float>
	{
		float sum = 0.0f;
		float sumSquared = 0.0f;
		for (int i = 0; i < samples; i++)
		{
			float x = rejectionSample();
			float y = m_f(x) / m_pdf(x);
			sum += y;
			sumSquared += y * y;
		}

		float mean = sum / samples;
		float variance = (sumSquared / samples) - (mean * mean);
		return { mean, variance };
	}

private:
	auto rejectionSample() -> float
	{
		float max = m_pdf(1.0f);
		while (true)
		{
			float x = dis(gen);
			float u = dis(gen);
			if (u * max <= m_pdf(x))
				return x;
		}
	}

	std::function<float(float)> m_f;	// Function to integrate
	std::function<float(float)> m_pdf;	// Probability density function

	std::random_device rd;
	std::mt19937 gen{ rd() };
	std::uniform_real_distribution<float> dis{ 0.0f, 1.0f };
};

template<size_t N>
static void estimate(MonteCarlo& mc, std::array<int, N> sampleCounts)
{
	ScopeTimer<std::chrono::milliseconds> timer;

	for (auto samples : sampleCounts)
	{
		auto [mean, variance] = mc.estimate(samples);
		std::cout << std::format("{:>10} Samples - Estimate: {:<10} Variance: {:<10}\n", samples, mean, variance);
	}
}

auto main() -> int
{
	std::cout << "Monte Carlo Integration\n";
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

	return 0;
}
