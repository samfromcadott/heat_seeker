import bpy
import sys
import os

# ex = bpy.data.texts["exporter.py"].as_module()
from . import exporter

# Only needed if you want to add into a dynamic menu
def menu_func_export(self, context):
	self.layout.operator(exporter.MapExporter.bl_idname, text="Heat Seeker Map (.hsm)")


# Register and add to the "file selector" menu (required to use F3 search "Text Export Operator" for quick access).
def register():
	bpy.utils.register_class(exporter.MapExporter)
	bpy.types.TOPBAR_MT_file_export.append(menu_func_export)


def unregister():
	bpy.utils.unregister_class(exporter.MapExporter)
	bpy.types.TOPBAR_MT_file_export.remove(menu_func_export)


if __name__ == "__main__":
	register()

	# Test call.
	bpy.ops.export_test.some_data('INVOKE_DEFAULT')
