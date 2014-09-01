#ifndef DT3_SESSION
#define DT3_SESSION
//==============================================================================
///	
///	File: Session.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseClass.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Class
//==============================================================================

class Session: public BaseClass {
    public:
        DEFINE_TYPE(Session,BaseClass)
		DEFINE_CREATE_AND_CLONE

							Session					(void);	
							Session					(const Session &rhs);
        Session &			operator =				(const Session &rhs);	
        virtual				~Session				(void);
                
        virtual void		archive                 (const std::shared_ptr<Archive> &archive);

};

//==============================================================================
//==============================================================================

} // DT3

#endif
