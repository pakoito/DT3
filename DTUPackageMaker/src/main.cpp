//==============================================================================
///	
///	FileProcess: 			PackageMaker.cpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <map>
#include <string>
#include <iomanip>
#include <cassert>
#include <sstream>

#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <stdint.h>

#include "zlib.h"

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
	std::string		file;
	std::string		path;
	int64_t         start;
	int64_t         length;
	int64_t         uncompressed_length;
    time_t          time;
};

//==============================================================================
//==============================================================================

void write (std::ofstream &file, int64_t v)
{
    assert(sizeof(v) == 8);

	file.write((const char *) &v, sizeof(v));
}

void write (std::ofstream &file, uint32_t v)
{
    assert(sizeof(v) == 4);
	file.write((const char *) &v, sizeof(v));
}

void write (std::ofstream &file, std::string v)
{
	char c;
	
	for (int i = 0; i < v.size(); ++i) {
		c = v[i];
		file.write(&c, sizeof(c));
	}
	
	c = 0;
	file.write(&c, sizeof(c));
}

//==============================================================================
//==============================================================================

void scanDir (std::string path, std::list<FileProcess> &files)
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
                    
                    struct stat buf;
                    int s = ::stat( (f.path + SEP + f.file).c_str(), &buf);
                    f.time = buf.st_ctime;

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
                    
                    struct stat buf;
                    int s = ::stat( (f.path + SEP + f.file).c_str(), &buf);
                    f.time = buf.st_ctime;

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

void processIndex (std::ofstream &outfile, std::list<FileProcess> &files)
{
	write (outfile, (uint32_t) files.size());
	
	std::list<FileProcess>::iterator file;
	for (file = files.begin(); file != files.end(); ++file) {
		write (outfile, file->file);
		write (outfile, file->start);
		write (outfile, file->length);
		write (outfile, file->uncompressed_length);
	}
}

//==============================================================================
//==============================================================================

void processFileProcess (std::ofstream &outfile, FileProcess &f, std::list<std::string> &compression_types, int64_t &total_compressed_size, int64_t &total_uncompressed_size)
{
	unsigned char buffer[1024*8];

	// Get location of beginning of file
	f.start = outfile.tellp();

	std::cout << "Processing " << f.file << " at location " << f.start << std::endl;
    
    
    // Check if this is an extension that should be compressed
    std::string ext = f.file.substr(f.file.find_last_of('.')+1);
    if ( std::find(compression_types.begin(), compression_types.end(), ext) != compression_types.end()) {
    
        std::cout << "  Compressing... ";
    
        // Read the entire file into a buffer
        std::vector<unsigned char> uncompressed_buffer;
        std::vector<unsigned char> compressed_buffer;
        
        std::string path = f.path + SEP + f.file;
        std::ifstream infile( path.c_str(), std::ios::in | std::ios::binary );
        
        int size = -1;
        while (!infile.eof() && size) {
            infile.read((char *) buffer, sizeof(buffer));
            size = infile.gcount();

            uncompressed_buffer.insert(uncompressed_buffer.end(), buffer, buffer+size);
        }

        // Use zlib to compress the buffer
        z_stream strm;
        strm.zalloc = 0;
        strm.zfree = 0;
        strm.next_in = reinterpret_cast<Bytef *>(&uncompressed_buffer[0]);
        strm.avail_in = uncompressed_buffer.size();
        strm.next_out = buffer;
        strm.avail_out = sizeof(buffer);

        deflateInit(&strm, Z_BEST_COMPRESSION);

        // Compress file
        while (strm.avail_in != 0) {
            int res = deflate(&strm, Z_NO_FLUSH);
            assert(res == Z_OK);
            
            if (strm.avail_out == 0) {
                compressed_buffer.insert(compressed_buffer.end(), buffer, buffer + sizeof(buffer));
                strm.next_out = buffer;
                strm.avail_out = sizeof(buffer);
            }
        }
        
        // Finish off
        int deflate_res = Z_OK;
        while (deflate_res == Z_OK) {
            if (strm.avail_out == 0) {
                compressed_buffer.insert(compressed_buffer.end(), buffer, buffer + sizeof(buffer));
                strm.next_out = buffer;
                strm.avail_out = sizeof(buffer);
            }
            deflate_res = deflate(&strm, Z_FINISH);
        }

        // End the stream
        assert(deflate_res == Z_STREAM_END);
        compressed_buffer.insert(compressed_buffer.end(), buffer, buffer + sizeof(buffer) - strm.avail_out);
        deflateEnd(&strm);
        
        // write compressed file to package
        outfile.write( (char*) &compressed_buffer[0], compressed_buffer.size());

        // Location of end of file
        f.length = (int64_t) (outfile.tellp()) - f.start;
        f.uncompressed_length = (int64_t) uncompressed_buffer.size();

        std::cout   << "Done! Before " << uncompressed_buffer.size() << " bytes, after " << compressed_buffer.size()
                    << " bytes (" << compressed_buffer.size() * 100 / uncompressed_buffer.size() << "%)" << std::endl;
        
        total_compressed_size += compressed_buffer.size();
        total_uncompressed_size += uncompressed_buffer.size();

    } else {

        // Copy file into package

        std::string path = f.path + SEP + f.file;
        std::ifstream infile( path.c_str(), std::ios::in | std::ios::binary );

        assert(infile.good());
        
        int size = -1;
        while (!infile.eof() && size) {
            infile.read((char *) buffer, sizeof(buffer));
            size = infile.gcount();
            
            outfile.write((char *) buffer, size);
        }
        
        infile.close();

        // Location of end of file
        f.length = (int64_t) (outfile.tellp()) - f.start;
        f.uncompressed_length = (int64_t) f.length;
        
        total_compressed_size += f.length;
        total_uncompressed_size += f.length;
    }
}

