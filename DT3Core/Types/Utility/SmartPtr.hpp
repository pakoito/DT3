#ifndef DT3_SMARTPTR
#define DT3_SMARTPTR
//==============================================================================
///	
///	File: SmartPtr.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Utility/Callback.hpp"
#include "DT3Core/Types/Utility/Assert.hpp"
#include <atomic>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
// Forward Declarations
//==============================================================================

template <typename T> class WeakPtr;
template <typename T> class SharedPtr;
template <typename T> class SharedPointee;

//==============================================================================
//==============================================================================

class SmartPtrControl {
    public:
        SmartPtrControl (void)
            :   _shared_count   (0),
                _weak_count     (0)
        {}
    
        ~SmartPtrControl (void)
        {}

    public:
    
		/// Description
		/// \param param description
		/// \return description
        void    incr_shared_ref (void)
        {
            _shared_count.fetch_add(1, std::memory_order_relaxed);
        }
    
		/// Description
		/// \param param description
		/// \return description
        DTboolean    decr_shared_ref (void)
        {
            ASSERT(_shared_count > 0);
            _shared_count.fetch_sub(1, std::memory_order_relaxed);
            
            DTboolean del = (_shared_count == 0);
            
            if (del && _weak_count == 0) {
                delete this;
            }
            
            return del;
        }

		/// Description
		/// \param param description
		/// \return description
        void    incr_weak_ref (void)
        {
            _weak_count.fetch_add(1, std::memory_order_relaxed);
        }
    
		/// Description
		/// \param param description
		/// \return description
        void    decr_weak_ref (void)
        {
            ASSERT(_weak_count > 0);
            _weak_count.fetch_sub(1, std::memory_order_relaxed);
            if (_shared_count == 0 && _weak_count == 0) {
                delete this;
            }
        }
    
		/// Description
		/// \param param description
		/// \return description
        DTint   use_count   (void) const
        {
            return _shared_count;
        }

    private:
        std::atomic<DTint>   _shared_count;
        std::atomic<DTint>   _weak_count;
};

//==============================================================================
//==============================================================================

template <typename T>
class WeakPtr {
    public:
        WeakPtr (void)
        {
            _control = new SmartPtrControl();
            _data = NULL;

            _control->incr_weak_ref();
        }
    
        //
        // Copy constructors
        //
    
        WeakPtr (const WeakPtr &rhs)
        {
            _control = rhs._control;  // Shared control block
            _data = rhs._data;

            _control->incr_weak_ref();
        }

        template <typename U>
        WeakPtr (const WeakPtr<U> &rhs)
        {
            static_assert(std::is_convertible<U*, T*>::value , "No implicit conversion between pointer types");
            
            _control = rhs._control;  // Shared control block
            _data = rhs._data;

            _control->incr_weak_ref();
        }
    
        template <typename U>
        WeakPtr (const SharedPtr<U> &rhs)
        {
            static_assert(std::is_convertible<U*, T*>::value , "No implicit conversion between pointer types");
            
            _control = rhs._control;  // Shared control block
            _data = rhs._data;

            _control->incr_weak_ref();
        }
    
        //
        // Assignment
        //
    
        template <typename U>
        WeakPtr& operator = (const WeakPtr &rhs)
        {
            static_assert(std::is_convertible<U*, T*>::value , "No implicit conversion between pointer types");

            // Store old references
            SmartPtrControl *old_control = _control;
            
            // Wire up new controls and data
            _control = rhs._control;  // Shared control block
            _data = rhs._data;

            _control->incr_weak_ref();
            old_control->decr_weak_ref();
        }

        template <typename U>
        WeakPtr& operator = (const WeakPtr<U> &rhs)
        {
            static_assert(std::is_convertible<U*, T*>::value , "No implicit conversion between pointer types");
            
            // Store old references
            SmartPtrControl *old_control = _control;
            
            // Wire up new controls and data
            _control = rhs._control;  // Shared control block
            _data = rhs._data;

            _control->incr_weak_ref();
            old_control->decr_weak_ref();
        }
    
        template <typename U>
        WeakPtr<T>& operator = (const SharedPtr<U> &rhs)
        {
            static_assert(std::is_convertible<U*, T*>::value , "No implicit conversion between pointer types");
            
            // Store old references
            SmartPtrControl *old_control = _control;
            
            // Wire up new controls and data
            _control = rhs._control;  // Shared control block
            _data = rhs._data;
            
            _control->incr_weak_ref();
            old_control->decr_weak_ref();
        }
    
        //
        // Free reference
        //

        ~WeakPtr (void)
        {
            ASSERT(_control->use_count() >= 0);
            _control->decr_weak_ref();
        }

    public:
		/// Description
		/// \param param description
		/// \return description
        void        reset       (void)
        {
            _control->decr_weak_ref();
            ASSERT(_control->use_count() >= 0);
            
            _control = new SmartPtrControl();
            _control->incr_weak_ref();
            _data = NULL;
        }
    
        /// Description
		/// \param param description
		/// \return description
        DTint       use_count   (void)
        {
            ASSERT(_control->use_count() >= 0);
            return _control->use_count();
        }

		/// Description
		/// \param param description
		/// \return description
        DTboolean   expired     (void)
        {
            ASSERT(_control->use_count() >= 0);
            return _control->use_count() == 0;
        }
    
