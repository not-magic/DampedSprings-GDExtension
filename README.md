# DampedSprings-GDExtension

A native GDExtension for Godot 4 that provides damped-spring value smoothing:

- [DampedSpringParameters](../../wiki/DampedSpringParameters) — a shareable `Resource` holding the spring's tuning: `time` (seconds to settle to within 2% of the target when critically damped) and `damping` (`0.0` = critically damped, `> 0.0` = overdamped, `< 0.0` = underdamped/oscillatory).
- [DampedSpring](../../wiki/DampedSpring), [DampedSpring2D](../../wiki/DampedSpring2D), [DampedSpring3D](../../wiki/DampedSpring3D) — An instance of a spring for `double`, `Vector2`, and `Vector3` respectively. Call `update(delta, value, target_value, parameters)` once per frame/physics-tick, passing in the current value and target, to get back the new value.

```gdscript
@export var spring_params: DampedSpringParameters
var spring := DampedSpring2D.new()

func _process(delta: float) -> void:
	sprite.position = spring.update(delta, sprite.position, target_position, spring_params)
```

Springs update in this way so that multiple springs can share the same tuning values.

## API Reference

The wiki page contains the [API documentation](../../wiki/). It is also available as part of the Godot editor documentation browser.
