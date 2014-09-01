#ifndef DT3_PNGWRITER
#define DT3_PNGWRITER
//==============================================================================
///	
///	File: PNGWriter.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include <memory>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class FilePath;
class TextureResource2D;

//==============================================================================
/// Class
//==============================================================================

class PNGWriter {
    private:
								PNGWriter			(void);	
								PNGWriter			(const PNGWriter &rhs);
        PNGWriter &             operator =			(const PNGWriter &rhs);
								~PNGWriter			(void);

	public:
		/// Write an image to a png file
		/// \param pathname Path of file to write
		/// \param tex Texture
        /// \return error
        static DTerr            write               (const FilePath &pathname, const std::shared_ptr<TextureResource2D> &tex);


};

//==============================================================================
//==============================================================================

} // DT3

#endif
