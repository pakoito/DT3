//==============================================================================
///	
///	File: InAppPurchasesiOS.mm
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3InAppPurchases/iOS/InAppPurchasesiOS.hpp"
#include "DT3Core/System/StaticInitializer.hpp"
#include "DT3Core/Types/Threads/ThreadMainTaskQueue.hpp"

#import <StoreKit/StoreKit.h>

//==============================================================================
// Product Request Delegate
//==============================================================================

@interface ProductsRequestDelegate : NSObject <SKProductsRequestDelegate, SKRequestDelegate> {
    NSArray                                     *products;

    DT3::InAppPurchases::requestProductInfoCB   _fn;
    void                                        *_data;
}

@property DT3::InAppPurchases::requestProductInfoCB fn;
@property void* data;
@end

@implementation ProductsRequestDelegate

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
    std::vector<DT3::InAppPurchasesProduct> returned_products;

    [products release];
    products = response.products;
    [products retain];

    for(SKProduct *product in products) {
        
        DT3::InAppPurchasesProduct p;
        
        // Copy description
        const char *descrp = [product.localizedDescription UTF8String];
        if (!descrp)
            p.set_description("Unknown");
        else
            p.set_description(descrp);

        // Copy Title
        const char *title = [product.localizedTitle UTF8String];
        if (!title)
            p.set_title("Unknown");
        else
            p.set_title(title);

        // Copy price
        NSNumberFormatter *numberFormatter = [[NSNumberFormatter alloc] init];
        [numberFormatter setFormatterBehavior:NSNumberFormatterBehavior10_4];
        [numberFormatter setNumberStyle:NSNumberFormatterCurrencyStyle];
        [numberFormatter setLocale:product.priceLocale];
        NSString *formattedString = [numberFormatter stringFromNumber:product.price];
        p.set_price([formattedString UTF8String]);
        [numberFormatter release];
        
        // Identifier
        const char* identifier = [product.productIdentifier UTF8String];
        if (!identifier)
            continue;
        p.set_product_identifier(identifier);
        
        // Remember the product
        p.set_data(product);
        
        returned_products.push_back(p);
    }
    
    // Do callback
    if (self.fn) {
        (self.fn)(returned_products, DT3::DT3_ERR_NONE, _data);
    }
}

- (void)request:(SKRequest *)request didFailWithError:(NSError *)error
{
    std::vector<DT3::InAppPurchasesProduct> returned_products;

    // Do callback
    if (self.fn) {
        (self.fn)(returned_products, DT3::DT3_ERR_UNKNOWN, _data);
    }
}
@end

//==============================================================================
//==============================================================================

@interface StoreQueueObserver : NSObject <SKPaymentTransactionObserver> {
    DT3::InAppPurchases::restoreProductCB       _restore_cb;
    void                                        *_restore_data;
    DT3::InAppPurchases::purchaseProductCB      _purchase_cb;
    void                                        *_purchase_data;
}

@property DT3::InAppPurchases::restoreProductCB     restore_cb;
@property DT3::InAppPurchases::purchaseProductCB    purchase_cb;
@property void *restore_data;
@property void *purchase_data;

@end

