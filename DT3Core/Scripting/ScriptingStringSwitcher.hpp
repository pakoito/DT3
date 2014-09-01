#ifndef DT3_SCRIPTINGSTRINGSWITCHER
#define DT3_SCRIPTINGSTRINGSWITCHER
//==============================================================================
///	
///	File: ScriptingStringSwitcher.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingBase.hpp"
#include <string>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Switcher for strings.
//==============================================================================

class ScriptingStringSwitcher: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingStringSwitcher,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingStringSwitcher				(void);	
									ScriptingStringSwitcher				(const ScriptingStringSwitcher &rhs);
        ScriptingStringSwitcher &				operator =				(const ScriptingStringSwitcher &rhs);	
        virtual                     ~ScriptingStringSwitcher			(void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

		DEFINE_ACCESSORS(string1, set_string1, std::string, _in_string1);
		DEFINE_ACCESSORS(string2, set_string2, std::string, _in_string2);
		DEFINE_ACCESSORS(string3, set_string3, std::string, _in_string3);
		DEFINE_ACCESSORS(string4, set_string4, std::string, _in_string4);
		DEFINE_ACCESSORS(string5, set_string5, std::string, _in_string5);
		DEFINE_ACCESSORS(string6, set_string6, std::string, _in_string6);
		DEFINE_ACCESSORS(string7, set_string7, std::string, _in_string7);
		DEFINE_ACCESSORS(string8, set_string8, std::string, _in_string8);
		DEFINE_ACCESSORS(string9, set_string9, std::string, _in_string9);
		DEFINE_ACCESSORS(string10, set_string10, std::string, _in_string10);
		DEFINE_ACCESSORS(string11, set_string11, std::string, _in_string11);
		DEFINE_ACCESSORS(string12, set_string12, std::string, _in_string12);
		DEFINE_ACCESSORS(string13, set_string13, std::string, _in_string13);
		DEFINE_ACCESSORS(string14, set_string14, std::string, _in_string14);
		DEFINE_ACCESSORS(string15, set_string15, std::string, _in_string15);
		DEFINE_ACCESSORS(string16, set_string16, std::string, _in_string16);

	private:		
		Plug<DTboolean>				_in1;
		Plug<DTboolean>				_in2;
		Plug<DTboolean>				_in3;
		Plug<DTboolean>				_in4;
		Plug<DTboolean>				_in5;
		Plug<DTboolean>				_in6;
		Plug<DTboolean>				_in7;
		Plug<DTboolean>				_in8;
		Plug<DTboolean>				_in9;
		Plug<DTboolean>				_in10;
		Plug<DTboolean>				_in11;
		Plug<DTboolean>				_in12;
		Plug<DTboolean>				_in13;
		Plug<DTboolean>				_in14;
		Plug<DTboolean>				_in15;
		Plug<DTboolean>				_in16;

		Plug<std::string>			_in_string1;
		Plug<std::string>			_in_string2;
		Plug<std::string>			_in_string3;
		Plug<std::string>			_in_string4;
		Plug<std::string>			_in_string5;
		Plug<std::string>			_in_string6;
		Plug<std::string>			_in_string7;
		Plug<std::string>			_in_string8;
		Plug<std::string>			_in_string9;
		Plug<std::string>			_in_string10;
		Plug<std::string>			_in_string11;
		Plug<std::string>			_in_string12;
		Plug<std::string>			_in_string13;
		Plug<std::string>			_in_string14;
		Plug<std::string>			_in_string15;
		Plug<std::string>			_in_string16;

		Plug<std::string>			_out;
        DTboolean                   _buffer_output;
		
};

//==============================================================================
//==============================================================================

} // DT3

#endif
