#pragma once

#include <chrono>
#include <iostream>

class Timer
{
public:
	Timer() : m_start{ std::chrono::high_resolution_clock::now() } {}
	~Timer() = default;

	void reStart() { m_start = std::chrono::high_resolution_clock::now(); }

	template <typename Unit>
	auto elapsed() const -> Unit
	{
		const auto end = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<Unit>(end - m_start);
	}

	auto elapsedNanos() const -> float { return elapsed<std::chrono::nanoseconds>().count(); }
	auto elapsedMicros() const -> float { return elapsed<std::chrono::microseconds>().count(); }
	auto elapsedMillis() const -> float { return elapsed<std::chrono::milliseconds>().count(); }
	auto elapsedSeconds() const -> float { return elapsed<std::chrono::seconds>().count(); }

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
};

template <typename Unit = std::chrono::microseconds>
class ScopeTimer
{
public:
	ScopeTimer(std::string_view name = "Duration", std::ostream& os = std::cout)
		: m_name{ name }, m_os{ os }
	{
	}
	~ScopeTimer() { m_os << std::format("{}: {:>8}\n", m_name, m_timer.elapsed<Unit>()); }

private:
	Timer m_timer;
	std::string m_name;
	std::ostream& m_os;
};