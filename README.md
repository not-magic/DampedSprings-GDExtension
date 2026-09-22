# DampedSprings-GDExtension

A native GDExtension for Godot 4 that provides damped-spring value smoothing:

- `DampedSpringParameters` — a shareable `Resource` holding the spring's tuning: `time` (seconds to settle to within 2% of the target when critically damped) and `damping` (`0.0` = critically damped, `> 0.0` = overdamped, `< 0.0` = underdamped/oscillatory).
- `DampedSpring`, `DampedSpring2D`, `DampedSpring3D` — lightweight spring state (`value`, `target_value`, `velocity`) for `double`, `Vector2`, and `Vector3` respectively. Call `update(delta, parameters)` once per frame/physics-tick to advance `value` toward `target_value`.

```gdscript
@export var spring_params: DampedSpringParameters
var spring := DampedSpring2D.new()

func _process(delta: float) -> void:
	spring.target_value = target_position
	sprite.position = spring.update(delta, spring_params)
```

Built with C++ against [`godot-cpp`](https://github.com/godotengine/godot-cpp) (vendored as a submodule). Originally scaffolded from the [Godot GDExtension C++ example tutorial](https://docs.godotengine.org/en/stable/tutorials/scripting/cpp/gdextension_cpp_example.html). See `CLAUDE.md` for build instructions and architecture notes.
