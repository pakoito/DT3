#ifndef DT3_BASEINCLUDE
#define DT3_BASEINCLUDE
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

#include "DT3Core/Types/Memory/Memory.hpp"
#include "AppConfig.hpp"

#include <stdint.h>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

#define DT3_BINARY_TOKEN 0xDEADBEEF

//==============================================================================
/// Compiler checks
//==============================================================================

#define DT3_VISUALC 1
#define DT3_GCC 2

#if defined(__GNUC__)
	#define DT3_COMPILER  DT3_GCC
#endif

#if defined(_MSC_VER)
	#define DT3_COMPILER  DT3_VISUALC
#endif

//==============================================================================
/// CPU checks
//==============================================================================

#define DT3_INTEL 1
#define DT3_ARM 2

#if (defined(__i386__) || defined(__i386) || defined(__x86_64__) || defined(__ia64__))
	#define DT3_CPU	DT3_INTEL
#elif defined(__arm__) || defined(__arm)
	#define DT3_CPU DT3_ARM
#endif

//==============================================================================
/// Endian checks
//==============================================================================

#define DT3_LIL_ENDIAN	1234
#define DT3_BIG_ENDIAN	4321

#if (defined(__i386__) || defined(__i386)) || \
     defined(__ia64__) || defined(WIN32) || \
    (defined(__alpha__) || defined(__alpha)) || \
     defined(__arm__) || \
    (defined(__mips__) && defined(__MIPSEL__)) || \
     defined(__SYMBIAN32__) || \
     defined(__x86_64__) || \
     defined(__LITTLE_ENDIAN__) || \
     defined(DT3_FORCE_LITTLE_ENDIAN)
	#define DT3_BYTEORDER	DT3_LIL_ENDIAN
#else
	#define DT3_BYTEORDER	DT3_BIG_ENDIAN
#endif

//==============================================================================
/// OS checks
//==============================================================================

#define DT3_WINDOWS 1
#define DT3_MACOSX 2
#define DT3_LINUX 3
#define DT3_IOS 4
#define DT3_ANDROID 5

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
	#define DT3_OS	DT3_WINDOWS
#elif (defined(__APPLE__) && (DT3_CPU == DT3_ARM)) || TARGET_IPHONE_SIMULATOR
	#define DT3_OS	DT3_IOS
#elif defined(__APPLE__) && (DT3_CPU == DT3_INTEL)
	#define DT3_OS	DT3_MACOSX
#elif defined(ANDROID)
	#define DT3_OS	DT3_ANDROID
#elif defined(linux) || defined(__linux) || defined(__linux__)
	#define DT3_OS	DT3_LINUX
#else
	#error Platform not supported
#endif

//==============================================================================
// Pointer size
//==============================================================================

#define DT3_32_BIT 4
#define DT3_64_BIT 8

#if (defined(_LP64) || defined(__LP64__))
    #define DT3_POINTER_SIZE DT3_64_BIT
#else
    #define DT3_POINTER_SIZE DT3_32_BIT
#endif

//==============================================================================
/// Debug Flags
//==============================================================================

#if DT3_COMPILER == DT3_VISUALC
	#ifdef _DEBUG
		#define DT3_DEBUG 1
	#else
		#ifdef DT3_DEBUG
			#undef DT3_DEBUG
		#endif
	#endif
#endif

#if DT3_COMPILER == DT3_GCC
	#ifdef NDEBUG
		#ifdef DT3_DEBUG
			#undef DT3_DEBUG
		#endif
	#else
		#define DT3_DEBUG 1
	#endif
#endif

//==============================================================================
/// Restrict
//==============================================================================

#if defined(__GNUC__)
	#define DT3_RESTRICT  __restrict
#endif

#if defined(_MSC_VER)
	#define DT3_RESTRICT  __restrict
#endif

//==============================================================================
// Alignment
//==============================================================================

#if DT3_OS == DT3_ANDROID || DT3_OS == DT3_IOS
    #define DT3_ALIGN 16
#else
    #define DT3_ALIGN 32
#endif

//==============================================================================
/// Analytics
//==============================================================================

#ifndef DT3_USE_ANALYTICS
    #define DT3_USE_ANALYTICS 0
#endif

//==============================================================================
/// Allocation guards
//==============================================================================

#ifndef DT3_GUARD_ALLOCATIONS
    #define DT3_GUARD_ALLOCATIONS 0
#endif

//==============================================================================
/// Clear Graphics on suspend
//==============================================================================