@implementation StoreQueueObserver

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{

    for (SKPaymentTransaction *transaction in transactions) {
        std::string product = [transaction.payment.productIdentifier UTF8String];

        switch (transaction.transactionState) {
            case SKPaymentTransactionStatePurchased: {

                    if (self.purchase_cb) {
                        std::shared_ptr<DT3::LatentCall> lc = DT3::make_latent_call(self.purchase_cb, product, DT3::DT3_ERR_NONE, _purchase_data);
                        DT3::ThreadMainTaskQueue::add_task(lc);
                    }
                
                    // Remove the transaction from the payment queue.
                    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];

                } break;
                
            case SKPaymentTransactionStateFailed: {
            
                    NSLog(@"IAP Error: %@", transaction.error);

                    if (self.restore_cb) {
                        std::shared_ptr<DT3::LatentCall> lc = DT3::make_latent_call(self.restore_cb, product, DT3::DT3_ERR_UNKNOWN, _restore_data);
                        DT3::ThreadMainTaskQueue::add_task(lc);
                    }
                
                    if (self.purchase_cb) {
                        std::shared_ptr<DT3::LatentCall> lc = DT3::make_latent_call(self.purchase_cb, product, DT3::DT3_ERR_UNKNOWN, _purchase_data);
                        DT3::ThreadMainTaskQueue::add_task(lc);
                    }

                    // Remove the transaction from the payment queue.
                    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];

                } break;
                
            case SKPaymentTransactionStateRestored: {
                
                    if (self.restore_cb) {
                        std::shared_ptr<DT3::LatentCall> lc = DT3::make_latent_call(self.restore_cb, product, DT3::DT3_ERR_NONE, _restore_data);
                        DT3::ThreadMainTaskQueue::add_task(lc);
                    }

                    // Remove the transaction from the payment queue.
                    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];

                } break;
                
            default:
                break;
        }

    }

}

@end

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

GLOBAL_STATIC_INITIALIZATION(0,InAppPurchasesiOS::initialize_static())
GLOBAL_STATIC_DESTRUCTION(0,InAppPurchasesiOS::destroy_static())

namespace {
    ProductsRequestDelegate *_delegate;
    StoreQueueObserver      *_observer;
}

//==============================================================================
//==============================================================================

void InAppPurchasesiOS::initialize_static (void)
{
    _delegate = [[ProductsRequestDelegate alloc] init];
    _observer = [[StoreQueueObserver alloc] init];
    [[SKPaymentQueue defaultQueue] addTransactionObserver:_observer];
}

void InAppPurchasesiOS::destroy_static (void)
{
    [[SKPaymentQueue defaultQueue] removeTransactionObserver:_observer];
    [_observer release];
    _observer = NULL;

    [_delegate release];
    _delegate = NULL;
}

//==============================================================================
//==============================================================================

DTerr InAppPurchasesiOS::request_products_info  (   const std::vector<std::string> &products,
                                                    InAppPurchases::requestProductInfoCB fn,
                                                    void *data)
{
    NSMutableSet *p = [NSMutableSet setWithCapacity: (NSUInteger) products.size()];
    
    for (DTint i = 0; i < products.size(); ++i) {
        NSString *product_string = [NSString stringWithUTF8String:products[i].c_str()];
        [p addObject:product_string];
    }
    
    _delegate.fn = fn;
    _delegate.data = data;

    SKProductsRequest *request = [[SKProductsRequest alloc] initWithProductIdentifiers:p];
    request.delegate = _delegate;
    [request start];

    return DT3_ERR_NONE;
}

DTerr InAppPurchasesiOS::restore_products   (   InAppPurchases::restoreProductCB fn,
                                                void *data)
{
    if ([[[SKPaymentQueue defaultQueue] transactions] count] > 0)
        return DT3_ERR_UNKNOWN;

    _observer.restore_cb = fn;
    _observer.restore_data = data;
    _observer.purchase_cb = nil;
    _observer.purchase_data = nil;

    [[SKPaymentQueue defaultQueue] restoreCompletedTransactions];

    return DT3_ERR_NONE;
}

DTerr InAppPurchasesiOS::make_purchase  (   const InAppPurchasesProduct &product,
                                            InAppPurchases::purchaseProductCB fn,
                                            void *data)
{
    if ([[[SKPaymentQueue defaultQueue] transactions] count] > 0)
        return DT3_ERR_UNKNOWN;

    _observer.restore_cb = nil;
    _observer.restore_data = nil;
    _observer.purchase_cb = fn;
    _observer.purchase_data = data;

    SKPayment *payment = [SKPayment paymentWithProduct: (SKProduct*) product.data()];
    [[SKPaymentQueue defaultQueue] addPayment:payment];

    return DT3_ERR_NONE;
}
//==============================================================================
//==============================================================================

} // DT3

