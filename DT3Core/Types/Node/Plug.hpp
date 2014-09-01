#ifndef DT3_PLUG
#define DT3_PLUG
//==============================================================================
///	
///	File: Plug.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/Node/PlugInfo.hpp"
#include "DT3Core/Types/Utility/TypeTraits.hpp"
#include "DT3Core/Types/FileBuffer/TextBufferStream.hpp"
#include <vector>
#include <string>
#include <mutex>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class PlugNode;

//==============================================================================
/// Class
//==============================================================================

class PlugBase {
    public:         
        DEFINE_TYPE_SIMPLE_BASE(PlugBase)
    
                                        PlugBase                    (void);
                                        PlugBase                    (const PlugBase &rhs);
        PlugBase &                      operator =                  (const PlugBase &rhs);
                                        ~PlugBase                   (void);
	
	public:
		/// Called to initialize the class
		static void                     initialize_static           (void);

		/// Return a pointer to the owning node of the Plug
		/// \return Owner node
		PlugNode*                       owner                       (void) const                {	return info().plug_to_node(this);           }

		/// Returns the name of the plug
		/// \return Name of the plug
		const std::string&              name                        (void) const                {	return info().name();                       }
		
        /// Returns the full name of object that uniquely identifies it
		/// \return full name of object
        std::string                     full_name                   (void) const;
    
        /// Returns the value of the plug as a string
        std::string                     value_as_string             (void) const                {   return info().value_as_string(this);            }


		/// Returns the underlying type name of the plug. This is unique for each type of plug.
		/// \return Type name of plug
        const DTcharacter*              plug_type                   (void) const                {   return info().plug_type();                  }



		/// Returns the dirty flag for the plug
		/// \return dirty flag
        DTboolean                       is_dirty                    (void) const                {	return _is_dirty;                           }
		
		/// Sets the dirty flag for the plug and connected plugs
		void                            set_dirty                   (void);

		/// Unsets the dirty flag for the plug
        void                            set_clean                   (void)                      {   _is_dirty = info().is_always_dirty();       }


		
		/// Returns no draw flag
		/// \return No draw flag
        DTboolean                       is_no_draw                  (void) const                {	return info().is_no_draw();                 }
	
		/// Returns is input flag
		/// \return is input
        DTboolean                       is_input                    (void) const                {	return info().is_input();                   }
		
		/// Returns is output flag
		/// \return is output
        DTboolean                       is_output                   (void) const                {	return info().is_output();                  }
		
		/// Returns is single output flag (i.e. can only connect to one other plug)
		/// \return is single output
        DTboolean                       is_single_output            (void) const                {	return info().is_single_output();           }
		
    
		
		/// Returns incoming connection
		/// \return incoming connection
        PlugBase*                       incoming_connection         (void) const;

		/// Returns true if there is an incoming connection
		/// \return has incoming connection
        DTboolean                       has_incoming_connection     (void) const;

		/// Set an incoming plug connection
		/// \param incoming incoming plug
		DTboolean                       set_incoming_connection     (PlugBase* incoming);

		/// Remove an incoming connection
		/// \param incoming incoming plug
		void                            remove_incoming_connection  (void);



		/// Returns all outgoing connections
		/// \return outgoing connections
        std::vector<PlugBase*>          outgoing_connections        (void) const;

		/// Returns true if there is at least one outgoing connection
		/// \return has outgoing connection
        DTboolean                       has_outgoing_connection     (void) const;

		/// Add an outgoing plug connection
		/// \param outgoing outgoing plug
        /// \return success
		DTboolean                       add_outgoing_connection     (PlugBase* outgoing);
		
		/// Remove an outgoing connection
		/// \param outgoing outgoing plug
		void                            remove_outgoing_connection  (PlugBase* outgoing);

		/// Remove all outgoing connections
		void                            remove_outgoing_connections (void);
				
		
		
		/// Checks to see if this plug is compatible with another one
		/// \param p Other plug
		/// \return True if compatible, false if not
        DTboolean                       is_compatible               (const PlugBase* p) const   {	return p->plug_type() == plug_type();       }
	
		// Compute the nodes feeding this one. This could take a while.
        void                            compute                     (void) const;
    
	protected:
		/// Return a pointer to the owning node of the Plug
		/// \return Owner node
		PlugInfo&                       info                        (void) const                {	return PlugInfo::get_info(_info_index); }

