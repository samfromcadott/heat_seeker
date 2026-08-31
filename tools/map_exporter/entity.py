import bpy

# mesh = bpy.data.texts["mesh.py"].as_module()
from . import mesh

tag_names = ["NO_EXPORT", "NO_MODEL", "NO_RENDER", "NO_COLLIDE", "CONVEX"]

def make_entities():
	entities = {}
	entities["ENT"] = {}

	# Convert objects to engine entities
	for object in bpy.data.objects:
		if "NO_EXPORT" in object.keys() and object["NO_EXPORT"] == True:
			continue

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

		if K == "INHERIT":
			entity["INHERIT"] = object[K]
		elif K in tag_names:
			if object[K]: add_tag(entity, K)
		else:
			# Split component name and members
			C = K.split('.')
			comp = C[0]
			member = C[1]

			# If component already in entity
			if comp in entity["COMP"]:
				entity["COMP"][comp][member] = object[K]
			else:
				entity["COMP"][comp] = {member: object[K]}
		

def add_tag(entity, tag):
	if "TAGS" in entity: entity["TAGS"].append(tag)
	else: entity["TAGS"] = [tag]
