#ifndef DT2_DEVICEGRAPHICSDX11DATASTREAM
#define DT2_DEVICEGRAPHICSDX11DATASTREAM
//==============================================================================
///	
///	File: DeviceGraphicsDX11DataStream.hpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "pch.h"

#include "WinUtils.hpp"
#include "BaseClass.hpp"

//==============================================================================
//==============================================================================

namespace DT2 {

//==============================================================================
//==============================================================================

class DeviceGraphicsDX11DataStream: public BaseClass {
    public:
        DEFINE_TYPE(DeviceGraphicsDX11DataStream,BaseClass)
		DEFINE_CREATE
         
                                    DeviceGraphicsDX11DataStream	(void);	
	private:
                                    DeviceGraphicsDX11DataStream	(const DeviceGraphicsDX11DataStream &rhs);
        DeviceGraphicsDX11DataStream &		operator =			(const DeviceGraphicsDX11DataStream &rhs);
	public:
        virtual                     ~DeviceGraphicsDX11DataStream	(void);

        virtual void				archiveRead		(Archive *archive);
        virtual void				archiveWrite		(Archive *archive);
	        
	public:
		enum {
			FMT_2_FLOAT = 0,
			FMT_3_FLOAT = 1,
			FMT_4_FLOAT = 2,
			FMT_4_BYTE = 3,
			FMT_4_USHORT = 4
		};
		
		/// Description
		/// \param param description
		/// \return description
		void *						allocateData		(DTuint size, DTuint format, DTboolean dynamic);
	
		/// Description
		/// \param param description
		/// \return description
		void						uploadData			(void);

		/// Description
		/// \param param description
		/// \return description
		void						clearData			(void);
	

		/// Description
		/// \param param description
		/// \return description
		DTuint						getFormat			(void)	{	return _format;		}
		
		/// Description
		/// \param param description
		/// \return description
		void*						map					(DTboolean read, DTboolean write);

		/// Description
		/// \param param description
		/// \return description
		void						unmap				(void);
		
	private:

		friend class DeviceGraphicsDX11Geometry;
		friend class DeviceGraphicsDX11Particles;
		void*						getData			(void);
        ID3D11Buffer*               getBuffer       (void)  {    return _buffer;    }
      
	private:
		DTuint						_size;
		DTbyte						*_data;
		
		ID3D11Buffer			    *_buffer;
		DTboolean					_dynamic;
		DTuint						_format;
};

//==============================================================================
//==============================================================================

} // DT2

#endif
