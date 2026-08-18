#include "Integrator.h"

#include "Dynamics.h"

State apply_state_derivative(
    State state, 
    StateDerivative state_derivative, 
    double d_time
)
{
    return {
        add_vectors(
            state.position,
            scale_vector(
                state_derivative.position_derivative,
                d_time
            )
        ), 
        add_vectors(
            state.velocity,
            scale_vector(
                state_derivative.velocity_derivative,
                d_time
            )
        )
    };
}

State euler_integrate(
    State current_state,
    Vector2D thrust_acceleration,
    double mu,
    double d_time
)
{
    StateDerivative state_derivative = 
        calculate_state_derivative(
            current_state, 
            thrust_acceleration, 
            mu
        );

    return apply_state_derivative(
        current_state,
        state_derivative,
        d_time
    );
}

State rk4_integrate(
    State current_state,
    Vector2D thrust_acceleration,
    double mu,
    double d_time
)
{
    // --------------------------------------------------------
    // k1: derivative at beginning of timestep
    // --------------------------------------------------------
    StateDerivative k1 = 
        calculate_state_derivative(
            current_state,
            thrust_acceleration,
            mu
        );
    
    // --------------------------------------------------------
    // k2: derivative at midpoint using k1
    // --------------------------------------------------------
    State state_2 = 
        apply_state_derivative(
            current_state,
            k1,
            d_time / 2.0
        );

    StateDerivative k2 = 
        calculate_state_derivative(
            state_2,
            thrust_acceleration,
            mu
        );
    
    // --------------------------------------------------------
    // k3: derivative at midpoint using k2
    // --------------------------------------------------------
    State state_3 = 
        apply_state_derivative(
            current_state,
            k2,
            d_time / 2.0
        );
        
    StateDerivative k3 = 
        calculate_state_derivative(
            state_3,
            thrust_acceleration,
            mu
        );
    
    // --------------------------------------------------------
    // k4: derivative at end of timestep using k3
    // --------------------------------------------------------
    State state_4 = 
        apply_state_derivative(
            current_state,
            k3,
            d_time
        );
        
    StateDerivative k4 = 
        calculate_state_derivative(
            state_4,
            thrust_acceleration,
            mu
        );

    // --------------------------------------------------------
    // Weighted RK4 combination
    // y_next = y + dt/6 * (k1 + 2k2 + 2k3 + k4)
    // --------------------------------------------------------
    StateDerivative weighted_derivative;
    
    weighted_derivative.position_derivative = {
        (
            k1.position_derivative.x +
            2.0 * k2.position_derivative.x +
            2.0 * k3.position_derivative.x +
            k4.position_derivative.x
        ) / 6.0,

        (
            k1.position_derivative.y +
            2.0 * k2.position_derivative.y +
            2.0 * k3.position_derivative.y +
            k4.position_derivative.y
        ) / 6.0
    };

    weighted_derivative.velocity_derivative = {
        (
            k1.velocity_derivative.x +
            2.0 * k2.velocity_derivative.x +
            2.0 * k3.velocity_derivative.x +
            k4.velocity_derivative.x
        ) / 6.0,

        (
            k1.velocity_derivative.y +
            2.0 * k2.velocity_derivative.y +
            2.0 * k3.velocity_derivative.y +
            k4.velocity_derivative.y
        ) / 6.0
    };

    return apply_state_derivative(
        current_state,
        weighted_derivative,
        d_time
    );
}