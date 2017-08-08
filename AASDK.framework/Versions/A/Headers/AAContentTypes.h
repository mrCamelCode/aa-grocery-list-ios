//
//  AAContentTypes.h
//  AASDK
//
//  Created by hollarab on 1/11/17.
//  Copyright © 2017 LameSauce Software. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 \brief An item the user may add to their list.
 
 See: \ref payload_content
 */
@interface AADetailedListItem : NSObject

/** For AdAdapted use */
@property (nonnull) NSString* payloadId;
/** For AdAdapted use */
@property (nonnull) NSString* trackingId;
/** Text to add to list */
@property (nonnull) NSString* productTitle;
/** https image URL */
@property (nullable) NSURL* productImageURL;
/** product brand */
@property (nullable) NSString* productBrand;
/** A category for the product */
@property (nullable) NSString* productCategory;
/** barcode */
@property (nullable) NSString* productBarcode;
/** discount - things like 10% or $3 off */
@property (nullable) NSString* productDiscount;


/**
 Creates AADetailedListItem from dictionary with referance to payload

 @param dictionary valid data
 @param payloadId id for payload
 @return null or valid object
 */
+(nullable instancetype)parseFromItemDictionary:(nullable NSDictionary*)dictionary forPayload:(nonnull NSString*)payloadId;

/**
 Returns item as a dictionary.

 @return a dictionary that includes a payload_id field along with the object's properties.
 */
- (nullable NSDictionary*)toDictionary;

@end



/**
 \brief A container for one or more AADetailedListItems that may have it's own branding and message.
 */
@interface AAContentPayload : NSObject

/** For AdAdapted use */
@property (nonnull) NSString *payloadId;
/** Message to display the user */
@property (nullable) NSString *payloadMessage;
/** https image URL */
@property (nullable) NSURL* payloadImageURL;
/** always 'detailed_list_items' at this time */
@property (nonnull) NSString *payloadType;
/** array of items */
@property (nonnull) NSArray<AADetailedListItem*> *detailedListItems;

/** 
 Create from a dictionary
 
 \param dictionary dictionary*/
+(nullable instancetype)parseFromDictionary:(nullable NSDictionary*)dictionary;

/** \brief Mark payload received
 
 \param list the name of the list it was added to or nil
 */
- (void)reportPayloadReceivedOntoList:(nullable NSString*)list;

/** \brief Mark payload rejected */
- (void)reportPayloadRejected;

@end
