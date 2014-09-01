//==============================================================================
///	
///	File: ImageTree.cpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include	"ImageTree.hpp"
#include	<fstream>
#include	<iostream>

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ImageTree::ImageTree (void)
	:	_root				(NULL)
{  

}
		
ImageTree::ImageTree (const ImageTree &rhs)
    :	_root				(rhs._root)
{   

}

ImageTree & ImageTree::operator = (const ImageTree &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {
		_root = rhs._root;
	}
    return (*this);
}
			
ImageTree::~ImageTree (void)
{
	delete _root;
}

//==============================================================================
//==============================================================================

void ImageTree::setSize (int width, int height, int pad)
{
	delete _root;

	// Build a new full node
	_root = new Node();

	_root->_rect._min_x = 0-pad;
	_root->_rect._max_x = width+2*pad;
	_root->_rect._min_y = 0-pad;
	_root->_rect._max_y = height+2*pad;
	
	_result.allocate(width,height);
}

bool ImageTree::addImageRecursive (Node *node, Image *img, const std::string &name, const std::string &out_name)
{
	// if we reached the end of the tree, exit
	if (node == NULL) {
		return false;

	// If we are not in a leaf then descend further
	} else if (node->_nodes[0] != NULL || node->_nodes[1] != NULL) {
		if (addImageRecursive (node->_nodes[0], img, name, out_name))	return true;
		if (addImageRecursive (node->_nodes[1], img, name, out_name))	return true;
		return false;

	// We are in a leaf
	} else {
		// Check if node is already an image
		if (node->_image)
			return false;
	
		int node_width = node->_rect._max_x - node->_rect._min_x;
		int node_height = node->_rect._max_y - node->_rect._min_y;

		// Check for image being too large
		if (img->getHeight() > node_height || img->getWidth() > node_width)
			return false;

		// Check for exact size
		if (img->getHeight() == node_height && img->getWidth() == node_width) {
			node->_image = img;
			_result.copy(img, node->_rect._min_x, node->_rect._min_y);
			
			// Report what was done
			std::cout	<< name << "|"
						<< out_name << "|"
						<< "  SubRect=" << "|"
						<< node->_rect._min_x << "|"
						<< node->_rect._max_x << "|"
						<< node->_rect._min_y << "|"
						<< node->_rect._max_y << "|"
						<< "  OrigRect=" << "|"
						<< node->_rect._min_x + img->getOrigX() << "|"
						<< node->_rect._min_x + img->getOrigX() + img->getOrigWidth() << "|"
						<< node->_rect._min_y + img->getOrigY() << "|"
						<< node->_rect._min_y + img->getOrigY() + img->getOrigHeight() << std::endl;
			
			return true;
		}

		// Split the node up perpendicular to the axis of greater difference
		int dw = node_width - img->getWidth();
		int dh = node_height - img->getHeight();


		if (dw > dh) {
			// Split left/right
			node->_nodes[0] = new Node();
			node->_nodes[0]->_rect = Rectangle(	node->_rect._min_x, 
												node->_rect._min_x + img->getWidth(),
												node->_rect._min_y,
												node->_rect._max_y);

			node->_nodes[1] = new Node();
			node->_nodes[1]->_rect = Rectangle(	node->_rect._min_x + img->getWidth(),
												node->_rect._max_x, 
												node->_rect._min_y,
												node->_rect._max_y);

			if (addImageRecursive (node->_nodes[0], img, name, out_name))	return true;
			if (addImageRecursive (node->_nodes[1], img, name, out_name))	return true;
			return false;
	
		} else {
			// Split top/bottom
			node->_nodes[0] = new Node();
			node->_nodes[0]->_rect = Rectangle(	node->_rect._min_x, 
												node->_rect._max_x, 
												node->_rect._min_y,
												node->_rect._min_y + img->getHeight());

			node->_nodes[1] = new Node();
			node->_nodes[1]->_rect = Rectangle(	node->_rect._min_x, 
												node->_rect._max_x, 
												node->_rect._min_y + img->getHeight(),
												node->_rect._max_y);

			if (addImageRecursive (node->_nodes[0], img, name, out_name))	return true;
			if (addImageRecursive (node->_nodes[1], img, name, out_name))	return true;
			return false;

		}
	}

}

bool ImageTree::addImage (Image *img, const std::string &name, const std::string &out_name)
{
	return addImageRecursive (_root, img, name, out_name);
}

//==============================================================================
//==============================================================================
