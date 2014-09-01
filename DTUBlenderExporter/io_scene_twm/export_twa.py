#==============================================================================
#	
#	File:			export_twa.py
#	Author:			Tod Baudais
#					Copyright (C) 2000-2011. All rights reserved.
#	
#	Date Created:	6/6/2011
#	Changes:		-none-
#	
#==============================================================================

__author__ = ["Robert Pringle & Tod Baudais"]
__version__ = '0.2'
__bpydoc__ = """\
This script exports a TWA File.
"""

import bpy
import os
import struct
import operator
from operator import attrgetter
import mathutils
from mathutils import *

# TWA 
MAGIC = 0x5E11E70E								# Smellytoad + 1
VERSION = (1 << 24) | (0 << 16) | (0 << 16)	   # 1.0.0

FILE = 0
ANIMATION = 1
ANIMATION_TRACKS = 2
ANIMATION_TRACK = 3

rot = mathutils.Matrix([[1.0,0.0,0.0,0.0],[0.0,1.0,0.0,0.0],[0.0,0.0,1.0,0.0],[0.0,0.0,0.0,1.0]])

#==============================================================================
#==============================================================================

from bpy.props import *

class TwaExportOptions:

	def __init__(self): 
		self.selection = False
	
#==============================================================================
#==============================================================================

class TwaExporterImpl():
	
	def writeFloat(self, file, value):		
		b = struct.pack("!f",value)
		file.write(b)

	def writeUnsignedInt(self, file, value):
		b = struct.pack("!I",value)
		file.write(b)
	
	def writeUnsignedShort(self, file, value):		
		b = struct.pack("!H",value)
		file.write(b)
	
	def writeUnsignedByte(self, file, value):
		b = struct.pack("!B",value)
		file.write(b)
	
	def writeMat44(self, file, mat44):
		dir(mat44)
	
		t = Matrix(mat44).to_translation()
		q = Matrix(mat44).to_3x3().to_quaternion()
				
		self.writeFloat(file, q.w)
		self.writeFloat(file, q.x)
		self.writeFloat(file, q.y)
		self.writeFloat(file, q.z)

		self.writeFloat(file, t.x)
		self.writeFloat(file, t.y)
		self.writeFloat(file, t.z)
	
		#self.writeFloat(file, mat44[0][0])
		#self.writeFloat(file, mat44[0][1])
		#self.writeFloat(file, mat44[0][2])
		
		#self.writeFloat(file, mat44[1][0])
		#self.writeFloat(file, mat44[1][1])
		#self.writeFloat(file, mat44[1][2])
		
		#self.writeFloat(file, mat44[2][0])
		#self.writeFloat(file, mat44[2][1])
		#self.writeFloat(file, mat44[2][2])
		
		#self.writeFloat(file, mat44[3][0])
		#self.writeFloat(file, mat44[3][1])
		#self.writeFloat(file, mat44[3][2])
		
		
	def writeString(self, file, value):
		for c in value:
			self.writeUnsignedByte(file, ord(c))
		self.writeUnsignedByte(file,0)
	
	
	def beginExportSection(self, file, section):
		self.writeUnsignedInt(file,section)
		size_location = file.tell()
		self.writeUnsignedInt(file,0)
		
		return size_location
	
	def endExportSection(self, file, size_location):
		save_location = file.tell() 
		file.seek(size_location, os.SEEK_SET)
		self.writeUnsignedInt(file, save_location - size_location - 4)	# sizeof(int)
		file.seek(save_location, os.SEEK_SET)
	
