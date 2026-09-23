#include "damped_spring_3d.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/error_macros.hpp>

using namespace godot;

void DampedSpring3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_velocity"), &DampedSpring3D::get_velocity);
	ClassDB::bind_method(D_METHOD("set_velocity", "velocity"), &DampedSpring3D::set_velocity);
	ClassDB::bind_method(D_METHOD("update", "delta", "value", "target_value", "parameters"), &DampedSpring3D::update);

	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "velocity"), "set_velocity", "get_velocity");
}

DampedSpring3D::DampedSpring3D() {
}

DampedSpring3D::~DampedSpring3D() {
}

void DampedSpring3D::set_velocity(const Vector3 p_velocity) {
	velocity = p_velocity;
}

Vector3 DampedSpring3D::get_velocity() const {
	return velocity;
}

Vector3 DampedSpring3D::update(const double p_delta, const Vector3 p_value, const Vector3 p_target_value, const Ref<DampedSpringParameters> &p_parameters) {
	ERR_FAIL_COND_V(p_parameters.is_null(), p_value);

	// F = -k*(value - target) - c*velocity, mass = 1.
	const Vector3 acceleration = -p_parameters->get_spring_constant() * (p_value - p_target_value) - p_parameters->get_damping_constant() * velocity;
	velocity += acceleration * p_delta;
	velocity = velocity.limit_length(p_parameters->get_max_velocity());

	Vector3 new_value = p_value + velocity * p_delta;

	const Vector3 offset = new_value - p_target_value;
	const double max_distance = p_parameters->get_max_distance();
	if (offset.length_squared() > max_distance * max_distance) {
		new_value = p_target_value + offset.limit_length(max_distance);
	}

	return new_value;
}
