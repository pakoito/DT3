//==============================================================================
///
///	File: InAppPurchasesAndroid.cpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3InAppPurchases/Android/InAppPurchasesAndroid.hpp"
#include "DT3InAppPurchases/InAppPurchases.hpp"
#include "DT3Core/System/StaticInitializer.hpp"
#include "DT3Core/Types/Threads/ThreadMainTaskQueue.hpp"
#include "DT3Core/Types/Utility/Assert.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include DT3_HAL_INCLUDE_PATH

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

GLOBAL_STATIC_INITIALIZATION(0,InAppPurchasesAndroid::initialize())
GLOBAL_STATIC_DESTRUCTION(0,InAppPurchasesAndroid::destroy())

//==============================================================================
//==============================================================================

extern JavaVM* gJavaVM;
extern jobject gJavaObjActivity;
// extern jobject gJavaObjContext;

extern jclass load_class (JNIEnv* env, const char *clazz);

//==============================================================================
//==============================================================================

extern "C" {
    
// native void requestProductInfoCB (ArrayList<String> products, int error, long fn_opaque, long data_opaque);
JNIEXPORT void JNICALL JAVA_REQUEST_PRODUCT_INFO_CB (JNIEnv *env, jobject activity, jobject products, jint error, jlong fn_opaque, jlong data_opaque)
{
    LOG_MESSAGE << "JAVA_REQUEST_PRODUCT_INFO_CB called";
    	    	
    // Build an array of products
    jclass c_array_list = env->FindClass("java/util/ArrayList");
	ASSERT(c_array_list);
    jmethodID m_iterator = env->GetMethodID(c_array_list, "iterator", "()Ljava/util/Iterator;");
    ASSERT(m_iterator);
        
    jobject o_iter = env->CallObjectMethod(products, m_iterator);

    jclass c_iterator = env->FindClass("java/util/Iterator");
    ASSERT(c_iterator);

    jmethodID m_has_next = env->GetMethodID(c_iterator, "hasNext", "()Z");
    jmethodID m_next = env->GetMethodID(c_iterator, "next", "()Ljava/lang/Object;");

	// Build product structure
    jclass c_product = env->FindClass(DT3_ANDROID_ACTIVITY_JNI_TYPE "$InAppPurchasesProduct");
	ASSERT(c_product);
	
    // Get fields
    jfieldID f_localized_description = env->GetFieldID(c_product, "localized_description", "Ljava/lang/String;");
	ASSERT(f_localized_description);

    jfieldID f_localized_title = env->GetFieldID(c_product, "localized_title", "Ljava/lang/String;");
	ASSERT(f_localized_title);

    jfieldID f_price = env->GetFieldID(c_product, "price", "Ljava/lang/String;");
	ASSERT(f_price);

    jfieldID f_product_identifier = env->GetFieldID(c_product, "product_identifier", "Ljava/lang/String;");
	ASSERT(f_product_identifier);

    std::vector<InAppPurchasesProduct> p;

    while (env->CallBooleanMethod(o_iter, m_has_next)) {
	    jobject o_product = env->CallObjectMethod(o_iter, m_next);

        // Get values
        jstring s_localized_description = (jstring) env->GetObjectField(o_product, f_localized_description);
        jstring s_localized_title = (jstring) env->GetObjectField(o_product, f_localized_title);
        jstring s_price = (jstring) env->GetObjectField(o_product, f_price);
        jstring s_product_identifier = (jstring) env->GetObjectField(o_product, f_product_identifier);
        
        // Get values
        InAppPurchasesProduct pp;
        pp.set_description(env->GetStringUTFChars(s_localized_description, 0));
        pp.set_title(env->GetStringUTFChars(s_localized_title, 0));
        pp.set_price(env->GetStringUTFChars(s_price, 0));
        pp.set_product_identifier(env->GetStringUTFChars(s_product_identifier, 0));

        p.push_back(pp);

    }
    
    InAppPurchases::requestProductInfoCB fn = (InAppPurchases::requestProductInfoCB) fn_opaque;
    void *data = (void*) data_opaque;
    
    (*fn)(p, DT3_ERR_NONE, data);
}

// native void restoreProductCB (String product, int error, long fn_opaque, long data_opaque);
JNIEXPORT void JNICALL JAVA_RESTORE_PRODUCT_CB (JNIEnv *env, jobject activity, jstring product, jint error, jlong fn_opaque, jlong data_opaque)
{
    LOG_MESSAGE << "JAVA_RESTORE_PRODUCT_CB called";
    
    std::string p = env->GetStringUTFChars(product, 0);

    InAppPurchases::restoreProductCB fn = (InAppPurchases::restoreProductCB) fn_opaque;
    void *data = (void*) data_opaque;
    
    (*fn)(p, DT3_ERR_NONE, data);
}

// native void purchaseProductCB (String product, int error, long fn_opaque, long data_opaque);
JNIEXPORT void JNICALL JAVA_PURCHASE_PRODUCT_CB (JNIEnv *env, jobject activity, jstring product, jint error, jlong fn_opaque, jlong data_opaque)
{
    LOG_MESSAGE << "JAVA_PURCHASE_PRODUCT_CB called";

    std::string p = env->GetStringUTFChars(product, 0);

    InAppPurchases::purchaseProductCB fn = (InAppPurchases::purchaseProductCB) fn_opaque;
    void *data = (void*) data_opaque;
    
    (*fn)(p, DT3_ERR_NONE, data);
}

} // extern "C"

