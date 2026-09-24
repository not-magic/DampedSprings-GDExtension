#pragma once

// Shared semi-implicit Euler integration step for a damped harmonic
// oscillator (mass = 1): F = -k*(value - target) - c*velocity.
//
// Header-only and engine-independent -- it only uses +, -, and scalar *
// on T, so it works for `double` (DampedSpring), `Vector2` (DampedSpring2D),
// and `Vector3` (DampedSpring3D) alike, and can be exercised directly by a
// plain native test program with no godot-cpp linking required.
//
// Advances `r_velocity` in place by one step and returns the new value.
// `p_spring_constant`/`p_damping_constant` are the derived coefficients
// (DampedSpringParameters::get_spring_constant()/get_damping_constant()),
// not the raw `time`/`damping` tuning values.
template <typename T>
T damped_spring_update(double p_delta, const T &p_value, const T &p_target_value, double p_spring_constant, double p_damping_constant, T &r_velocity) {
	const T acceleration = -p_spring_constant * (p_value - p_target_value) - p_damping_constant * r_velocity;
	r_velocity += acceleration * p_delta;
	return p_value + r_velocity * p_delta;
}

// Same step, but for blending smoothly between two tunings instead of
// snapping from one to the other: the spring/damping constants used for
// this step are lerped between (p_spring_constant_a, p_damping_constant_a)
// and (p_spring_constant_b, p_damping_constant_b) by p_weight (0 = fully A,
// 1 = fully B). Implements its own lerp rather than taking a dependency on
// godot-cpp's Math::lerp, to keep this header engine-independent.
template <typename T>
T damped_spring_update_blend(double p_delta, const T &p_value, const T &p_target_value, double p_spring_constant_a, double p_damping_constant_a, double p_spring_constant_b, double p_damping_constant_b, double p_weight, T &r_velocity) {
	const double spring_constant = p_spring_constant_a + (p_spring_constant_b - p_spring_constant_a) * p_weight;
	const double damping_constant = p_damping_constant_a + (p_damping_constant_b - p_damping_constant_a) * p_weight;
	return damped_spring_update(p_delta, p_value, p_target_value, spring_constant, damping_constant, r_velocity);
}
