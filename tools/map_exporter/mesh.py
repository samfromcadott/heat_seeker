import bpy
import bmesh
import array
import struct

def convert_model(object):	
	# Convert to BMesh
	dg = bpy.context.evaluated_depsgraph_get()

	old_mesh = bmesh.new()
	old_mesh.from_object(object, dg)
	
	# Triangulate
	bmesh.ops.triangulate(old_mesh, faces=old_mesh.faces[:])
	
	# Convert to a dictionary
	new_model = {}
	new_model["MESH"] = []
	
	# Divide mesh based on material
	for mat in object.material_slots:
		mesh = convert_mesh(old_mesh, mat)
		new_model["MESH"].append(mesh)
	
	return new_model

def convert_mesh(mesh, mat):
	# Convert to dict
	new_mesh = {}
	vert = []
	norm = []
	uv = []
	
	uv_layer = mesh.loops.layers.uv.active
	# normal_layer = mesh.loops.layers.normal
	
	# Add verticies
	for f in mesh.faces:
		if f.material_index != mat.slot_index: continue
	
		for loop in f.loops:
			vert.append(loop.vert.co.x)
			vert.append(loop.vert.co.y)
			vert.append(loop.vert.co.z)
		
			n = loop.vert.normal if f.smooth else f.normal
			norm.append(n.x)
			norm.append(n.y)
			norm.append(n.z)
			
			uv.append(loop[uv_layer].uv.x)
			uv.append(1.0 - loop[uv_layer].uv.y)
	
	new_mesh["VERT"] = array.array("f", vert).tobytes()
	# new_mesh["VERT"] = struct.pack(f'>{len(vert)}f', *vert)
	new_mesh["NORM"] = array.array("f", norm).tobytes()
	new_mesh["UV"] = array.array("f", uv).tobytes()
	new_mesh["MAT"] = mat.name
	
	return new_mesh
