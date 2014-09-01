#ifndef DT3_SCRIPTINGSUBTITLEDRIVER
#define DT3_SCRIPTINGSUBTITLEDRIVER
//==============================================================================
///	
///	File: ScriptingSubtitleDriver.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingBase.hpp"
#include "DT3Core/Types/FileBuffer/FilePath.hpp"
#include <vector>
#include <string>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class FilePath;
class Tokenizer;

//==============================================================================
/// Class
//==============================================================================

class ScriptingSubtitleDriver: public ScriptingBase {
		/// Switcher for strings.

    public:
        DEFINE_TYPE(ScriptingSubtitleDriver,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingSubtitleDriver     (void);	
									ScriptingSubtitleDriver     (const ScriptingSubtitleDriver &rhs);
        ScriptingSubtitleDriver&    operator =                  (const ScriptingSubtitleDriver &rhs);	
        virtual                     ~ScriptingSubtitleDriver    (void);
    
        virtual void                archive                     (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize                  (void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute                     (const PlugBase *plug);

		/// Get the subtitles file
		/// \return subtitles file
        const FilePath&             subtitles                   (void) const   {   return _subtitles.as_ref();  }

		/// Set the subtitles file
		/// \param subtitles subtitles
        void                        set_subtitles               (const FilePath& subtitles);

	private:	
        void                        parse_subtitle_block        (Tokenizer &tokenizer);
        void                        parse_subtitles_block       (Tokenizer &tokenizer);
        void                        import                      (const FilePath &pathname);
      	
        struct Subtitle {
            DTfloat                 _time;
            std::string             _text;
        };
        
        std::vector<Subtitle>       _subtitle_array;
    
		Plug<FilePath>              _subtitles;
		Plug<DTfloat>               _time;

		Plug<std::string>			_out;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
