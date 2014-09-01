#ifndef DT3_PLACEABLEOBJECT
#define DT3_PLACEABLEOBJECT
//==============================================================================
///	
///	File: PlaceableObject.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Objects/ObjectBase.hpp"
#include "DT3Core/Types/Math/Sphere.hpp"
#include "DT3Core/Types/Math/Vector3.hpp"
#include "DT3Core/Types/Math/Matrix4.hpp"

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
/// Base object for the different placeable types of objects in the engine.
//==============================================================================

class PlaceableObject: public ObjectBase {
    public:
        DEFINE_TYPE(PlaceableObject,ObjectBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
		
                                    PlaceableObject			(void);	
                                    PlaceableObject			(const PlaceableObject &rhs);
        PlaceableObject &           operator =				(const PlaceableObject &rhs);	
        virtual                     ~PlaceableObject		(void);
        
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
  
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);

		/// Called to initialize the object
		virtual void				uninitialize            (void);
		
	
		/// Returns whether there is a parent
		/// \return has parent object
		inline DTboolean			has_parent				(void) const	{	return _parent_transform.has_incoming_connection();		}

		/// Returns the parent object to this object
		/// \return parent object
		PlaceableObject*            parent                  (void) const;

		/// Returns the top-most parent object to this object
		/// \return parent object
		PlaceableObject*            top_parent              (void) const;

		
		/// Adds a child object to this object
		/// \param child child object to add
		void						add_child				(PlaceableObject *child);
		void						add_child				(const std::shared_ptr<PlaceableObject> &child);

		/// Removes a child object from this object
		/// \param child child object to remove
		void						unparent_child          (PlaceableObject *child);
		void						unparent_child          (const std::shared_ptr<PlaceableObject> &child);

		/// Unparents all children objects from this object
		/// \param child child object to unparent
		void						unparent_children       (void);

		/// Removes a child object from this object
		/// \param child child object to remove
		void						remove_child            (PlaceableObject *child);
		void						remove_child            (const std::shared_ptr<PlaceableObject> &child);

		/// Removes all children objects from this object
		/// \param child child object to remove
		void						remove_children         (void);
		
		/// Returns list of children of this object
		/// \return children list
		std::list<PlaceableObject*> children                (void) const;
		        

		/// Returns the velocity of the object.
		/// \return velocity
        const Vector3 &				velocity                (void) const                        {   return _velocity.as_ref();              }

		/// Sets the velocity of the object.
		/// \param velocity velocity
        void						set_velocity            (const Vector3 &velocity)           {   _velocity = velocity;                   }


		/// Returns the angular velocity of the object.
		/// \return angular velocity
        const Vector3&				angular_velocity        (void) const                        {   return _angular_velocity.as_ref();      }

		/// Sets the angular velocity of the object.
		/// \param angular_velocity angular velocity
        void						set_angular_velocity    (const Vector3 &angular_velocity)   {   _angular_velocity = angular_velocity;   }
        

		/// Gets the radius of the objects bounding sphere
		/// \return radius of object
        DTfloat						radius                  (void) const                        {   return _radius;                         }

		/// Sets the radius of the objects bounding sphere
		/// \param radius radius of object
        void						set_radius              (DTfloat radius)                    {   ASSERT(radius >= 0.0F);	_radius = radius;   }


		/// Returns the bounding sphere of the object
		/// \return bounding sphere
        Sphere                      bounding_sphere         (void) const;


		// Transform routines
		
		/// Returns the scale of the object.
		/// \return scale of object
		Vector3						scale                   (void) const;

		/// Sets the scale of the object.
		/// \param scale scale of object
        void						set_scale				(const Vector3 &scale);


		/// Returns the local scale of the object.
		/// \return local scale
        inline Vector3              scale_local             (void) const;

		/// Sets the local scale of the object.
		/// \param scale scale of object
        inline void					set_scale_local			(const Vector3 &scale);


	
		/// Returns the translation of the object.
		/// \return translation
        const Vector3 &				translation             (void) const;

		/// Sets the translation of the object.
		/// \param translation translation
        void						set_translation         (const Vector3 &translation);

		/// Returns the local translation of the object.
		/// \return local translation
        inline const Vector3 &		translation_local		(void) const                    {	return _translation.as_ref();	}

		/// Sets the local translation of the object.
		/// \param translation local translation
        inline void					set_translation_local   (const Vector3 &translation)	{   _translation = translation;		}


		/// Returns the orientation of the object as a 3x3 matrix
		/// \return orientation
        const Matrix3 &				orientation             (void) const;


		/// Sets the orientation of the object with a 3x3 matrix
		/// \param orientation orientation
        void						set_orientation         (const Matrix3 &orientation);

