//==============================================================================
///	
///	File: LightingTools_cmd.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================
//
// convert final_minus_x.png final_plus_x.png final_minus_y.png final_plus_y.png final_minus_z.png final_plus_z.png -append out.png
//
//==============================================================================

#include "DT3Core/System/Command.hpp"

#ifdef DT3_COMMANDS

#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Globals.hpp"
#include "DT3Core/System/System.hpp"
#include "DT3Core/World/World.hpp"
#include "DT3Core/Types/Utility/CommandRegistry.hpp"
#include "DT3Core/Types/Utility/CommandResult.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include "DT3Core/Types/Math/Color4b.hpp"
#include "DT3Core/Types/Media/PNGWriter.hpp"
#include "DT3Core/Resources/ResourceTypes/TextureResource2D.hpp"
#include "DT3Core/Objects/CameraObject.hpp"
#include <thread>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

class LightingTools_cmd: public Command {
    public:
        DEFINE_TYPE(LightingTools_cmd,Command);
        DEFINE_CREATE
        
        void register_commands (void) {
            CommandRegistry::register_command("GenerateIrradianceMap", &LightingTools_cmd::do_generate_irradiance_map);
            CommandRegistry::register_command("UnitLUT", &LightingTools_cmd::do_unit_LUT);
            CommandRegistry::register_command("UnitLUT2D", &LightingTools_cmd::do_unit_LUT2D);
            CommandRegistry::register_command("UnitTex", &LightingTools_cmd::do_unit_tex);
        }

        static CommandResult do_generate_irradiance_map (CommandContext &ctx, const CommandParams &p);
        static CommandResult do_unit_LUT                (CommandContext &ctx, const CommandParams &p);
        static CommandResult do_unit_LUT2D              (CommandContext &ctx, const CommandParams &p);
        static CommandResult do_unit_tex                (CommandContext &ctx, const CommandParams &p);
    
    
};

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_COMMAND(LightingTools_cmd)

//==============================================================================
//==============================================================================

namespace {
    const DTint PLUS_X = 0;
    const DTint MINUS_X = 1;
    const DTint PLUS_Y = 2;
    const DTint MINUS_Y = 3;
    const DTint PLUS_Z = 4;
    const DTint MINUS_Z = 5;
    const DTint NUM_SIDES = 6;
    const DTint SRC_SIZE = 512;

