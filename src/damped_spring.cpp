#include "damped_spring.h"
#include "damped_spring_math.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/error_macros.hpp>

#include <algorithm>
#include <cmath>

using namespace godot;

void DampedSpring::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_velocity"), &DampedSpring::get_velocity);
	ClassDB::bind_method(D_METHOD("set_velocity", "velocity"), &DampedSpring::set_velocity);
	ClassDB::bind_method(D_METHOD("update", "delta", "value", "target_value", "parameters"), &DampedSpring::update);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "velocity"), "set_velocity", "get_velocity");
}

DampedSpring::DampedSpring() {
	velocity = 0.0;
}

DampedSpring::~DampedSpring() {
}

void DampedSpring::set_velocity(const double p_velocity) {
	velocity = p_velocity;
}

double DampedSpring::get_velocity() const {
	return velocity;
}

double DampedSpring::update(const double p_delta, const double p_value, const double p_target_value, const Ref<DampedSpringParameters> &p_parameters) {
	ERR_FAIL_COND_V(p_parameters.is_null(), p_value);

	double new_value = damped_spring_step(p_value, p_target_value, velocity, p_parameters->get_spring_constant(), p_parameters->get_damping_constant(), p_delta);

	const double max_velocity = p_parameters->get_max_velocity();
	velocity = std::clamp(velocity, -max_velocity, max_velocity);

	const double max_distance = p_parameters->get_max_distance();
	const double offset = new_value - p_target_value;
	if (std::abs(offset) > max_distance) {
		new_value = p_target_value + std::copysign(max_distance, offset);
	}

	return new_value;
}
