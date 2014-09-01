#ifndef DT3_TYPETRAITS
#define DT3_TYPETRAITS
//==============================================================================
///	
///	File: BaseInclude.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include <string>
#include <memory>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

namespace TypeTraitsInfo {

//==============================================================================
// Abstract classes disable "default_value"
//==============================================================================

template<bool B, class U = void>
struct default_value_handler {
};
 
template<class U>
struct default_value_handler<true, U> {
	static U                    default_value(void) {	return U();						}
};

template <typename U> struct Info: public default_value_handler<!std::is_abstract<U>::value, U> {
	static const DTcharacter*	name(void)          {	return U::class_id_static();	}
	static const DTcharacter*	name_caps(void)     {	return U::class_id_static();	}
    enum { isFundamental = 0 };
};

//==============================================================================
//==============================================================================

template <typename U> struct Info<U*> {
	static U*                   default_value(void) {	return (U*) 0;					}
	static const DTcharacter*	name(void)          {	return U::class_id_static();	}
	static const DTcharacter*	name_caps(void)     {	return U::class_id_static();	}
    enum { isFundamental = 0 };
};

template <typename U> struct Info<std::shared_ptr<U>> {
	static std::shared_ptr<U>	default_value(void) {	return NULL;					}
	static const DTcharacter*	name(void)          {	return U::class_id_static();	}
	static const DTcharacter*	name_caps(void)     {	return U::class_id_static();	}
    enum { isFundamental = 0 };
};

template <> struct Info<void> {
	static void					default_value(void)	{	return;			}
	static const DTcharacter*	name(void)          {	return "void";	}
	static const DTcharacter*	name_caps(void)     {	return "Void";	}
    enum { isFundamental = 1 };
};

template <> struct Info<DTboolean> {
	static DTboolean			default_value(void)	{	return false;	}
	static const DTcharacter*	name(void)          {	return "bool";	}
	static const DTcharacter*	name_caps(void)     {	return "Bool";	}
    enum { isFundamental = 1 };
};

template <> struct Info<DTbyte> {
	static DTbyte				default_value(void)	{	return 0;		}
	static const DTcharacter*	name(void)          {	return "byte";	}
	static const DTcharacter*	name_caps(void)     {	return "Byte";	}
    enum { isFundamental = 1 };
};

template <> struct Info<DTshort> {
	static DTshort				default_value(void)	{	return 0;		}
	static const DTcharacter*	name(void)          {	return "short";	}
	static const DTcharacter*	name_caps(void)     {	return "Short";	}
    enum { isFundamental = 1 };
};

template <> struct Info<DTint> {
	static DTint				default_value(void)	{	return 0;		}
	static const DTcharacter*	name(void)          {	return "int";	}
	static const DTcharacter*	name_caps(void)     {	return "Int";	}
    enum { isFundamental = 1 };
};

template <> struct Info<DTint64> {
	static DTint64				default_value(void)	{	return 0;		}
	static const DTcharacter*	name(void)          {	return "int64";	}
	static const DTcharacter*	name_caps(void)     {	return "Int64";	}
    enum { isFundamental = 1 };
};

#if DT3_SIZES_ARE_DISTINCT_TYPES
template <> struct Info<DTsize> {
	static DTsize				default_value(void)	{	return 0;		}
	static const DTcharacter*	name(void)          {	return "size";	}
	static const DTcharacter*	name_caps(void)     {	return "Size";	}
    enum { isFundamental = 1 };
};
#endif

template <> struct Info<DTubyte> {
	static DTubyte				default_value(void)	{	return 0;		}
	static const DTcharacter*	name(void)          {	return "ubyte";	}
	static const DTcharacter*	name_caps(void)     {	return "UByte";	}
    enum { isFundamental = 1 };
};

template <> struct Info<DTushort> {
	static DTushort				default_value(void)	{	return 0;		}
	static const DTcharacter*	name(void)          {	return "ushort";}
	static const DTcharacter*	name_caps(void)     {	return "UShort";}
    enum { isFundamental = 1 };
};

template <> struct Info<DTuint> {
	static DTuint				default_value(void)	{	return 0;		}
	static const DTcharacter*	name(void)          {	return "uint";	}
	static const DTcharacter*	name_caps(void)     {	return "UInt";	}
    enum { isFundamental = 1 };
};

template <> struct Info<DTuint64> {
	static DTuint64				default_value(void)	{	return 0;		}
	static const DTcharacter*	name(void)          {	return "uint64";}
	static const DTcharacter*	name_caps(void)     {	return "UInt64";}
    enum { isFundamental = 1 };
};

template <> struct Info<DTfloat> {
	static DTfloat				default_value(void)	{	return 0.0F;	}
	static const DTcharacter*	name(void)          {	return "float";	}
	static const DTcharacter*	name_caps(void)     {	return "Float";	}
    enum { isFundamental = 1 };
};

template <> struct Info<DTdouble> {
	static DTdouble				default_value(void)	{	return 0.0;		}
	static const DTcharacter*	name(void)          {	return "double";}
	static const DTcharacter*	name_caps(void)     {	return "Double";}
    enum { isFundamental = 1 };
};

template <> struct Info<DTcharacter> {
	static DTcharacter			default_value(void)	{	return '\0';	}
	static const DTcharacter*	name(void)          {	return "char";	}
	static const DTcharacter*	name_caps(void)     {	return "Char";	}
    enum { isFundamental = 1 };
};

template <> struct Info<std::string> {
	static std::string          default_value(void)	{	return "";		}
	static const DTcharacter*	name(void)          {	return "string";}
	static const DTcharacter*	name_caps(void)     {	return "String";}
    enum { isFundamental = 0 };
};

template <> struct Info<std::wstring> {
	static std::wstring         default_value(void)	{	return std::wstring();  }
	static const DTcharacter*	name(void)          {	return "wstring";}
	static const DTcharacter*	name_caps(void)     {	return "wString";}
    enum { isFundamental = 1 };
};

} // TypeTraitsDefaults

