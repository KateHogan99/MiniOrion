#include "Output.h"
#include "Simulation.h"
#include "TestCases.h"
#include "Validation.h"

#include <iostream>
#include <stdexcept>

int main()
{
    SimulationConfig config = create_circular_orbit_test();

    try
    {
        std::vector<SimulationStep> results =
            run_simulation(config);

        print_results_table(results);

        print_summary(results, config);

        validate_simulation(results, config);
    }
    catch (const std::runtime_error& error)
    {
        std::cerr << "Simulation error: "
                  << error.what()
                  << "\n";

        return 1;
    }

    return 0;
}
