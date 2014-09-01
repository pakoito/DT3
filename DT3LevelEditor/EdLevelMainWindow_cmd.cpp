//==============================================================================
///	
///	File: EdLevelMainWindow_cmd.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/System/Command.hpp"

#ifdef DT3_COMMANDS

// Editor include
// Qt include
#include <QtWidgets/QApplication>
#include <QtGui/QClipboard>
#include <QtCore/QMimeData>

// Engine includes
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/Types/Utility/CommandRegistry.hpp"
#include "DT3Core/Types/Utility/CommandResult.hpp"
#include "DT3Core/Types/Utility/CommandContext.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/Types/Node/PlugNodeUtils.hpp"
#include "DT3Core/Types/Node/Group.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveBinaryBufferWriter.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveBinaryBufferReader.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveObjectQueue.hpp"
#include "DT3Core/Types/FileBuffer/FilePath.hpp"
#include "DT3Core/World/World.hpp"
#include "DT3Core/World/WorldNode.hpp"
#include DT3_HAL_INCLUDE_PATH


//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

class EdLevelMainWindow_cmd: public Command {
    public:
        DEFINE_TYPE(EdLevelMainWindow_cmd,Command);
        DEFINE_CREATE
        
        void register_commands (void) {
            CommandRegistry::register_command("Cut", &EdLevelMainWindow_cmd::do_cut);
            CommandRegistry::register_command("Copy", &EdLevelMainWindow_cmd::do_copy);
            CommandRegistry::register_command("Paste", &EdLevelMainWindow_cmd::do_paste);
            CommandRegistry::register_command("OpenInEditor", &EdLevelMainWindow_cmd::do_open_in_editor);
            CommandRegistry::register_command("TestAssert", &EdLevelMainWindow_cmd::do_test_assert);
        }

        static CommandResult do_cut (CommandContext &ctx, const CommandParams &p);
        static CommandResult do_copy (CommandContext &ctx, const CommandParams &p);
        static CommandResult do_paste (CommandContext &ctx, const CommandParams &p);
        static CommandResult do_open_in_editor (CommandContext &ctx, const CommandParams &p);
        static CommandResult do_clear (CommandContext &ctx, const CommandParams &p);
        static CommandResult do_test_assert (CommandContext &ctx, const CommandParams &p);
};

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_COMMAND(EdLevelMainWindow_cmd)

//==============================================================================
//==============================================================================

CommandResult EdLevelMainWindow_cmd::do_cut (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 1) {
        return CommandResult(false, "Usage: Cut", CommandResult::UPDATE_NONE);
    }
    
    do_copy(ctx,p);
    do_clear(ctx,p);

    return CommandResult(true, "Cut", CommandResult::UPDATE_SCRIPT | CommandResult::UPDATE_SOUND | CommandResult::UPDATE_WORLD | CommandResult::UPDATE_HIERARCHY | CommandResult::UPDATE_PROPERTIES | CommandResult::UPDATE_ANIMATION);
}

//==============================================================================
//==============================================================================

CommandResult EdLevelMainWindow_cmd::do_copy (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 1) {
        return CommandResult(false, "Usage: Copy", CommandResult::UPDATE_NONE);
    }
    
    const std::list<std::shared_ptr<PlugNode>>& selection = ctx.selection();
    std::list<std::shared_ptr<PlugNode>> items_plugs;
    std::list<std::shared_ptr<WorldNode>> items_world;
    
    PlugNodeUtils::copy_nodes(selection, items_plugs);
    
    // Build a list of world nodes
    FOR_EACH (i,items_plugs) {
        std::shared_ptr<WorldNode> item_world = checked_cast<WorldNode>(*i);
        if (item_world) {
            items_world.push_back(item_world);
        }
    }
    
    std::shared_ptr<Group> group = Group::create();
    group->add_nodes(items_world);
    
    std::shared_ptr<ArchiveBinaryBufferWriter> writer = ArchiveBinaryBufferWriter::create();
    writer->set_ignore_streamable_flag(true);
    ArchiveObjectQueue::queue_out_tree(writer, group);

    QMimeData *data = new QMimeData();
    data->setData("application/Group", QByteArray( (const char *) writer->stream().buffer(), (DTuint) writer->stream().size()) );
    
    LOG_MESSAGE << "Copied " << writer->stream().size() << " bytes to clipboard";
    
    QApplication::clipboard()->setMimeData(data);
    
    return CommandResult(true, "Copy", CommandResult::UPDATE_NONE);
}

