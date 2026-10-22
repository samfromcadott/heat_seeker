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

		# Get light
		if object.type == "LIGHT":
			l = bpy.data.lights[object.name]
			e["COMP"]["HSE::Light"] = {}
			e["COMP"]["HSE::Light"]["power"] = l.energy
			e["COMP"]["HSE::Light"]["color"] = {
				"x": l.color[0],
				"y": l.color[1],
				"z": l.color[2],
				"w": 1.0
			}

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
			add_component(object, entity, K)


def add_tag(entity, tag):
	if "TAGS" in entity: entity["TAGS"].append(tag)
	else: entity["TAGS"] = [tag]

def add_component(object, entity, K):
	# Split component name and members
	C = K.split('.')
	comp = C[0]

	# Add component
	if comp not in entity["COMP"]:
		entity["COMP"][comp] = {}

	# Loop through members
	prop = entity["COMP"][comp]
	for i in range( 1, len(C) ):
		# If on the deepest member
		if i == len(C)-1:
			prop[ C[i] ] = object[K]
			break

		# Add member
		if C[i] not in prop:
			prop[ C[i] ] = {}

		prop = prop[ C[i] ] # Go deeper
