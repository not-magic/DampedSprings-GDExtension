#pragma once

namespace godot {

// One semi-implicit Euler integration step (mass = 1) for a damped
// harmonic oscillator: F = -k*(value - target) - c*velocity. Advances
// `p_velocity` in place and returns the new value.
//
// Deliberately free of any Godot engine dependency (no Object/ClassDB,
// no Vector2/Vector3 include) so it can be shared, unmodified, between
// DampedSpring/DampedSpring2D/DampedSpring3D::update() and
// tests/test_damped_spring.cpp, which exercises it as a plain native
// program without a running Godot process. T must support +, -, unary -,
// and multiplication by a double (double, Vector2, Vector3, ...).
template <typename T>
T damped_spring_step(const T &p_value, const T &p_target_value, T &p_velocity, double p_spring_constant, double p_damping_constant, double p_delta) {
	const T acceleration = -p_spring_constant * (p_value - p_target_value) - p_damping_constant * p_velocity;
	p_velocity += acceleration * p_delta;
	return p_value + p_velocity * p_delta;
}

} // namespace godot
