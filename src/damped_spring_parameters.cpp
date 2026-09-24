#include "damped_spring_parameters.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/math_defs.hpp>

using namespace godot;

namespace {
constexpr double MIN_TIME = 0.001;
}

void DampedSpringParameters::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_time"), &DampedSpringParameters::get_time);
	ClassDB::bind_method(D_METHOD("set_time", "p_time"), &DampedSpringParameters::set_time);
	ClassDB::bind_method(D_METHOD("get_damping"), &DampedSpringParameters::get_damping);
	ClassDB::bind_method(D_METHOD("set_damping", "p_damping"), &DampedSpringParameters::set_damping);
	ClassDB::bind_method(D_METHOD("get_spring_constant"), &DampedSpringParameters::get_spring_constant);
	ClassDB::bind_method(D_METHOD("get_damping_constant"), &DampedSpringParameters::get_damping_constant);
	ClassDB::bind_method(D_METHOD("setup", "p_time", "p_damping"), &DampedSpringParameters::setup);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "time", PROPERTY_HINT_RANGE, "0.001,10,0.001,or_greater"), "set_time", "get_time");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "damping", PROPERTY_HINT_RANGE, "-0.9,0.9,0.01,or_less,or_greater"), "set_damping", "get_damping");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "spring_constant", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR | PROPERTY_USAGE_READ_ONLY), "", "get_spring_constant");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "damping_constant", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR | PROPERTY_USAGE_READ_ONLY), "", "get_damping_constant");
}

DampedSpringParameters::DampedSpringParameters() {
	setup(1.0, 0.0);
}

DampedSpringParameters::~DampedSpringParameters() {
}

void DampedSpringParameters::set_time(const double p_time) {
	setup(p_time, damping);
}

double DampedSpringParameters::get_time() const {
	return time;
}

void DampedSpringParameters::set_damping(const double p_damping) {
	setup(time, p_damping);
}

double DampedSpringParameters::get_damping() const {
	return damping;
}

void DampedSpringParameters::setup(const double p_time, const double p_damping) {

	time = std::max(MIN_TIME, p_time);
	damping = std::max(-0.9, std::min(0.9, p_damping));

	// The zeta math below treats a positive value as underdamped and a
	// negative value as overdamped; invert here so that, from the
	// caller's perspective, high (positive) `damping` is overdamped and
	// low (negative) `damping` is underdamped.
	const double effective_damping = -damping;

	double zeta;

	if (effective_damping > 0.0f)
	{
		// Underdamped: overshoot is the first peak amplitude.
		double logOvershoot = std::log(effective_damping);

		zeta = -logOvershoot /
			std::sqrt(
				Math_PI * Math_PI +
				logOvershoot * logOvershoot);
	}
	else if (effective_damping == 0.0f)
	{
		// Critically damped.
		zeta = 1.0f;
	}
	else
	{
		// Overdamped.
		// Map negative effective_damping to a damping ratio.
		//
		// -0.1 -> zeta = 1.1
		// -0.5 -> zeta = 1.5
		// -1.0 -> zeta = 2.0
		zeta = 1.0f - effective_damping;
	}

	// Choose natural frequency based on duration.
	//
	// For underdamped springs, duration is the first peak time.
	// For overdamped springs, this is only an approximate mapping.
	double omegaN;

	if (zeta < 1.0f)
	{
		double omegaD = Math_PI / time;
		omegaN = omegaD / std::sqrt(1.0f - zeta * zeta);
	}
	else
	{
		// Approximate time constant mapping.
		omegaN = 4.0f / time;
	}

	spring_constant = omegaN * omegaN;
	damping_constant = 2.0f * zeta * omegaN;
}

double DampedSpringParameters::get_spring_constant() const {
	return spring_constant;
}

double DampedSpringParameters::get_damping_constant() const {
	return damping_constant;
}