    void generate_maps (DTint  side,
                        DTfloat gain,
                        DTfloat exponent, 
                        DTint dst_size,
                        DTfloat num_samples,
                        std::shared_ptr<TextureResource2D> src_tex,
                        std::shared_ptr<TextureResource2D> dst_tex)
    {
        const DTfloat HALF_PIXEL = 1.0F / dst_size * 0.5F;

        for (DTint y = 0; y < dst_size; ++y) {
            for (DTint x = 0; x < dst_size; ++x) {
                
                DTfloat s = static_cast<DTfloat>(x) / static_cast<DTfloat>(dst_size) + HALF_PIXEL - 0.5F;
                DTfloat t = static_cast<DTfloat>(y) / static_cast<DTfloat>(dst_size) + HALF_PIXEL - 0.5F;
                
                Vector3 v;
                
                switch (side) {
                    case PLUS_X:    v = Vector3(0.5F, t, s);
                                    break;
                    case MINUS_X:   v = Vector3(-0.5F, t, -s);
                                    break;
                    case PLUS_Y:    v = Vector3(-s, 0.5F, -t);
                                    break;
                    case MINUS_Y:   v = Vector3(-s, -0.5F, t);
                                    break;
                    case PLUS_Z:    v = Vector3(-s, t, 0.5F);
                                    break;
                    case MINUS_Z:   v = Vector3(s, t, -0.5F);
                                    break;
                };
                
                v.normalize();
                
                DTfloat rc=0.0F,gc=0.0F,bc=0.0F;
                
                for (DTint sample = 0; sample < num_samples; ++sample) {
                
                    Vector3 r = MoreMath::random_vector();   // Random direction
                    
                    // Constrain to hemisphere
                    DTfloat dot = Vector3::dot(r, v);
                    if (dot < 0.0F) {
                        r = r - dot * 2.0F * v; // Flip direction
                        dot = -dot;
                    }
                    
                    r.normalize();
                                        
                    //
                    // Sample from src textures
                    //
                    
                    // Find major axis
                    DTfloat mx = std::abs(r.x);
                    DTfloat my = std::abs(r.y);
                    DTfloat mz = std::abs(r.z);
                    
                    DTint ma;
                
                    if (mx >= my && mx >= mz)       {   ma = (r.x > 0.0F) ? PLUS_X : MINUS_X;   }
                    else if (my >= mx && my >= mz)  {   ma = (r.y > 0.0F) ? PLUS_Y : MINUS_Y;   }
                    else                            {   ma = (r.z > 0.0F) ? PLUS_Z : MINUS_Z;   }
                
                    // Calculate texcoords into image
                    DTfloat tc_s;
                    DTfloat tc_t;
                    DTfloat tc_ma;
                    
                    switch (ma) {
                        case PLUS_X:    tc_s = -r.z;    tc_t = -r.y;    tc_ma = r.x;
                                        break;
                        case MINUS_X:   tc_s = +r.z;    tc_t = -r.y;    tc_ma = r.x;
                                        break;
                        case PLUS_Y:    tc_s = +r.x;    tc_t = +r.z;    tc_ma = r.y;
                                        break;
                        case MINUS_Y:   tc_s = +r.x;    tc_t = +r.z;    tc_ma = r.y;
                                        break;
                        case PLUS_Z:    tc_s = +r.x;    tc_t = -r.y;    tc_ma = r.z;
                                        break;
                        case MINUS_Z:   tc_s = -r.x;    tc_t = -r.y;    tc_ma = r.z;
                                        break;
                    };
                    
                    DTint xs = static_cast<DTint>((tc_s / std::abs(tc_ma) + 1.0F) * 0.5F * SRC_SIZE);
                    DTint ys = static_cast<DTint>((tc_t / std::abs(tc_ma) + 1.0F) * 0.5F * SRC_SIZE);
                    
                    Color4b c = src_tex->pixel(xs,ys);
                    rc += std::pow(c.r_as_float(), exponent) * dot;
                    gc += std::pow(c.g_as_float(), exponent) * dot;
                    bc += std::pow(c.b_as_float(), exponent) * dot;
                }
                
                // Set final pixel
                dst_tex->set_pixel(dst_size-x-1, dst_size-y-1, Color4b(rc*gain/num_samples, gc*gain/num_samples, bc*gain/num_samples, 1.0F));
                
            }
        }		
            
        return;
    }

};

