//==============================================================================
///	
///	File: AssetDownloader.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Network/AssetDownloader.hpp"
#include "DT3Core/Types/Network/NetworkSocket.hpp"
#include "DT3Core/Types/Network/NetworkAddress.hpp"
#include "DT3Core/Types/Network/NetworkPacket.hpp"
#include "DT3Core/Types/Network/AssetDownloader.hpp"
#include "DT3Core/Types/Network/URL.hpp"
#include "DT3Core/Types/FileBuffer/FilePath.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/Types/Utility/TimerHires.hpp"
#include "DT3Core/Types/Utility/LatentCall.hpp"
#include "DT3Core/Types/Threads/ThreadTaskQueue.hpp"
#include "DT3Core/Devices/DeviceNetwork.hpp"
#include "DT3Core/System/System.hpp"
#include "DT3Core/System/FileManager.hpp"
#include <map>
#include DT3_HAL_INCLUDE_PATH

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

//IMPLEMENT_FACTORY_CREATION(AssetDownloader)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

AssetDownloader::AssetDownloader (void)
    :   _status         (STATUS_IDLE),
        _current_size   (0),
        _total_size     (0)
{

}
			
AssetDownloader::~AssetDownloader (void)
{

}

//==============================================================================
//==============================================================================

void AssetDownloader::update_status (Status status, DTsize size, DTsize total_size)
{
    _status = status;
    if (_status_cb)
        (*_status_cb)(status, size, total_size);
}

//==============================================================================
//==============================================================================

void AssetDownloader::append_data (std::string &data, BinaryFileStream &temp_file)
{

    // Search for HTTP header
    if (_total_size == 0) {
    
        std::size_t header_pos = data.find("\r\n\r\n");
        if (header_pos != std::string::npos) {
        
            // Parse the header
            std::string header = data.substr(0,header_pos);
            std::vector<std::string> header_items = MoreStrings::split(header, "\r\n");
            
            std::map<std::string,std::string> header_items_map;

            // Find the content length
            for (std::size_t i = 0; i < header_items.size(); ++i) {
                std::vector<std::string> header_item = MoreStrings::split(header_items[i],":");

                // Validate entry
                if (header_item.size() != 2)
                    continue;

                std::string param = MoreStrings::trim(header_item[0]);
                std::string value = MoreStrings::trim(header_item[1]);
                
                LOG_MESSAGE << "Header: " << param << " " << value;
                
                header_items_map[param] = value;
            }
            
            auto i = header_items_map.find("Content-Length");
            if (i != header_items_map.end())
                _total_size = MoreStrings::cast_from_string<DTsize>(i->second);
        
            // Strip off header
            data = data.substr(header_pos+4);
        }
        
    // Append data
    }
    
    if (_total_size > 0) {
        
        // Write out data to a file
        temp_file.write_raw( (DTubyte*) &(data[0]), data.size());
        
        // Update sizes
        _current_size += data.size();
        data.clear();
    }

    // Update progress
    update_status (STATUS_DOWNLOADING, _current_size, _total_size);
}

void AssetDownloader::finalize_data (std::string &data, BinaryFileStream &temp_file)
{
    // Write out data to a file
    temp_file.write_raw( (DTubyte*) &(data[0]), data.size());

    // Update sizes
    _current_size += data.size();
    data.clear();

    // Update progress
    update_status (STATUS_DOWNLOADING, _current_size, _total_size);
}

//==============================================================================
//==============================================================================

