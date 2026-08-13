import bpy
import sys
import os
import json

# ent = bpy.data.texts["read_entities.py"].as_module()
from . import flunn
from . import entity

# ExportHelper is a helper class, defines filename and
# invoke() function which calls the file selector.
from bpy_extras.io_utils import ExportHelper
from bpy.props import StringProperty, BoolProperty, EnumProperty
from bpy.types import Operator

class MapExporter(Operator, ExportHelper):
    """This appears in the tooltip of the operator and in the generated docs"""
    bl_idname = "export_test.some_data"  # Important since its how bpy.ops.import_test.some_data is constructed.
    bl_label = "Export Some Data"

    # ExportHelper mix-in class uses this.
    filename_ext = ".hsm"

    filter_glob: StringProperty(
        default="*.hsm",
        options={'HIDDEN'},
        maxlen=255,  # Max internal buffer length, longer would be clamped.
    )

    def execute(self, context):
        return self.write_map(context, self.filepath)
    
    def write_map(self, context, filepath):
        print("Writing map...")
        f = open(filepath, "wb")

        data = entity.make_entities()
        data["MAT"] = [m.name for m in bpy.data.materials]

        j = flunn.dumps( data )
        f.write(j)
        f.close()
    
        return {'FINISHED'}
