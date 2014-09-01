#ifndef DT3_ARCHIVEDATA
#define DT3_ARCHIVEDATA
//==============================================================================
///	
///	File: ArchiveData.hpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseClass.hpp"
#include "DT3Core/Types/Node/Plug.hpp"
#include "DT3Core/Types/Node/Event.hpp"
#include "DT3Core/Types/FileBuffer/Stream.hpp"
#include <string>
#include <vector>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class PlugBase;

//==============================================================================
/// Constants
//==============================================================================

extern const DTint DATA_PERSISTENT;
extern const DTint DATA_SETTABLE;

extern const DTint DATA_NET_SERVER_WRITE_CLIENT_READ;
extern const DTint DATA_NET_CLIENT_WRITE_SERVER_READ;
extern const DTint DATA_NET_CLIENT_SERVER_BIDIRECTIONAL;
extern const DTint DATA_NET_INHERITED;

extern const DTint DATA_FLUSH_UI;
extern const DTint DATA_DIRTY;

//==============================================================================
/// Class
//==============================================================================

class ArchiveData: public BaseClass {
    public:
        DEFINE_TYPE(ArchiveData,BaseClass)
		DEFINE_CLONE
         
                                        ArchiveData				(void);
                                        ArchiveData				(const ArchiveData &rhs);
        ArchiveData &                   operator =              (const ArchiveData &rhs);
		virtual                         ~ArchiveData			(void);
								
	public:
		/// Writes the value to a stream
		/// \param s stream
		virtual void                    value                   (Stream &s) const	{					}

		/// Reads the value from a stream
		/// \param s stream
		virtual void                    set_value				(Stream &s) const	{					}


		
		/// Returns plug
		/// \return plug
		virtual PlugBase*               plug                    (void) const			{	return NULL;	}

		/// Returns event
		/// \return event
		virtual Event*                  event                   (void) const			{	return NULL;	}
    

		/// Returns kind of data as a string
		/// \return data kind
		virtual const DTcharacter*      data_kind               (void) const			{	return NULL;	}
        
        
        //
        // Additional traits
        //
        
        /// Add an enumeration for pulldown menus
        /// \param s enumeration
        /// \return this
        ArchiveData&                    add_enum                (const std::string &s);
		
		/// Returns if this item is an enum
		/// \return is title
		virtual DTboolean               is_enum                 (void) const			{	return _enums.size() > 0;	}
        
        /// Returns enums
        const std::vector<std::string>& enums                   (void)                  {   return _enums;  }



        /// Add an enumeration for pulldown menus
        /// \param s enumeration
        /// \return this
        ArchiveData&                    add_range               (DTfloat range_min, DTfloat range_max);
		
		/// Returns if this item is an enum
		/// \return is title
		virtual DTboolean               is_range                (void) const			{	return (_range_max > _range_min);	}
        
        /// Returns minimum range
        DTfloat                         range_min               (void) const            {   return _range_min;  }

        /// Returns maximum range
        DTfloat                         range_max               (void) const            {   return _range_max;  }


        /// Add an enumeration for pulldown menus
        /// \param s enumeration
        /// \return this
        ArchiveData&                    set_bitfield            (DTboolean bitfield);
		
		/// Returns if this item is an enum
		/// \return is title
		virtual DTboolean               is_bitfield             (void) const			{	return _bitfield;	}

        
        /// Add a flag
        /// \param s enumeration
        /// \return this
        ArchiveData&                    add_flags               (DTuint flags);
		        
		/// Returns the flags for this data
		/// \return flags
		DTuint                          flags                   (void) const			{	return _flags;	}
        
    
    
		/// Sets the title of this data
		/// \param s title
        /// \return this data
		ArchiveData&                    set_title				(const std::string &s);

		/// Returns the title of this data
		/// \return title
		std::string                     title                   (void) const			{	return _title;	}


    protected:
		DTuint                          _flags;
		std::string                     _title;
    
        std::vector<std::string>        _enums;
        DTfloat                         _range_min;
        DTfloat                         _range_max;
    
        DTboolean                       _bitfield;
};

//==============================================================================
/// This class does the dirty work for the C++ fundamental types
//==============================================================================

template <typename T>
class ArchiveDataVariable: public ArchiveData {
	public:
        DEFINE_TYPE(ArchiveDataVariable,ArchiveData)
		DEFINE_CLONE
		
		ArchiveDataVariable (const ArchiveDataVariable& rhs)
			:	ArchiveData	(rhs),
				_data(rhs._data)
		{

		}
		
