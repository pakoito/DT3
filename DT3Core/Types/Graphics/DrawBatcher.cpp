//==============================================================================
///	
///	File: DrawBatcher.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Graphics/DrawBatcher.hpp"
#include "DT3Core/Types/Graphics/DrawUtils.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/Resources/ResourceTypes/MaterialResource.hpp"
#include "DT3Core/Resources/ResourceTypes/ShaderResource.hpp"
#include "DT3Core/System/System.hpp"
#include "DT3Core/System/SystemCallbacks.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

DrawBatcher::DrawBatcher (void)
	:	_type           (DT3GL_PRIM_TRI_STRIP),
		_format         (FMT_V),
		_cur_index		(-1),
        _flush_callback (NULL)
{
    SystemCallbacks::screen_opened_cb().add(make_callback(this, &type::screen_opened));
    SystemCallbacks::screen_closed_cb().add(make_callback(this, &type::screen_closed));
}

DrawBatcher::~DrawBatcher (void)
{
	flush();

    SystemCallbacks::screen_opened_cb().remove(make_callback(this, &type::screen_opened));
    SystemCallbacks::screen_closed_cb().remove(make_callback(this, &type::screen_closed));
}

//==============================================================================
//==============================================================================

void DrawBatcher::screen_opened (DTuint width, DTuint height)
{
    LOG_MESSAGE << "DrawBatcher::screen_opened";
}

void DrawBatcher::screen_closed (void)
{
    LOG_MESSAGE << "DrawBatcher::screen_closed";

    _batch_v_buffer.reset();
    _batch_n_buffer.reset();
    _batch_t0_buffer.reset();
    _batch_t1_buffer.reset();
    _batch_c_buffer.reset();
    _batch_cf_buffer.reset();
}

//==============================================================================
//==============================================================================

void	 DrawBatcher::batch_begin ( const std::shared_ptr<CameraObject> &camera,
                                    const std::shared_ptr<MaterialResource> &material,
                                    const std::shared_ptr<ShaderResource> &shader,
                                    const Matrix4 &transform,
                                    DT3GLPrimitiveType type,
                                    DTushort fmt,
                                    DTsize size_hint)
{
	// flush for materials and format changes
	if (_material != material || _format != fmt || _transform != transform)
		flush();

    _camera_object = camera;
	_material = material;
    _shader = shader;
    _transform = transform;
	_format = fmt;
	_type = type;
    
    _size_hint = size_hint;

    if (fmt & FMT_V) {
        if (!_batch_v_raw)
            _batch_v_raw = std::shared_ptr<DTubyte>(new DTubyte[_size_hint * sizeof(Vector3)]);
    } else
        _batch_v_raw.reset();
    
    if (fmt & FMT_N) {
        if (!_batch_n_raw)
            _batch_n_raw = std::shared_ptr<DTubyte>(new DTubyte[_size_hint * sizeof(Vector3)]);
    } else
        _batch_n_raw.reset();

    if (fmt & FMT_T0) {
        if (!_batch_t0_raw)
            _batch_t0_raw = std::shared_ptr<DTubyte>(new DTubyte[_size_hint * sizeof(Vector2)]);
    } else
        _batch_t0_raw.reset();

    if (fmt & FMT_T1) {
        if (!_batch_t1_raw)
            _batch_t1_raw = std::shared_ptr<DTubyte>(new DTubyte[_size_hint * sizeof(Vector2)]);
    } else
        _batch_t1_raw.reset();

    if (fmt & FMT_C) {
        if (!_batch_c_raw)
            _batch_c_raw = std::shared_ptr<DTubyte>(new DTubyte[_size_hint * sizeof(Color4b)]);
    } else
        _batch_c_raw.reset();

    if (fmt & FMT_CF) {
        if (!_batch_cf_raw)
            _batch_cf_raw = std::shared_ptr<DTubyte>(new DTubyte[_size_hint * sizeof(Color4f)]);
    } else
        _batch_cf_raw.reset();
}

void	 DrawBatcher::batch_end (void)
{
	// Does nothing yet
}

void	 DrawBatcher::batch_split (void)
{
	if (_cur_index > _size_hint - 3) {
		flush();
    }

}

//==============================================================================
//==============================================================================

