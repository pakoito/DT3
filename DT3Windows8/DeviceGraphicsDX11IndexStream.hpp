#ifndef DT2_DEVICEGRAPHICSDX11INDEXSTREAM
#define DT2_DEVICEGRAPHICSDX11INDEXSTREAM
//==============================================================================
///	
///	File: DeviceGraphicsDX11IndexStream.hpp
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

class DeviceGraphicsDX11IndexStream: public BaseClass {
    public:
        DEFINE_TYPE(DeviceGraphicsDX11IndexStream,BaseClass)
		DEFINE_CREATE
         
                                         DeviceGraphicsDX11IndexStream	(void);	
	private:
                                         DeviceGraphicsDX11IndexStream	(const DeviceGraphicsDX11IndexStream &rhs);
        DeviceGraphicsDX11IndexStream&   operator =			(const DeviceGraphicsDX11IndexStream &rhs);
	public:
        virtual                          ~DeviceGraphicsDX11IndexStream	(void);

        virtual void				archiveRead	(Archive *archive);
        virtual void				archiveWrite	(Archive *archive);
	        
	public:
		/// Description
		/// \param param description
		/// \return description
		void *						allocateData		(DTuint size, DTboolean dynamic);

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
		void*						map					(DTboolean read, DTboolean write);

		/// Description
		/// \param param description
		/// \return description
		void						unmap				(void);
		
	private:
		friend class DeviceGraphicsDX11Geometry;
		void*						getData			(void);
        ID3D11Buffer*               getBuffer       (void)  {    return _buffer;    }

	private:
		DTuint						_size;
		DTbyte						*_data;
		
		ID3D11Buffer				*_buffer;
		DTboolean					_dynamic;
};

//==============================================================================
//==============================================================================

} // DT2

#endif
