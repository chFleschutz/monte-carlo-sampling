#pragma once

#include <chrono>
#include <iostream>

template<typename Unit = std::chrono::microseconds>
class ScopeTimer
{
public:
	ScopeTimer()
		: m_start{ std::chrono::high_resolution_clock::now() }
	{
	}

	~ScopeTimer()
	{
		const auto end = std::chrono::high_resolution_clock::now();
		const auto duration = std::chrono::duration_cast<Unit>(end - m_start).count();
		std::cout << std::format("Duration: {:>10}{}\n", duration, unitName());
	}

private:
	constexpr auto unitName() const -> const char*
	{
		if constexpr (std::is_same_v<Unit, std::chrono::nanoseconds>)
			return "ns";
		if constexpr (std::is_same_v<Unit, std::chrono::microseconds>)
			return "us";
		if constexpr (std::is_same_v<Unit, std::chrono::milliseconds>)
			return "ms";
		if constexpr (std::is_same_v<Unit, std::chrono::seconds>)
			return "s";
		if constexpr (std::is_same_v<Unit, std::chrono::minutes>)
			return "m";
		if constexpr (std::is_same_v<Unit, std::chrono::hours>)
			return "h";
		return "units";
	}

	std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
};