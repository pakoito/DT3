 #ifndef DT3_BASECLASS
#define DT3_BASECLASS
//==============================================================================
///	
///	File: BaseClass.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include <memory>
#include <atomic>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class Archive;

//==============================================================================
/// Class
//==============================================================================

class BaseClass: public std::enable_shared_from_this<BaseClass> {
    public:
        DEFINE_TYPE_BASE(BaseClass)
         
                                    BaseClass                       (void);
                                    BaseClass                       (const BaseClass &rhs);
        BaseClass &					operator =                      (const BaseClass &rhs);	
        virtual                     ~BaseClass                      (void);

        virtual void                archive                         (const std::shared_ptr<Archive> &archive);
        virtual void                archive_done                    (const std::shared_ptr<Archive> &archive)  {};
		
	public:
		
		//
		// Exception safe initialization
		//
		
		/// Called to initialize the object
		virtual void				initialize                      (void);

		/// Called to uninitialize the object
		virtual void				uninitialize                    (void);


		//
		// Archiving
		//
		
		/// Returns the unique ID for this object. The object ID is guaranteed to be unique per
        /// execution of the applcation.
		/// \return unique ID
        inline DTuint				unique_id                       (void) const			{   return _unique_id;					}

		/// Forces the object to get a new unique ID
		void						new_unique_id                   (void);
        
		/// Sets whether the object will be streamed or not
		/// \param streamable object is streamable
		void						set_streamable                  (DTboolean streamable)	{	_streamable = streamable;			}

		/// Returns whether the object will be streamed or not
		/// \return streamable
		DTboolean					streamable                      (void) const			{	return _streamable;					}

    private:				
        static std::atomic<DTuint>  _unique_id_counter;
        DTuint						_unique_id;
		DTboolean					_streamable;
};
												
//==============================================================================
//==============================================================================


} // DT3

#endif
