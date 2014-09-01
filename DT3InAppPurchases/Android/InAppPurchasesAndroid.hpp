#ifndef DT3_INAPPPURCHASESANDROID
#define DT3_INAPPPURCHASESANDROID
//==============================================================================
///	
///	File: InAppPurchasesAndroid.hpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3InAppPurchases/InAppPurchases.hpp"
#include "DT3InAppPurchases/InAppPurchasesProduct.hpp"
#include <jni.h>

namespace DT3 {

//==============================================================================
/// Base object for Mac implementation of in app purchases.
//==============================================================================

class InAppPurchasesAndroid {

    public:
        DEFINE_TYPE_SIMPLE_BASE(InAppPurchasesAndroid)

    private:
                                        InAppPurchasesAndroid	(void);	
                                        InAppPurchasesAndroid	(const InAppPurchasesAndroid &rhs);
        InAppPurchasesAndroid&       	operator =              (const InAppPurchasesAndroid &rhs);	
        virtual                         ~InAppPurchasesAndroid	(void);
    		
	public:
		/// Description
		/// \param param description
		/// \return description
        static void                     initialize              (void);

		/// Description
		/// \param param description
		/// \return description
        static void                     destroy                 (void);
    
    
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
// cd project/src 
// javah -classpath ../bin/classes com.smellslikedonkey.Whatever.MainActivity
//==============================================================================

extern "C" {
    /*
     * Method:    requestProductInfoCB
     * Signature: (Ljava/util/ArrayList;IJJ)V
     */
    JNIEXPORT void JNICALL JAVA_REQUEST_PRODUCT_INFO_CB (JNIEnv *, jobject, jobject, jint, jlong, jlong);

    /*
     * Method:    restoreProductCB
     * Signature: (Ljava/lang/String;IJJ)V
     */
    JNIEXPORT void JNICALL JAVA_RESTORE_PRODUCT_CB (JNIEnv *, jobject, jstring, jint, jlong, jlong);

    /*
     * Method:    purchaseProductCB
     * Signature: (Ljava/lang/String;IJJ)V
     */
    JNIEXPORT void JNICALL JAVA_PURCHASE_PRODUCT_CB (JNIEnv *, jobject, jstring, jint, jlong, jlong);

} // extern "C"

//==============================================================================
//==============================================================================

} // DT3

#endif
