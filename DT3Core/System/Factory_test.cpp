//==============================================================================
///	
///	File: Factory_test.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/System/UnitTest.hpp"

#if DT3_UNIT_TESTS

#include "DT3Core/System/Factory.hpp"
#include "DT3Core/World/WorldNode.hpp"
#include "DT3Core/World/World.hpp"

#include "DT3Core/Types/FileBuffer/ArchiveBinaryBufferWriter.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveObjectQueue.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveBinaryBufferReader.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
// Define the test
//==============================================================================

class Factory_test: public UnitTest {
    public:
        DEFINE_TYPE(Factory_test,UnitTest);
        DEFINE_CREATE
        
        virtual void run_test (void);
};

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_UNIT_TEST (Factory_test)

//==============================================================================
// Implement the test
//==============================================================================

extern std::map<std::string, std::shared_ptr<CreatorBase>>&	factory_map	(void);

void Factory_test::run_test (void)
{    
    std::shared_ptr<World> world = World::create();
    
    for (auto i = factory_map().begin(); i != factory_map().end(); ++i) {
        
        if (i->first.find("DeviceGraphics") != std::string::npos)
            continue;
        
        LOG_MESSAGE << "Creating " << i->first << "...";
        std::shared_ptr<BaseClass> b = Factory::create_object(i->first);
        
        LOG_MESSAGE << " Class size " << b->class_size() << " bytes for " << i->first;
        
        // Add the object to the world briefly
        if (b->isa(WorldNode::kind())) {
            std::shared_ptr<WorldNode> node = checked_static_cast<WorldNode>(b);
            
            world->add_node(node);
            world->remove_node(node);
            world->clean();
        }
        
        // We queue out the object into a binary stream and then read it in again
        std::shared_ptr<ArchiveBinaryBufferWriter> writer = ArchiveBinaryBufferWriter::create();
        ArchiveObjectQueue::queue_out_tree(writer, b);

        std::shared_ptr<ArchiveBinaryBufferReader> reader = ArchiveBinaryBufferReader::create();
        reader->stream().set_buffer( writer->stream().buffer(), writer->stream().size() );
        std::shared_ptr<BaseClass> bb = ArchiveObjectQueue::queue_in_tree(reader);
        
        if (writer->stream().p() != reader->stream().g()) {
            LOG_MESSAGE << " Warning: Streaming mismatch for object " << i->first;
        }
        
        // Testing the compute chain
        if (b->isa(PlugNode::kind())) {
            std::shared_ptr<PlugNode> p = checked_static_cast<PlugNode>(b);
            
            // NOTE: If this test fails, it means that the "compute" method in an
            // object is not properly calling its parent. Look for the line:
            //
            //    if (super_type::compute(plug))  return true;
            //
            // in each of the exiting nodes for an example.
        
            TEST_ASSERTION(p->compute(PlugNode::test_compute_chain_plug()) == true);
            
            if (!p->compute(PlugNode::test_compute_chain_plug())) {
                LOG_MESSAGE << "Fix me!";
            }
        }
        
    }
    
    // Tick and draw to make sure the world was cleaned up properly
    world->tick(1.0F);
    world->draw(0.0F);
    world->draw_diagnostics(0.0F);
}

//==============================================================================
//==============================================================================

} // DT3

#endif // DT_UNIT_TESTS

