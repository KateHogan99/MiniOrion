@echo off

cl /std:c++17 /EHsc /I include/** ^
src\main.cpp ^
src\configuration\Configuration.cpp ^
src\experiments\Experiments.cpp ^
src\experiments\ConvergenceExperiment.cpp ^
src\experiments\ManeuverExperiment.cpp ^
src\experiments\OrbitExperiment.cpp ^
src\gnc\Control.cpp ^
src\gnc\Guidance.cpp ^
src\output\Output.cpp ^
src\physics\OrbitalMechanics.cpp ^
src\physics\Vector2D.cpp ^
src\simulation\Dynamics.cpp ^
src\simulation\Integrator.cpp ^
src\simulation\Simulation.cpp ^
src\simulation\TestCases.cpp ^
src\validation\GNCValidation.cpp ^
src\validation\Validation.cpp ^
/Fe:simulation.exe