CommandResult LightingTools_cmd::do_generate_irradiance_map (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 7) {
        return CommandResult(false, "Usage: GenerateIrradianceMap level position gain exponent size numsamples", CommandResult::UPDATE_NONE);
    }
    
    Globals::set_global("SYS_GEN_IRRADIANCE", "1", DT3::Globals::VOLATILE);
    
    DTfloat gain = MoreStrings::cast_from_string<DTfloat>(p[3]);
    DTfloat exponent = MoreStrings::cast_from_string<DTfloat>(p[4]);
    DTint dst_size = MoreStrings::cast_from_string<DTint>(p[5]);
    DTint num_samples = MoreStrings::cast_from_string<DTint>(p[6]);
    
    //
    // Load the Level
    //
    
	std::shared_ptr<World> world = checked_cast<World>(Factory::create_object_from_stream(FilePath(p[1]), NULL, NULL));
    if (!world) {
        Globals::set_global("SYS_GEN_IRRADIANCE", "0", DT3::Globals::VOLATILE);

        CommandResult cr;
        cr.set_message("Unable to open level");
        cr.set_result(false);
        return cr;
    }
    
    
    
    //
    // Generate env textures from OpenGL
    //
        
    std::shared_ptr<TextureResource2D> src_tex[NUM_SIDES];
    std::shared_ptr<TextureResource2D> dst_tex[NUM_SIDES];
    
    for (DTint i = 0; i < NUM_SIDES; ++i) {
        src_tex[i] = TextureResource2D::create();
        src_tex[i]->allocate_rgba_textels(SRC_SIZE,SRC_SIZE, false);

        dst_tex[i] = TextureResource2D::create();
        dst_tex[i]->allocate_rgba_textels(dst_size,dst_size, false);
    }
    
    // Setup the viewport
    DTint x, y, width, height;
    System::renderer()->viewport (x, y, width, height);
	System::renderer()->set_viewport (0,0,SRC_SIZE,SRC_SIZE);


    // Build an appropriate camera
    std::shared_ptr<CameraObject> camera = world->camera();
    camera->set_translation(MoreStrings::cast_from_string<Vector3>(p[2]));
    camera->set_frustum(1.0F, 10000.0F, 90.0F, 1.0F/System::renderer()->screen_aspect());
    
    // Draw +x
    camera->set_orientation( Matrix3(   0.0F,   0.0F,   -1.0F,
                                        0.0F,   1.0F,   0.0F,
                                        1.0F,   0.0F,   0.0F    ));
    world->draw(0.0F);
    System::renderer()->screenshot(src_tex[PLUS_X], Rectangle(0,SRC_SIZE,0,SRC_SIZE));
    PNGWriter::write(FilePath("plus_x.png"), src_tex[PLUS_X]);

    // Draw -x
    camera->set_orientation( Matrix3(   0.0F,   0.0F,   1.0F,
                                        0.0F,   1.0F,   0.0F,
                                        -1.0F,  0.0F,   0.0F    ));
    world->draw(0.0F);
    System::renderer()->screenshot(src_tex[MINUS_X], Rectangle(0,SRC_SIZE,0,SRC_SIZE));
    PNGWriter::write(FilePath("minus_x.png"), src_tex[MINUS_X]);


    // Draw +y
    camera->set_orientation( Matrix3(   -1.0F,  0.0F,   0.0F,
                                        0.0F,   0.0F,   -1.0F,
                                        0.0F,   -1.0F,   0.0F    ));
    world->draw(0.0F);
    System::renderer()->screenshot(src_tex[PLUS_Y], Rectangle(0,SRC_SIZE,0,SRC_SIZE));
    PNGWriter::write(FilePath("plus_y.png"), src_tex[PLUS_Y]);

    // Draw -y
    camera->set_orientation( Matrix3(   1.0F,  0.0F,   0.0F,
                                        0.0F,   0.0F,  1.0F,
                                        0.0F,   1.0F,  0.0F    ));
    world->draw(0.0F);
    System::renderer()->screenshot(src_tex[MINUS_Y], Rectangle(0,SRC_SIZE,0,SRC_SIZE));
    PNGWriter::write(FilePath("minus_y.png"), src_tex[MINUS_Y]);


    // Draw +z
    camera->set_orientation( Matrix3(   -1.0F,  0.0F,   0.0F,
                                        0.0F,   1.0F,   0.0F,
                                        0.0F,   0.0F,   -1.0F    ));
    world->draw(0.0F);
    System::renderer()->screenshot(src_tex[PLUS_Z], Rectangle(0,SRC_SIZE,0,SRC_SIZE));
    PNGWriter::write(FilePath("plus_z.png"), src_tex[PLUS_Z]);

    // Draw -z
    camera->set_orientation( Matrix3(   1.0F,   0.0F,   0.0F,
                                        0.0F,   1.0F,   0.0F,
                                        0.0F,   0.0F,   1.0F    ));
    world->draw(0.0F);
    System::renderer()->screenshot(src_tex[MINUS_Z], Rectangle(0,SRC_SIZE,0,SRC_SIZE));
    PNGWriter::write(FilePath("minus_z.png"), src_tex[MINUS_Z]);


    //
    // Generate each of the faces of the resulting cube map
    //
    
    std::thread t1(std::bind(generate_maps, 0, gain, exponent, dst_size, num_samples, src_tex[0], dst_tex[0]));
    std::thread t2(std::bind(generate_maps, 1, gain, exponent, dst_size, num_samples, src_tex[1], dst_tex[1]));
    std::thread t3(std::bind(generate_maps, 2, gain, exponent, dst_size, num_samples, src_tex[2], dst_tex[2]));
    std::thread t4(std::bind(generate_maps, 3, gain, exponent, dst_size, num_samples, src_tex[3], dst_tex[3]));
    std::thread t5(std::bind(generate_maps, 4, gain, exponent, dst_size, num_samples, src_tex[4], dst_tex[4]));
    std::thread t6(std::bind(generate_maps, 5, gain, exponent, dst_size, num_samples, src_tex[5], dst_tex[5]));
    
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();
	
    PNGWriter::write(FilePath("final_plus_x.png"), dst_tex[PLUS_X]);
    PNGWriter::write(FilePath("final_minus_x.png"), dst_tex[MINUS_X]);
    PNGWriter::write(FilePath("final_plus_y.png"), dst_tex[PLUS_Y]);
    PNGWriter::write(FilePath("final_minus_y.png"), dst_tex[MINUS_Y]);
    PNGWriter::write(FilePath("final_plus_z.png"), dst_tex[PLUS_Z]);
    PNGWriter::write(FilePath("final_minus_z.png"), dst_tex[MINUS_Z]);


    // Finish up
	System::renderer()->set_viewport (x, y, width, height);
    
    Globals::set_global("SYS_GEN_IRRADIANCE", "0", DT3::Globals::VOLATILE);
    
    CommandResult cr;
    cr.set_message("Done generating Irradiance maps");
    cr.set_result(true);
    return cr;
}

