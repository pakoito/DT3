//==============================================================================
///	
///	File: Image.hpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#ifndef IMAGE
#define IMAGE

//==============================================================================
/// Includes
//==============================================================================

#include <string>
#include "png.h"

//==============================================================================
/// Class
//==============================================================================

class Image {
    public:		
								Image			(void);	
								Image			(const Image &rhs);
        Image &					operator =		(const Image &rhs);	
        virtual					~Image			(void);
    	
	public:

		struct Pixel {
			Pixel(void) { r = g = b = a = 0; }
			unsigned char r,g,b,a;
		};
		
		/// Description
		/// \param param description
		/// \return description
		bool					load		(const std::string &s);

		/// Description
		/// \param param description
		/// \return description
		bool					save		(const std::string &s);

		/// Description
		/// \param param description
		/// \return description
		void					allocate	(int width, int height);
			
		/// Description
		/// \param param description
		/// \return description
		void					trim		(void);

		/// Description
		/// \param param description
		/// \return description
		void					pad			(int p);

		/// Description
		/// \param param description
		/// \return description
		void					blackOutTransparency		(void);

		/// Description
		/// \param param description
		/// \return description
		Pixel&					getPixel	(int x, int y) const;

		/// Description
		/// \param param description
		/// \return description
		void					setPixel	(int x, int y, Pixel &p);

		/// Description
		/// \param param description
		/// \return description
		int						getWidth	(void) const		{	return _window_width;	}

		/// Description
		/// \param param description
		/// \return description
		int						getHeight	(void) const		{	return _window_height;	}


		/// Description
		/// \param param description
		/// \return description
		int						getOrigX	(void) const		{	return _orig_x;	}

		/// Description
		/// \param param description
		/// \return description
		int						getOrigY	(void) const		{	return _orig_y;	}

		/// Description
		/// \param param description
		/// \return description
		int						getOrigWidth	(void) const	{	return _orig_width;	}

		/// Description
		/// \param param description
		/// \return description
		int						getOrigHeight	(void) const	{	return _orig_height;	}


		/// Description
		/// \param param description
		/// \return description
		void					copy		(Image *img, int xoff, int yoff);

	private:
		int				_data_width;
		int				_data_height;

		int				_window_x;
		int				_window_y;
		int				_window_width;
		int				_window_height;
		
		int				_orig_x;
		int				_orig_y;
		int				_orig_width;
		int				_orig_height;
		
		Pixel*			_data;
};

//==============================================================================
//==============================================================================

#endif