//==============================================================================
/// Unconst types
//==============================================================================

template <class U> struct UnConst
{
	typedef U Result;
	enum { isConst = 0 };
};
       
template <class U> struct UnConst<const U>
{
	typedef U Result;
	enum { isConst = 1 };
};

//==============================================================================
/// Unref types
//==============================================================================

template <class U> struct UnRef
{
	typedef U Result;
	enum { isRef = 0 };
};
       
template <class U> struct UnRef<U&>
{
	typedef U Result;
	enum { isRef = 1 };
};

//==============================================================================
/// Unptr types
//==============================================================================

template <class U> struct UnPtr
{
	typedef U Result;
	enum { isPtr = 0 };
};
       
template <class U> struct UnPtr<U*>
{
	typedef U Result;
	enum { isPtr = 1 };
};

//==============================================================================
/// Base types
//==============================================================================

template <class U> struct UnAll
{
	typedef U Result;
};
       
template <class U> struct UnAll<const U>
{
	typedef U Result;
};

template <class U> struct UnAll<U&>
{
	typedef U Result;
};

template <class U> struct UnAll<const U&>
{
	typedef U Result;
};

//==============================================================================
/// TypeTraits
//==============================================================================

template <class T>
class TypeTraits: public TypeTraitsInfo::Info<typename UnAll<T>::Result> {
	public:
		enum { isConst = UnConst<T>::isConst };
		enum { isRef = UnRef<T>::isRef };
		enum { isPtr = UnPtr<T>::isPtr };

		typedef typename UnConst<T>::Result NonConstType;
		typedef typename UnRef<T>::Result NonRefType;
		typedef typename UnPtr<T>::Result NonPtrType;
		
		typedef typename UnAll<T>::Result BaseType;

};

//==============================================================================
//==============================================================================

} // DT3

#endif


