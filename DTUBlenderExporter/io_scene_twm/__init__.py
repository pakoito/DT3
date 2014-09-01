#==============================================================================
#	
#	File: 			export_twm.py
#	Author:			Tod Baudais
#					Copyright (C) 2000-2011. All rights reserved.
#	
#	Date Created:	6/6/2011
#	Changes:		-none-
#	
#==============================================================================

bl_info = {
    "name": "TW",
    "author": "Rob Pringle & Tod Baudais",
    "version": (0, 1),
    "blender": (2, 5, 7),
    "api": 36103,
    "location": "File > Export > TW",
    "description": "Export TW",
    "warning": "",
    "wiki_url": ""\
        "",
    "tracker_url": ""\
        "",
    "category": "Import-Export"}

if "bpy" in locals():
    import imp
    imp.reload(export_twm)
    imp.reload(export_twa)
else:
  
    from . import export_twm
    from . import export_twa

import bpy

#def menu_import(self, context):
#    self.layout.operator(import_twm.RawImporter.bl_idname, text="TankWarsModel (.twm)").filepath = "*.twm"


def menu_export_twm(self, context):
    import os
    default_path = os.path.splitext(bpy.data.filepath)[0] + ".twm"
    self.layout.operator(export_twm.TwmExporter.bl_idname, text="TWModel (.twm)").filepath = default_path

def menu_export_twa(self, context):
    import os
    default_path = os.path.splitext(bpy.data.filepath)[0] + ".twa"
    self.layout.operator(export_twa.TwaExporter.bl_idname, text="TWAnimation (.twa)").filepath = default_path


def register():
    bpy.utils.register_module(__name__)

    bpy.types.INFO_MT_file_export.append(menu_export_twm)
    bpy.types.INFO_MT_file_export.append(menu_export_twa)

def unregister():
    bpy.utils.unregister_module(__name__)

    bpy.types.INFO_MT_file_export.remove(menu_export_twm)
    bpy.types.INFO_MT_file_export.remove(menu_export_twa)
   

if __name__ == "__main__":
    register()