void AssetDownloader::downloader_task   (   const URL url,
                                            const FilePath save_path,
                                            DTfloat timeout)
{
    std::shared_ptr<DeviceNetwork> network = System::network_manager();

    //
    // Connecting Phase
    //
    
    // Update status
    update_status (STATUS_CONNECTING, 0,0);
    
    // Resolve the host
    NetworkSocket socket;
    NetworkAddress ip; 
    
    DTerr err = network->resolve_host( ip, url.hostname(), url.port() );
    if (err != DT3_ERR_NONE) {
        LOG_MESSAGE << "HTTPRequest: Unable to resolve host.";
        update_status (STATUS_ERROR, 0,0);
        return;
    }
        
    err = network->tcp_open(socket,ip);
    if (err != DT3_ERR_NONE) {
        LOG_MESSAGE << "HTTPRequest: Unable to resolve host.";
        update_status (STATUS_ERROR, 0,0);
        return;
    }
        
	// Send http request
    std::string server = url.hostname();
    std::string path = url.path();
    
    // Note:
    // HTTP/1.1 defines the "close" connection option for the sender to signal that the connection 
    // will be closed after completion of the response. For example,
    //      Connection: close
    // in either the request or the response header fields indicates that the connection SHOULD NOT
    // be considered `persistent' (section 8.1) after the current request/response is complete.
    // HTTP/1.1 applications that do not support persistent connections MUST include the "close" 
    // connection option in every message.

    std::string request =   "GET " + path + " HTTP/1.1\r\n" +
                            "Host: " + server + "\r\n" +
                            "User-Agent: DT3\r\n" +
                            "Accept: */*\r\n" +
                            "Cache-Control: max-age=0\r\n" +
                            "Connection: close\r\n" +
                            "\r\n";

    LOG_MESSAGE << "Sending...";
    LOG_MESSAGE << request;
    
    NetworkPacket packet_request;
    packet_request.set_data(request);
    
    err = network->tcp_send(packet_request, socket);
    if (err != DT3_ERR_NONE) {
        network->tcp_close(socket);
        
        LOG_MESSAGE << "HTTPRequest: Unable to send packet.";
        update_status (STATUS_ERROR, 0,0);
        return;
    }
    
    //
    // Downloading Phase
    //
    
    update_status (STATUS_DOWNLOADING, 0,0);

    // Hash the URL
    FilePath temp_file_path = FilePath(HAL::save_dir().full_path() + "/" + MoreStrings::cast_to_string(MoreStrings::hash(url.full_url())));
    
    // Create a temporary file
    BinaryFileStream temp_file;
    err = FileManager::open(temp_file, temp_file_path, false);
    
    if (err != DT3_ERR_NONE) {
        network->tcp_close(socket);
        
        LOG_MESSAGE << "HTTPRequest: Unable to open file for writing.";
        update_status (STATUS_ERROR, 0,0);
        return;
    }
    
    LOG_MESSAGE << "HTTPRequest: Opened temp file at " << temp_file_path.full_path();

    
    // Temporary storage for buffer
    std::string data;
        
    // Timer for timeout
    TimerHires timeout_timer;
    
    do {

        // Get the results
        NetworkPacket packet;
        packet.set_data_size(1024*16);

        err = network->tcp_recv(packet,socket);
        
        // If we recieved some data, append it to the buffer
        if (packet.data_size() > 0) {
            data += packet.data_as_string();
            append_data(data, temp_file);
            timeout_timer.reset_abs_time();
        }
        
        // Check complete
        if ( (_current_size == _total_size) && (_total_size > 0) ) {
            break;
        }
        
        // If we hit our timeout, then we abort
        if (timeout_timer.abs_time() > timeout) {
            network->tcp_close(socket);
            
            LOG_MESSAGE << "Http request timed out!";
            update_status (STATUS_ERROR, 0,0);
            
            // Remove the temporary file
            temp_file_path.del();
            return;
        }
        
        // Check for cancel
        if (_cancelled) {
            network->tcp_close(socket);
            
            LOG_MESSAGE << "Http request cancelled!";
            update_status (STATUS_CANCELLED, 0,0);
            
            // Remove the temporary file
            temp_file_path.del();
            return;
        }
        
    } while (err == DT3_ERR_NET_WOULD_BLOCK || err == DT3_ERR_NONE);
    
    // Close the connection, we're done
    network->tcp_close(socket);
    
    // Close off the stream
    finalize_data(data, temp_file);

    //
    // Finalizing phase
    //
    
    // Move the file
    save_path.del();
    temp_file_path.move(save_path);
    
    LOG_MESSAGE << "Saving file to " << save_path.full_path();


    // Final update of status
    update_status (STATUS_COMPLETE, _current_size, _total_size);
}

//==============================================================================
//==============================================================================

void AssetDownloader::asset_at_url (const URL &url,
                                    const FilePath &save_path,
                                    const std::shared_ptr<Callback<Status, DTsize, DTsize> > &status_cb,
                                    DTfloat timeout)
{
    _status_cb = status_cb;
    
    _status = STATUS_IDLE;
    _current_size = 0;
    _total_size = 0;
    
    _cancelled = false;
    
    // Start up the thread to do the download
    ThreadTaskQueue::add_task(make_latent_call(this, &AssetDownloader::downloader_task, url, save_path, timeout));
}


void AssetDownloader::cancel (void)
{
    _cancelled = true;
}

//==============================================================================
//==============================================================================

DTboolean AssetDownloader::is_done (void) const
{
    return  _status == AssetDownloader::STATUS_ERROR ||
            _status == AssetDownloader::STATUS_COMPLETE ||
            _status == AssetDownloader::STATUS_CANCELLED;
}

DTboolean AssetDownloader::is_idle (void) const
{
    return  _status == AssetDownloader::STATUS_IDLE ||
            _status == AssetDownloader::STATUS_ERROR ||
            _status == AssetDownloader::STATUS_COMPLETE ||
            _status == AssetDownloader::STATUS_CANCELLED;
}

//==============================================================================
//==============================================================================

} // DT3
