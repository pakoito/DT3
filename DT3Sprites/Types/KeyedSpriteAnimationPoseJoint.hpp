#ifndef DT3_KEYEDSPRITEANIMATIONPART
#define DT3_KEYEDSPRITEANIMATIONPART
//==============================================================================
///	
///	File: KeyedSpriteAnimationPoseJoint.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "Matrix.hpp"
#include "Array.hpp"
#include "BaseClass.hpp"
#include "PoolAllocator.hpp"
#include "MaterialResource.hpp"
#include "KeyedSpriteAnimationKeyframe.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

class Archive;
class DrawBatcherQuads;

//==============================================================================
//==============================================================================

class KeyedSpriteAnimationPoseJoint: public BaseClass {
    public:
        DEFINE_TYPE(KeyedSpriteAnimationPoseJoint, BaseClass)   
		DEFINE_CREATE_AND_CLONE
		DEFINE_POOL_ALLOCATOR

         
												KeyedSpriteAnimationPoseJoint	(void);	
												KeyedSpriteAnimationPoseJoint	(const KeyedSpriteAnimationPoseJoint &rhs);
        KeyedSpriteAnimationPoseJoint &         operator =                      (const KeyedSpriteAnimationPoseJoint &rhs);		
		virtual									~KeyedSpriteAnimationPoseJoint	(void);
    
        void									archiveRead                 (Archive *archive);
        void									archiveWrite				(Archive *archive);
		
	public:

		DEFINE_ACCESSORS_REFERENCED(getJointToObjectTransform, setJointToObjectTransform, Matrix3, _joint_to_object_transform);

		/// Sets the name of the bone
		/// \param param description
		/// \return description
        void                                    setName                     (const String &name);

		/// Returns the name of the bone
		/// \param param description
		/// \return description
        const String                            getName                     (void) const                                {   return _name;						}
		
		/// Returns the name hash of the object
		/// \param param description
		/// \return description
        DTuint                                  getNameHash                 (void) const								{   return _name_hash;					}

		/// Description
		/// \param param description
		/// \return description
        void                                    setWithKey                  (const KeyedSpriteAnimationKeyframe& key);
        
		/// Description
		/// \param param description
		/// \return description
        KeyedSpriteAnimationKeyframe            getAsKey                    (void) const;
        


		DEFINE_ACCESSORS_REFERENCED(getTranslation, setTranslation, Vector2, _translation);
		DEFINE_ACCESSORS(getRotation, setRotation, DTfloat, _rotation);

		/// Description
		/// \param param description
		/// \return description
		DTfloat									getRotationDegrees			(void) const;

		/// Description
		/// \param param description
		/// \return description
		void									setRotationDegrees			(const DTfloat rotation);

		DEFINE_ACCESSORS(getVisible, setVisible, DTboolean, _visible);
		DEFINE_ACCESSORS(getScale, setScale, DTfloat, _scale);
		
		DEFINE_ACCESSORS(getPlusX, setPlusX, DTfloat, _plus_x);
		DEFINE_ACCESSORS(getMinusX, setMinusX, DTfloat, _minus_x);
		DEFINE_ACCESSORS(getPlusY, setPlusY, DTfloat, _plus_y);
		DEFINE_ACCESSORS(getMinusY, setMinusY, DTfloat, _minus_y);

		DEFINE_ACCESSORS(getPlusU, setPlusU, DTfloat, _plus_u);
		DEFINE_ACCESSORS(getMinusU, setMinusU, DTfloat, _minus_u);
		DEFINE_ACCESSORS(getPlusV, setPlusV, DTfloat, _plus_v);
		DEFINE_ACCESSORS(getMinusV, setMinusV, DTfloat, _minus_v);

		DEFINE_ACCESSORS(getOrder, setOrder, DTfloat, _order);
        DEFINE_ACCESSORS(getGroups, setGroups, DTushort, _groups);

		/// Description
		/// \param param description
		/// \return description
        void                            setGridSizeX                (DTint size_x);
        
		/// Description
		/// \param param description
		/// \return description
        DTint                           getGridSizeX                (void) const;

		/// Description
		/// \param param description
		/// \return description
        void                            setGridSizeY                (DTint size_y);
        
		/// Description
		/// \param param description
		/// \return description
        DTint                           getGridSizeY                (void) const;
        
		/// Description
		/// \param param description
		/// \return description
        Array<Vector2>&                 gridPoints                  (void)       {    return _grid; }
        const Array<Vector2>&           gridPoints                  (void) const {    return _grid; }

		/// Description
		/// \param param description
		/// \return description
        void                            resetGrid                   (void);

        /// Description
		/// \param param description
		/// \return description
        void                            draw                        (DrawBatcherQuads &b);

	private:
        void                            allocateGrid                (void);

		friend class KeyedSpriteResource;
        friend class KeyedSpriteAnimationTrack;

		String                                  _name;
        DTuint                                  _name_hash;
        
		Matrix3                                 _joint_to_object_transform;
		
		KeyedSpriteAnimationPoseJoint*			_parent;
		Array<KeyedSpriteAnimationPoseJoint*>	_children;
        
		Vector2                                 _translation;
		DTfloat                                 _rotation;
		DTfloat                                 _scale;
		
		DTfloat                                 _plus_x;
		DTfloat                                 _minus_x;
		DTfloat                                 _plus_y;
		DTfloat                                 _minus_y;

		DTfloat                                 _plus_u;
		DTfloat                                 _minus_u;
		
		DTfloat                                 _plus_v;
		DTfloat                                 _minus_v;

		DTboolean                               _visible;
        DTfloat                                 _order;

        DTint                                   _grid_size_x;
        DTint                                   _grid_size_y;
    
        DTushort                                _groups;
        
        Array<Vector2>                          _grid;

};

//==============================================================================
//==============================================================================

} // DT3

#endif

