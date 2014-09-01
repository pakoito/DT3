//==============================================================================
///	
///	File: 			CameraiOS.cpp
///	Author:			Tod Baudais
///					Copyright (C) 2000-2007. All rights reserved.
///	
///	Date Created:	11/27/2009
///	Changes:		-none-
///	
//==============================================================================

#include "CameraiOS.hpp"

#include "Factory.hpp"
#include "System.hpp"
#include "ConsoleStream.hpp"

#import <AVFoundation/AVFoundation.h>
#import <MobileCoreServices/UTCoreTypes.h>
#import <MediaPlayer/MediaPlayer.h>

//==============================================================================
//==============================================================================

@interface MovieInputInstance : NSObject <AVCaptureVideoDataOutputSampleBufferDelegate>
{
	AVCaptureSession    *_session;
}
@end

//==============================================================================
// An object wrapper for the MPMoviePlayerController
//==============================================================================

@implementation MovieInputInstance

- (id)init
{

    NSError *error = nil;

    // Find a suitable AVCaptureDevice
    NSArray *captureDevices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
    if ([captureDevices count] == 2) {

        // Create a device input with the device and add it to the session.
        AVCaptureDeviceInput *input = [ AVCaptureDeviceInput 
                                        deviceInputWithDevice:[captureDevices objectAtIndex:0]
                                        error:&error];
        
        if (input) {
            
            // Create the session
            _session = [[AVCaptureSession alloc] init];

            // Configure the session to produce lower resolution video frames.
            _session.sessionPreset = AVCaptureSessionPresetMedium;

            [_session addInput:input];

            // Create a VideoDataOutput and add it to the session
            AVCaptureVideoDataOutput *output = [[[AVCaptureVideoDataOutput alloc] init] autorelease];
            [_session addOutput:output];

            // Configure your output.
            dispatch_queue_t queue = dispatch_queue_create("cameraQueue", NULL);
            [output setSampleBufferDelegate:self queue:queue];
            dispatch_release(queue);

            // Specify the pixel format
            output.videoSettings =  [NSDictionary dictionaryWithObject:
                                    [NSNumber numberWithInt:kCVPixelFormatType_32BGRA] 
                                    forKey:(id)kCVPixelBufferPixelFormatTypeKey];

            // If you wish to cap the frame rate to a known value, such as 15 fps, set 
            // minFrameDuration.
            output.minFrameDuration = CMTimeMake(1, 20);
            
            [_session startRunning];
        }
    }

	return [super init];
}

- (void)dealloc
{	
    if (_session) {
        [_session stopRunning];
        [_session release];
        _session = NULL;
    }

	[super dealloc];
}

//==============================================================================
//==============================================================================

// Delegate routine that is called when a sample buffer was written
- (void)captureOutput:(AVCaptureOutput *)captureOutput 
         didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer 
         fromConnection:(AVCaptureConnection *)connection
{     

    // Get a CMSampleBuffer's Core Video image buffer for the media data
    CVImageBufferRef imageBuffer = CMSampleBufferGetImageBuffer(sampleBuffer); 
    // Lock the base address of the pixel buffer
    CVPixelBufferLockBaseAddress(imageBuffer, 0); 

    // Get the number of bytes per row for the pixel buffer
    void *baseAddress = CVPixelBufferGetBaseAddress(imageBuffer);

    // Get the number of bytes per row for the pixel buffer
    size_t bytesPerRow = CVPixelBufferGetBytesPerRow(imageBuffer);
    
    // Get the pixel buffer width and height
    size_t width = CVPixelBufferGetWidth(imageBuffer);
    size_t height = CVPixelBufferGetHeight(imageBuffer);

    // Build a texture resource
    DT2::DTint width_pow_2 = DT2::MoreMath::nextPower2(width);
    DT2::DTint height_pow_2 = DT2::MoreMath::nextPower2(height);

    if (DT2::CameraiOS::_texture.isNull() || DT2::CameraiOS::_texture->getWidth() != width_pow_2 || DT2::CameraiOS::_texture->getHeight() != height_pow_2) {
        DT2::CameraiOS::_texture = makeSmartPtr(DT2::TextureResource::create());
        DT2::CameraiOS::_texture->allocateBuffer(width_pow_2, height_pow_2, DT2::TextureResource::FORMAT_BGRA);
        DT2::CameraiOS::_texture->setDataCanPurge(false);
    }
        
    DT2::CameraiOS::_texture->replaceSubRectangle(0, 0, (const DT2::DTubyte*) baseAddress, width, height, bytesPerRow);
    DT2::CameraiOS::_texture->setRecacheData(true);
    
    // Calculate rectangle
    DT2::Rectangle r (0.0F, (DT2::DTfloat) width/ (DT2::DTfloat) width_pow_2, 0.0F, (DT2::DTfloat) height/ (DT2::DTfloat) height_pow_2);
    
    // Set the texture
    DT2::CameraiOS::getFrameCapturedCB().fire(DT2::CameraiOS::_texture, r);

    CVPixelBufferUnlockBaseAddress(imageBuffer,0);
}

@end

//==============================================================================
//==============================================================================

namespace DT2 {

//==============================================================================
//==============================================================================

std::shared_ptr<TextureResource>                                CameraiOS::_texture;
Callback2List<std::shared_ptr<TextureResource>, Rectangle&>     CameraiOS::_frameCapturedCB;
MovieInputInstance                                       *_movie_input_instance = NULL;;

//==============================================================================
//==============================================================================

DTboolean CameraiOS::hasVideoInput (void)
{
    if (![UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera])
        return false;
        
    NSArray *sources = [UIImagePickerController availableMediaTypesForSourceType:UIImagePickerControllerSourceTypeCamera];
    if(![sources containsObject:@"public.movie"])
        return false;
        
    NSArray *captureDevices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
    if ([captureDevices count] != 2) {
        return false;
    }

    return true;
}

void CameraiOS::startRecording (void)
{
    if (!_movie_input_instance) {
        _movie_input_instance = [[MovieInputInstance alloc] init];
    } else {
        [_movie_input_instance retain];
    }
}

void CameraiOS::stopRecording (void)
{
    [_movie_input_instance release];
}

DTboolean CameraiOS::isRecording (void)
{
    return (_movie_input_instance != NULL);
}
      
//==============================================================================
//==============================================================================

} // DT2