    private:
        T                       *_data;
        SmartPtrControl         *_control;
};

//==============================================================================
//==============================================================================

template <typename T>
class SharedPtr {
    public:
        SharedPtr (void)
        {
            _control = new SmartPtrControl();
            _data = NULL;

            _control->incr_shared_ref();
        }

        //
        // Copy constructors
        //

        SharedPtr (const SharedPtr &rhs)
        {
            _control = rhs._control;
            _data = rhs._data;

            _control->incr_shared_ref();
        }

        template <typename U>
        SharedPtr (const SharedPtr<U> &rhs)
        {
            static_assert(std::is_convertible<U*, T*>::value , "No implicit conversion between pointer types");

            _control = rhs._control;
            _data = rhs._data;

            _control->incr_shared_ref();
        }

        template <typename U>
        SharedPtr (const WeakPtr<U> &rhs)
        {
            static_assert(std::is_convertible<U*, T*>::value , "No implicit conversion between pointer types");

            _control = rhs->_control;
            _control->incr_shared_ref();

            if (rhs->expired()) _data = NULL;
            else                _data = rhs._data;
        }
    
        //
        // Assignment
        //

        SharedPtr&      operator =  (const SharedPtr &rhs)
        {
            // Store old references
            T *old_data = _data;
            SmartPtrControl *old_control = _control;

            // Wire up new controls and data
            _control = rhs._control;
            _data = rhs._data;

            _control->incr_shared_ref();
            old_control->decr_shared_ref();

            // Decrement ref count on old references
            if (old_control->decr_shared_ref())
                delete old_data;

            return *this;
        }

        template <typename U>
        SharedPtr&      operator =  (const SharedPtr<U> &rhs)
        {
            static_assert(std::is_convertible<U*, T*>::value , "No implicit conversion between pointer types");

            // Store old references
            T *old_data = _data;
            SmartPtrControl *old_control = _control;

            // Wire up new controls and data
            _control = rhs._control;
            _data = rhs._data;

            _control->incr_shared_ref();
            old_control->decr_shared_ref();

            // Decrement ref count on old references
            if (old_control->decr_shared_ref())
                delete old_data;

            return *this;
        }

        template <typename U>
        SharedPtr&      operator =  (const WeakPtr<U> &rhs)
        {
        
            // Store old references
            T *old_data = _data;
            SmartPtrControl *old_control = _control;

            // Wire up new controls and data
            _control = rhs._control;

            if (rhs->expired()) _data = NULL;
            else                _data = rhs._data;

            _control->incr_shared_ref();
            old_control->decr_shared_ref();

            // Decrement ref count on old references
            if (old_control->decr_shared_ref())
                delete old_data;

            return *this;
        }

        //
        // Free reference
        //

        ~SharedPtr (void)
        {
            // Decrement ref count on old references
            if (_control->decr_shared_ref())
                delete _data;
        }

    public:
        /// Description
		/// \param param description
		/// \return description
        void        reset       (void)
        {
            // Decrement ref count on old references
            if (_control->decr_shared_ref())
                delete _data;
            
            _control = new SmartPtrControl();
            _data = NULL;

            _control->incr_shared_ref();
        }

        /// Description
		/// \param param description
		/// \return description
        DTint       use_count   (void)
        {
            return _control->use_count();
        }

        /// Description
		/// \param param description
		/// \return description
        T*          get         (void)
        {
            return _data;
        }
    
        /// Description
		/// \param param description
		/// \return description
        const T*    get         (void) const
        {
            return _data;
        }

		/// Description
		/// \param param description
		/// \return description
        T& operator*() const
        {
            ASSERT(_data);
            return *_data;
        }

		/// Description
		/// \param param description
		/// \return description
        T* operator->() const
        {
            ASSERT(_data);
            return _data;
        }

        /// Description
		/// \param param description
		/// \return description
        explicit operator bool() const
        {
            return _data != NULL;
        }
    
    
    
        /// Description
		/// \param param description
		/// \return description
        template <typename U>
        SharedPtr<T> static_pointer_cast (const SharedPtr<U> &rhs)
        {
            SharedPtr<T> sp;
            sp._control->decr_shared_ref(); // Delete old control
            
            sp._control = rhs._control;
            sp._data = static_cast<T>(rhs._data);
            
            sp._control->incr_shared_ref();
            return sp;
        }
    
    private:
        template<typename U> friend class SharedPtr;
        template<typename U> friend class SharedPointee;
    
        T                       *_data;
        SmartPtrControl         *_control;
};

//==============================================================================
//==============================================================================

template <typename T>
class SharedPointee {
    public:
        SharedPointee (void)
            :   _control    (NULL)
        {}
    
        SharedPointee (const SharedPointee &rhs)
            :   _control    (NULL)
        {}

        SharedPointee& operator = (const SharedPointee &rhs)
        {
            return *this;
        }
    
        virtual ~SharedPointee (void)
        {
            
        }
    
    public:
        /// Description
		/// \param param description
		/// \return description
        SharedPtr<T>        as_shared               (void)
        {
            if (!_control)
                _control = new SmartPtrControl();
            
            SharedPtr<T> sp;
            sp._control->incr_shared_ref();
            sp._data = static_cast<T*>(this);
        }

    private:
        SmartPtrControl     *_control;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
