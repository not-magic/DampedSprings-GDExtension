// Standalone convergence test for damped_spring_step() (src/damped_spring_math.h).
//
// It exercises the exact per-step integration formula that
// DampedSpring/DampedSpring2D/DampedSpring3D::update() calls, but does so
// natively -- no Godot engine, no ClassDB, no GDExtension loading -- since
// the math header has no engine dependency. Build and run with `scons tests`.
//
// The check: a spring tuned to settle in ~1 second should reach ~the same
// place after 1 simulated second regardless of how finely that second is
// sliced into update() calls (1/24s steps vs 1/4800s steps), and the
// numerical error against the closed-form solution should shrink as the
// step size shrinks.

#include "../src/damped_spring_math.h"

#include <cmath>
#include <cstdio>

using namespace godot;

namespace {

int failures = 0;

void expect_near(double p_actual, double p_expected, double p_tolerance, const char *p_label) {
	const double diff = std::fabs(p_actual - p_expected);
	if (diff > p_tolerance) {
		std::fprintf(stderr, "FAIL: %s -- expected %.6f, got %.6f (|diff| %.6f > tolerance %.6f)\n", p_label, p_expected, p_actual, diff, p_tolerance);
		failures++;
	} else {
		std::printf("PASS: %s (expected %.6f, got %.6f)\n", p_label, p_expected, p_actual);
	}
}

// Simulates a spring (value 1.0 -> target 0.0, starting at rest) for
// `p_total_time` seconds using a fixed step size `p_delta`.
double simulate(double p_spring_constant, double p_damping_constant, double p_delta, double p_total_time) {
	double value = 1.0;
	double velocity = 0.0;
	const double target_value = 0.0;
	const int steps = static_cast<int>(p_total_time / p_delta + 0.5);
	for (int i = 0; i < steps; i++) {
		value = damped_spring_step(value, target_value, velocity, p_spring_constant, p_damping_constant, p_delta);
	}
	return value;
}

} // namespace

int main() {
	// Critically damped spring tuned to settle to within 2% of its target
	// in ~1 second (matches DampedSpringParameters at time=1.0, damping=0.0):
	// omega0 = 5.8339, spring_constant = omega0^2, damping_constant = 2*omega0.
	constexpr double OMEGA0 = 5.8339;
	constexpr double SPRING_CONSTANT = OMEGA0 * OMEGA0;
	constexpr double DAMPING_CONSTANT = 2.0 * OMEGA0;
	constexpr double TOTAL_TIME = 1.0;

	// Closed-form solution for a critically damped 2nd order system
	// starting at value=1, velocity=0, target=0:
	//   x(t) = (1 + omega0*t) * e^(-omega0*t)
	const double analytic = (1.0 + OMEGA0 * TOTAL_TIME) * std::exp(-OMEGA0 * TOTAL_TIME);

	// Delta times spanning common frame rates down to a very fine step.
	const double deltas[] = { 1.0 / 24.0, 1.0 / 30.0, 1.0 / 60.0, 1.0 / 120.0, 1.0 / 240.0, 1.0 / 4800.0 };
	const int num_deltas = sizeof(deltas) / sizeof(deltas[0]);

	double previous_error = -1.0;
	for (int i = 0; i < num_deltas; i++) {
		const double delta = deltas[i];
		const double result = simulate(SPRING_CONSTANT, DAMPING_CONSTANT, delta, TOTAL_TIME);
		const double error = std::fabs(result - analytic);

		char label[96];
		std::snprintf(label, sizeof(label), "settles near analytic value at delta=1/%.0f", 1.0 / delta);
		expect_near(result, analytic, 0.05, label);

		// As delta shrinks, the numerical result should converge toward the
		// analytic solution -- update() shouldn't depend on a specific
		// fixed timestep to behave correctly.
		if (previous_error >= 0.0 && error > previous_error + 1e-9) {
			std::fprintf(stderr, "FAIL: error did not shrink as delta decreased (delta=1/%.0f, error=%.6f, previous_error=%.6f)\n", 1.0 / delta, error, previous_error);
			failures++;
		}
		previous_error = error;
	}

	// The coarsest and finest delta times should converge on comparable
	// results rather than diverging from each other.
	const double coarse = simulate(SPRING_CONSTANT, DAMPING_CONSTANT, deltas[0], TOTAL_TIME);
	const double fine = simulate(SPRING_CONSTANT, DAMPING_CONSTANT, deltas[num_deltas - 1], TOTAL_TIME);
	expect_near(coarse, fine, 0.05, "coarse and fine delta times converge to comparable results");

	if (failures == 0) {
		std::printf("All damped spring convergence tests passed.\n");
		return 0;
	}
	std::fprintf(stderr, "%d damped spring convergence test(s) failed.\n", failures);
	return 1;
}