        // Data indices
		DTuint                          _info_index:14;         // Index into Plug info pool
		DTuint                          _connection_index:15;   // Index into connections pool

        // Flags
		DTuint                          _is_computing:1;
		DTuint                          _is_dirty:1;
		DTuint                          _will_compute:1;
				
    private:
    
        DTboolean                       try_set_dirty               (void);
        static DTboolean                try_compute                 (PlugBase *p);

        //
        // Connection list pool
        //
    
        struct PlugConnections {
            PlugBase                    *_incoming;
            std::vector<PlugBase*>      _outgoing;
            PlugConnections             *_next_free;
        };

        static PlugConnections          _pool[DT3_PLUG_CONNECTION_POOL_SIZE];
        static PlugConnections          *_free_list;
        static std::mutex               _free_list_mutex;

        PlugConnections&                connections                 (void);
        PlugConnections&                connections                 (void) const;
        void                            free_connections            (void);
};

//==============================================================================
/// Class
//==============================================================================

template <typename T>
class Plug: public PlugBase {
    public:
        DEFINE_TYPE_SIMPLE(Plug,PlugBase)
    
							Plug                (DTushort info)                 {	_info_index = info;              }
								
							Plug                (DTushort info, const T &rhs)   {	_info_index = info;
                                                                                    _is_dirty = true;
                                                                                    _value = rhs;
                                                                                }
    
							Plug                (const Plug<T> &rhs)            {	_info_index = rhs._info_index;
                                                                                    _is_dirty = true;
                                                                                    _value = rhs;
                                                                                }
                                                                                														
		Plug<T>&            operator =          (const T &rhs)                  {	if (_value != rhs) {
                                                                                        set_dirty();
                                                                                        _value = rhs;
                                                                                        set_clean();
                                                                                    }
                                                                                    return *this;		
                                                                                }
				
		Plug<T>&            operator =          (const Plug<T> &rhs)            {	if (_value != rhs._value) {
                                                                                        set_dirty();
                                                                                        _value = rhs._value;
                                                                                        set_clean();
                                                                                    }
                                                                                    return *this;		
                                                                                }
				
		// Dereference operator
		const T&            as_ref_no_compute	(void) const            {	return _value;                  }
        const T&            as_ref              (void) const            {	compute();		return _value;	}
		T&                  as_ref_no_compute	(void)                  {	return _value;                  }
		T&                  as_ref              (void)                  {	compute();		return _value;	}
    
                            operator T          (void) const			{	compute();		return _value;	}
		const T&            operator *          (void) const			{	compute();		return _value;	}
		const T*            operator ->         (void) const			{	compute();		return &_value;	}
		T&                  operator *          (void)                  {	compute();		return _value;	}
		T*                  operator ->         (void)                  {	compute();		return &_value;	}
					
					
		// Comparison operators
		int                 operator ==         (const T &rhs)			{	compute(); return _value == rhs;                               }
		int                 operator ==         (const Plug<T> &rhs)	{	compute(); rhs.compute(); return _value == rhs._value;    }

		int                 operator !=         (const T &rhs)			{	compute(); return _value != rhs;                               }
		int                 operator !=         (const Plug<T> &rhs)	{	compute(); rhs.compute(); return _value != rhs._value;    }
				
		/// Returns the value without compute function beign called
        /// \return value of the plug
		const T&            value_without_compute(void) const           {	return _value;	}
    
    
        //
        // Functions important to the internal functions of the plug
        //
    
        /// Copy the values
		/// \param dst Destination value
		/// \param src Source value
        static void         internal_copy               (PlugBase *dst, const PlugBase *src)
        {
            Plug<T>* dst_p = static_cast<Plug<T>*>(dst);
			const Plug<T>* src_p = static_cast<const Plug<T>*>(src);
            
            dst_p->_value = src_p->_value;
            dst_p->set_clean();
        }
    
        /// Returns the type of the plug
        static const DTcharacter* internal_type         (void)
        {
            return TypeTraits<BaseType>::name_caps();
        }
    
        /// Returns the value of the plug as a string
        static std::string internal_value_as_string     (const PlugBase *p)
        {
            const Plug<T>* pp = static_cast<const Plug<T>*>(p);
            
            TextBufferStream s;
            s << pp->value_without_compute();
            return s.buffer();
        }

    
		/// Underlying type of plug
		typedef T	BaseType;
		
	private:
		T	_value;
        
};

//==============================================================================
//==============================================================================

} // DT3

#endif
