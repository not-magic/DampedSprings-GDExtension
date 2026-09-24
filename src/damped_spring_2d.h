#pragma once

#include "damped_spring_parameters.h"
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/variant/vector2.hpp>

namespace godot {

class DampedSpring2D : public Object {
	GDCLASS(DampedSpring2D, Object)

private:
	Vector2 velocity;

protected:
	static void _bind_methods();

public:
	DampedSpring2D();
	~DampedSpring2D();

	void set_velocity(const Vector2 p_velocity);
	Vector2 get_velocity() const;

	Vector2 update(const double p_delta, const Vector2 p_value, const Vector2 p_target_value, const Ref<DampedSpringParameters> &p_parameters);
	Vector2 update_blend(const double p_delta, const Vector2 p_value, const Vector2 p_target_value, const Ref<DampedSpringParameters> &p_parameters_a, const Ref<DampedSpringParameters> &p_parameters_b, const double p_weight);
	Vector2 update_constants(const double p_delta, const Vector2 p_value, const Vector2 p_target_value, const double p_spring_constant, const double p_damping_constant);
};

} // namespace godot
