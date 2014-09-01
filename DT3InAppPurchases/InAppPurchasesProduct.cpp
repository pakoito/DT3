//==============================================================================
///	
///	File: InAppPurchasesProduct.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3InAppPurchases/InAppPurchasesProduct.hpp"
#include "DT3Core/System/Factory.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION(InAppPurchasesProduct)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

InAppPurchasesProduct::InAppPurchasesProduct (void)
    :   _data   (NULL)
{

}
		
InAppPurchasesProduct::InAppPurchasesProduct (const InAppPurchasesProduct &rhs)
    :   BaseClass               (rhs),
        _localized_description  (rhs._localized_description),
        _localized_title        (rhs._localized_title),
        _price                  (rhs._price),
        _product_identifier     (rhs._product_identifier),
        _data                   (rhs._data)
{
    
}

InAppPurchasesProduct & InAppPurchasesProduct::operator = (const InAppPurchasesProduct &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		BaseClass::operator = (rhs);

        _localized_description = rhs._localized_description;
        _localized_title = rhs._localized_title;
        _price = rhs._price;
        _product_identifier = rhs._product_identifier;
        _data = rhs._data;
    }
    return (*this);
}
			
InAppPurchasesProduct::~InAppPurchasesProduct (void)
{

}

//==============================================================================
//==============================================================================

} // DT3

