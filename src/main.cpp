#include <cmath>
#include <format>
#include <functional>
#include <iostream>
#include <random>

class MonteCarlo
{
public:
	MonteCarlo(std::function<float(float)> f, std::function<float(float)> pdf)
		: m_f(f), m_pdf(pdf)
	{
	}

	auto estimate(int samples, float min = 0.0f, float max = 1.0f) -> std::pair<float, float>
	{
		std::uniform_real_distribution<float> dis(min, max);

		float sum = 0.0f;
		float sumSquared = 0.0f;
		for (int i = 0; i < samples; i++)
		{
			float x = dis(gen);
			float y = m_f(x) / m_pdf(x);
			sum += y;
			sumSquared += y * y;
		}

		float mean = sum / samples;
		float variance = (sumSquared / samples) - (mean * mean);
		return { mean, variance };
	}

private:
	std::function<float(float)> m_f;	// Function to integrate
	std::function<float(float)> m_pdf;	// Probability density function

	std::random_device rd;
	std::mt19937 gen{ rd() };
};

static void estimate(MonteCarlo& mc, int samples)
{
	auto [mean, variance] = mc.estimate(samples);
	std::cout << std::format("{:>10} Samples - Estimate: {:<10} Variance: {:<10}\n", samples, mean, variance);
}

auto main() -> int
{
	auto f = [](float x) -> float
		{
			return 5.0f * std::pow(x, 4);
		};

	auto pdf = [](float x) -> float
		{
			return 1.0f;
		};

	MonteCarlo mc(f, pdf);
	estimate(mc, 100);
	estimate(mc, 10'000);
	estimate(mc, 1'000'000);

	return 0;
}
