//==============================================================================
///	
///	File: ImageTree.hpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#ifndef IMAGETREE
#define IMAGETREE

//==============================================================================
/// Includes
//==============================================================================

#include "Image.hpp"

//==============================================================================
/// Class
//==============================================================================

class ImageTree {
    public:		
								ImageTree		(void);	
								ImageTree		(const ImageTree &rhs);
        ImageTree &				operator =		(const ImageTree &rhs);	
        virtual					~ImageTree		(void);
    	
	public:
		
		/// Description
		/// \param param description
		/// \return description
		void					setSize         (int width, int height, int pad);
		
		/// Description
		/// \param param description
		/// \return description
		bool					addImage		(Image *img, const std::string &name, const std::string &out_name);

		/// Description
		/// \param param description
		/// \return description
		Image&					result			(void)		{	return _result;		}
		
	private:
		

		struct Rectangle {
			Rectangle (void)	{}
			Rectangle (int min_x, int max_x, int min_y, int max_y)
				:	_min_x	(min_x), _max_x	(max_x), _min_y	(min_y), _max_y	(max_y)	{}

			int _min_x;
			int _max_x;
			int _min_y;
			int _max_y;
		};
		
		struct Node {
			Node(void)	{	_image = NULL;	_nodes[0] = _nodes[1] = NULL;	}
			~Node(void)	{	delete _nodes[0]; delete _nodes[1];				}

			Image		*_image;
			Rectangle	_rect;

			Node		*_nodes[2];
		};

		Image		_result;
		Node		*_root;

		bool					addImageRecursive   (Node *node, Image *img, const std::string &img_name, const std::string &out_name);
};

//==============================================================================
//==============================================================================

#endif
