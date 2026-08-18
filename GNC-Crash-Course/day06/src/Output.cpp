#include "Output.h"

#include <iomanip>
#include <iostream>

std::string integrator_name(Integrator integrator)
{
    switch (integrator)
    {
        case Integrator::Euler:
            return "Euler";

        case Integrator::RK4:
            return "RK4";

        default:
            return "Unknown";
    }
}

void print_results_table(const std::vector<SimulationStep>& results)
{
    std::cout << "\n";

    std::cout
        << std::left
        << std::setw(8)  << "Time"
        << std::setw(12) << "X"
        << std::setw(12) << "Y"
        << std::setw(12) << "VX"
        << std::setw(12) << "VY"
        << std::setw(12) << "Radius"
        << std::setw(12) << "Speed"
        << std::setw(14) << "Energy"
        << std::setw(14) << "Ang. Mom."
        << std::setw(14) << "Step Dist."
        << std::setw(14) << "Total Dist."
        << "\n";

    std::cout
        << std::string(138, '-')
        << "\n";

    for (const SimulationStep& result : results)
    {
        std::cout
            << std::left
            << std::setw(8)  << result.time
            << std::setw(12) << result.state.position.x
            << std::setw(12) << result.state.position.y
            << std::setw(12) << result.state.velocity.x
            << std::setw(12) << result.state.velocity.y
            << std::setw(12) << result.radius
            << std::setw(12) << result.speed
            << std::setw(14) << result.orbital_energy
            << std::setw(14) << result.angular_momentum
            << std::setw(14) << result.distance_this_step
            << std::setw(14) << result.total_distance
            << "\n";
    }
}

void print_summary(const std::vector<SimulationStep>& results, SimulationConfig config)
{
    const SimulationStep& initial = results.front();
    const SimulationStep& final = results.back();

    double final_displacement =
        calculate_displacement(
            initial.state.position,
            final.state.position
        );

    std::cout << "\n";
    std::cout
        << "============================================================\n"
        << "SIMULATION SUMMARY\n"
        << "============================================================\n";

    std::cout
        << "Integrator: "
        << integrator_name(config.integrator)
        << "\n";

    std::cout
        << "Time step: "
        << config.time_step
        << "\n";

    std::cout
        << "Simulation duration: "
        << final.time
        << "\n";

    std::cout
        << "\nInitial position: ("
        << initial.state.position.x
        << ", "
        << initial.state.position.y
        << ")\n";

    std::cout
        << "Final position:   ("
        << final.state.position.x
        << ", "
        << final.state.position.y
        << ")\n";

    std::cout
        << "\nInitial velocity: ("
        << initial.state.velocity.x
        << ", "
        << initial.state.velocity.y
        << ")\n";

    std::cout
        << "Final velocity:   ("
        << final.state.velocity.x
        << ", "
        << final.state.velocity.y
        << ")\n";

    std::cout
        << "\nInitial radius: "
        << initial.radius
        << "\n";

    std::cout
        << "Final radius:   "
        << final.radius
        << "\n";

    std::cout
        << "Final displacement: "
        << final_displacement
        << "\n";

    std::cout
        << "Total distance traveled: "
        << final.total_distance
        << "\n";

    if (config.mu != 0.0)
    {
        double energy_change =
            final.orbital_energy -
            initial.orbital_energy;

        double angular_momentum_change =
            final.angular_momentum -
            initial.angular_momentum;

        std::cout
            << "\nInitial specific orbital energy: "
            << initial.orbital_energy
            << "\n";

        std::cout
            << "Final specific orbital energy:   "
            << final.orbital_energy
            << "\n";

        std::cout
            << "Change in specific orbital energy: "
            << energy_change
            << "\n";

        std::cout
            << "\nInitial specific angular momentum: "
            << initial.angular_momentum
            << "\n";

        std::cout
            << "Final specific angular momentum:   "
            << final.angular_momentum
            << "\n";

        std::cout
            << "Change in specific angular momentum: "
            << angular_momentum_change
            << "\n";
    }
}