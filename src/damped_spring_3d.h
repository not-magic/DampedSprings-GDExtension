#pragma once

#include "damped_spring_parameters.h"
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/variant/vector3.hpp>

namespace godot {

class DampedSpring3D : public Object {
	GDCLASS(DampedSpring3D, Object)

private:
	Vector3 velocity;

protected:
	static void _bind_methods();

public:
	DampedSpring3D();
	~DampedSpring3D();

	void set_velocity(const Vector3 p_velocity);
	Vector3 get_velocity() const;

	Vector3 update(const double p_delta, const Vector3 p_value, const Vector3 p_target_value, const Ref<DampedSpringParameters> &p_parameters);
};

} // namespace godot
