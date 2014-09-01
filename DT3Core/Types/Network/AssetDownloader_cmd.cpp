//==============================================================================
///	
///	File: AssetDownloader_cmd.cpp
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
#include "DT3Core/Types/Utility/CommandResult.hpp"
#include "DT3Core/Types/Utility/CommandRegistry.hpp"
#include "DT3Core/Types/Utility/CommandContext.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/Types/FileBuffer/FilePath.hpp"
#include "DT3Core/Types/Network/AssetDownloader.hpp"
#include "DT3Core/Types/Network/URL.hpp"
#include "DT3Core/Types/Threads/ThreadTaskQueue.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

class AssetDownloader_cmd: public Command {
    public:
        DEFINE_TYPE(AssetDownloader_cmd,Command);
        DEFINE_CREATE
        
        void register_commands (void) {
            CommandRegistry::register_command("DownloadAsset", &AssetDownloader_cmd::do_download_asset);
        }

        static CommandResult do_download_asset (CommandContext &ctx, const CommandParams &p);
};

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_COMMAND(AssetDownloader_cmd)

//==============================================================================
//==============================================================================

namespace {

    void downloader_status_callback(AssetDownloader::Status status, DTsize size, DTsize total_size)
    {
        std::string status_string;
        
        switch(status) {
            case AssetDownloader::STATUS_IDLE:           status_string = "STATUS_IDLE";          break;
            case AssetDownloader::STATUS_CONNECTING:     status_string = "STATUS_CONNECTING";    break;
            case AssetDownloader::STATUS_DOWNLOADING:    status_string = "STATUS_DOWNLOADING";   break;
            case AssetDownloader::STATUS_COMPLETE:       status_string = "STATUS_COMPLETE";      break;
            case AssetDownloader::STATUS_ERROR:          status_string = "STATUS_ERROR";         break;
            default:
                break;
        }
        
        LOG_MESSAGE << "Status Change: " << status_string << " size = " << size << " total size = " << total_size;
    }

}

CommandResult AssetDownloader_cmd::do_download_asset (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 4) {
        return CommandResult(false, "Usage: DownloadAsset url file timeout", CommandResult::UPDATE_NONE);
    }

    AssetDownloader downloader;
    
    downloader.asset_at_url(    URL(p[1]),
                                FilePath(p[2]),
                                make_callback(&downloader_status_callback),
                                MoreStrings::cast_from_string<DTfloat>(p[3]));
    
    // Wait
    while ( !downloader.is_done() ) {
        ThreadTaskQueue::run_queue();
    }


    return CommandResult(true, "DownloadAsset done", CommandResult::UPDATE_NONE);
}

//==============================================================================
//==============================================================================

} // DT3

#endif // DT3_COMMANDS

