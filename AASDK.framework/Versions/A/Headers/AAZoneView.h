//
//  AAImageViewHolder.h
//  AASDK
//
//  Created by hollarab on 8/18/13.
//  Copyright (c) 2013 Ad Adapted . All rights reserved.
//

#import <UIKit/UIKit.h>
/** enumeration to describe the type and source of the ad data */
typedef enum : NSInteger {
    kTypeUnsupportedAd=0,
    kAdAdaptedJSONAd=1,
    kAdAdaptedImageAd=2,
    kAdAdaptedHTMLAd=3,
} AdTypeAndSource;

/** \brief Super class for rendering ads from AdAdapted.
 
Requires a zone id provided from AdAdapted.
 */
@class AAZoneView;

/** \brief implemented by the UIViewController that contains a AAZoneView */
@protocol AAZoneViewOwner <NSObject>
@required

/** \brief generally returns `self`
 
 \returns a UIViewController object from which the popup may be presented.
 */
- (UIViewController *)viewControllerForPresentingModalView;

@optional
/**
 \brief OPTIONAL - Capture URL that user clicked on in popup.
 
 When User touches an anchor tag in the popup that has the prefix "internal:" this is called

 \param view the object that launched the popup
 \param urlString URL, in the popup, that was clicked.  This is interpreted by the delegate app for internal navigation, or what not.
 
 See \ref popuphooks on how to tailor popup html content to pass data to this method
 */
- (void)zoneView:(AAZoneView*)view hadPopupSendURLString:(NSString*)urlString;

/** \brief OPTIONAL - ad has loaded and rendered 
 
 \param view the AAZoneView sub-class that was loaded
 */
- (void)zoneViewDidLoadZone:(AAZoneView *)view;

/** \brief OPTIONAL - an error occurred 
 
 \param view the AAZoneView sub-class that failed to load an ad
 */
- (void)zoneViewDidFailToLoadZone:(AAZoneView *)view;

/** \brief OPTIONAL - modal presentation is going to happen, due to user interaction with ad
 \param view the AAZoneView sub-class that was interacted with
 */
- (void)willPresentModalViewForZone:(AAZoneView *)view;

/** \brief OPTIONAL - modal dismissal is going to happen, due to user interaction with popup 
  \param view the AAZoneView sub-class that was interacted with
 */
- (void)didDismissModalViewForZone:(AAZoneView *)view;

/** \brief OPTIONAL - the user has interacted with a popup, and your app is going to the background 
  \param view the AAZoneView sub-class that was interacted with
 */
- (void)willLeaveApplicationFromZone:(AAZoneView *)view;

/** \brief OPTIONAL - if the zone delegates call to action handling to the client, this will be called.
 
 NOTE: this is in progress and not supported by the API at this time.  To turn on, you should have `@"PRIVATE_CUSTOM_DELEGATE_ZONES_CTA":@[@"ZONEID1",@"ZONEID2"]` in your options dictionary when starting the SDK.
 
 When the user interacts with these specified zones, this method is called. */
- (void)handleCallToActionForZone:(AAZoneView*)view;
@end


/** \brief UIView subclass that renders ads
 
 a superclass for convenience classes (that you really should be using)
 
 see \ref zoneusage for detailed usage instructions in both IB and programatically */
@interface AAZoneView : UIView

/** \brief the ad zone */
@property (nonatomic, copy) NSString *zoneId;

/** \brief the delegate that provides a UIViewController */
@property (nonatomic, weak) id<AAZoneViewOwner> zoneOwner;

/** \brief an AdTypeAndSource */
@property (nonatomic, readonly) AdTypeAndSource type;

/** \brief constructor
 
 use AAAdAdaptedZoneView and AAMoPubBannerZoneView instead */
- (id)initWithFrame:(CGRect)frame forZone:(NSString*)zoneId zoneType:(AdTypeAndSource)type delegate:(id<AAZoneViewOwner>)delegate;

/** \brief let the AAZoneView know it needs to rotate, and maybe load another ad 
 
 \param newOrientation the new orientation (the one you're going to).
 
 see \ref zoneusage for more details
 */
 - (void)rotateToOrientation:(UIInterfaceOrientation)newOrientation;

/** \brief the size, according the server, of the content 
 
 \returns the size of the ad, as returned from the API
 */
- (CGSize)adContentViewSizeForOrientation:(UIInterfaceOrientation)orientation;

/** \brief load next ad
 
 MoPub ads will flicker even if they don't load new content - AdAdapted ZoneView are noop if ad is unchanged */
- (void)advanceToNextAd;

/** \brief close the zone's popup ad
 
 \returns true if the popup will close in response to this request
 
 only responds if the AAZoneViewOwner's zoneView:hadPopupSendURLString: method is called.  allows the application the ability to close the popup ad once the app has finished doing what it was getting into. */
- (BOOL)closePopup;


/** \brief close the zone's popup ad
 
 \param handler  is called once the popup view controller has been removed.
 
 
 \returns true if the popup will close in response to this request.  handler() is called only in the case where the return value is true.
 
 only responds if the AAZoneViewOwner's zoneView:hadPopupSendURLString: method is called.  allows the application the ability to close the popup ad once the app has finished doing what it was getting into. */
- (BOOL)closePopupWithCompletionHandler:(void (^)())handler;

/** \brief used by Interface Builder

 \param value an NSNumber that gets converted to AdTypeAndSource
 
 have to convert from NSNumber to int when setting type from Interface Builder.  using AAAdAdaptedZoneView and AAMoPubBannerZoneView means you can ignore this. */
- (void)setZoneType:(NSNumber*)value;

/** \brief in JSON Ads, this hook is how you report user interaction
 
 since JSON Ads are built by you (the client developer) you need a way to report the user interacted, and the popup (or whatnot) needs to be opened.
 */
- (void)userInteractedWithAd;

/** \brief only to be used when AAZoneView can't be removed, but is no longer visible

 reports an impression has ended.  this should only be called if you're obscuring an ad (rather than removing it).
 
 see \ref zoneusage_zoneLifeCycle */
- (void)wasHidden;

/** \brief only to be used when AAZoneView couldn't be removed, but is no longer obscured
 
 calling this before wasHidden has no effect.   reports an impression has re-started for an ad that wasHidden has been called on.  this should only be called if you're obscuring an ad (rather than removing it).
 
 see \ref zoneusage_zoneLifeCycle */
- (void)wasUnHidden;

@end
