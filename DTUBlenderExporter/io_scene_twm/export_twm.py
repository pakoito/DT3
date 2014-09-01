#==============================================================================
#	
#	File:			export_twm.py
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
This script exports a TWM File.
"""

import bpy
import os
import struct
import operator
from operator import attrgetter
import mathutils
from mathutils import *

# TWM 
MAGIC = 0x5E11E70D								# Smellytoad
VERSION = (1 << 24) | (0 << 16) | (0 << 16)	   # 1.0.0

FILE = 0
MESHES = 1
MESHES_MESH = 2
MESHES_MESH_NAME = 3
MESHES_MESH_POSITIONS = 4
MESHES_MESH_NORMALS = 5
MESHES_MESH_UV_SETS = 6
MESHES_MESH_UVS = 7
MESHES_MESH_SKINNING = 8
MESHES_MESH_SKINNING_JOINTS = 9
MESHES_MESH_SKINNING_INFLUENCES = 10
MESHES_MESH_INDICES = 12
			
SKELETON = 13
DEBUG = False

rot = mathutils.Matrix([[1.0,0.0,0.0,0.0],[0.0,1.0,0.0,0.0],[0.0,0.0,1.0,0.0],[0.0,0.0,0.0,1.0]])

#==============================================================================
# Primitives
#==============================================================================

class Vertex3:
	def __init__(self, xx, yy, zz):
		self.x = xx
		self.y = yy
		self.z = zz
		
		
class UV:
	def __init__(self, ss, tt):
		self.s = ss
		self.t = tt
		
		
class Triangle:
	def __init__(self, ii1, ii2, ii3):

		self.i1 = ii1
		self.i2 = ii2
		self.i3 = ii3
		

class Weights:
	def __init__(self, i1, w1, i2, w2, i3, w3, i4, w4):

		self.i1 = i1
		self.i2 = i2
		self.i3 = i3
		self.i4 = i4
		
		# Normalize weights
		total = w1+w2+w3+w4
		if total > 0.0:
			w1 = w1 / total
			w2 = w2 / total
			w3 = w3 / total
			w4 = w4 / total
		else:
			w1 = 0.0
			w2 = 0.0
			w3 = 0.0
			w4 = 0.0

		self.w1 = w1
		self.w2 = w2
		self.w3 = w3
		self.w4 = w4

#==============================================================================
#==============================================================================

class TriangulatedMesh:
	
	def __init__(self, obj,  mesh):

		self.object = obj
		self.mesh = mesh
		self.name = mesh.name
		

		
		#self.weightDict = self.buildWeights(obj, mesh)
		#self.dumpWeights()
		#print(self.weightDict)
		
		self.triangulateMesh(obj, mesh,2)
		#self.WriteMeshSkinWeights(obj, mesh)
		
		
	def uniqueVert(self, mesh, idx):
		#x = mesh.vertices[idx].co.x
		#y = mesh.vertices[idx].co.y
		#z = mesh.vertices[idx].co.z
		
		v = (rot * mathutils.Matrix.Translation(mesh.vertices[idx].co)).to_translation()
		self.vertices.append(Vertex3( v[0], v[1], v[2] ))
		
		#x = mesh.vertices[idx].normal.x
		#y = mesh.vertices[idx].normal.y
		#z = mesh.vertices[idx].normal.z

		v = (rot * mathutils.Matrix.Translation(mesh.vertices[idx].normal)).to_translation()
		self.normals.append(Vertex3( v[0], v[1], v[2] ))
		
		#print("added vertex [%f, %f, %f]" % (x, y, z) )
		
		return len(self.vertices) - 1
		
	def uniqueVerts(self, mesh, i1, i2, i3):
		i1 = self.uniqueVert(mesh, i1)
		i2 = self.uniqueVert(mesh, i2)
		i3 = self.uniqueVert(mesh, i3)
		
		self.indices.append(Triangle(i1, i2, i3))
		
		
	def uniqueUV(self, uv_set, uv_face, idx):
		s = uv_face.uv[idx][0]
		t = uv_face.uv[idx][1]
		self.uv_sets[uv_set].append(UV(s, t))
		
	def uniqueUVs(self, uv_set, uv_face, i1, i2, i3):
		self.uniqueUV(uv_set, uv_face, i1)
		self.uniqueUV(uv_set, uv_face, i2)
		self.uniqueUV(uv_set, uv_face, i3)
		
		
	def uniqueWeight(self, obj, mesh, num_influences, idx):
	
		vg = mesh.vertices[idx].groups
		vgs = sorted(vg, key=operator.attrgetter('weight'), reverse=True)
		
		# calculate max number of weights
		max_weights = min(num_influences, len(vgs))
		
		i1 = i2 = i3 = i4 = 0
		w1 = w2 = w3 = w4 = 0.0
		
		if (max_weights >= 1):
			i1 = vgs[0].group
			w1 = vgs[0].weight
		if (max_weights >= 2):
			i2 = vgs[1].group
			w2 = vgs[1].weight
		if (max_weights >= 3):
			i3 = vgs[2].group
			w3 = vgs[2].weight
		if (max_weights >= 4):
			i4 = vgs[3].group
			w4 = vgs[3].weight
	
		#print("Weights: %i %f %i %f %i %f %i %f" % (i1, w1, i2, w2, i3, w3, i4, w4))
		self.weights.append(Weights(i1, w1, i2, w2, i3, w3, i4, w4))
	
	def uniqueWeights(self, obj, mesh, num_influences, i1, i2, i3):
		self.uniqueWeight(obj, mesh, num_influences, i1)
		self.uniqueWeight(obj, mesh, num_influences, i2)
		self.uniqueWeight(obj, mesh, num_influences, i3)
					
	def triangulateMesh(self, obj, mesh, num_influences):
		self.vertices = []
		self.normals = []
		self.weights = []
		self.uv_sets = [[]] * len(mesh.uv_textures)
		self.indices = []
		self.jointNames = []
	
		# Vertices and normals
		for face in mesh.faces:

			fvi = face.vertices
			
			# verts and normals
			self.uniqueVerts(mesh, fvi[0], fvi[1], fvi[2])
			if len(fvi) == 4:		# Triangulate
				self.uniqueVerts(mesh, fvi[2], fvi[3], fvi[0])	
			
			# uvs
			uv_channels = mesh.uv_textures			
			for uvc in range(len(uv_channels)):		
				self.uniqueUVs(uvc, uv_channels[uvc].data[face.index],	0, 1, 2)
				if len(fvi) == 4:	# Triangulate
					self.uniqueUVs(uvc, uv_channels[uvc].data[face.index], 2, 3, 0)

			#weights
			self.uniqueWeights(obj, mesh, num_influences, fvi[0], fvi[1], fvi[2])
			if len(fvi) == 4:		# Triangulate
				self.uniqueWeights(obj, mesh, num_influences, fvi[2], fvi[3], fvi[0])
				
		# Joint names
		for vgroup in obj.vertex_groups:
			print("name:",(vgroup.name),"index:",vgroup.index)
			self.jointNames.append(vgroup.name)
		
		
			
		print("Num Verts = %i" % len(self.vertices))
		print("Num Weights = %i" % len(self.weights))
		
		print("Num Tris = %i" % int(len(self.vertices)/3) )
		
		
#==============================================================================
#==============================================================================
		
	def vec3Equal(self, v1, v2, tolerance=0.0):
		if (v1.x != v2.x or v1.y != v2.y or v1.z != v2.z):
			return False
		
		return True
		
	def uvEqual(self, uv1, uv2, toloerance=0.0):
		if (uv1.s != uv2.s or uv1.t != uv2.t):
			return False
			
		return True
			
	def collapseVerts(self):

		num_verts = len(self.vertices)
			
		
		v1 = 0
		while v1 < num_verts:
		
										
			v2 = v1 + 1
			while v2 < num_verts:
				#print ("%d, %d" % (v1, v2))
				
				verts_equal = True

				verts_equal = self.vec3Equal(self.vertices[v1], self.vertices[v2])
			
				if verts_equal and len(self.uv_sets) >=1:
					uvs = self.uv_sets[0]
					verts_equal = self.uvEqual(uvs[v1], uvs[v2])
	
				if verts_equal and len(self.uv_sets) >=2:
					uvs = self.uv_sets[1]
					verts_equal = self.uvEqual(uvs[v1], uvs[v2])
					
				if verts_equal and len(self.normals) > 0:
					verts_equal = self.vec3Equal(self.normals[v1], self.normals[v2])
			
				if verts_equal:
					#Now we just have to remove vertex v2. Easy huh?
					#Remove v2 from the list
					del self.vertices[v2]
					del self.normals[v2]
					if (len(self.uv_sets) >=1):
						del self.uv_sets[0][v2]
					if (len(self.uv_sets) >=2):
						del self.uv_sets[1][v2] 
					if (self.weights):
						del self.weights[v2]
						
					#if(_weights_index_stream)		_weights_index_stream->Get(v) = _weights_index_stream->Get(v+1);
					#if(_weights_strength_stream)	_weights_strength_stream->Get(v) = _weights_strength_stream->Get(v+1);
		
					# renumber the triangle indices
					for f in range(0, len(self.indices) ):
						if self.indices[f].i1 == v2:
							self.indices[f].i1 = v1
						if self.indices[f].i2 == v2:
							self.indices[f].i2 = v1
						if self.indices[f].i3 == v2:
							self.indices[f].i3 = v1
							
						if self.indices[f].i1 > v2:
							self.indices[f].i1 = self.indices[f].i1 - 1
						if self.indices[f].i2 > v2:
							self.indices[f].i2 = self.indices[f].i2 - 1
						if self.indices[f].i3 > v2:
							self.indices[f].i3 = self.indices[f].i3 - 1
							
					
					#Get set up for the next round
					v2 = v2 -1
					num_verts = num_verts - 1
						
				v2 = v2 + 1		
			v1 = v1 + 1 # ugh. stupid python

	def optimize(self):
		self.collapseVerts()		

#==============================================================================
#==============================================================================

from bpy.props import *

class TwmExportOptions:
	def __init__(self): 
		self.apply_modifiers = False
		self.optimize = False
		self.selection = False
	
#==============================================================================
#==============================================================================

class TwmExporterImpl():

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
		self.writeFloat(file, mat44[0][0])
		self.writeFloat(file, mat44[0][1])
		self.writeFloat(file, mat44[0][2])
		self.writeFloat(file, mat44[0][3])
		
		self.writeFloat(file, mat44[1][0])
		self.writeFloat(file, mat44[1][1])
		self.writeFloat(file, mat44[1][2])
		self.writeFloat(file, mat44[1][3])
		
		self.writeFloat(file, mat44[2][0])
		self.writeFloat(file, mat44[2][1])
		self.writeFloat(file, mat44[2][2])
		self.writeFloat(file, mat44[2][3])
		
		self.writeFloat(file, mat44[3][0])
		self.writeFloat(file, mat44[3][1])
		self.writeFloat(file, mat44[3][2])
		self.writeFloat(file, mat44[3][3])
		
		
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

	def writeMeshName(self, file, name):

		size_location = self.beginExportSection(file, MESHES_MESH_NAME)
		
		# write string
		print("Writing name: %s" % (name))
		self.writeString(file, name)
		
		self.endExportSection(file, size_location)
	
#==============================================================================
#==============================================================================
	
	def writeMeshPositions(self, file, vertices):
		size_location = self.beginExportSection(file, MESHES_MESH_POSITIONS)
	
		# write vertices
		self.writeUnsignedInt(file, len(vertices))
		print("Writing positions: %d " % len(vertices))
		for idx in range(len(vertices)):
			#TWuint idx = vertices[idx].positionIndex
			self.writeFloat(file, vertices[idx].x)
			self.writeFloat(file, vertices[idx].y)
			self.writeFloat(file, vertices[idx].z)
			
		self.endExportSection(file, size_location)
		
	def writeMeshNormals(self, file, normals, vertices):
	
		if (len(normals) <= 0):
			return
	
		size_location = self.beginExportSection(file, MESHES_MESH_NORMALS)
	
		# write normals
		self.writeUnsignedInt(file, len(vertices))
		print("Writing normals: %d " % len(vertices))
	
		
		for idx in range(len(vertices)):
			#TWuint idx = vertices[i].normalIndex
			self.writeFloat(file, normals[idx].x)
			self.writeFloat(file, normals[idx].y)
			self.writeFloat(file, normals[idx].z)
			
		
		self.endExportSection(file, size_location)

#==============================================================================
#==============================================================================

	def writeMeshUVs(self, file, uvs, vertices, uv_set):

		size_location = self.beginExportSection(file, MESHES_MESH_UVS)
		
		self.writeUnsignedInt(file,len(vertices))
		print("Writing uvs: %i" % len(vertices))
		
		for uv in uvs:
			self.writeFloat(file, uv.s)
			self.writeFloat(file, uv.t)
			
		self.endExportSection(file, size_location)

	
	def writeMeshUVSets(self, file, uv_sets, vertices):
		
		num_uv_sets = len(uv_sets)
		
		if num_uv_sets == 0:
			return
	
		size_location = self.beginExportSection(file, MESHES_MESH_UV_SETS)
		
		# write uvs
		self.writeUnsignedInt(file, num_uv_sets)
		print("Writing uv set: %i" % num_uv_sets)
	
		if num_uv_sets > 2:
			num_uv_sets = 2
			
		for i in range(num_uv_sets):
			self.writeMeshUVs(file, uv_sets[i], vertices, i)
		
		self.endExportSection(file, size_location)
		
#==============================================================================
#==============================================================================

	def writeMeshJoints(self, file, jointNames):
		size_location = self.beginExportSection(file, MESHES_MESH_SKINNING_JOINTS)	
		
		# write normals
		self.writeUnsignedInt(file, len(jointNames))
		print("Writing joints: %i " % len(jointNames))
		
		for j in jointNames:
			self.writeString(file, j)
		
		self.endExportSection(file, size_location)
		
		
	def writeMeshInfluences(self, file, weights):
		if (len(weights) <= 0):
			return
	
		size_location = self.beginExportSection(file, MESHES_MESH_SKINNING_INFLUENCES)
	
		# write normals
		self.writeUnsignedInt(file, len(weights))
		print("Writing weights: %i " % len(weights))
	
		for idx in range(len(weights)):
			#TWuint idx = vertices[i].normalIndex
			self.writeUnsignedInt(file, weights[idx].i1)
			self.writeFloat(file, weights[idx].w1)
			self.writeUnsignedInt(file, weights[idx].i2)
			self.writeFloat(file, weights[idx].w2)
			self.writeUnsignedInt(file, weights[idx].i3)
			self.writeFloat(file, weights[idx].w3)
			self.writeUnsignedInt(file, weights[idx].i4)
			self.writeFloat(file, weights[idx].w4)

		self.endExportSection(file, size_location)


	def writeMeshSkinning(self, file, jointNames, weights):
		if (len(jointNames) <= 0):
			return		
	
		size_location = self.beginExportSection(file, MESHES_MESH_SKINNING)
	
		self.writeMeshJoints(file, jointNames)
		self.writeMeshInfluences(file, weights)
		
		self.endExportSection(file, size_location)
		
#==============================================================================
#==============================================================================

	def writeMeshIndices(self, file, indices):

		size_location = self.beginExportSection(file, MESHES_MESH_INDICES)
	
		self.writeUnsignedInt(file, len(indices))
		
		print ("Writing indices: %i" % len(indices))
		
		for i in range(len(indices)):
			self.writeUnsignedShort(file, indices[i].i1)
			self.writeUnsignedShort(file, indices[i].i2)
			self.writeUnsignedShort(file, indices[i].i3)
			
		self.endExportSection(file, size_location)
		
#==============================================================================
#==============================================================================

	def writeMesh(self, file,tri_mesh):

		size_location = self.beginExportSection(file, MESHES_MESH)
		
		#
		# write out mesh parts
		#
		self.writeMeshName(file, tri_mesh.name)
		self.writeMeshPositions(file,tri_mesh.vertices)
		self.writeMeshNormals(file, tri_mesh.normals, tri_mesh.vertices)
		self.writeMeshUVSets(file, tri_mesh.uv_sets, tri_mesh.vertices)
		self.writeMeshSkinning(file, tri_mesh.jointNames, tri_mesh.weights)
		self.writeMeshIndices(file, tri_mesh.indices)
		
		self.endExportSection(file, size_location)
	
	
	def writeMeshes(self, scene, file, objects_list, options):
		size_location = self.beginExportSection(file, MESHES)
				
		# count the meshes in our selection
		meshCount = 0
		for obj in objects_list:
			if obj.type == 'MESH':
				meshCount = meshCount + 1
				
		#
		# Meshes header
		#	
		
		self.writeUnsignedInt(file, meshCount)
		print("Writing out %i meshes" % meshCount)
		
		if (options.bindpose):
			for obj in objects_list:
				if obj.type == 'ARMATURE':
					arm = obj.data
					arm.pose_position = 'REST'
	
			# Force an update
			cur_frame = scene.frame_current
			scene.frame_set(cur_frame)

	
		for obj in objects_list:
			if obj.type == 'MESH':
				matrix = obj.matrix_world
				if (options.apply_modifiers):
					mesh = obj.to_mesh(scene, True, "PREVIEW")
				else:
					mesh= obj.data
				
				
				tri_mesh = TriangulatedMesh(obj, mesh)
				if options.optimize:
					tri_mesh.optimize()
				self.writeMesh(file,tri_mesh)
		
		self.endExportSection(file, size_location)
		
#==============================================================================
#==============================================================================

	def writeSkeletonJoints(self, file, bones, parent, indent):
		
		#print("Writing %i bones" % len(bones) )
		self.writeUnsignedInt(file, len(bones) )
		
		if len(bones) > 0:
			
			for b in bones:
								
				self.writeString(file, b.name)
			
				local = Matrix()
				if (parent != None):
					local = parent.inverted() * b.matrix_local
				else:
					print("No Parent")
					local = rot * b.matrix_local	# Rotate root
					
				world = rot * b.matrix_local
					
				self.writeMat44(file, local)
				self.writeMat44(file, world)
				
				# Print to log
				sp = ""
				for i in range(indent):
					sp += " "
				#print("%s%s  (%f, %f, %f)" % (sp,b.name, local.to_translation()[0], local.to_translation()[1], local.to_translation()[2]))
				
				print("%s%s\n%s%f %f %f %f\n%s%f %f %f %f\n%s%f %f %f %f\n%s%f %f %f %f" % (sp, b.name, sp, local[0][0], local[1][0], local[2][0], local[3][0], sp, local[0][1], local[1][1], local[2][1], local[3][1], sp, local[0][2], local[1][2], local[2][2], local[3][2], sp, local[0][3], local[1][3], local[2][3], local[3][3]))

				self.writeSkeletonJoints(file,b.children, b.matrix_local, indent+1)

			

	def writeSkeleton(self, file, objects_list, options):
		

		# count the armatures in our selection
		skeletonCount = 0
		for obj in objects_list:
			if obj.type == 'ARMATURE':
				skeletonCount = skeletonCount + 1
				
		if (skeletonCount > 1):
			print("Error: Cannot export more than one skeleton")
			return
				
		#
		# Skeletons header
		#	
			
		size_location = self.beginExportSection(file, SKELETON)

		for obj in objects_list:
		
			if obj.type == 'ARMATURE':
				arm = obj.data
				
				parentList = [Bone for Bone in arm.bones if Bone.parent is None]
				self.writeSkeletonJoints(file, parentList, None, 0)
				
		
		self.endExportSection(file, size_location)
		
#==============================================================================
#==============================================================================

	def writeScene(self, scene, filepath, objects_list, options):
	
		print("running writeScene to : %s " % filepath)
		file = open(filepath, 'wb')
		
		self.writeUnsignedInt(file,MAGIC)
		self.writeUnsignedInt(file,VERSION)
		
		size_location = self.beginExportSection(file, FILE)
		
		print("********************")
		print("* Writing Mesh     *")
		print("********************")
		
		self.writeMeshes(scene, file, objects_list, options)

		print("********************")
		print("* Writing Skeleton *")
		print("********************")

		self.writeSkeleton(file, objects_list, options)
		
		self.endExportSection(file, size_location)
		
		file.close()
		
		return {'FINISHED'}
	
	
#==============================================================================
#==============================================================================

class TwmExporter(bpy.types.Operator):
	'''Save Raw triangle mesh data'''
	bl_idname = "export_mesh.twm"
	bl_label = "Export TWM"
	
	filepath = StringProperty(name="File Path", description="Filepath used for exporting the TWM file", maxlen= 1024, default= "", subtype='FILE_PATH')
	check_existing = BoolProperty(name="Check Existing", description="Check and warn on overwriting existing files", default=True, options={'HIDDEN'})
	

	apply_modifiers = BoolProperty(name="Apply Modifiers", description="Use transformed mesh data from each object", default=True)
	optimize = BoolProperty(name="Optimize", description="Optimize Mesh", default=False)
	selection = BoolProperty(name="Selection Only", description="Export Selection Only", default=True)
	bindpose = BoolProperty(name="Force Bind Pose", description="Force Mesh To Bind Pose for animation", default=True)
	
	def execute(self, context):

		options = TwmExportOptions()
		options.apply_modifiers = self.apply_modifiers
		options.optimize = self.optimize
		options.selection = self.selection
		options.bindpose = self.bindpose
		
		if (options.selection):
			objects_list = context.selected_objects
		else:
			objects_list = context.scene.objects
		
		impl = TwmExporterImpl()
		impl.writeScene(context.scene, self.filepath, objects_list, options)
		
		return {'FINISHED'}
	
	def invoke(self, context, event):
		wm = context.window_manager
		wm.fileselect_add(self)
		return {'RUNNING_MODAL'}
		
#==============================================================================
#==============================================================================