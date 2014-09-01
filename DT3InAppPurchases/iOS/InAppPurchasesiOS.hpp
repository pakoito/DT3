#ifndef DT3_INAPPPURCHASESIOS
#define DT3_INAPPPURCHASESIOS
//==============================================================================
///	
///	File: InAppPurchasesiOSiOS.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "InAppPurchases.hpp"
#include "InAppPurchasesProduct.hpp"

namespace DT3 {

//==============================================================================
/// Base object for iOS implementation of in app purchases.
//==============================================================================

class InAppPurchasesiOS {
    public:
        DEFINE_TYPE_SIMPLE_BASE(InAppPurchasesiOS)

    private:
                                        InAppPurchasesiOS           (void);
                                        InAppPurchasesiOS           (const InAppPurchasesiOS &rhs);
        InAppPurchasesiOS&              operator =                  (const InAppPurchasesiOS &rhs);
        virtual                         ~InAppPurchasesiOS          (void);
    		
	public:
		/// Description
		/// \param param description
		/// \return description
        static void                     initialize_static           (void);

		/// Description
		/// \param param description
		/// \return description
        static void                     destroy_static              (void);
    
    
		/// Requests a list of products from the store
        /// \param products products info to request
		/// \param fn completion callback
		/// \param data data passed to callback
		/// \return Error
        static DTerr                    request_products_info       (   const std::vector<std::string> &products,
                                                                        InAppPurchases::requestProductInfoCB fn,
                                                                        void *data);

		/// List of products to restore
		/// \param fn completion callback
		/// \param data data passed to callback
		/// \return Error
        static DTerr                    restore_products            (   InAppPurchases::restoreProductCB fn,
                                                                        void *data);

		/// Makes a purchase of a product from the store
        /// \param products product to purchase
		/// \param fn completion callback
		/// \param data data passed to callback
		/// \return Error
        static DTerr                    make_purchase               (   const InAppPurchasesProduct &product,
                                                                        InAppPurchases::purchaseProductCB fn,
                                                                        void *data);
};

//==============================================================================
//==============================================================================

} // DT3

#endif