		/// Constructor if data is a variable
		/// \param title name of data
		/// \param data reference to data
		/// \param flags flags for data
		ArchiveDataVariable (std::string title, T &data, DTuint flags) 
			:	_data(&data)
		{
            set_title(title);
			add_flags(flags);
		}
		
	private:
		ArchiveDataVariable& operator = (const ArchiveDataVariable &rhs);

	public:
		
		
		/// Writes the value to a stream
		/// \param s stream
		virtual void                value                   (Stream &s) const {
			T &data = *_data;
			s << data;
		}
		
		/// Reads the value from a stream
		/// \param s stream
		virtual void                set_value				(Stream &s) const {
			s >> *_data;
		}

        /// Returns data kind
		virtual const DTcharacter*  data_kind               (void) const        {	return TypeTraits<T>::name();	}

	private:
		T       *_data;
};

// Template specialization to make sure this isn't used for pointers, refs, 
// or consts.

template <typename T>
class ArchiveDataVariable<T*> {};	// Does not handle reference counting properly

template <typename T>
class ArchiveDataVariable<T&> {};

template <typename T>
class ArchiveDataVariable<const T> {};

template <typename T>
class ArchiveDataVariable<const T&> {};

//==============================================================================
/// This class does the dirty work for the plug types
//==============================================================================

template <typename T>
class ArchiveDataPlug: public ArchiveData {
	public:
        DEFINE_TYPE(ArchiveDataPlug,ArchiveData)
		DEFINE_CLONE
		
		ArchiveDataPlug (const ArchiveDataPlug& rhs)
			:	ArchiveData	(rhs),
				_plug(rhs._plug)
		{

		}
		
		/// Constructor if data is a variable
		/// \param title name of data
		/// \param data reference to data
		/// \param flags flags for data
		ArchiveDataPlug (Plug<T> &plug, DTuint flags)
			:	_plug(&plug)
		{
            set_title(_plug->name());
			add_flags(flags);
		}
		
	private:
		ArchiveDataPlug& operator = (const ArchiveDataPlug &rhs);

	public:

		/// Writes the value to a stream
		/// \param s stream
		virtual void                value                   (Stream &s) const {
			T val = *_plug;
			s << val;
		}
		
		/// Reads the value from a stream
		/// \param s stream
		virtual void                set_value				(Stream &s) const {
			T val = TypeTraits<typename TypeTraits<T>::BaseType>::default_value();
			s >> val;
			*_plug = val;
		}
				
		/// Returns plug
		/// \return plug
		virtual PlugBase*           plug                    (void) const			{	return _plug;	}


        /// Returns data kind
		virtual const DTcharacter*  data_kind               (void) const			{	return TypeTraits<T>::name();	}

	private:
		Plug<T>   *_plug;
};

//==============================================================================
/// This class does the dirty work for the event types
//==============================================================================

class ArchiveDataEvent: public ArchiveData {
	public:
        DEFINE_TYPE(ArchiveDataEvent,ArchiveData)
		DEFINE_CLONE
		
		ArchiveDataEvent (const ArchiveDataEvent& rhs)
			:	ArchiveData	(rhs),
				_event(rhs._event)
		{

		}
		
		/// Constructor if data is a variable
		/// \param title name of data
		/// \param data reference to data
		/// \param flags flags for data
		ArchiveDataEvent (Event &event, DTuint flags) 
			:	_event  (&event)
		{
            set_title(_event->name());
			add_flags(flags);
		}
		
	private:
		ArchiveDataEvent& operator = (const ArchiveDataEvent &rhs);

	public:

		/// Writes the value to a stream
		/// \param s stream
		virtual void                value                   (Stream &s) const {
		}
		
		/// Reads the value from a stream
		/// \param s stream
		virtual void                set_value				(Stream &s) const {
		}
				
		/// Returns event
		/// \return event
		virtual Event*              event                   (void) const		{	return _event;	}

        /// Returns data kind
		virtual const DTcharacter*  data_kind               (void) const        {	return "Event";	}

	private:
		Event		*_event;
};

//==============================================================================
/// This class does the dirty work when you want to access data with
/// accessor member functions instead of by member variable.
//==============================================================================

template <typename T, typename U, typename V>
class ArchiveDataVariableAccessors: public ArchiveData {
	public:
        DEFINE_TYPE(ArchiveDataVariableAccessors,ArchiveData)
		DEFINE_CLONE

	private:
		typedef U (T::*GetFN)(void) const;
		typedef void (T::*SetFN)(V);
		
	public:

