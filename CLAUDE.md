# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project overview

A native GDExtension for Godot 4, built with C++ against `godot-cpp` (bindings, vendored as a git submodule pinned to branch `4.5`). Originally scaffolded from the [Godot GDExtension C++ example tutorial](https://docs.godotengine.org/en/stable/tutorials/scripting/cpp/gdextension_cpp_example.html); the codebase now provides `DampedSpring`, `DampedSpring2D`, and `DampedSpring3D` — damped-spring value smoothers (scalar `double`, `Vector2`, and `Vector3` respectively) exposed to GDScript.

## Build

Build system is SCons, driven by the top-level `SConstruct`, which pulls in `godot-cpp/SConstruct` and compiles everything under `src/*.cpp` into a shared library at `project/addons/DampedSprings/bin/`.

```bash
# First-time setup: fetch the godot-cpp submodule (required — it's not checked out by default)
git submodule update --init --recursive

# Build (debug, matches host platform/arch by default)
scons

# Build release
scons target=template_release

# Build for a specific platform (e.g. cross-compiling)
scons platform=<windows|linux|macos> target=<template_debug|template_release>
```

Output binary name/path is computed in `SConstruct` (`project/addons/DampedSprings/bin/{prefix}DampedSprings{suffix}{ext}`) and must match one of the paths listed in `project/addons/DampedSprings/DampedSprings.gdextension`, which Godot reads to load the extension per-platform/arch/build-type.

```bash
# Build and run the native unit tests (tests/)
scons tests
```

There is no lint tooling in this repo. Verification is: does the extension build with `scons` and behave correctly when loaded in the Godot editor/project at `project/`, plus `scons tests` for the native convergence test below (`tests/` is a separate, non-default SCons target — a plain `scons` build doesn't touch it).

## Architecture

- `src/register_types.h` / `src/register_types.cpp` — the GDExtension entry point. `dampedsprings_library_init` (the `entry_symbol` referenced in `project/addons/DampedSprings/DampedSprings.gdextension`) sets up `godot::GDExtensionBinding::InitObject`, registers `initialize_example_module`/`uninitialize_example_module` as init/terminate hooks, and sets the minimum initialization level to `MODULE_INITIALIZATION_LEVEL_SCENE`. New classes must be registered here via `GDREGISTER_CLASS(...)` inside `initialize_example_module`, gated on `p_level == MODULE_INITIALIZATION_LEVEL_SCENE`.
- `src/damped_spring_parameters.h`/`.cpp` — `DampedSpringParameters : public Resource`, a standalone, shareable config object (`GDREGISTER_CLASS`, instantiable directly from GDScript, e.g. `DampedSpringParameters.new()`). Owns the `time`/`damping` properties: `time` is the characteristic resolve time (seconds; clamped to a small positive minimum), and `damping` is the deviation from critically damped (`0.0` = critically damped, `> 0.0` = overdamped, `< 0.0` = underdamped/oscillatory, clamped so the effective damping ratio never goes negative). `setup(time, damping)` sets both and recomputes the private `spring_constant`/`damping_constant` fields (`k = 1/time²`, `c = 2·(1+damping)/time`, mass = 1) in one pass; `set_time`/`set_damping` each call `setup()` under the hood. `get_spring_constant()`/`get_damping_constant()` expose the cached coefficients for other classes to read. Also owns `max_velocity`/`max_distance` (each clamped to `>= 0`, defaulting to `1e9` so they're effectively unbounded unless a caller lowers them): the three spring classes' `update()` clamp the persisted `velocity` to at most `max_velocity`, and clamp the returned value to at most `max_distance` from `target_value`, guarding against blow-ups from lag spikes or a `target_value` that jumps far away. Being a plain `Resource` (not tied to any one spring instance) means one `DampedSpringParameters` can be shared across many springs — e.g. all UI elements using the same "snappy" tuning reference the same resource.
- `src/damped_spring.h`/`.cpp`, `src/damped_spring_2d.h`/`.cpp`, `src/damped_spring_3d.h`/`.cpp` — `DampedSpring`/`DampedSpring2D`/`DampedSpring3D : public Object`, damped harmonic oscillators for `double`/`Vector2`/`Vector3`. Each holds *only* `velocity` (typed to match the class) as a bound property — no value, no target, and no spring/damping tuning of its own, so the same instance can be reused for a different value/target/parameters each call (`velocity` is the one piece of state that must persist between calls; everything else is a per-call input). `update(delta, value, target_value, parameters)` (bound as `update(p_delta, p_value, p_target_value, p_parameters)`) inlines the one semi-implicit Euler step (`F = -k*(value - target) - c*velocity`, mass = 1) directly rather than calling a shared helper: it advances `velocity` by `acceleration * delta`, clamps `velocity` to `parameters->get_max_velocity()` *before* using it to advance the position (clamping after would let the position already reflect the pre-clamp velocity), computes `new_value = value + velocity * delta`, then clamps `new_value` to within `parameters->get_max_distance()` of `target_value` (via `std::clamp`/`copysign` for the scalar `DampedSpring`; the 2D/3D classes compare `length_squared()` against `max_distance²` to avoid a `sqrt()` on the common no-clamp path, only calling `Vector2`/`Vector3::limit_length()` when it's actually exceeded) before returning it — the caller owns storing the result (e.g. into `sprite.position`) and passing it back in on the next call. `ERR_FAIL_COND_V` guards a null `parameters` ref, returning `p_value` unchanged. Because they derive from `Object` rather than `Node`/`Resource`, they're meant to be instantiated (e.g. `DampedSpring.new()`) and driven manually — not placed in the scene tree or saved as a resource file. These are also the reference pattern for any new class in this codebase: `GDCLASS(ClassName, BaseClass)` macro, a `_bind_methods()` static method registering properties/methods via `ClassDB::bind_method` + `ADD_PROPERTY`, plain getter/setter pairs per property.
- `tests/test_damped_spring.cpp` — a native (non-Godot) convergence test that inlines the same integration step against a plain `double` (mirroring the scalar `DampedSpring` case, no clamping): simulates a spring tuned to settle in ~1 second using several fixed delta times (1/24s down to 1/4800s) and checks each result lands close to the closed-form critically-damped solution, and that error shrinks as delta shrinks — i.e. `update()`'s behavior doesn't depend on a specific fixed timestep. Built via a separate, plain (non-cross-compiling) `Environment()` in `SConstruct` — the `tests` SCons target is independent of the GDExtension's `env`/`platform`/`target` build variables since the test binary needs to run on the host machine, not the GDExtension's target platform.
- `project/` — the Godot project used to load and exercise the extension. `project/addons/DampedSprings/DampedSprings.gdextension` maps each platform/arch/build-type combination to its compiled library path (the `DampedSprings` binary/library name is the extension's build identity and is independent of the `DampedSpring` class name).
- `godot-cpp/` — vendored submodule providing the C++ bindings to the Godot engine API; not modified as part of this project's work.

When adding a new GDExtension class: create the header/source under `src/` following the `DampedSpring` pattern, then register it with `GDREGISTER_CLASS` in `register_types.cpp`.
