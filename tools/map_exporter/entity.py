import bpy

# mesh = bpy.data.texts["mesh.py"].as_module()
from . import mesh

def make_entities():
	entities = {}
	entities["ENT"] = {}

	# Convert objects to engine entities
	for object in bpy.data.objects:
		e = {}
		e["COMP"] = {}

		# Get position
		e["COMP"]["Position"] = {
			"x": object.location.x,
			"y": object.location.y,
			"z": object.location.z
		}

		# Get rotation
		q = object.matrix_world.to_quaternion()
		e["COMP"]["Rotation"] = {"w": q.w, "x": q.x, "y": q.y, "z": q.z}

		# Get other properties
		get_props(object, e)

		# Get mesh
		if object.type == "MESH":
			e["MODEL"] = mesh.convert_model(object)

		entities["ENT"][object.name] = e

	return entities

def get_props(object, entity):
	for K in object.keys():
		if K in '_RNA_UI': continue
		entity["COMP"][K] = object[K]
		
