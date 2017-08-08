//
//  AAAdAdaptedZoneView.h
//  AASDK
//
//  Created by hollarab on 9/4/14.
//  Copyright (c) 2014 Ad Adapted . All rights reserved.
//

#import <AASDK/AASDK.h>

/** \brief  Subclass of AAZoneView configured for AdAdapted image and html content */
@interface AAAdAdaptedZoneView : AAZoneView

/** \brief Constructor 
 
 \param frame where's it goin'
 \param zoneId the zone it's for
 \param delegate the owner that implements AAZoneViewOwner
 
 \returns a configured object
*/
- (id)initWithFrame:(CGRect)frame forZone:(NSString*)zoneId delegate:(id<AAZoneViewOwner>)delegate;

@end
