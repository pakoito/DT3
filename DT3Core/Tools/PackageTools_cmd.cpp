//==============================================================================
///	
///	File: PackageTools_cmd.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/System/Command.hpp"

#ifdef DT3_COMMANDS

#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/FileManager.hpp"
#include "DT3Core/Types/Utility/CommandRegistry.hpp"
#include "DT3Core/Types/Utility/CommandResult.hpp"
#include "DT3Core/Types/FileBuffer/FilePath.hpp"
#include "DT3Core/Types/FileBuffer/BinaryFileStream.hpp"
#include "DT3Core/Types/Utility/DirectoryLister.hpp"
#include "DT3Core/Types/Utility/Compressor.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include <algorithm>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

class PackageTools_cmd: public Command {
    public:
        DEFINE_TYPE(PackageTools_cmd,Command);
        DEFINE_CREATE
        
        void register_commands (void) {
            CommandRegistry::register_command("MakePackage", &PackageTools_cmd::do_make_package);
        }

        static CommandResult do_make_package (CommandContext &ctx, const CommandParams &p);
};

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_COMMAND(PackageTools_cmd)

//==============================================================================
//==============================================================================

namespace {
    struct FileProcess {
        std::string		file;
        FilePath		path;
        int64_t         start;
        int64_t         length;
        int64_t         uncompressed_length;
        time_t          time;
    };
}


CommandResult PackageTools_cmd::do_make_package (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() <= 2) {
        return CommandResult(false, "Usage: MakePackage package.pkg [file or directory]... [-compress ext]...", CommandResult::UPDATE_NONE);
    }
    
    //
    // Parse command line parameters
    //
    
    DirectoryLister lister;
    std::list<std::string> compression_types;
    
    for (DTuint i = 0; i < p.count();) {
        
        // Check for compression flag
        if (p[i] == "-compress") {
            compression_types.push_back(p[i+1]);
            i+=2;
            continue;
        }
        
        // Try to add the file or directory
        FilePath pathname(p[i]);
        if (pathname.exists()) {
            lister.scan_path(pathname); // If this is a path to a file, it should just add it to the list
        }
        
        i+=1;
    }
    
    if (lister.num_files() <= 0) {
        return CommandResult(true, "Unable to find files to package", CommandResult::UPDATE_NONE);
    }

    //
    // Check dates to see if update is needed
    //
    
    std::list<FileProcess> files;
    
    DTboolean needs_rebuild = false;
    
    FilePath package_pathname(p[1]);
    DTuint64 package_modification_date = package_pathname.modification_date();
    
    for (DTuint i = 0; i < lister.num_files(); ++i) {
        FilePath file_pathname = lister.file_path(i);
        DTuint64 file_modification_date = file_pathname.modification_date();
    
        if (file_modification_date > package_modification_date) {
            needs_rebuild = true;
        }
        
        FileProcess process;
        process.file = file_pathname.file_name();
        process.path = file_pathname;
        process.start = 0;
        process.length = file_pathname.length();
        process.uncompressed_length = process.length;
        process.time = file_modification_date;
    }
    
    if (!needs_rebuild) {
        return CommandResult(true, "Package file already up to date", CommandResult::UPDATE_NONE);
    }
    
    //
    // Build package file
    //
    
    BinaryFileStream outfile;
    
    // Open the file
	DTerr error = FileManager::open(outfile, package_pathname, false);
	if (error != DT3_ERR_NONE)
		return CommandResult(true, "Unable to open package file for writing", CommandResult::UPDATE_NONE);
    
    // Write magic number
	const DTuint MAGIC = 0x5041434B;	// i.e. PACK
    outfile << MAGIC;
    
    
    //
    // Write placeholder indices
    //
    
    DTsize start_of_index = outfile.p();

    for (auto &f : files) {
        outfile << f.file;
        outfile << f.start;
        outfile << f.length;
        outfile << f.uncompressed_length;
    }
    
    
    //
    // Write files
    //
    
    DTsize total_compressed_size = 0;
    DTsize total_uncompressed_size = 0;

    for (auto &f : files) {
    
        std::vector<DTubyte> src_data;
        src_data.resize(f.length);
        
        // Read file into buffer
        BinaryFileStream infile;
        FileManager::open(infile, f.path, true);
        infile.read_raw(&src_data[0], src_data.size());
        
        // Should we compress this?
        if (std::find(compression_types.begin(), compression_types.end(), f.path.file_ext()) != compression_types.end()) {
            std::vector<DTubyte> compressed_src_data;
            compressed_src_data.resize(src_data.size());
    
            DTsize compressed_size = Compressor::deflate (  &src_data[0], src_data.size(),
                                                            &compressed_src_data[0], compressed_src_data.size() );
            if (compressed_size > 0 && compressed_size < (DTsize) src_data.size()) {
                compressed_src_data.resize(compressed_size);
                std::swap(compressed_src_data, src_data);
            }
        }
        
        // Patch up file entry
        f.start = outfile.p();
        f.length = src_data.size();
        
        LOG_MESSAGE << "Processing " << f.file << " at location " << f.start << " (length = " << f.length << ")";
        
        // Tally sizes
        total_compressed_size += f.length;
        total_uncompressed_size += f.uncompressed_length;
        
        // Write the data
        outfile.write_raw(&src_data[0], src_data.size());
    }
    
    //
    // Write correct indices
    //
    
    outfile.seek_p(start_of_index, Stream::FROM_BEGINNING);
    
    for (auto &f : files) {
        outfile << f.file;
        outfile << f.start;
        outfile << f.length;
        outfile << f.uncompressed_length;
    }
    
    // Add an extra byte for good measure
    outfile << (DTubyte) 0;

    //
    // Report
    //

    std::map<std::string, DTsize> type_sizes;
    std::multimap<DTsize, std::string> file_sizes;
    DTsize total_size = 0;
    
    for (auto &f : files) {
        std::string ext = f.path.file_ext();
    
        if (type_sizes.find(ext) == type_sizes.end())   
            type_sizes[ext] = f.length;
        else 
            type_sizes[ext] += f.length;
            
        total_size += f.length;
        
        file_sizes.insert(std::make_pair(f.length, f.file));
    }
    
    LOG_MESSAGE << "Sizes for types";
    
    for (auto j = type_sizes.begin(); j != type_sizes.end(); ++j) {
        LOG_MESSAGE << "  " << j->first
                    << "   " << j->second
                    << "  (" << (j->second * 100.0F / total_size) << "%)";
    }
    
    LOG_MESSAGE << "Largest files";

    DTint count = 0;
    for (auto k = file_sizes.rbegin(); k != file_sizes.rend() && count < 25; ++k, ++count) {
        LOG_MESSAGE << "  " << k->second
                    << "   " << k->first;
    }

    LOG_MESSAGE << "Data Sizes";
    LOG_MESSAGE << " Total compressed size:   " << (DTfloat) total_compressed_size/1024/1024 << " MB";
    LOG_MESSAGE << " Total uncompressed size: " << (DTfloat) total_uncompressed_size/1024/1024 << " MB";
    LOG_MESSAGE << " Total savings due to compression: " << (DTfloat) (total_uncompressed_size - total_compressed_size)/1024/1024 << " MB";

    return CommandResult(true, "Package written", CommandResult::UPDATE_NONE);
}

//==============================================================================
//==============================================================================

} // DT3

#endif // DT3_COMMANDS

