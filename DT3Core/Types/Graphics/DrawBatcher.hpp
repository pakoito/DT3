#ifndef DT3_DRAWBATCHER
#define DT3_DRAWBATCHER
//==============================================================================
///	
///	File: DrawBatcher.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/Math/Vector2.hpp"
#include "DT3Core/Types/Math/Vector3.hpp"
#include "DT3Core/Types/Math/Matrix4.hpp"
#include "DT3Core/Types/Math/Color4b.hpp"
#include "DT3Core/Types/Math/Color4f.hpp"
#include "DT3Core/Types/Utility/Callback.hpp"
#include "DT3Core/Devices/DeviceGraphics.hpp"
#include <memory>
#include <vector>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class CameraObject;
class MaterialResource;
class ShaderResource;

class DT3GLAttribBufferResource;

class Vector2;
class Vector3;
class Color4f;
class Color4b;

#undef BATCH_V
#undef BATCH_N
#undef BATCH_T0
#undef BATCH_T1
#undef BATCH_C
#undef BATCH_CF

#define BATCH_V     ((Vector3*) _batch_v_raw.get())
#define BATCH_N     ((Vector3*) _batch_n_raw.get())
#define BATCH_T0    ((Vector2*) _batch_t0_raw.get())
#define BATCH_T1    ((Vector2*) _batch_t1_raw.get())
#define BATCH_C     ((Color4b*) _batch_c_raw.get())
#define BATCH_CF    ((Color4f*) _batch_cf_raw.get())

//==============================================================================
/// Class
//==============================================================================

class DrawBatcher {
 	public:
        DEFINE_TYPE_SIMPLE_BASE(DrawBatcher)   

							DrawBatcher			(void);
    
    private:
							DrawBatcher			(const DrawBatcher &rhs);
        DrawBatcher &		operator =			(const DrawBatcher &rhs);
    
    public:
                            ~DrawBatcher		(void);

	public:
		enum BatchFormat {
			FMT_V = 1 << 0,
			FMT_N = 1 << 1,
			FMT_T0 = 1 << 2,
			FMT_T1 = 1 << 3,
			FMT_C = 1 << 4,
			FMT_CF = 1 << 5,
            
            FMT_V_C = FMT_V | FMT_C,
            FMT_V_T0 = FMT_V | FMT_T0
		};
        
        DEFINE_ACCESSORS (flush_callback, set_flush_callback, std::shared_ptr<Callback<DrawBatcher*>>, _flush_callback)

		/// Begin a draw batch
		/// \param material Material to use when drawing
		/// \param transform Transform for drawing
		/// \param type Batch type
		/// \param fmt Batch format
		void			batch_begin			(   const std::shared_ptr<CameraObject> &camera,
                                                const std::shared_ptr<MaterialResource> &material,
                                                const std::shared_ptr<ShaderResource> &shader,
                                                const Matrix4 &transform,
                                                DT3GLPrimitiveType type,
                                                DTushort fmt,
                                                DTsize size_hint = 1024);

		/// Designate a point in drawing where the batch can be split
        void            batch_split         (void);

		/// End the batch
		void			batch_end			(void);
    
    
		/// Vertex definition
        DrawBatcher&    add (void)                                          {   ++_cur_index;   return *this;   }

        DrawBatcher&    v   (const Vector3 &v)                              {   ASSERT(_format & FMT_V);  ASSERT(_cur_index < _size_hint); BATCH_V[_cur_index] = v;                return *this;    }
        DrawBatcher&    v   (DTfloat x, DTfloat y, DTfloat z)               {   ASSERT(_format & FMT_V);  ASSERT(_cur_index < _size_hint); BATCH_V[_cur_index] = Vector3(x,y,z);   return *this;    }

        DrawBatcher&    n   (const Vector3 &n)                              {   ASSERT(_format & FMT_N);  ASSERT(_cur_index < _size_hint); BATCH_V[_cur_index] = n;                return *this;    }
        DrawBatcher&    n   (DTfloat x, DTfloat y, DTfloat z)               {   ASSERT(_format & FMT_N);  ASSERT(_cur_index < _size_hint); BATCH_V[_cur_index] = Vector3(x,y,z);   return *this;    }