#ifndef DT3_CLEAR_GRAPHICS_ON_SUSPEND
    #define DT3_CLEAR_GRAPHICS_ON_SUSPEND 1
#endif

//==============================================================================
/// Deprecated
//==============================================================================

#if DT3_COMPILER == DT3_VISUALC
	#define DT3_DEPRECATED __declspec(deprecated)
#endif

#if DT3_COMPILER == DT3_GCC
	#define DT3_DEPRECATED declspec(deprecated)
#endif

//==============================================================================
/// Profiler
//==============================================================================

#ifndef DT3_USE_PROFILER
    #if DT3_OS == DT3_ANDROID || DT3_OS == DT3_IOS
        #define DT3_USE_PROFILER 0
    #else
        #define DT3_USE_PROFILER 1
    #endif
#endif

//==============================================================================
/// Commands Framework
//==============================================================================

// Enable or Disable commands framework
#ifndef DT3_COMMANDS
    #if DT3_OS == DT3_ANDROID || DT3_OS == DT3_IOS
        #define DT3_COMMANDS 0
    #elif DT3_DEPLOYMENT
        #define DT3_COMMANDS 0
    #else
        #define DT3_COMMANDS 1
    #endif
#endif

//==============================================================================
/// Unit testing framework
//==============================================================================

#ifndef DT3_UNIT_TESTS
    #if DT3_OS == DT3_ANDROID || DT3_OS == DT3_IOS
        #define DT3_UNIT_TESTS 0
    #else
        #define DT3_UNIT_TESTS 1
    #endif
#endif

//==============================================================================
// Audio Parameters
//==============================================================================

#ifndef DT3_AUDIO_STREAMING_BUFFER_SIZE
#define DT3_AUDIO_STREAMING_BUFFER_SIZE (1024 * 16)     // size of streaming chunk
#endif

//==============================================================================
//==============================================================================

#if DT3_OS == DT3_IOS || DT3_OS == DT3_MACOSX
    #define DT3_HAL_INCLUDE_PATH "DT3Apple/HAL.hpp"
#elif DT3_OS == DT3_ANDROID
    #define DT3_HAL_INCLUDE_PATH "DT3Android/HAL.hpp"
#elif DT3_OS == DT3_WINDOWS
    #define DT3_HAL_INCLUDE_PATH "DT3Windows8/HAL.hpp"
#elif DT3_OS == DT3_LINUX
    #define DT3_HAL_INCLUD_PATH "DT3Linux/HAL.hpp"
#endif

//==============================================================================
// Thread local storage
//==============================================================================

#if DT3_OS == DT3_IOS || DT3_OS == DT3_ANDROID
    #define DT3_THREAD_LOCAL
#elif DT3_OS == DT3_MACOSX
    #define DT3_THREAD_LOCAL __thread
#else
    #define DT3_THREAD_LOCAL thread_local
#endif

//==============================================================================
// Default Pool sizes
//==============================================================================

#ifndef DT3_PLUG_POOL_SIZE
#define DT3_PLUG_POOL_SIZE 1024
#endif

#ifndef DT3_EVENT_POOL_SIZE
#define DT3_EVENT_POOL_SIZE 1024
#endif

#ifndef DT3_PLUG_CONNECTION_POOL_SIZE
#define DT3_PLUG_CONNECTION_POOL_SIZE 2048
#endif

#ifndef DT3_EVENT_CONNECTION_POOL_SIZE
#define DT3_EVENT_CONNECTION_POOL_SIZE 2048
#endif

//==============================================================================
//==============================================================================

typedef bool            DTboolean;
typedef int8_t          DTbyte;
typedef int16_t         DTshort;
typedef int32_t         DTint;
typedef uint8_t         DTubyte;
typedef uint16_t        DTushort;
typedef uint32_t        DTuint;
typedef float			DTfloat;
typedef double          DTdouble;
typedef	char            DTcharacter;
typedef	wchar_t         DTwcharacter;
typedef int             DTerr;
typedef int64_t         DTint64;
typedef uint64_t        DTuint64;
typedef DTint64         DTsize;
typedef DTint64         DToffset;
typedef long            DTlong;
typedef unsigned long   DTulong;

#define DTBYTE_MAX		(127)
#define DTBYTE_MIN		(-128)

#define DTSHORT_MAX		(32767)
#define DTSHORT_MIN		(-32768)

#define DTINT_MAX		(2147483647)
#define DTINT_MIN		(-2147483648)

