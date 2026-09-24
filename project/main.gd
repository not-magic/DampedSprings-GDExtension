extends Node2D

@onready var drag_sphere: MeshInstance2D = $DragSphere
@onready var follow_sphere: MeshInstance2D = $FollowSphere
@onready var follow_sphere_2: MeshInstance2D = $FollowSphere2

@export var spring_params:DampedSpringParameters
@export var clamp_spring_params:DampedSpringParameters
@export var clamp_dist := 200

var spring = DampedSpring2D.new()
var spring2 = DampedSpring2D.new()

var _is_dragging := false
var _drag_offset : Vector2

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	var mp := get_viewport().get_mouse_position()
	
	if Input.is_action_pressed("MousePressed"):
		if _is_dragging:
			drag_sphere.position = mp + _drag_offset
		else:
			if mp.distance_to(drag_sphere.position) < drag_sphere.scale.x*0.5:
				_is_dragging = true
				_drag_offset = drag_sphere.position - mp
	else:
		_is_dragging = false
		
	follow_sphere.position = spring.update(delta, follow_sphere.position, drag_sphere.position, spring_params)
	follow_sphere_2.position = spring2.update_blend(delta, follow_sphere_2.position, drag_sphere.position, spring_params, clamp_spring_params, clampf(remap(follow_sphere_2.position.distance_to(drag_sphere.position), clamp_dist, clamp_dist*2, 0, 1), 0, 1))
