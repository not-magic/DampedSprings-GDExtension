#pragma once

#include "damped_spring_parameters.h"
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/ref.hpp>

namespace godot {

class DampedSpring : public Object {
	GDCLASS(DampedSpring, Object)

private:
	double velocity;

protected:
	static void _bind_methods();

public:
	DampedSpring();
	~DampedSpring();

	void set_velocity(const double p_velocity);
	double get_velocity() const;

	double update(const double p_delta, const double p_value, const double p_target_value, const Ref<DampedSpringParameters> &p_parameters);
};

} // namespace godot