		ArchiveDataVariableAccessors (const ArchiveDataVariableAccessors& rhs)
			:	ArchiveData	(rhs),
				_object(rhs._object),
				_get(rhs._get),
				_set(rhs._set)
		{

		}

		/// Constructor if data is a variable
		/// \param title name of data
		/// \param object object for accessor functions
		/// \param get accessor function for retrieving data
		/// \param set accessor function for setting data
		/// \param flags flags for data
		ArchiveDataVariableAccessors (std::string title, T* object, GetFN get = NULL, SetFN set = NULL, DTuint flags = 0) {
			_object = object;
            set_title(title);
			add_flags(flags);
	
			_get = get;
			_set = set;
		}
		
	private:
		ArchiveDataVariableAccessors& operator = (const ArchiveDataVariableAccessors &rhs);

	public:

		
		/// Writes the value to a stream
		/// \param s stream
		virtual void                    value				(Stream &s) const {
			typename TypeTraits<V>::BaseType val = (U)(_object->*_get)();
			s << val;
		}
		
		/// Reads the value from a stream
		/// \param s stream 
		virtual void                    set_value				(Stream &s) const {
			typename TypeTraits<V>::BaseType val = TypeTraits<typename TypeTraits<V>::BaseType>::default_value();
			s >> val;
			(_object->*_set)(val);
		}
				
        /// Returns data kind
		virtual const DTcharacter*      data_kind        (void) const			{	return TypeTraits<U>::name();	}

	private:
		T           *_object;
		GetFN       _get;
		SetFN       _set;

};

//==============================================================================
/// This class does the dirty work for the C++ fundamental types
//==============================================================================

class ArchiveDataIgnore: public ArchiveData {
	public:
        DEFINE_TYPE(ArchiveDataIgnore,ArchiveData)
		DEFINE_CLONE
		
		ArchiveDataIgnore (const ArchiveDataIgnore& rhs)
			:	ArchiveData	(rhs)
		{

		}
		
		/// Constructor if data is a variable
		/// \param title name of data
		/// \param data reference to data
		/// \param flags flags for data
		ArchiveDataIgnore (std::string title) 
		{
            set_title(title);
			add_flags(DATA_PERSISTENT);
		}
		
	private:
		ArchiveDataIgnore& operator = (const ArchiveDataIgnore &rhs);

	public:
        /// Returns data kind
		virtual const DTcharacter*      data_kind        (void) const			{	return NULL;	}

};


//==============================================================================
//==============================================================================

// ignore data
#define ARCHIVE_DATA_IGNORE(TYP)        ArchiveDataIgnore(TYP)

//
// Create data object for plug
//

template <typename T>
ArchiveDataPlug<T> makeArchiveDataPlug (Plug<T> &plug, DTuint flags)
{
	return ArchiveDataPlug<T>(plug, flags);
}

#define ARCHIVE_PLUG(P,V)               makeArchiveDataPlug(P,V)

//
// Create data object for event
//

inline ArchiveDataEvent makeArchiveDataEvent (Event &event, DTuint flags)
{
	return ArchiveDataEvent(event, flags);
}

#define ARCHIVE_EVENT(E,V)              makeArchiveDataEvent(E,V)


//
// Is this only a bit confusing? This function is used to build an ArchiveData
// object that points to member functions and the type that is passed is 
// figured out from the functions at compile time. Syntax is really goofy 
// though :(
//

template<typename T, typename U, typename V>
ArchiveDataVariableAccessors<T,U,V> makeArchiveDataVariableAccessors (      std::string title,
																			T* object, 
																			U (T::*get)(void) const, 
																			void (T::*set)(V), 
																			DTuint flags)
{
	return ArchiveDataVariableAccessors<T,U,V>(title,object,get,set,flags);
}

#define ARCHIVE_DATA_ACCESSORS(T,G,S,F) makeArchiveDataVariableAccessors(T,this,&G,&S,F)


//
// Create the ArchiveDataVariable objects
//

template<typename T>
ArchiveDataVariable<T> makeArchiveDataVariable (std::string title, T &data, DTuint flags)
{
	return ArchiveDataVariable<T>(title,data,flags);
}

#define ARCHIVE_DATA(TYP,F)             makeArchiveDataVariable(#TYP,TYP,F)
#define ARCHIVE_DATA_NAMED(N,TYP,F)     makeArchiveDataVariable(N,TYP,F)
#define ARCHIVE_DATA_RAW(TYP,F)         makeArchiveDataVariable("",TYP,F)

//==============================================================================
//==============================================================================

} // DT3

#endif
