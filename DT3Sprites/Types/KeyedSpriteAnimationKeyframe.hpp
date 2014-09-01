#ifndef DT3_KEYEDSPRITEANIMATIONKEYFRAME
#define DT3_KEYEDSPRITEANIMATIONKEYFRAME
//==============================================================================
///	
///	File: KeyedSpriteAnimationKeyframe.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "Matrix.hpp"
#include "MaterialResource.hpp"
#include "Array.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

class Archive;

//==============================================================================
//==============================================================================

class KeyedSpriteAnimationKeyframe {
    public:
        DEFINE_TYPE_SIMPLE_BASE(KeyedSpriteAnimationKeyframe)   
	
         
										KeyedSpriteAnimationKeyframe	(void);	
										KeyedSpriteAnimationKeyframe	(const KeyedSpriteAnimationKeyframe &rhs);
        KeyedSpriteAnimationKeyframe &	operator =						(const KeyedSpriteAnimationKeyframe &rhs);		
										~KeyedSpriteAnimationKeyframe	(void);
    
        void							archiveRead                     (Archive *archive);
        void							archiveWrite					(Archive *archive);
		
	public:
		/// Sets index of the keyframe
		/// \param keyframe_cache index
		void                            setKeyframeCache            (DTuint keyframe_cache)		{	_keyframe_cache = keyframe_cache;	}
		
		/// Returns index of the key frame
		/// \return keyframe index
		DTuint                          getKeyframeCache            (void) const				{	return _keyframe_cache;				}
        
		/// Returns a unique ID for this key
		/// \param k key index
		/// \return ID
        virtual DTint                   getKeyID                    (void) const                {   return _id;                         }


		DEFINE_ACCESSORS(getTime, setTime, DTfloat, _time);
	
		DEFINE_ACCESSORS_REFERENCED(getTranslation, setTranslation, Vector2, _translation);
		DEFINE_ACCESSORS(getRotation, setRotation, DTfloat, _rotation);

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
        void                            flipHorz                    (void);

		/// Description
		/// \param param description
		/// \return description
        void                            flipVert                    (void);

	private:
        void                            allocateGrid                (void);
    
		friend class KeyedSpriteAnimationTrack;
		friend class KeyedSpriteAnimationPoseJoint;

        DTint                               _id;
        DTuint                              _keyframe_cache;

		Vector2								_translation;
		DTfloat								_rotation;
		DTfloat								_scale;
		
		DTfloat								_plus_x;
		DTfloat								_minus_x;
		DTfloat								_plus_y;
		DTfloat								_minus_y;

		DTfloat								_plus_u;
		DTfloat								_minus_u;
		
		DTfloat								_plus_v;
		DTfloat								_minus_v;

		DTboolean                           _visible;
        DTfloat                             _order;
        
        DTint                               _grid_size_x;
        DTint                               _grid_size_y;
        Array<Vector2>                      _grid;
	
		DTfloat								_time;
};

//==============================================================================
//==============================================================================

inline DTboolean operator < (const KeyedSpriteAnimationKeyframe &a, const KeyedSpriteAnimationKeyframe &b)
{
    return a.getTime() < b.getTime();
}

inline DTboolean operator > (const KeyedSpriteAnimationKeyframe &a, const KeyedSpriteAnimationKeyframe &b)
{
    return a.getTime() > b.getTime();
}

//==============================================================================
//==============================================================================


} // DT3

#endif