//==============================================================================
//==============================================================================

void InAppPurchasesAndroid::initialize (void)
{

}

void InAppPurchasesAndroid::destroy (void)
{

}

//==============================================================================
//==============================================================================

DTerr InAppPurchasesAndroid::request_products_info (    const std::vector<std::string> &products,
                                                        InAppPurchases::requestProductInfoCB fn,
                                                        void *data)
{
    // int requestProductsInfo (ArrayList<String> products, long fn_opaque, long data_opaque)

    JNIEnv* env = NULL;
    DTboolean needs_detach = false;
    int env_stat = gJavaVM->GetEnv( (void**) &env, JNI_VERSION_1_6);
    if (env_stat == JNI_EDETACHED) {
        gJavaVM->AttachCurrentThread(&env, 0);
        needs_detach = true;
    }
    ASSERT(env);

    jclass c_activity = load_class (env, DT3_ANDROID_ACTIVITY_JNI_TYPE);
	ASSERT(c_activity);
	    
    jmethodID m_request_products_info = env->GetMethodID(c_activity, "requestProductsInfo", "(Ljava/util/ArrayList;JJ)I");
	ASSERT(m_request_products_info);

    // Build an array of products
    jclass c_array_list = env->FindClass("java/util/ArrayList");
	ASSERT(c_array_list);
    jobject o_products = env->NewObject(c_array_list, env->GetMethodID(c_array_list, "<init>", "()V"));

    for (DTint n = 0; n < products.size(); ++n) {
        jstring o_str = env->NewStringUTF(products[n].c_str());
        env->CallBooleanMethod(o_products, env->GetMethodID(c_array_list, "add", "(Ljava/lang/Object;)Z"), o_str);
    }
    
    // Call JNI
    env->CallIntMethod(gJavaObjActivity, m_request_products_info, o_products, (jlong) fn, (jlong) data);
    
    if (needs_detach)
        gJavaVM->DetachCurrentThread();

    return DT3_ERR_NONE;
}

DTerr InAppPurchasesAndroid::restore_products ( InAppPurchases::restoreProductCB fn,
                                                void *data)
{
    // int restoreProducts (long fn_opaque, long data_opaque)
    
    JNIEnv* env = NULL;
    DTboolean needs_detach = false;
    int env_stat = gJavaVM->GetEnv( (void**) &env, JNI_VERSION_1_6);
    if (env_stat == JNI_EDETACHED) {
        gJavaVM->AttachCurrentThread(&env, 0);
        needs_detach = true;
    }
    ASSERT(env);

    jclass c_activity = load_class (env, DT3_ANDROID_ACTIVITY_JNI_TYPE);
	ASSERT(c_activity);
	    
    jmethodID m_restore_products = env->GetMethodID(c_activity, "restoreProducts", "(JJ)I");
	ASSERT(m_restore_products);

    // Call JNI
    env->CallIntMethod(gJavaObjActivity, m_restore_products, (jlong) fn, (jlong) data);

    if (needs_detach)
        gJavaVM->DetachCurrentThread();

    return DT3_ERR_NONE;
}

