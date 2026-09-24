#pragma once

#include <godot_cpp/classes/resource.hpp>

namespace godot {

class DampedSpringParameters : public Resource {
	GDCLASS(DampedSpringParameters, Resource)

private:
	double time;
	double damping;

	// Derived spring/damping coefficients (mass = 1) for a damped harmonic
	// oscillator, with `damping` == 0.0 being critically damped, > 0.0
	// overdamped, and < 0.0 underdamped. At `damping` == 0.0, `time` is the
	// number of seconds for the response to settle to within 2% of its
	// target (e.g. `time` == 1.0 settles in 1 second); other damping values
	// reuse the same time scaling, so it's only exact at `damping` == 0.0.
	// Recalculated whenever `time` or `damping` is set.
	double spring_constant;
	double damping_constant;

protected:
	static void _bind_methods();

public:
	DampedSpringParameters();
	~DampedSpringParameters();

	void set_time(const double p_time);
	double get_time() const;

	void set_damping(const double p_damping);
	double get_damping() const;

	void setup(const double p_time, const double p_damping);

	double get_spring_constant() const;
	double get_damping_constant() const;
};

} // namespace godot
