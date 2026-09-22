#include "damped_spring_2d.h"
#include "damped_spring_math.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/error_macros.hpp>

using namespace godot;

void DampedSpring2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_velocity"), &DampedSpring2D::get_velocity);
	ClassDB::bind_method(D_METHOD("set_velocity", "velocity"), &DampedSpring2D::set_velocity);
	ClassDB::bind_method(D_METHOD("update", "delta", "value", "target_value", "parameters"), &DampedSpring2D::update);

	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "velocity"), "set_velocity", "get_velocity");
}

DampedSpring2D::DampedSpring2D() {
}

DampedSpring2D::~DampedSpring2D() {
}

void DampedSpring2D::set_velocity(const Vector2 p_velocity) {
	velocity = p_velocity;
}

Vector2 DampedSpring2D::get_velocity() const {
	return velocity;
}

Vector2 DampedSpring2D::update(const double p_delta, const Vector2 p_value, const Vector2 p_target_value, const Ref<DampedSpringParameters> &p_parameters) {
	ERR_FAIL_COND_V(p_parameters.is_null(), p_value);

	return damped_spring_step(p_value, p_target_value, velocity, p_parameters->get_spring_constant(), p_parameters->get_damping_constant(), p_delta);
}
