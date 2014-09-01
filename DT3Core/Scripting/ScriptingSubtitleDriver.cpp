//==============================================================================
///	
///	File: ScriptingSubtitleDriver.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingSubtitleDriver.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/Utility/Tokenizer.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingSubtitleDriver,"Subtitles",NULL)
IMPLEMENT_PLUG_NODE(ScriptingSubtitleDriver)

IMPLEMENT_PLUG_INFO_INDEX(_subtitles)
IMPLEMENT_PLUG_INFO_INDEX(_time)
IMPLEMENT_PLUG_INFO_INDEX(_out)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingSubtitleDriver)

	PLUG_INIT(_subtitles,"Subtitles")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_time,"Time")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingSubtitleDriver::ScriptingSubtitleDriver (void)
    :   _subtitles      (PLUG_INFO_INDEX(_subtitles)),
		_time           (PLUG_INFO_INDEX(_time)),
		_out			(PLUG_INFO_INDEX(_out))
{  

}
		
ScriptingSubtitleDriver::ScriptingSubtitleDriver (const ScriptingSubtitleDriver &rhs)
    :   ScriptingBase	(rhs),
		_subtitles      (rhs._subtitles),
		_time           (rhs._time),
		_out			(rhs._out)
{   

}

ScriptingSubtitleDriver & ScriptingSubtitleDriver::operator = (const ScriptingSubtitleDriver &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_subtitles = rhs._subtitles;
		_time = rhs._time;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingSubtitleDriver::~ScriptingSubtitleDriver (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingSubtitleDriver::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingSubtitleDriver::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	
    *archive << ARCHIVE_DATA_ACCESSORS("Subtitles", ScriptingSubtitleDriver::subtitles, ScriptingSubtitleDriver::set_subtitles, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_time, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT);
        					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingSubtitleDriver::parse_subtitle_block (Tokenizer &tokenizer)
{
	tokenizer.assume_next_token("{");
    
    Subtitle s;

	while (true) {
		std::string token = tokenizer.next_token_string_no_substitute();
		
		// Handle Preprocessor
		if (tokenizer.parse_preprocessor_macros(token))
			continue;
	
		// Are we at the end of the block
		if (token == "}")	break;
		
		if (MoreStrings::iequals(token,"time"))       {	tokenizer.assume_next_token("=");     s._time = tokenizer.next_token_number();                  continue;   }
		if (MoreStrings::iequals(token,"text"))       {	tokenizer.assume_next_token("=");     s._text = tokenizer.next_token_string_no_substitute();    continue;   }
		
		tokenizer.syntax_error("Unknown Token " + token);
	};
    
    _subtitle_array.push_back(s);
}

void ScriptingSubtitleDriver::parse_subtitles_block (Tokenizer &tokenizer)
{
	tokenizer.assume_next_token("{");

	while (true) {
		std::string token = tokenizer.next_token_string_no_substitute();
		
		// Handle Preprocessor
		if (tokenizer.parse_preprocessor_macros(token))
			continue;
	
		// Are we at the end of the block
		if (token == "}")	break;
		
		if (MoreStrings::iequals(token,"subtitle"))   {	parse_subtitle_block(tokenizer);    continue;	}
		
		tokenizer.syntax_error("Unknown Token " + token);
	};
}

//==============================================================================
//==============================================================================

void ScriptingSubtitleDriver::set_subtitles (const FilePath& subtitles)
{
    _subtitles = subtitles;

    _subtitle_array.clear();

	Tokenizer tokenizer;
	tokenizer.load_token_stream (subtitles);
	
	while (!tokenizer.is_done()) {
		std::string token = tokenizer.next_token_string_no_substitute();
		
		// Handle Preprocessor
		if (tokenizer.parse_preprocessor_macros(token))
			continue;
		
		if (MoreStrings::iequals(token,"subtitles"))  {	parse_subtitles_block(tokenizer);	continue;	}
	};

}

//==============================================================================
//==============================================================================

DTboolean ScriptingSubtitleDriver::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out) {

        (*_out).clear();
        
        if (_subtitle_array.size() == 0) {
            _out = "";
        } else if (_time < _subtitle_array[0]._time) {
            _out = _subtitle_array[0]._text;
        } else if (_time > _subtitle_array[_subtitle_array.size() - 1]._time) {
            _out = _subtitle_array[_subtitle_array.size() - 1]._text;
        } else {
        
            for (DTint i = 0; i < (DTint) _subtitle_array.size() - 1; ++i) {
                if (_time >= _subtitle_array[i]._time && _time < _subtitle_array[i+1]._time) {
                    _out = _subtitle_array[i]._text;
                    break;
                }        
            }
            
        }

		_out.set_clean();
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3

