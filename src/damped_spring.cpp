#include "damped_spring.h"
#include "damped_spring_math.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/error_macros.hpp>

using namespace godot;

void DampedSpring::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_value"), &DampedSpring::get_value);
	ClassDB::bind_method(D_METHOD("set_value", "p_value"), &DampedSpring::set_value);
	ClassDB::bind_method(D_METHOD("get_target_value"), &DampedSpring::get_target_value);
	ClassDB::bind_method(D_METHOD("set_target_value", "p_target_value"), &DampedSpring::set_target_value);
	ClassDB::bind_method(D_METHOD("get_velocity"), &DampedSpring::get_velocity);
	ClassDB::bind_method(D_METHOD("set_velocity", "p_velocity"), &DampedSpring::set_velocity);
	ClassDB::bind_method(D_METHOD("update", "p_delta", "p_parameters"), &DampedSpring::update);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "value"), "set_value", "get_value");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "target_value"), "set_target_value", "get_target_value");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "velocity"), "set_velocity", "get_velocity");
}

DampedSpring::DampedSpring() {
	value = 0.0;
	target_value = 0.0;
	velocity = 0.0;
}

DampedSpring::~DampedSpring() {
}

void DampedSpring::set_value(const double p_value) {
	value = p_value;
}

double DampedSpring::get_value() const {
	return value;
}

void DampedSpring::set_target_value(const double p_target_value) {
	target_value = p_target_value;
}

double DampedSpring::get_target_value() const {
	return target_value;
}

void DampedSpring::set_velocity(const double p_velocity) {
	velocity = p_velocity;
}

double DampedSpring::get_velocity() const {
	return velocity;
}

double DampedSpring::update(const double p_delta, const Ref<DampedSpringParameters> &p_parameters) {
	ERR_FAIL_COND_V(p_parameters.is_null(), value);

	value = damped_spring_step(value, target_value, velocity, p_parameters->get_spring_constant(), p_parameters->get_damping_constant(), p_delta);
	return value;
}
