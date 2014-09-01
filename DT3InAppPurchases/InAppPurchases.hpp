#ifndef DT3_INAPPPURCHASES
#define DT3_INAPPPURCHASES
//==============================================================================
///	
///	File: InAppPurchases.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Utility/LatentCall.hpp"
#include "DT3Core/Types/Utility/Callback.hpp"
#include <vector>

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class InAppPurchasesProduct;

//==============================================================================
/// Base object for interface of in app purchases.
//==============================================================================

class InAppPurchases {
    public:
        DEFINE_TYPE_SIMPLE_BASE(InAppPurchases)

    private:
                                        InAppPurchases      (void);	
                                        InAppPurchases      (const InAppPurchases &rhs);
        InAppPurchases&                 operator =          (const InAppPurchases &rhs);	
        virtual                         ~InAppPurchases     (void);
    		
	public:
    
        typedef void (*requestProductInfoCB)(std::vector<InAppPurchasesProduct> products, DTerr error, void *data);
        typedef void (*restoreProductCB)(std::string, DTerr error, void *data);
        typedef void (*purchaseProductCB)(std::string, DTerr error, void *data);
    
    
		/// Requests a list of products from the store
        /// \param products products info to request
		/// \param fn completion callback
		/// \param data data passed to callback
		/// \return Error
        static DTerr                    request_products_info       (   const std::vector<std::string> &products,
                                                                        requestProductInfoCB fn,
                                                                        void *data);

		/// List of products to restore
		/// \param fn completion callback
		/// \param data data passed to callback
		/// \return Error
        static DTerr                    restore_products            (   restoreProductCB fn,
                                                                        void *data);

		/// Makes a purchase of a product from the store
        /// \param products product to purchase
		/// \param fn completion callback
		/// \param data data passed to callback
		/// \return Error
        static DTerr                    make_purchase               (   const InAppPurchasesProduct &product,
                                                                        purchaseProductCB fn,
                                                                        void *data);
    
    

};

//==============================================================================
//==============================================================================

} // DT3

#endif