		/// Returns the local orientation of the object as a 3x3 matrix
		/// \return local orientation
        inline const Matrix3 &		orientation_local       (void) const				{	return _orientation.as_ref();	}

		/// Sets the local orientation of the object with a 3x3 matrix
		/// \param orientation orientation
        inline void                 set_orientation_local   (const Matrix3 &orientation){	_orientation = orientation;     }


		/// Returns the transform for the object
		/// \return object transform
        const Matrix4				transform               (void) const                {   return _world_transform;            }
		
		/// Returns the inverse transform for the object
		/// \return object inverse transform
        const Matrix4				transform_inversed      (void) const                {   return _world_transform_inversed;   }
		
		/// set transform for the object
		/// \param transform transform
        void						set_transform			(const Matrix4 &transform);


		/// Returns the local transform for the object
		/// \return object local transform
        const Matrix4				transform_local         (void) const                {   return _transform;                  }
	
		/// Returns the local inverse transform for the object
		/// \return local inverse transform
        const Matrix4				transform_local_inversed(void) const                {   return _transform_inversed;         }
	
		/// Set local transform for the object
		/// \param transform transform
        void						set_transform_local		(const Matrix4 &transform);



		/// Convert a global point to a local one
		/// \param rg global point
		/// \return local point
		Vector3						global_to_local_point	(const Vector3 &rg) const;

		/// Convert a local point to a global one
		/// \param rl local point
		/// \return global point
		Vector3						local_to_global_point	(const Vector3 &rl) const;


		/// Convert a local velocity to a global one
		/// \param rl local velocity
		/// \return global velocity
		Vector3						local_to_global_velocity(const Vector3 &rl) const;


	
		// Orientation helpers

		/// Rotates the object around its X axis
		/// \param angle angle
        void						rotate_around_X         (const DTfloat angle);

		/// Rotates the object around its Y axis
		/// \param angle angle
        void						rotate_around_Y         (const DTfloat angle);

		/// Rotates the object around its Z axis
		/// \param angle angle
        void						rotate_around_Z         (const DTfloat angle);


		/// Return forward vector of object
		/// \return forward vector (-z)
		Vector3						forwards                (void) const;

		/// Return backwards vector of object
		/// \return backwards vector (z)
		Vector3						backwards               (void) const;

		/// Return left vector of object
		/// \return left vector (-x)
		Vector3						left_axis               (void) const;

		/// Return right vector of object
		/// \return right vector (x)
		Vector3						right_axis              (void) const;

		/// Return up vector of object
		/// \return up vector (y)
		Vector3						up_axis                 (void) const;
		
		/// Return down vector of object
		/// \return down vector (-y)
		Vector3						down_axis               (void) const;
		
		

		/// Sets the orientation of this object relative to another object around X
		/// \param obj1 other object
		/// \param angle angle
		void						set_orientation_relative_angle_X	(PlaceableObject *obj1, DTfloat angle);

		/// Sets the orientation of this object relative to the character object around Y
		/// \param obj1 other object
		/// \param angle angle
		void						set_orientation_relative_angle_Y	(PlaceableObject *obj1, DTfloat angle);

		/// Sets the orientation of this object relative to the character object around Z
		/// \param obj1 other object
		/// \param angle angle
		void						set_orientation_relative_angle_Z	(PlaceableObject *obj1, DTfloat angle);
		
		/// Sets the relative translation (in local space)
		/// \param obj1 other object
		/// \param pos pos
		void						set_translation_relative            (PlaceableObject *obj1, Vector3 pos);
		
		
		
        // Enable and disable picking in the editor for this object
		/// \param pickable can pick
		void						set_pickable            (DTboolean pickable)		{   _pickable = pickable;					}
		
		/// Is the object pickable in the editor
		/// \return pickable
		DTboolean					is_pickable             (void) const				{   return _pickable;						}

		/// Computes the value of the node
		/// \param plug plug to compute
		DTboolean					compute                 (const PlugBase *plug);

	private:
		DTboolean					_pickable;

        Plug<DTfloat>				_radius;
        Plug<Vector3>				_velocity;
        Plug<Vector3>				_angular_velocity;
		
		// Current transform
		Plug<Vector3>               _translation;
		Plug<Matrix3>				_orientation;
        Plug<Vector3>               _scale;

		Plug<Matrix4>				_transform;
		Plug<Matrix4>				_transform_inversed;

		// Calculated World transform
		Plug<Vector3>               _world_translation;
		Plug<Matrix3>				_world_orientation;
		Plug<Vector3>				_world_scale;

		Plug<Matrix4>				_world_transform;
		Plug<Matrix4>				_world_transform_inversed;

		// parents transform
		Plug<Matrix4>               _parent_transform;

};

//==============================================================================
//==============================================================================

} // DT3

#endif