//==============================================================================
//==============================================================================

CommandResult LightingTools_cmd::do_unit_LUT (CommandContext &ctx, const CommandParams &p)
{
    std::shared_ptr<TextureResource2D> tex = TextureResource2D::create();
    
    const DTint SIZE = 16;
    
    tex->allocate_rgba_textels(SIZE, SIZE*SIZE, false);

    for (DTint b = 0; b < SIZE; ++b) {
        for (DTint g = 0; g < SIZE; ++g) {
            for (DTint r = 0; r < SIZE; ++r) {
                
                Color4b c(  static_cast<DTfloat>(r) / (SIZE-1),
                            static_cast<DTfloat>(g) / (SIZE-1),
                            static_cast<DTfloat>(b) / (SIZE-1),
                            1.0F);
                tex->set_pixel(r, g + b*SIZE, c);
            }
        }
    }
    
    PNGWriter::write(FilePath("UnitLUT.png"), tex);

    CommandResult cr;
    cr.set_message("Done generating unit LUT");
    cr.set_result(true);
    return cr;

}

CommandResult LightingTools_cmd::do_unit_LUT2D (CommandContext &ctx, const CommandParams &p)
{
    std::shared_ptr<TextureResource2D> tex = TextureResource2D::create();
    
    const DTint SIZE = 128;
    
    tex->allocate_rgba_textels(SIZE, 1, false);

    for (DTint rgb = 0; rgb < SIZE; ++rgb) {
        Color4b c(  static_cast<DTfloat>(rgb) / (SIZE-1),
                    static_cast<DTfloat>(rgb) / (SIZE-1),
                    static_cast<DTfloat>(rgb) / (SIZE-1),
                    1.0F);
        tex->set_pixel(rgb, 0, c);
    }
    
    PNGWriter::write(FilePath("UnitLUT2D.png"), tex);

    CommandResult cr;
    cr.set_message("Done generating unit LUT");
    cr.set_result(true);
    return cr;

}

CommandResult LightingTools_cmd::do_unit_tex (CommandContext &ctx, const CommandParams &p)
{
    std::shared_ptr<TextureResource2D> tex = TextureResource2D::create();
    
    const DTint SIZE = 256;
    
    tex->allocate_rgba_textels(SIZE, SIZE, false);

    for (DTint g = 0; g < SIZE; ++g) {
        for (DTint r = 0; r < SIZE; ++r) {
            Color4b c(  static_cast<DTfloat>(r) / (SIZE-1),
                        static_cast<DTfloat>(g) / (SIZE-1),
                        0.0F, 1.0F);
            tex->set_pixel(r, g, c);
        }
    }
    
    PNGWriter::write(FilePath("UnitTex.png"), tex);

    CommandResult cr;
    cr.set_message("Done generating unit texture");
    cr.set_result(true);
    return cr;
}

//==============================================================================
//==============================================================================

} // DT3

#endif // DT3_COMMANDS

