//==============================================================================
///
///	File: InAppPurchasesMac.mm
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3InAppPurchases/Mac/InAppPurchasesMac.hpp"
#include "DT3InAppPurchases/InAppPurchases.hpp"
#include "DT3Core/System/StaticInitializer.hpp"
#include "DT3Core/Types/Threads/ThreadMainTaskQueue.hpp"
#include DT3_HAL_INCLUDE_PATH

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

GLOBAL_STATIC_INITIALIZATION(0,InAppPurchasesMac::initialize())
GLOBAL_STATIC_DESTRUCTION(0,InAppPurchasesMac::destroy())

//==============================================================================
//==============================================================================

void InAppPurchasesMac::initialize (void)
{

}

void InAppPurchasesMac::destroy (void)
{

}

//==============================================================================
//==============================================================================

DTerr InAppPurchasesMac::request_products_info  (   const std::vector<std::string> &products,
                                                    InAppPurchases::requestProductInfoCB fn,
                                                    void *data)
{
    std::vector<InAppPurchasesProduct> p;
    (*fn)(p, DT3_ERR_NONE, data);
    
    return DT3_ERR_NONE;
}

DTerr InAppPurchasesMac::restore_products   (   InAppPurchases::restoreProductCB fn,
                                                void *data)
{
    (*fn)("", DT3_ERR_NONE, data);

    return DT3_ERR_NONE;
}

DTerr InAppPurchasesMac::make_purchase  (   const InAppPurchasesProduct &product,
                                            InAppPurchases::purchaseProductCB fn,
                                            void *data)
{
    (*fn)("", DT3_ERR_NONE, data);
    
    return DT3_ERR_NONE;
}

//==============================================================================
//==============================================================================

} // DT3