DTerr InAppPurchasesAndroid::make_purchase (    const InAppPurchasesProduct &product,
                                                InAppPurchases::purchaseProductCB fn,
                                                void *data)
{
    // int makePurchase (InAppPurchasesProduct product, long fn_opaque, long data_opaque)
    
    JNIEnv* env = NULL;
    DTboolean needs_detach = false;
    int env_stat = gJavaVM->GetEnv( (void**) &env, JNI_VERSION_1_6);
    if (env_stat == JNI_EDETACHED) {
        gJavaVM->AttachCurrentThread(&env, 0);
        needs_detach = true;
    }
    ASSERT(env);

    jclass c_activity = load_class (env, DT3_ANDROID_ACTIVITY_JNI_TYPE);
	ASSERT(c_activity);
	
    jclass c_product = load_class (env, DT3_ANDROID_ACTIVITY_JNI_TYPE "$InAppPurchasesProduct");
	ASSERT(c_product);
	    
    jmethodID m_make_purchase = env->GetMethodID(c_activity, "makePurchase", "(L" DT3_ANDROID_ACTIVITY_JNI_TYPE "$InAppPurchasesProduct;JJ)I");
	ASSERT(m_make_purchase);
		
	jmethodID i = env->GetMethodID(c_product, "<init>", "()V");
	ASSERT(i);
	
    jobject o_product = env->NewObject(c_product, env->GetMethodID(c_product, "<init>", "()V"));
    
    // Get fields
    jfieldID f_localized_description = env->GetFieldID(c_product, "localized_description", "Ljava/lang/String;");
	ASSERT(f_localized_description);

    jfieldID f_localized_title = env->GetFieldID(c_product, "localized_title", "Ljava/lang/String;");
	ASSERT(f_localized_title);

    jfieldID f_price = env->GetFieldID(c_product, "price", "Ljava/lang/String;");
	ASSERT(f_price);

    jfieldID f_product_identifier = env->GetFieldID(c_product, "product_identifier", "Ljava/lang/String;");
	ASSERT(f_product_identifier);
	
	LOG_MESSAGE << "Purchasing";
	LOG_MESSAGE << "  " << product.description();
	LOG_MESSAGE << "  " << product.title();
	LOG_MESSAGE << "  " << product.price();
	LOG_MESSAGE << "  " << product.product_identifier();
	
	// Set values
    jstring s_localized_description = env->NewStringUTF(product.description().c_str());
    env->SetObjectField(o_product, f_localized_description, s_localized_description);

    jstring s_localized_title = env->NewStringUTF(product.title().c_str());
    env->SetObjectField(o_product, f_localized_title, s_localized_title);

    jstring s_price = env->NewStringUTF(product.price().c_str());
    env->SetObjectField(o_product, f_price, s_price);

    jstring s_product_identifier = env->NewStringUTF(product.product_identifier().c_str());
    env->SetObjectField(o_product, f_product_identifier, s_product_identifier);
	
    // Call JNI
    env->CallIntMethod(gJavaObjActivity, m_make_purchase, o_product, (jlong) fn, (jlong) data);
    
    env->DeleteLocalRef(s_localized_description);
    env->DeleteLocalRef(s_localized_title);
    env->DeleteLocalRef(s_price);
    env->DeleteLocalRef(s_product_identifier);

    if (needs_detach)
        gJavaVM->DetachCurrentThread();

    return DT3_ERR_NONE;
}

//==============================================================================
//==============================================================================

} // DT3

