//==============================================================================
///	
///	File: InAppPurchases.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "InAppPurchases.hpp"

#if DT3_OS == DT3_MACOSX
#include "InAppPurchasesMac.hpp"
#elif DT3_OS == DT3_IOS
#include "InAppPurchasesiOS.hpp"
#elif DT3_OS == DT3_ANDROID
#include "InAppPurchasesAndroid.hpp"
#endif

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

#if DT3_OS == DT3_MACOSX
typedef InAppPurchasesMac InAppPurchasesImpl;
#elif DT3_OS == DT3_IOS
typedef InAppPurchasesiOS InAppPurchasesImpl;
#elif DT3_OS == DT3_ANDROID
typedef InAppPurchasesAndroid InAppPurchasesImpl;
#endif

//==============================================================================
//==============================================================================

DTerr InAppPurchases::request_products_info (   const std::vector<std::string> &products,
                                                requestProductInfoCB fn,
                                                void *data)
{
    return InAppPurchasesImpl::request_products_info(products,fn,data);
}

DTerr InAppPurchases::restore_products  (   restoreProductCB fn,
                                            void *data)
{
    return InAppPurchasesImpl::restore_products(fn,data);
}

DTerr InAppPurchases::make_purchase (   const InAppPurchasesProduct &product,
                                        purchaseProductCB fn,
                                        void *data)
{
    return InAppPurchasesImpl::make_purchase (product,fn,data);
}

//==============================================================================
//==============================================================================

} // DT3