//==============================================================================
//==============================================================================

CommandResult EdLevelMainWindow_cmd::do_paste (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 1) {
        return CommandResult(false, "Usage: Paste", CommandResult::UPDATE_NONE);
    }
    
    const QMimeData *mime_data = QApplication::clipboard()->mimeData();
    if (!mime_data) {
        return CommandResult(true, "Paste", CommandResult::UPDATE_NONE);
    }

    QByteArray data = mime_data->data("application/Group");
    
    std::shared_ptr<ArchiveBinaryBufferReader> reader = ArchiveBinaryBufferReader::create();
    reader->stream().set_buffer( data.data(), data.length() );
    
    std::shared_ptr<Group> group = checked_cast<Group>(ArchiveObjectQueue::queue_in_tree(reader));
    if (!group)
        return CommandResult(false, "Not pasted", CommandResult::UPDATE_NONE);
        
    std::list<std::shared_ptr<WorldNode>> items_world = group->nodes();
    
    // Find center
    Vector3 center(0.0F,0.0F,0.0F);
    
    FOR_EACH (i, items_world) {
        center += (*i)->node_position();
    }
    
    center /= items_world.size();
    
    Vector2 delta = ctx.selection_rectangle().center() - Vector2(center);
    
    // Add nodes to world with an offset
    FOR_EACH (i, items_world) {
        (*i)->set_node_position( (*i)->node_position() + Vector3(delta) );
        ctx.world()->add_node_unique_name(*i);
    }
        
    group->remove_all_nodes();

    return CommandResult(true, "Paste", CommandResult::UPDATE_SCRIPT | CommandResult::UPDATE_SOUND | CommandResult::UPDATE_WORLD | CommandResult::UPDATE_HIERARCHY | CommandResult::UPDATE_PROPERTIES | CommandResult::UPDATE_ANIMATION);
}

//==============================================================================
//==============================================================================

CommandResult EdLevelMainWindow_cmd::do_open_in_editor (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 1) {
        return CommandResult(false, "Usage: OpenInEditor", CommandResult::UPDATE_NONE);
    }
    
    std::list<std::shared_ptr<PlugNode>> selection = ctx.selection();  // Copy the selection
    
    FOR_EACH (i,selection) {
        std::shared_ptr<WorldNode> node = checked_static_cast<WorldNode>(*i);
        if (!node)
            continue;
    
        HAL::launch_editor(FilePath(node->file()));

    }

    
    return CommandResult(true, "Opened", CommandResult::UPDATE_NONE);
}

//==============================================================================
//==============================================================================

CommandResult EdLevelMainWindow_cmd::do_clear (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 1) {
        return CommandResult(false, "Usage: Clear", CommandResult::UPDATE_NONE);
    }

    std::list<std::shared_ptr<PlugNode>> selection = ctx.selection();  // Copy the selection
    
    FOR_EACH (i,selection) {
        std::shared_ptr<WorldNode> node = checked_static_cast<WorldNode>(*i);
        if (!node)
            continue;

        ctx.world()->remove_node(node);
    }
    
    ctx.world()->clean();

    // Change the selection
    std::list<std::shared_ptr<PlugNode>> new_selection;
    
    ctx.clear_selection();
    ctx.set_selection(new_selection);


    return CommandResult(true, "Clear", CommandResult::UPDATE_SCRIPT | CommandResult::UPDATE_SOUND | CommandResult::UPDATE_WORLD | CommandResult::UPDATE_HIERARCHY | CommandResult::UPDATE_PROPERTIES | CommandResult::UPDATE_ANIMATION);
}

//==============================================================================
//==============================================================================

CommandResult EdLevelMainWindow_cmd::do_test_assert (CommandContext &ctx, const CommandParams &p)
{
    for (DTuint i = 0; i < 10; ++i) {
        ASSERT(false);
    }

    for (DTuint i = 0; i < 10; ++i) {
        ASSERT(false);
    }

    return CommandResult(true, "TestAssert", CommandResult::UPDATE_NONE);
}

//==============================================================================
//==============================================================================

} // DT3

#endif // DT3_COMMANDS

