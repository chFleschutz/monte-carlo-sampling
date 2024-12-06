#pragma once 

#include <functional>
#include <random>

class MonteCarlo
{
public:
	/// @brief Creates a Monte Carlo integrator
	/// @param f Function to integrate
	/// @param pdf Probability density function (Integral of pdf over [0, 1] must be 1)
	MonteCarlo(std::function<float(float)> f, std::function<float(float)> pdf)
		: m_f(f), m_pdf(pdf)
	{
	}

	/// @brief Estimates the integral of the function f between [0, 1] using Monte Carlo integration
	/// @param samples Number of samples to take
	/// @return The mean and variance of the estimate
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