#define DTUBYTE_MAX		(255)
#define DTUBYTE_MIN		(0)

#define DTUSHORT_MAX	(65535)
#define DTUSHORT_MIN	(0)

#define DTUINT_MAX		(4294967295U)
#define DTUINT_MIN		(0U)


// Make sure NULL exists
#ifndef NULL
#define NULL (0)
#endif

//==============================================================================
/// For class creation
//==============================================================================

// Class kind structure. Used for various class information
struct DTclasskind {
	DTclasskind(DTclasskind	*super_class, DTcharacter *class_name)
	:	super	(super_class),
		name	(class_name)
	{}
	
	DTclasskind	*super;
	DTcharacter	*name;
};

#define DEFINE_TYPE_BASE(cid)	\
	typedef cid type;                                                                                                                           \
	inline static DTcharacter*          class_id_static (void)         	{   return (DTcharacter*) #cid;                                     }   \
	inline virtual DTcharacter*         class_id_child (void) const		{   return class_id_static();                                       }   \
	inline DTcharacter*                 class_id (void) const         	{   return class_id_static();                                       }   \
	inline static DTclasskind*          kind (void)						{	static DTclasskind kind(NULL, class_id_static());                   \
                                                                            return &kind;                                                   }	\
	inline virtual DTboolean            isa (DTclasskind* k) const		{	return k == kind();                                             }   \
    inline virtual const DTcharacter*   file (void)                     {   return __FILE__;                                                }   \
    inline virtual DTsize               class_size (void) const         {   return static_cast<DTsize>(sizeof(type));                       }

#define DEFINE_TYPE(cid,sid) \
	typedef cid type;                                                                                                                           \
	typedef sid super_type;                                                                                                                     \
	inline static DTcharacter*          class_id_static (void)         	{   return (DTcharacter*) #cid;                                     }   \
	inline virtual DTcharacter*         class_id_child (void) const		{   return class_id_static();                                       }   \
	inline DTcharacter*                 class_id (void) const         	{   return class_id_static();                                       }   \
	inline static DTclasskind*          kind (void)						{	static DTclasskind kind(super_type::kind(), class_id_static());     \
                                                                            return &kind;                                                   }	\
	inline virtual DTboolean            isa (DTclasskind* k) const		{	DTclasskind *cur = kind();                                          \
                                                                            while (cur) {                                                       \
                                                                                if (cur == k)	return true;                                    \
                                                                                cur = cur->super;                                               \
                                                                            }                                                                   \
                                                                            return false;                                                       \
                                                                        }                                                                       \
    inline virtual const DTcharacter*   file (void)                     {   return __FILE__;                                                }   \
    inline virtual DTsize               class_size (void) const         {   return static_cast<DTsize>(sizeof(type));                       }
				
#define DEFINE_TYPE_SIMPLE_BASE(cid) \
	typedef cid type;                                                                                                                           \
	inline static DTcharacter*          class_id_static (void)         	{   return (DTcharacter*) #cid;                                     }   \
	inline DTcharacter*                 class_id (void) const         	{   return class_id_static();                                       }   \
	inline DTcharacter*                 class_id_child (void) const		{   return class_id_static();                                       }   \
	inline static DTclasskind*          kind (void)						{	static DTclasskind kind(NULL, class_id_static());                   \
                                                                            return &kind;                                                   }

#define DEFINE_TYPE_SIMPLE(cid,sid) \
	typedef cid type;                                                                                                                           \
	typedef sid super_type;                                                                                                                     \
	inline static DTcharacter*          class_id_static (void)         	{   return (DTcharacter*) #cid;                                     }   \
	inline DTcharacter*                 class_id (void) const         	{   return class_id_static();                                       }   \
	inline DTcharacter*                 class_id_child (void) const		{   return class_id_static();                                       }   \
	inline static DTclasskind*          kind (void)						{	static DTclasskind kind(NULL, class_id_static());                   \
                                                                            return &kind;                                                   }

	
#define DEFINE_CLONE\
	inline virtual std::shared_ptr<BaseClass>   clone(void) const		{	std::shared_ptr<type> obj = std::shared_ptr<type>(new type(*this),  \
                                                                                [](type *o) {                                                   \
                                                                                    o->uninitialize();                                          \
                                                                                    delete o;                                                   \
                                                                                }                                                               \
                                                                            );                                                                  \
																			obj->initialize();                                                  \
																			return obj;                                                         \
																		}															

#define DEFINE_CREATE\
	inline static std::shared_ptr<type>         create(void)            {	std::shared_ptr<type> obj = std::shared_ptr<type>(new type(),       \
                                                                                [](type *o) {                                                   \
                                                                                    o->uninitialize();                                          \
                                                                                    delete o;                                                   \
                                                                                }                                                               \
                                                                            );                                                                  \
																			obj->initialize();                                                  \
																			return obj;                                                         \
																		}

#define DEFINE_CREATE_AND_CLONE\
	DEFINE_CLONE\
	DEFINE_CREATE
	
//==============================================================================
/// Macro to build accessors
//==============================================================================

#define DEFINE_ACCESSORS(G,S,T,V)                               \
inline T G(void) const			{	return V;				}	\
inline void S(T var_)			{	V = var_;				}

#define DEFINE_ACCESSORS_STATIC(G,S,T,V)                        \
inline static T G(void)			{	return V;				}	\
inline static void S(T var_)    {	V = var_;				}

#define DEFINE_ACCESSORS_RANGED(G,S,T,V,LOW,HI)                 \
inline T G(void) const			{	return V;				}	\
inline void S(T var_)			{	if (var_ < LOW) V = LOW;    \
                                    else if (var_ > HI) V = HI; \
                                    else V = var_;          }

#define DEFINE_ACCESSORS_REFERENCED(G,S,T,V)                    \
inline T& G(void)               {	return V;				}	\
inline void S(const T &var_)	{	V = var_;				}

#define DEFINE_ACCESSORS_REFERENCED_CONST(G,S,T,V)              \
inline const T& G(void) const	{	return V;				}	\
inline void S(const T &var_)	{	V = var_;				}
	
//==============================================================================
/// Array size macro
//==============================================================================

//#define ARRAY_SIZE(a) (static_cast<DTint>(sizeof(a))/static_cast<DTint>(sizeof(a[0])))
template<typename T, size_t N>
constexpr size_t ARRAY_SIZE(T (&)[N]) { return N; }

//==============================================================================
/// Convert to string
/// See: http://www.c-faq.com/ansi/stringize.html
//==============================================================================

#define TO_STRING(s) #s
#define TO_EXPANDED_STRING(s) TO_STRING(s)

//==============================================================================
/// For each
//==============================================================================

#define FOR_EACH(I,C)    for (auto I = C.begin(); I != C.end(); ++I)

//==============================================================================
/// Global initialization macro
//==============================================================================

#define GLOBAL_INITIALIZATION(s) \
namespace { \
	struct initialization_class { \
		initialization_class() { \
			s; \
		} \
	} initialization_obj; \
}

#define GLOBAL_DESTRUCTION(s) \
namespace { \
	struct destruction_class { \
		~destruction_class() { \
			s; \
		} \
	} destruction_obj; \
}

#define GLOBAL_STATIC_INITIALIZATION(P,s) \
namespace { \
    struct initialization_class: public StaticInitializerCallback { \
        initialization_class() { \
            set_priority(P); \
        } \
        void initialize() { \
            s; \
        } \
    } initialization_obj; \
}

#define GLOBAL_STATIC_DESTRUCTION(P,s) \
namespace { \
    struct destruction_class: public StaticInitializerCallback { \
        destruction_class() { \
            set_priority(P); \
        } \
        void destroy() { \
            s; \
        } \
    } destruction_obj; \
}

//==============================================================================
// Error codes
//==============================================================================

extern const DTerr DT3_ERR_NONE;
extern const DTerr DT3_ERR_UNKNOWN;

extern const DTerr DT3_ERR_FILE_OPEN_FAILED;
extern const DTerr DT3_ERR_FILE_READ_FAILED;
extern const DTerr DT3_ERR_FILE_WRONG_TYPE;
extern const DTerr DT3_ERR_FILE_NOT_IN_PACKAGE;

extern const DTerr DT3_ERR_FACTORY_BAD_CLASS;
extern const DTerr DT3_ERR_ARCHIVE_TOO_NEW;

extern const DTerr DT3_ERR_TOKENIZE_FAILED;

extern const DTerr DT3_ERR_NET_WOULD_BLOCK;
extern const DTerr DT3_ERR_NET_UNKNOWN;

extern const DTerr DT3_ERR_THREAD_FAIL;

extern const DTerr DT3_ERR_NAT_SUCCESS;
extern const DTerr DT3_ERR_NAT_FAIL;

//==============================================================================
//==============================================================================

} //DT3

#endif


