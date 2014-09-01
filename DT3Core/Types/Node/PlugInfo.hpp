#ifndef DT3_PLUGINFO
#define DT3_PLUGINFO
//==============================================================================
///	
///	File: PlugInfo.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/Utility/Assert.hpp"
#include <string>
#include <vector>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class PlugNode;
class PlugBase;
struct DTnodekind;

//==============================================================================
/// Class
//==============================================================================

class PlugInfo {
   public:
                                        PlugInfo            (void);
	private:
                                        PlugInfo            (const PlugInfo &rhs);
        PlugInfo &                      operator =          (const PlugInfo &rhs);
		
	public:
        virtual                         ~PlugInfo           (void);
		
		
	public:
		/// Initialize the static PlugInfo structure
		/// \param name Name of plug
		/// \param offset Byte offset to the plug in the object
		/// \param plug_node_info next plug info in linked list
		/// \return This plug info
		PlugInfo&                       init                (   std::string name,
                                                                DTsize offset,
                                                                DTnodekind *plug_node_info,
                                                                const DTcharacter* (*type)(void),
                                                                void (*copy)(PlugBase *dst, const PlugBase *src),
                                                                std::string (*internal_value_as_string_fn)(const PlugBase *p));
    
		/// Returns plug name
		/// \return plug name
		inline const std::string&       name                (void) const				{	return _name;								}


        /// Copy values
        inline void                     copy                (PlugBase *dst, const PlugBase *src)    {   (*_copy_fn)(dst,src);           }

		/// Returns plug name
        /// \param p plug
		/// \return value of the plug as a string
        inline std::string              value_as_string     (const PlugBase *p)         {   return (*_internal_value_as_string_fn)(p);  }


		/// Returns plug type
		/// \return plug type
		inline const DTcharacter*       plug_type           (void) const				{	return (*_type_fn)();                       }


		/// Returns no draw flag
		/// \return No draw flag
		inline DTboolean                is_no_draw          (void) const				{	return _is_no_draw;							}
		
		/// Sets the no draw flag
		/// \param flag no draw flag
		/// \return This event info
		inline PlugInfo&                set_no_draw         (const DTboolean flag)		{	_is_no_draw = flag;	return *this;			}

		
		/// Returns is single output flag (i.e. can only connect to one other plug)
		/// \return is single output
		inline DTboolean                is_single_output    (void) const				{	return _is_single_output;                   }
		
		/// Sets is single output flag (i.e. can only connect to one other plug)
		/// \param flag single output flag
		/// \return This event info
		inline PlugInfo&                set_single_output   (const DTboolean flag)		{	_is_single_output = flag;	return *this;   }

		
		/// Returns is input flag
		/// \return is input
		inline DTboolean                is_input            (void) const				{	return _is_input;							}
		
		/// Sets the is input flag
		/// \param flag is input flag
		/// \return This event info
		inline PlugInfo&                set_input           (const DTboolean flag)		{	_is_input = flag;	return *this;			}


		/// Returns is output flag
		/// \return is output
		inline DTboolean                is_output           (void) const				{	return _is_output;							}
		
		/// Sets the is output flag
		/// \param flag is output flag
		/// \return This event info
		inline PlugInfo&                set_output          (const DTboolean flag)		{	_is_output = flag;	return *this;			}



		/// Returns wether the plug requires computing
		/// \return requires compute
		inline DTboolean                requires_compute        (void) const            {	return _is_compute;							}
		
		/// Sets wether the plug requires computing or not
		/// \param flag requires compute
		inline void                     set_requires_compute    (const DTboolean flag)  {	_is_compute = flag;                         }


		/// Returns wether the plug is always dirty
		/// \return is always dirty
		inline DTboolean                is_always_dirty     (void) const				{	return _is_always_dirty;                    }

		/// Sets wether the plug is always dirty
		/// \param flag always dirty
		inline void                     set_always_dirty    (const DTboolean flag)      {	_is_always_dirty = flag;                    }


		/// Sets which plugs are affected by this plug
		/// \param other other plug
		/// \return plug info
		PlugInfo&                       affects             (DTushort other);

		/// Returns which plugs are affected by this plug
		/// \return affected plugs
		const std::vector<PlugInfo*>&   affects             (void) const				{	return _affects;							}


		/// Sets a plug that affects this plug
		/// \return plug info
		PlugInfo&                       affected_by         (DTushort other);

		/// Returns which plugs that affects this plug
		/// \return affected by plugs
		const std::vector<PlugInfo*>&   affected_by         (void) const				{	return _affected_by;						}


		/// Returns the next info object in the list
		/// \return Next info object
		PlugInfo*                       next_info           (void) const				{	return _next;								}


		/// Given the plug, return a pointer to the node
		/// \param plug plug
		/// \return owner node
		PlugNode*                       plug_to_node        (const PlugBase *plug) {
			ASSERT(_offset != 0);
			return reinterpret_cast<PlugNode*>( (DTubyte*) plug - _offset);
		}

		/// Given the node, return a pointer to the plug
		/// \param node node
		/// \return Plug
		PlugBase*                       node_to_plug        (const PlugNode *node){
			ASSERT(_offset != 0);
			return reinterpret_cast<PlugBase*>( (DTubyte*) node + _offset);
		}	


        //
        // Plug Info pool
        //

        /// Returns the next free index in the pool
		/// \return Next free index
        static DTushort                 get_free_index      (void);

        /// Returns the info at the specified index
		/// \param i index
        static PlugInfo&                get_info            (DTushort i);

    private:
		DTubyte                         _is_input:1;
		DTubyte                         _is_output:1;
		DTubyte                         _is_compute:1;
		DTubyte                         _is_no_draw:1;
		DTubyte                         _is_single_output:1;
        DTubyte                         _is_always_dirty:1;

		DTsize                          _offset;
		PlugInfo                        *_next;
    
		std::string                     _name;
    
        // Function pointer to copy routine
        void (*_copy_fn)(PlugBase *dst, const PlugBase *src);
        const DTcharacter* (*_type_fn)();
        std::string (*_internal_value_as_string_fn)(const PlugBase *p);
		
		// List of affects nodes when this one changes
		std::vector<PlugInfo*>          _affects;
		std::vector<PlugInfo*>          _affected_by;

        static PlugInfo                 _pool[DT3_PLUG_POOL_SIZE];
};

//==============================================================================
//==============================================================================

#define PLUG_INFO_INDEX(Plug)       Plug##Index

#define IMPLEMENT_PLUG_INFO_INDEX(Plug)	namespace {	DTushort PLUG_INFO_INDEX(Plug) = PlugInfo::get_free_index();	}

#define PLUG_OFFSET(Plug)			(	(DTubyte*) (static_cast<PlugBase*>(&(reinterpret_cast<type*>(0x99999999)->Plug))) - \
										(DTubyte*) (static_cast<PlugNode*>(reinterpret_cast<type*>(0x99999999)))	)

#define PLUG_INIT(Plug, Name)       PlugInfo::get_info(PLUG_INFO_INDEX(Plug)).init(         \
                                        Name,                                               \
                                        PLUG_OFFSET(Plug),                                  \
                                        plug_node_info,                                     \
                                        &decltype(Plug)::internal_type,                     \
                                        &decltype(Plug)::internal_copy,                     \
                                        &decltype(Plug)::internal_value_as_string           \
                                    )


//==============================================================================
//==============================================================================

} // DT3

#endif
