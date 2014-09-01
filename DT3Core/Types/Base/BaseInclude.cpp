//==============================================================================
///	
///	File: BaseInclude.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
// Error codes
//==============================================================================

const DTerr DT3_ERR_NONE = 0;
const DTerr DT3_ERR_UNKNOWN = 1;

const DTerr DT3_ERR_FILE_OPEN_FAILED = 1000;
const DTerr DT3_ERR_FILE_READ_FAILED = 1001;
const DTerr DT3_ERR_FILE_WRONG_TYPE = 1002;
const DTerr DT3_ERR_FILE_NOT_IN_PACKAGE = 1003;

const DTerr DT3_ERR_FACTORY_BAD_CLASS = 2000;
const DTerr DT3_ERR_ARCHIVE_TOO_NEW = 2001;

const DTerr DT3_ERR_TOKENIZE_FAILED = 3000;

const DTerr DT3_ERR_NET_WOULD_BLOCK = 4000;
const DTerr DT3_ERR_NET_UNKNOWN = 4001;

const DTerr DT3_ERR_THREAD_FAIL = 5000;

const DTerr	DT3_ERR_NAT_SUCCESS = 8000;
const DTerr	DT3_ERR_NAT_FAIL = 8001;

//==============================================================================
//==============================================================================

} // DT3