#==============================================================================
#==============================================================================

	def writeTrack(self, scene, file, obj, keyframes, posebone):
				
		keyframes_all = []
		keyframes_passed = []
		
		#		
		# Dump out all of the transforms at all of the keyframes
		#
		
		for keyframe in sorted(keyframes):
			scene.frame_set(keyframe)
			scene.update()

			mat = Matrix()
			if posebone.parent:
				mat = posebone.parent.matrix.inverted()
			else: 
				mat = rot
				
			pbmat = posebone.matrix.copy()
			mat *= pbmat   	
			
			time = float(keyframe - scene.frame_start) / float(scene.render.fps)
			
			keyframes_all.append( (time, mat) )
			
			
		#
		# Filter the keyframes
		#
		
		print("Num keyframes %d" % len(keyframes_all) )
		
		for keyframe_idx in range(len(keyframes_all)-1):
			#print(" Checking key %d" % keyframe_idx )

			if keyframe_idx == 0:
				print("Start %d" % keyframe_idx )
				keyframes_passed.append(keyframe_idx)
			elif keyframe_idx == (len(keyframes_all)-2):
				print("End %d" % keyframe_idx )
				keyframes_passed.append(keyframe_idx)

			elif (keyframes_all[keyframe_idx-1][1] != keyframes_all[keyframe_idx][1]) or (keyframes_all[keyframe_idx+1][1] != keyframes_all[keyframe_idx][1]):
				keyframes_passed.append(keyframe_idx)
			
		#
		# Write remaining keyframes
		#
		
		print("Num keyframes filtered %d" % len(keyframes_passed) )

		size_location = self.beginExportSection(file, ANIMATION_TRACK)
		self.writeUnsignedInt(file, len(keyframes_passed))

		for keyframe_idx in keyframes_passed:		
			time = keyframes_all[keyframe_idx][0]
			mat = keyframes_all[keyframe_idx][1]
		
			self.writeFloat(file, time)
			self.writeMat44(file, mat)
		
			print("%s %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f" % (posebone.name, time, mat[0][0], mat[1][0], mat[2][0], mat[3][0], mat[0][1], mat[1][1], mat[2][1], mat[3][1], mat[0][2], mat[1][2], mat[2][2], mat[3][2], mat[0][3], mat[1][3], mat[2][3], mat[3][3]))

		self.endExportSection(file, size_location)
		
		
	def writeTracks(self, scene, file, obj, keyframes):
		size_location = self.beginExportSection(file, ANIMATION_TRACKS)
		
		posebones = obj.pose.bones

		self.writeUnsignedInt(file, len(posebones))

		for posebone in posebones:
			print("Bone: %s" % posebone.name)
			self.writeString(file, posebone.name)
			self.writeTrack(scene, file, obj, keyframes, posebone)

		self.endExportSection(file, size_location)
		
		
	def writeAnimation(self, scene, file, obj, options):
	
		cur_frame = scene.frame_current		# Remember current frame
		
		print ('Start Frame: %i, End Frame: %i' % (scene.frame_start, scene.frame_end))
		print ('Frames Per Sec: %i' % scene.render.fps)

		if obj.animation_data == None:
			print("No animation data")
			return
			
		if obj.animation_data.action == None:
			print("No action data")
			return
			
		if obj.animation_data.action:
		
			keyframes = set([])
			fcurves = []
			
			if (not options.all_frames):
				for fcurve in obj.animation_data.action.fcurves:
					for keyframe in fcurve.keyframe_points:
						if keyframe.co[0] >= scene.frame_start and keyframe.co[0] <= scene.frame_end:
							keyframes.add(keyframe.co[0])
			else:
				keyframes = set(range(scene.frame_start, scene.frame_end))

			size_location = self.beginExportSection(file, ANIMATION)
					
			self.writeUnsignedInt(file, scene.render.fps)
			self.writeFloat(file, float(scene.frame_start) / float(scene.render.fps))
			self.writeFloat(file, float(scene.frame_end) / float(scene.render.fps))
			
			self.writeTracks(scene, file, obj, keyframes)
		
		scene.frame_set(cur_frame)	# Reset current frame
		
		self.endExportSection(file, size_location)

#==============================================================================
#==============================================================================

	def writeScene(self, scene, filepath, objects_list, options):

		print("running writeScene to : %s " % filepath)
		file = open(filepath, 'wb')
		
		self.writeUnsignedInt(file,MAGIC)
		self.writeUnsignedInt(file,VERSION)
		
		size_location = self.beginExportSection(file, FILE)
				
		#
		# Animations header
		#	
			
		for obj in objects_list:
		
			if obj.type == 'ARMATURE':
				self.writeAnimation(scene, file, obj, options)


		self.endExportSection(file, size_location)
		
		file.close()
		
		return {'FINISHED'}
	
	
#==============================================================================
#==============================================================================

class TwaExporter(bpy.types.Operator):
	'''Save Raw triangle mesh data'''
	bl_idname = "export_mesh.twa"
	bl_label = "Export TWA"
	
	filepath = StringProperty(name="File Path", description="Filepath used for exporting the TWA file", maxlen= 1024, default= "", subtype='FILE_PATH')
	check_existing = BoolProperty(name="Check Existing", description="Check and warn on overwriting existing files", default=True, options={'HIDDEN'})

	selection = BoolProperty(name="Selection Only", description="Export Selection Only", default=True)
	all_frames = BoolProperty(name="All Frames", description="Export All Frames", default=True)

	def execute(self, context):
		#export_twa(context, self.filepath, self.apply_modifiers, self.optimize)
		options = TwaExportOptions()
		options.selection = self.selection
		options.all_frames = self.all_frames
		
		#
		# Get Lists of objects
		#
		
		if (options.selection):
			objects_list = context.selected_objects
		else:
			objects_list = context.scene.objects

		impl = TwaExporterImpl()
		impl.writeScene(context.scene, self.filepath, objects_list, options)
		return {'FINISHED'}
	
	def invoke(self, context, event):
		wm = context.window_manager
		wm.fileselect_add(self)
		return {'RUNNING_MODAL'}
		
#==============================================================================
#==============================================================================