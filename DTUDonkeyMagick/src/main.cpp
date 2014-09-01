//==============================================================================
///	
///	File: main.cpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include <stdlib.h>
#include <iostream>
#include <string>

#include "Image.hpp"

//==============================================================================
//==============================================================================

void removeAlpha (Image &image)
{
	for (int y = 0; y < image.getHeight(); ++y) {
		for (int x = 0; x < image.getWidth(); ++x) {
			Image::Pixel p = image.getPixel(x,y);
            p.a = 255;
			image.setPixel(x, y, p);
		}
	}
}

//==============================================================================
//==============================================================================

void expandAlpha (Image &image)
{
    Image old_image(image);
    Image temp_image;
    
    bool modified = true;
    while (modified) {
        modified = false;
        temp_image = image;
    
        for (int y = 0; y < temp_image.getHeight(); ++y) {
            for (int x = 0; x < temp_image.getWidth(); ++x) {
            
                Image::Pixel p = temp_image.getPixel(x,y);
                                
                if (p.a > 0)
                    continue;
                                
                int x_min = x - 1;
                int x_max = x + 1;
                int y_min = y - 1;
                int y_max = y + 1;
                
                if (x_min < 0)                          x_min = 0;
                if (x_max > temp_image.getWidth()-1)     x_max = temp_image.getWidth()-1;
                if (y_min < 0)                          y_min = 0;
                if (y_max > temp_image.getHeight()-1)    y_max = temp_image.getHeight()-1;

                int r_sum = 0;
                int g_sum = 0;
                int b_sum = 0;
                int num = 0;
                
                for (int yy = y_min; yy <= y_max; ++yy) {
                    for (int xx = x_min; xx <= x_max; ++xx) {
                        p = temp_image.getPixel(xx,yy);
                        
                        if (p.a > 0) {
                            r_sum += p.r;
                            g_sum += p.g;
                            b_sum += p.b;
                            num += 1;
                        }
                    
                    }
                }
                
                if (num > 0) {
                    p.r = r_sum / num;
                    p.g = g_sum / num;
                    p.b = b_sum / num;
                    p.a = 255;
                    image.setPixel(x, y, p);
                    
                    modified = true;
                }
            }
        }
    }
    
    // Replace alpha
    for (int y = 0; y < temp_image.getHeight(); ++y) {
        for (int x = 0; x < temp_image.getWidth(); ++x) {
            Image::Pixel p_rgb = image.getPixel(x,y);
            Image::Pixel p_a = old_image.getPixel(x,y);
            
            p_rgb.a = p_a.a;
            
            image.setPixel(x, y, p_rgb);            
        }
    }

}

//==============================================================================
//==============================================================================

void fillAlphaBlack (Image &image)
{
	for (int y = 0; y < image.getHeight(); ++y) {
		for (int x = 0; x < image.getWidth(); ++x) {
			Image::Pixel p = image.getPixel(x,y);
            if (p.a == 0) {
                p.r = 0;
                p.g = 0;
                p.b = 0;
            }
			image.setPixel(x, y, p);
		}
	}

}

//==============================================================================
//==============================================================================

int main (int argc, char * const argv[])
{
	if (argc < 3) {
		std::cout << "Usage: " << argv[0] << " [-removealpha] [-expandalpha] [-fillalphablack] outfile infile" << std::endl;
		exit(1);
	}
        
    std::string outfile_path = argv[argc-2];
    std::string infile_path = argv[argc-1];
    
    Image image;
    image.load(infile_path);
    
    for (int i = 1; i < argc-2; ++i) {
        std::string arg = argv[i];
    
        if (arg == "-removealpha")      removeAlpha(image);
        if (arg == "-expandalpha")      expandAlpha(image);
        if (arg == "-fillalphablack")   fillAlphaBlack(image);
    }
    
    image.save(outfile_path);
	
    return 0;
}