//==============================================================================
//==============================================================================

int main (int argc, char * const argv[]) {
	if (argc < 3) {
		std::cout << "Usage: " << argv[0] << " outfile.pack [-compress ext] pathToScan" << std::endl;
		exit(1);
	}

    struct stat buf;
    
	// Scan the path and build the list of files
	std::list<FileProcess> files;
    std::list<std::string> compression_types;
	
	for (int i = 2; i < argc; ) {
    
        // Parse extension to compress
        if ( std::string(argv[i]) == "-compress" ) {
            compression_types.push_back(argv[i+1]);
    
            i+=2;
            continue;
        }
    
        // Get info for file
        int s = ::stat(argv[i], &buf);
    
        // Scan directories
        if (S_ISDIR(buf.st_mode)) {
            scanDir (argv[i], files);
            
        // Normal file
        } else if (S_ISREG(buf.st_mode)) {
            
            std::string path(argv[i]);
            size_t pos = path.find_last_of (SEP);
            
            FileProcess f;
            
            if (pos == std::string::npos) {
                f.file = argv[i];
                f.path = ".";
                f.time = buf.st_ctime;
            } else {
                f.file = path.substr(pos+1);
                f.path = path.substr(0,pos);
                f.time = buf.st_ctime;
            }

            // Ignore blank filenames and trim
            std::stringstream trimmer;
            trimmer << f.file;
            f.file.clear();
            trimmer >> f.file;
            
            if (!f.file.empty())
                files.push_back(f);
            
        }
        
        ++i;
    }
	
	// Check to see if there are any files
	if (files.size() <= 0) {
		std::cout << "Unable to find any files to package." << std::endl;
		exit(1);
	}
    
    
    // Get modification time for output file
    time_t      outfile_time = 0;
    int s = ::stat(argv[1], &buf);
    if (s == 0) 
        outfile_time = buf.st_ctime;
        
    // Check if any input files are newer than the output file
    std::list<FileProcess>::iterator i;
    for (i = files.begin(); i != files.end(); ++i) {
        time_t file_time = i->time;
    
        if (file_time > outfile_time) {
            std::cout << "Found new files to package." << std::endl;
            break;
        }
    }
    
    if (i == files.end()) {
		std::cout << "No new files to package." << std::endl;
        return 0;
    }
    
	// Open output file
	std::ofstream outfile(argv[1], std::ios::out | std::ios::binary);
	
	// write magic number
	uint32_t MAGIC = 0x5041434B;	// i.e. PACK
	write(outfile, MAGIC);
	
	// write the fake index as a placeholder
	processIndex (outfile, files);
    
    int64_t total_compressed_size = 0;
    int64_t total_uncompressed_size = 0;

	std::list<FileProcess>::iterator file;
	for (file = files.begin(); file != files.end(); ++file)
		processFileProcess (outfile, *file, compression_types, total_compressed_size, total_uncompressed_size);

	// write the real index instead of placeholder
	outfile.seekp(4,std::ios::beg);	// After magic
	processIndex (outfile, files);
	
	// Add an extra byte at the end of the archive
	outfile.seekp(0,std::ios::end);
	char padding[] = {0};
	outfile.write(padding, 1);
	
	outfile.close();
    
    
    //
    // Report
    //

    std::map<std::string, int64_t> type_sizes;
    std::multimap<int64_t, std::string> file_sizes;
    int64_t total_size = 0;
    
	for (file = files.begin(); file != files.end(); ++file) {
        std::string ext = file->file.substr(file->file.find_last_of('.') + 1);
    
        if (type_sizes.find(ext) == type_sizes.end())   
            type_sizes[ext] = file->length;
        else 
            type_sizes[ext] += file->length;
            
        total_size += file->length;
        
        file_sizes.insert(std::make_pair(file->length, file->file));
    }
    
    std::cout << std::endl;
    std::cout << "Sizes for types" << std::endl;
    
    std::map<std::string, int64_t>::iterator j;
    for (j = type_sizes.begin(); j != type_sizes.end(); ++j) {
        std::cout   << "  " << std::setw(6) << j->first 
                    << "   " << std::setw(10) << j->second 
                    << "  (" << std::setw(4) << (j->second * 100.0F / total_size) << "%)" << std::endl;
    }
    
    std::cout << std::endl;
    std::cout << "Largest files" << std::endl;

    std::multimap<int64_t, std::string>::reverse_iterator k;
    int count = 0;
    for (k = file_sizes.rbegin(); k != file_sizes.rend() && count < 25; ++k, ++count) {
        std::cout   << "  " << std::setw(30) << k->second 
                    << "   " << std::setw(10) << k->first << std::endl;
    }
    
    std::cout << std::endl;
    std::cout << "Data Sizes" << std::endl;
    std::cout << " Total compressed size:   " << (float) total_compressed_size/1024/1024 << " MB" << std::endl;
    std::cout << " Total uncompressed size: " << (float) total_uncompressed_size/1024/1024 << " MB" << std::endl;
    std::cout << " Total savings due to compression: " << (float) (total_uncompressed_size - total_compressed_size)/1024/1024 << " MB" << std::endl;
    

    return 0;
}

#if defined(_WIN32)

int _tmain(int argc, char* argv[]) {
    return main(argc,argv);
}

#endif
