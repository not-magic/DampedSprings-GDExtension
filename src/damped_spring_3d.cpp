#include "damped_spring_3d.h"
#include "damped_spring_math.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/error_macros.hpp>

using namespace godot;

void DampedSpring3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_velocity"), &DampedSpring3D::get_velocity);
	ClassDB::bind_method(D_METHOD("set_velocity", "velocity"), &DampedSpring3D::set_velocity);
	ClassDB::bind_method(D_METHOD("update", "delta", "value", "target_value", "parameters"), &DampedSpring3D::update);
	ClassDB::bind_method(D_METHOD("update_blend", "delta", "value", "target_value", "parameters_a", "parameters_b", "weight"), &DampedSpring3D::update_blend);
	ClassDB::bind_method(D_METHOD("update_constants", "delta", "value", "target_value", "spring_constant", "damping_constant"), &DampedSpring3D::update_constants);

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
	return damped_spring_update(p_delta, p_value, p_target_value, p_parameters->get_spring_constant(), p_parameters->get_damping_constant(), velocity);
}

Vector3 DampedSpring3D::update_blend(const double p_delta, const Vector3 p_value, const Vector3 p_target_value, const Ref<DampedSpringParameters> &p_parameters_a, const Ref<DampedSpringParameters> &p_parameters_b, const double p_weight) {
	ERR_FAIL_COND_V(p_parameters_a.is_null(), p_value);
	ERR_FAIL_COND_V(p_parameters_b.is_null(), p_value);
	return damped_spring_update_blend(p_delta, p_value, p_target_value, p_parameters_a->get_spring_constant(), p_parameters_a->get_damping_constant(), p_parameters_b->get_spring_constant(), p_parameters_b->get_damping_constant(), p_weight, velocity);
}

Vector3 DampedSpring3D::update_constants(const double p_delta, const Vector3 p_value, const Vector3 p_target_value, const double p_spring_constant, const double p_damping_constant) {
	return damped_spring_update(p_delta, p_value, p_target_value, p_spring_constant, p_damping_constant, velocity);
}
