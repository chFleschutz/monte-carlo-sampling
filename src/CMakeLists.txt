
add_executable(monte-carlo-sampling)

target_sources(monte-carlo-sampling
    PRIVATE 
        "main.cpp"
        "timer.h"
        "monte_carlo.h"
)
