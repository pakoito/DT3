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

#include <iostream>
#include <list>
#include <vector>
#include <sys/types.h>
#include <sstream>
#include <algorithm>

#include "Image.hpp"
#include "ImageTree.hpp"

#if defined(_WIN32)
    #include <Windows.h>

    #define S_ISDIR(f) (_S_IFDIR & (f))
    #define S_ISREG(f) (_S_IFREG & (f))
    #define SEP "\\"

#else
    #include <dirent.h>
    #define SEP "/"

#endif

//==============================================================================
//==============================================================================

struct FileProcess {
	FileProcess(void)	{	image = NULL;	}
	//~FileProcess(void)	{	delete image;	}

	std::string		file;
	std::string		path;
	Image*			image;
};

//==============================================================================
//==============================================================================

void Scan_Dir (std::string path, std::vector<FileProcess> &files)
{
	// Prime the queue
	std::list<std::string> dir_queue;
	dir_queue.push_back(path);

#if defined(_WIN32)

	while (dir_queue.size()) {
		std::string dir = dir_queue.back();
		dir_queue.pop_back();

        std::string dir_path = dir + SEP "*";

        WIN32_FIND_DATAA	FindFileData;
        HANDLE find_file = FindFirstFileExA(dir_path.c_str(),

                                            FindExInfoStandard,

                                            &FindFileData,

                                            FindExSearchNameMatch,

                                            NULL,

                                            0

                                            );

        if (find_file == INVALID_HANDLE_VALUE)
            return;
            
        do {
        
            // Skip invisible files
            if (FindFileData.cFileName[0] != '.') {
                
                if (FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
                    dir_queue.push_back(dir + SEP + FindFileData.cFileName);
                    
                } else {				
					FileProcess f;
					f.file = FindFileData.cFileName;
					f.path = dir;
                    
					files.push_back(f);

                }
                
            }
        } while (FindNextFileA(find_file, &FindFileData));		
	}

#else
	
	while (dir_queue.size()) {
		std::string dir = dir_queue.back();
		dir_queue.pop_back();

		// Build the name list
		struct dirent **namelist = NULL;
		int n = scandir(dir.c_str(), &namelist, NULL, NULL);
		
		// Process and free the name list
		for (int i = 0; i < n; ++i) {
			// Skip invisible files
			if (namelist[i]->d_name[0] != '.') {
				
				if (namelist[i]->d_type == DT_DIR) {
					dir_queue.push_back(dir + SEP + namelist[i]->d_name);
				} else {
					FileProcess f;
					f.file = namelist[i]->d_name;
					f.path = dir;
					files.push_back(f);
				}
				
			}
			::free(namelist[i]);
		}
		if (namelist) ::free(namelist);
		
	}

#endif

}

//==============================================================================
//==============================================================================

struct ImgSort {
	bool operator() (const FileProcess &i,const FileProcess &j) { 
		return i.image->getWidth() > j.image->getWidth();
	}
};

//==============================================================================
//==============================================================================

int main (int argc, char * const argv[]) {

	if (argc < 3) {
		std::cout << "Usage: " << argv[0] << " [-p padding] [-w width] [-h height] [-notrim] pathToScan [pathToScan]..." << std::endl;
		exit(1);
	}
	
	// Parse parameters
	int argi = 1, pad = 1, width = 1024, height = 1024, opt_no_trim = false;
	while (1) {
		if (argi >= argc)	{	std::cout << "Error in number of args" << std::endl;	}
		std::string arg(argv[argi]);
	
		if (arg == "-p") {	
			++argi;
			
			if (argi >= argc)	{	std::cout << "Error in number of args" << std::endl;	}
			std::stringstream ss(argv[argi]);
			
			ss >> pad;
			
			++argi;
			continue;
		} else if (arg == "-w") {
			++argi;
			
			if (argi >= argc)	{	std::cout << "Error in number of args" << std::endl;	}
			std::stringstream ss(argv[argi]);

			ss >> width;
			
			++argi;
			continue;
		} else if (arg == "-h") {
			++argi;
			
			if (argi >= argc)	{	std::cout << "Error in number of args" << std::endl;	}
			std::stringstream ss(argv[argi]);

			ss >> height;
			
			++argi;
			continue;
		} else if (arg == "-notrim") {
			++argi;
            
            opt_no_trim = true;
			
			continue;
		} else {
			break;
		}
	}
	
	// Check for at least 2 more parameters
	if (argi+2 >= argc)	{	std::cout << "Error in number of args" << std::endl;	}

	// Scan the path and build the list of files
	std::vector<FileProcess> files;
	
	for (int i = argi+1; i < argc; ++i)
		Scan_Dir (argv[i], files);
		
	// Check to see if there are any files
	if (files.size() <= 0) {
		std::cout << "Unable to find any files to atlas." << std::endl;
		exit(1);
	}
	
	// Iterate through the files
	std::vector<FileProcess>::iterator file;
	for (file = files.begin(); file != files.end(); ) {
	
		Image* image = new Image();
		
		if (image->load(file->path + SEP + file->file)) {
        
            // Check for file too large
            if (image->getWidth() > width || image->getHeight() > height) {
                std::cout << "File too large: " << file->path + SEP + file->file;
            
                delete image;
                file = files.erase(file);
                continue;
            }
        
            if (!opt_no_trim)
                image->trim();
            
			image->pad(pad);
			image->blackOutTransparency();
			
			file->image = image;
			++file;
		} else {
			delete image;
			file = files.erase(file);
		}
		
	}
	
	// Sort files
	std::sort(files.begin(), files.end(), ImgSort());
	
	// Strip output name
	std::string argv1_name(argv[argi]);
	std::string argv1_path("");
	
	if (argv1_name.find_last_of('/') != std::string::npos) {
		argv1_path = argv1_name.substr(0,argv1_name.find_last_of('/')+1);
		argv1_name = argv1_name.substr(argv1_name.find_last_of('/')+1);
	}
		
	if (argv1_name.find_last_of('.') != std::string::npos)
		argv1_name = argv1_name.substr(0,argv1_name.find_last_of('.'));

	
	// Process images
	int image_index = 0;
	while (files.size() > 0) {
		ImageTree tree;
		tree.setSize(width,height,pad);

		std::stringstream argv1_file_name;
		argv1_file_name << argv1_name << "." << image_index;

		// Build the atlas
		std::vector<FileProcess>::iterator file;
		for (file = files.begin(); file != files.end(); ) {
		
			// Strip file name
			std::string file_name(file->path + SEP + file->file);
			if (file_name.find_last_of(SEP) != std::string::npos)
				file_name = file_name.substr(file_name.find_last_of(SEP)+1);

			if (file_name.find_last_of('.') != std::string::npos)
				file_name = file_name.substr(0,file_name.find_last_of('.'));

			// Add the image
			if (tree.addImage(file->image, file_name, argv1_file_name.str())) {
				file = files.erase(file);
			} else {
				++file;
			}
			
		}
		
		std::stringstream argv1_file_path;
		argv1_file_path << argv1_path << argv1_file_name.str() << ".png";

		tree.result().save(argv1_file_path.str());
		++image_index;
	}
	
    return 0;
}
