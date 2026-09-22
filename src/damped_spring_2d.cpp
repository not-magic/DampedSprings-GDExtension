#include "damped_spring_2d.h"
#include "damped_spring_math.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/error_macros.hpp>

using namespace godot;

void DampedSpring2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_value"), &DampedSpring2D::get_value);
	ClassDB::bind_method(D_METHOD("set_value", "p_value"), &DampedSpring2D::set_value);
	ClassDB::bind_method(D_METHOD("get_target_value"), &DampedSpring2D::get_target_value);
	ClassDB::bind_method(D_METHOD("set_target_value", "p_target_value"), &DampedSpring2D::set_target_value);
	ClassDB::bind_method(D_METHOD("get_velocity"), &DampedSpring2D::get_velocity);
	ClassDB::bind_method(D_METHOD("set_velocity", "p_velocity"), &DampedSpring2D::set_velocity);
	ClassDB::bind_method(D_METHOD("update", "p_delta", "p_parameters"), &DampedSpring2D::update);

	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "value"), "set_value", "get_value");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "target_value"), "set_target_value", "get_target_value");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "velocity"), "set_velocity", "get_velocity");
}

DampedSpring2D::DampedSpring2D() {
}

DampedSpring2D::~DampedSpring2D() {
}

void DampedSpring2D::set_value(const Vector2 p_value) {
	value = p_value;
}

Vector2 DampedSpring2D::get_value() const {
	return value;
}

void DampedSpring2D::set_target_value(const Vector2 p_target_value) {
	target_value = p_target_value;
}

Vector2 DampedSpring2D::get_target_value() const {
	return target_value;
}

void DampedSpring2D::set_velocity(const Vector2 p_velocity) {
	velocity = p_velocity;
}

Vector2 DampedSpring2D::get_velocity() const {
	return velocity;
}

Vector2 DampedSpring2D::update(const double p_delta, const Ref<DampedSpringParameters> &p_parameters) {
	ERR_FAIL_COND_V(p_parameters.is_null(), value);

	value = damped_spring_step(value, target_value, velocity, p_parameters->get_spring_constant(), p_parameters->get_damping_constant(), p_delta);
	return value;
}
