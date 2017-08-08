//
//  AAAdAdaptedJSONZone.h
//  AASDK
//
//  Created by hollarab on 9/22/14.
//  Copyright (c) 2014 Ad Adapted . All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AAZoneView.h"

/** \brief each natively rendered is (or contains) a sub-class of this
 
    using this class ensures reporting of impressions is correct, and 
    that Call To Actions are handled correctly. */
@interface AAAdAdaptedJSONZoneView : AAZoneView

/** \brief constructor */
- (id)initWithFrame:(CGRect)frame forZone:(NSString*)zoneId delegate:(id<AAZoneViewOwner>)delegate;

/** \brief MUST OVERRIDE IN SUBCLASS
 \param adAssets the raw data about the Ad.
 
 NOTE: this is called on the main thread so you can more easily update UI elements.
 */
- (void)layoutAssets:(NSDictionary*)adAssets;


/** \brief DO NOT OVERRIDE
   when the user interacts with your ad, call [self userInteractedWithAd] to activate 
   the Call To Action, most likely to show a popup. */
- (void)userInteractedWithAd;

@end