        DrawBatcher&    t0  (const Vector2 &t0)                             {   ASSERT(_format & FMT_T0); ASSERT(_cur_index < _size_hint); BATCH_T0[_cur_index] = t0;              return *this;    }
        DrawBatcher&    t0  (DTfloat s, DTfloat t)                          {   ASSERT(_format & FMT_T0); ASSERT(_cur_index < _size_hint); BATCH_T0[_cur_index] = Vector2(s,t);    return *this;    }

        DrawBatcher&    t1  (const Vector2 &t1)                             {   ASSERT(_format & FMT_T1); ASSERT(_cur_index < _size_hint); BATCH_T1[_cur_index] = t1;              return *this;    }
        DrawBatcher&    t1  (DTfloat s, DTfloat t)                          {   ASSERT(_format & FMT_T1); ASSERT(_cur_index < _size_hint); BATCH_T1[_cur_index] = Vector2(s,t);    return *this;    }

        DrawBatcher&    c   (const Color4f &c)                              {   ASSERT(_format & FMT_C);  ASSERT(_cur_index < _size_hint); BATCH_C[_cur_index] = Color4b(c);       return *this;    }
        DrawBatcher&    c   (const Color4b &c)                              {   ASSERT(_format & FMT_C);  ASSERT(_cur_index < _size_hint); BATCH_C[_cur_index] = c;                return *this;    }
        DrawBatcher&    c   (DTfloat r, DTfloat g, DTfloat b, DTfloat a)    {   ASSERT(_format & FMT_C);  ASSERT(_cur_index < _size_hint); BATCH_C[_cur_index] = Color4b(r,g,b,a); return *this;    }

        DrawBatcher&    cf  (const Color4f &c)                              {   ASSERT(_format & FMT_CF); ASSERT(_cur_index < _size_hint); BATCH_CF[_cur_index] = c;               return *this;    }
        DrawBatcher&    cf  (const Color4b &c)                              {   ASSERT(_format & FMT_CF); ASSERT(_cur_index < _size_hint); BATCH_CF[_cur_index] = Color4f(c);      return *this;    }
        DrawBatcher&    cf  (DTfloat r, DTfloat g, DTfloat b, DTfloat a)    {   ASSERT(_format & FMT_CF); ASSERT(_cur_index < _size_hint); BATCH_CF[_cur_index] = Color4f(r,g,b,a);return *this;    }

	
		/// Perform the drawing of the batch
		void			flush				(void);

	private:
        void            screen_opened       (DTuint width, DTuint height);
        void            screen_closed       (void);

        DTsize                                      _size_hint;
    
		std::shared_ptr<CameraObject>               _camera_object;
		std::shared_ptr<MaterialResource>           _material;
        std::shared_ptr<ShaderResource>             _shader;

        Matrix4                                     _transform;
    
		DT3GLPrimitiveType                          _type;
		DTushort                                    _format;
		
		DTshort                                     _cur_index;
    
        std::shared_ptr<Callback<DrawBatcher*>>     _flush_callback;

        // These are byte arrays because constructors were getting called all the time
        std::shared_ptr<DTubyte>                    _batch_v_raw;
        std::shared_ptr<DTubyte>                    _batch_n_raw;
        std::shared_ptr<DTubyte>                    _batch_t0_raw;
        std::shared_ptr<DTubyte>                    _batch_t1_raw;
        std::shared_ptr<DTubyte>                    _batch_c_raw;
        std::shared_ptr<DTubyte>                    _batch_cf_raw;
    
        std::shared_ptr<DT3GLAttribBufferResource>        _batch_v_buffer;
        std::shared_ptr<DT3GLAttribBufferResource>        _batch_n_buffer;
        std::shared_ptr<DT3GLAttribBufferResource>        _batch_t0_buffer;
        std::shared_ptr<DT3GLAttribBufferResource>        _batch_t1_buffer;
        std::shared_ptr<DT3GLAttribBufferResource>        _batch_c_buffer;
        std::shared_ptr<DT3GLAttribBufferResource>        _batch_cf_buffer;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
