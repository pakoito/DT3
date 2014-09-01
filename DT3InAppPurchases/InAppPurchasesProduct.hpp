#ifndef DT3_INAPPPURCHASESPRODUCT
#define DT3_INAPPPURCHASESPRODUCT
//==============================================================================
///	
///	File: InAppPurchasesProduct.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseClass.hpp"
#include <string>

namespace DT3 {

//==============================================================================
/// Base object for the different placeable types of objects in the engine.
//==============================================================================

class InAppPurchasesProduct: public BaseClass {

    public:
        DEFINE_TYPE(InAppPurchasesProduct,BaseClass)
		DEFINE_CREATE_AND_CLONE

                                        InAppPurchasesProduct	(void);	
                                        InAppPurchasesProduct	(const InAppPurchasesProduct &rhs);
        InAppPurchasesProduct&          operator =              (const InAppPurchasesProduct &rhs);	
        virtual                         ~InAppPurchasesProduct	(void);
    		
	public:
        DEFINE_ACCESSORS(description, set_description, std::string, _localized_description)
        DEFINE_ACCESSORS(title, set_title, std::string, _localized_title)
        DEFINE_ACCESSORS(price, set_price, std::string, _price)
        DEFINE_ACCESSORS(product_identifier, set_product_identifier, std::string, _product_identifier)

        DEFINE_ACCESSORS(data, set_data, void*, _data)
    
    private:
        std::string         _localized_description;
        std::string         _localized_title;
        std::string         _price;
        std::string         _product_identifier;
    
        void*               _data;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
