//==============================================================================
///	
///	File: ScriptingConversions.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingConversions.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/Types/Math/Matrix2.hpp"
#include "DT3Core/Types/Math/Matrix3.hpp"
#include "DT3Core/Types/Math/Matrix4.hpp"
#include "DT3Core/Types/Math/Vector2.hpp"
#include "DT3Core/Types/Math/Vector3.hpp"
#include "DT3Core/Types/Math/Color4f.hpp"
#include <cmath>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

    
#define BUILD_CONVERSION_FRONT(FROMTYPE,FT,TOTYPE,TT,DEFAULTFT,DEFAULTTT)           \
namespace {                                                                         \
class Scripting##FROMTYPE##To##TOTYPE: public ScriptingBase {                       \
    public:                                                                         \
        DEFINE_TYPE(Scripting##FROMTYPE##To##TOTYPE,ScriptingBase)                  \
		DEFINE_CREATE_AND_CLONE                                                     \
		DEFINE_PLUG_NODE                                                            \
                                                                                    \
                                            Scripting##FROMTYPE##To##TOTYPE     (void);                                             \
                                            Scripting##FROMTYPE##To##TOTYPE     (const Scripting##FROMTYPE##To##TOTYPE &rhs);       \
        Scripting##FROMTYPE##To##TOTYPE&    operator =                          (const Scripting##FROMTYPE##To##TOTYPE &rhs);       \
        virtual                             ~Scripting##FROMTYPE##To##TOTYPE    (void) {}                                           \
                                                                                    \
        virtual void                archive             (const std::shared_ptr<Archive> &archive); \
                                                                                    \
	public:                                                                         \
 		DTboolean					compute					(const PlugBase *plug); \
	private:                                                                        \
		Plug<FT>                    _in;                                            \
		Plug<TT>                    _out;                                           \
        static DTushort             _inIndex;                                       \
        static DTushort             _outIndex;                                      \
};                                                                                  \
                                                                                    \
IMPLEMENT_FACTORY_CREATION_SCRIPT(Scripting##FROMTYPE##To##TOTYPE,"Conversion",NULL)\
IMPLEMENT_PLUG_NODE(Scripting##FROMTYPE##To##TOTYPE)                                \
                                                                                    \
DTushort Scripting##FROMTYPE##To##TOTYPE::_inIndex = PlugInfo::get_free_index();    \
DTushort Scripting##FROMTYPE##To##TOTYPE::_outIndex = PlugInfo::get_free_index();   \
                                                                                    \
BEGIN_IMPLEMENT_PLUGS(Scripting##FROMTYPE##To##TOTYPE)                              \
	PLUG_INIT(_in,"In")                                                             \
		.set_input(true)                                                            \
		.affects(_outIndex);                                                        \
	PLUG_INIT(_out,"Out")                                                           \
		.set_output(true);                                                          \
END_IMPLEMENT_PLUGS                                                                 \
                                                                                    \
Scripting##FROMTYPE##To##TOTYPE::Scripting##FROMTYPE##To##TOTYPE (void)             \
    :   _in			(_inIndex, DEFAULTFT),                                          \
		_out		(_outIndex, DEFAULTTT)                                          \
{}                                                                                  \
                                                                                    \
Scripting##FROMTYPE##To##TOTYPE::Scripting##FROMTYPE##To##TOTYPE                    \
    (const Scripting##FROMTYPE##To##TOTYPE &rhs)                                    \
    :   ScriptingBase	(rhs),                                                      \
		_in				(rhs._in),                                                  \
		_out			(rhs._out)                                                  \
{}                                                                                  \
                                                                                    \
Scripting##FROMTYPE##To##TOTYPE & Scripting##FROMTYPE##To##TOTYPE::                 \
    operator = (const Scripting##FROMTYPE##To##TOTYPE &rhs)                         \
{                                                                                   \
    if (&rhs != this) {                                                             \
		ScriptingBase::operator = (rhs);                                            \
		_in = rhs._in;                                                              \
		_out = rhs._out;                                                            \
	}                                                                               \
    return (*this);                                                                 \
}                                                                                   \
                                                                                    \
void Scripting##FROMTYPE##To##TOTYPE::archive (const std::shared_ptr<Archive> &archive)    \
{                                                                                   \
    ScriptingBase::archive(archive);                                           \
	archive->push_domain (class_id ());                                             \
	*archive << ARCHIVE_PLUG(_in, DATA_PERSISTENT | DATA_SETTABLE);                 \
	*archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT);                                \
    archive->pop_domain ();                                                         \
}                                                                                   \
                                                                                    \
DTboolean Scripting##FROMTYPE##To##TOTYPE::compute (const PlugBase *plug)           \
{                                                                                   \
	PROFILER(SCRIPTING);                                                            \
    if (super_type::compute(plug))  return true;                                    \
	if (plug == &_out) {                                                            


#define BUILD_CONVERSION_BACK(FROMTYPE,FT,TOTYPE,TT,DEFAULTFT,DEFAULTTT)            \
		_out.set_clean();                                                           \
		return true;                                                                \
	}                                                                               \
	return false;                                                                   \
}                                                                                   \
}   // namespace

//==============================================================================
//==============================================================================

#define BUILD_CONVERSION_NUMBER(FROMTYPE,FT,TOTYPE,TT,DEFAULTFT,DEFAULTTT)                      \
        BUILD_CONVERSION_FRONT(FROMTYPE,FT,TOTYPE,TT,DEFAULTFT,DEFAULTTT)                       \
		_out = static_cast<TT>(_in);                                                            \
        BUILD_CONVERSION_BACK(FROMTYPE,FT,TOTYPE,TT,DEFAULTFT,DEFAULTTT)                        

#define BUILD_CONVERSION_TO_VECTOR3(FROMTYPE,FT,DEFAULTFT)                                      \
        BUILD_CONVERSION_FRONT(FROMTYPE,FT,Vector3,Vector3,DEFAULTFT,Vector3(0.0F,0.0F,0.0F))   \
		_out = Vector3(*_in);                                                                   \
        BUILD_CONVERSION_BACK(FROMTYPE,FT,Vector3,Vector3,DEFAULTFT,Vector3(0.0F,0.0F,0.0F))                        

#define BUILD_CONVERSION_TO_STRING(FROMTYPE,FT,DEFAULTFT)                                       \
        BUILD_CONVERSION_FRONT(FROMTYPE,FT,String,std::string,DEFAULTFT,"")                     \
		_out = MoreStrings::cast_to_string(*_in);                                               \
        BUILD_CONVERSION_BACK(FROMTYPE,FT,String,std::string,DEFAULTFT,"")
    
#define BUILD_CONVERSION_FROM_STRING(TOTYPE,TT,DEFAULTTT)                                       \
        BUILD_CONVERSION_FRONT(String,std::string,TOTYPE,TT,"",DEFAULTTT)                       \
		_out = MoreStrings::cast_from_string<TT>(*_in);                                         \
        BUILD_CONVERSION_BACK(String,std::string,TOTYPE,TT,"",DEFAULTTT)                        

//==============================================================================
//==============================================================================

BUILD_CONVERSION_NUMBER (Bool,DTboolean,Short,DTshort,false,0)
BUILD_CONVERSION_NUMBER (Bool,DTboolean,UShort,DTushort,false,0)
BUILD_CONVERSION_NUMBER (Bool,DTboolean,Int,DTint,false,0L)
BUILD_CONVERSION_NUMBER (Bool,DTboolean,UInt,DTuint,false,0UL)
BUILD_CONVERSION_NUMBER (Bool,DTboolean,Float,DTfloat,false,0.0F)
BUILD_CONVERSION_NUMBER (Bool,DTboolean,Double,DTdouble,false,0.0)

BUILD_CONVERSION_NUMBER (Short,DTshort,UShort,DTushort,0,0)
BUILD_CONVERSION_NUMBER (Short,DTshort,Bool,DTboolean,0,false)
BUILD_CONVERSION_NUMBER (Short,DTshort,Int,DTint,0,0L)
BUILD_CONVERSION_NUMBER (Short,DTshort,UInt,DTuint,0,0UL)
BUILD_CONVERSION_NUMBER (Short,DTshort,Float,DTfloat,0,0.0F)
BUILD_CONVERSION_NUMBER (Short,DTshort,Double,DTdouble,0,0.0)

BUILD_CONVERSION_NUMBER (UShort,DTushort,Bool,DTboolean,0U,false)
BUILD_CONVERSION_NUMBER (UShort,DTushort,Short,DTshort,0U,0)
BUILD_CONVERSION_NUMBER (UShort,DTushort,Int,DTint,0U,0L)
BUILD_CONVERSION_NUMBER (UShort,DTushort,UInt,DTuint,0U,0UL)
BUILD_CONVERSION_NUMBER (UShort,DTushort,Float,DTfloat,0U,0.0F)
BUILD_CONVERSION_NUMBER (UShort,DTushort,Double,DTdouble,0U,0.0)

BUILD_CONVERSION_NUMBER (Int,DTint,Bool,DTboolean,0L,false)
BUILD_CONVERSION_NUMBER (Int,DTint,Short,DTshort,0L,0)
BUILD_CONVERSION_NUMBER (Int,DTint,UShort,DTushort,0L,0)
BUILD_CONVERSION_NUMBER (Int,DTint,UInt,DTuint,0L,0UL)
BUILD_CONVERSION_NUMBER (Int,DTint,Float,DTfloat,0L,0.0F)
BUILD_CONVERSION_NUMBER (Int,DTint,Double,DTdouble,0L,0.0)

BUILD_CONVERSION_NUMBER (UInt,DTuint,Bool,DTboolean,0UL,false)
BUILD_CONVERSION_NUMBER (UInt,DTuint,Short,DTshort,0UL,0)
BUILD_CONVERSION_NUMBER (UInt,DTuint,UShort,DTushort,0UL,0)
BUILD_CONVERSION_NUMBER (UInt,DTuint,Int,DTint,0UL,0L)
BUILD_CONVERSION_NUMBER (UInt,DTuint,Float,DTfloat,0UL,0.0F)
BUILD_CONVERSION_NUMBER (UInt,DTuint,Double,DTdouble,0UL,0.0)

BUILD_CONVERSION_NUMBER (Float,DTfloat,Bool,DTboolean,0.0F,false)
BUILD_CONVERSION_NUMBER (Float,DTfloat,Short,DTshort,0.0F,0)
BUILD_CONVERSION_NUMBER (Float,DTfloat,UShort,DTushort,0.0F,0)
BUILD_CONVERSION_NUMBER (Float,DTfloat,Int,DTint,0.0F,0L)
BUILD_CONVERSION_NUMBER (Float,DTfloat,UInt,DTuint,0.0F,0UL)
BUILD_CONVERSION_NUMBER (Float,DTfloat,Double,DTdouble,0.0F,0.0)

BUILD_CONVERSION_NUMBER (Double,DTdouble,Bool,DTboolean,0.0,false)
BUILD_CONVERSION_NUMBER (Double,DTdouble,Short,DTshort,0.0,0)
BUILD_CONVERSION_NUMBER (Double,DTdouble,UShort,DTushort,0.0,0)
BUILD_CONVERSION_NUMBER (Double,DTdouble,Int,DTint,0.0,0L)
BUILD_CONVERSION_NUMBER (Double,DTdouble,UInt,DTuint,0.0,0UL)
BUILD_CONVERSION_NUMBER (Double,DTdouble,Float,DTfloat,0.0,0.0F)

BUILD_CONVERSION_TO_VECTOR3(Float,DTfloat,0.0F)

BUILD_CONVERSION_TO_STRING(Bool,bool,false)
BUILD_CONVERSION_TO_STRING(Short,DTshort,0)
BUILD_CONVERSION_TO_STRING(UShort,DTushort,0U)
BUILD_CONVERSION_TO_STRING(Int,DTint,0L)
BUILD_CONVERSION_TO_STRING(UInt,DTuint,0UL)
BUILD_CONVERSION_TO_STRING(Float,DTfloat,0.0F)
BUILD_CONVERSION_TO_STRING(Double,DTdouble,0.0)
BUILD_CONVERSION_TO_STRING(Vector2,Vector2,Vector2(0.0F,0.0F))
BUILD_CONVERSION_TO_STRING(Vector3,Vector3,Vector3(0.0F,0.0F,0.0F))
BUILD_CONVERSION_TO_STRING(Matrix2,Matrix2,Matrix2(Matrix2::identity()))
BUILD_CONVERSION_TO_STRING(Matrix3,Matrix3,Matrix3(Matrix3::identity()))
BUILD_CONVERSION_TO_STRING(Matrix4,Matrix4,Matrix4(Matrix4::identity()))
BUILD_CONVERSION_TO_STRING(Color4f,Color4f,Color4f(1.0F,1.0F,1.0F,1.0F))

BUILD_CONVERSION_FROM_STRING(Bool,bool,false)
BUILD_CONVERSION_FROM_STRING(Short,DTshort,0)
BUILD_CONVERSION_FROM_STRING(UShort,DTushort,0U)
BUILD_CONVERSION_FROM_STRING(Int,DTint,0L)
BUILD_CONVERSION_FROM_STRING(UInt,DTuint,0UL)
BUILD_CONVERSION_FROM_STRING(Float,DTfloat,0.0F)
BUILD_CONVERSION_FROM_STRING(Double,DTdouble,0.0)
BUILD_CONVERSION_FROM_STRING(Vector2,Vector2,Vector2(0.0F,0.0F))
BUILD_CONVERSION_FROM_STRING(Vector3,Vector3,Vector3(0.0F,0.0F,0.0F))
BUILD_CONVERSION_FROM_STRING(Matrix2,Matrix2,Matrix2(Matrix2::identity()))
BUILD_CONVERSION_FROM_STRING(Matrix3,Matrix3,Matrix3(Matrix3::identity()))
BUILD_CONVERSION_FROM_STRING(Matrix4,Matrix4,Matrix4(Matrix4::identity()))
BUILD_CONVERSION_FROM_STRING(Color4f,Color4f,Color4f(1.0F,1.0F,1.0F,1.0F))

//==============================================================================
//==============================================================================

} // DT3