void	 DrawBatcher::flush (void)
{
	if (System::renderer() && _cur_index > 0) {
    
        if (_flush_callback)
            (*_flush_callback)(this);
        
        if (!_material)
            return;
    
        if (!_shader)
            return;
        
        DTint a_position = _shader->attrib_slot(DT3GL_ATTRIB_POSITION);
        DTint a_normal = _shader->attrib_slot(DT3GL_ATTRIB_NORMAL);
        DTint a_texcoord0 = _shader->attrib_slot(DT3GL_ATTRIB_TEXCOORD0);
        DTint a_texcoord1 = _shader->attrib_slot(DT3GL_ATTRIB_TEXCOORD1);
        DTint a_color = _shader->attrib_slot(DT3GL_ATTRIB_COLOR);
    
        if (_batch_v_raw && a_position >= 0) {
            if (!_batch_v_buffer)
                _batch_v_buffer = System::renderer()->create_buffer(_batch_v_raw.get(), _size_hint * sizeof(Vector3), DT3GL_BUFFER_FORMAT_3_FLOAT, DT3GL_ACCESS_CPU_WRITE);
            else
                System::renderer()->update_buffer(_batch_v_buffer,_batch_v_raw.get(), (_cur_index+1) * sizeof(Vector3), 0);
        
            _shader->attach_attribute_buffer(a_position, _batch_v_buffer);
        }

        if (_batch_n_raw && a_normal >= 0) {
            if (!_batch_n_buffer)
                _batch_n_buffer = System::renderer()->create_buffer(_batch_n_raw.get(), _size_hint * sizeof(Vector3), DT3GL_BUFFER_FORMAT_3_FLOAT, DT3GL_ACCESS_CPU_WRITE);
            else
                System::renderer()->update_buffer(_batch_n_buffer,_batch_n_raw.get(), (_cur_index+1) * sizeof(Vector3), 0);
        
            _shader->attach_attribute_buffer(a_normal, _batch_n_buffer);
        }

        if (_batch_t0_raw && a_texcoord0 >= 0) {
            if (!_batch_t0_buffer)
                _batch_t0_buffer = System::renderer()->create_buffer(_batch_t0_raw.get(), _size_hint * sizeof(Vector2), DT3GL_BUFFER_FORMAT_2_FLOAT, DT3GL_ACCESS_CPU_WRITE);
            else
                System::renderer()->update_buffer(_batch_t0_buffer,_batch_t0_raw.get(), (_cur_index+1) * sizeof(Vector2), 0);
        
            _shader->attach_attribute_buffer(a_texcoord0, _batch_t0_buffer);
        }

        if (_batch_t1_raw && a_texcoord1 >= 0) {
            if (!_batch_t1_buffer)
                _batch_t1_buffer = System::renderer()->create_buffer(_batch_t1_raw.get(), _size_hint * sizeof(Vector2), DT3GL_BUFFER_FORMAT_2_FLOAT, DT3GL_ACCESS_CPU_WRITE);
            else
                System::renderer()->update_buffer(_batch_t1_buffer,_batch_t1_raw.get(), (_cur_index+1) * sizeof(Vector2), 0);
        
            _shader->attach_attribute_buffer(a_texcoord1, _batch_t1_buffer);
        }

        if (_batch_c_raw && a_color >= 0) {
            if (!_batch_c_buffer)
                _batch_c_buffer = System::renderer()->create_buffer(_batch_c_raw.get(), _size_hint * sizeof(Color4b), DT3GL_BUFFER_FORMAT_4_UBYTE, DT3GL_ACCESS_CPU_WRITE);
            else
                System::renderer()->update_buffer(_batch_c_buffer,_batch_c_raw.get(), (_cur_index+1) * sizeof(Color4b), 0);
        
            _shader->attach_attribute_buffer(a_color, _batch_c_buffer);
        }

        if (_batch_cf_raw && a_color >= 0) {
            if (!_batch_cf_buffer)
                _batch_cf_buffer = System::renderer()->create_buffer(_batch_cf_raw.get(), _size_hint * sizeof(Color4f), DT3GL_BUFFER_FORMAT_4_FLOAT, DT3GL_ACCESS_CPU_WRITE);
            else
                System::renderer()->update_buffer(_batch_cf_buffer,_batch_cf_raw.get(), (_cur_index+1) * sizeof(Color4f), 0);
        
            _shader->attach_attribute_buffer(a_color, _batch_cf_buffer);
        }
        
        // Setup render state
        DrawUtils::activate_state(_camera_object, _material, _shader, _transform);
        
        // Draw!!
        System::renderer()->draw_arrays(_type, _cur_index+1);
					
		_cur_index = -1;
	}
}


//==============================================================================
//==============================================================================

} // DT3
