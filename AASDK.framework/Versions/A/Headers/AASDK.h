#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>

#import "AAZoneView.h"
#import "AAAdAdaptedZoneView.h"
#import "AAAdAdaptedJSONZoneView.h"
#import "AAContentTypes.h"

/** Keys for options NSDictionary */
#define AASDK_OPTION_USE_CACHED_IMAGES                  @"USE_CACHED_IMAGES"
#define AASDK_OPTION_IGNORE_ZONES                       @"IGNORE_ZONES"
#define AASDK_OPTION_KEYWORD_INTERCEPT                  @"KEYWORD_INTERCEPT"
#define AASDK_OPTION_TEST_MODE                          @"TEST_MODE"
#define AASDK_OPTION_TEST_MODE_API_VERSION              @"TEST_MODE_API_VERSION"
#define AASDK_OPTION_TEST_MODE_UNLOAD_AFTER_ONE         @"TEST_MODE_UNLOAD_AFTER_ONE"
#define AASDK_OPTION_DISABLE_ADVERTISING                @"DISABLE_ADVERTISING"
#define AASDK_OPTION_INIT_PARAMS                        @"INIT_PARAMS"


/** Log types to pass into registerDebugListenersFor:forMessageTypes: */
#define AASDK_DEBUG_GENERAL                             @"GENERAL"
#define AASDK_DEBUG_NETWORK                             @"NETWORK"
#define AASDK_DEBUG_NETWORK_DETAILED                    @"NETWORK_DETAILED"
#define AASDK_DEBUG_USER_INTERACTION                    @"USER_INTERACTION"
#define AASDK_DEBUG_AD_LAYOUT                           @"AD_LAYOUT"
#define AASDK_DEBUG_ALL                                 @"ALL"


/** keys used to report details in NSNotifications */
#define AASDK_KEY_ZONE_ID                           @"ZONE_ID"
#define AASDK_KEY_ZONE_IDS                          @"ZONE_IDS"
#define AASDK_KEY_ZONE_COUNT                        @"ZONE_COUNT"
#define AASDK_KEY_MESSAGE                           @"MESSAGE"
#define AASDK_KEY_TYPE                              @"TYPE"
#define AASDK_KEY_RECOVERY_SUGGESTION               @"RECOVERY_SUGGESTION"
#define AASDK_KEY_ZONE_ID                           @"ZONE_ID"
#define AASDK_KEY_PAYLOAD                           @"PAYLOAD"
#define AASDK_KEY_ZONE_VIEW                         @"ZONE_VIEW"
#define AASDK_KEY_CONTENT_PAYLOADS                  @"CONTENT_PAYLOADS"

/** keys used to report details in NSNotifications for Keyword Intercepts*/
#define AASDK_KEY_KI_REPLACEMENT_ID                 @"KI_REPLACEMENT_ID"
#define AASDK_KEY_KI_REPLACEMENT_TEXT               @"KI_REPLACEMENT_TEXT"
#define AASDK_KEY_KI_REPLACEMENT_ICON_URL           @"KI_REPLACEMENT_ICON"
#define AASDK_KEY_KI_REPLACEMENT_TAGLINE            @"KI_REPLACEMENT_TAGLINE"
#define AASDK_KEY_KI_TRIGGERED_ZONES                @"KI_TRIGGERED_ZONES"


@class AAAd, AAZone, CLLocation;

/**
 \brief A protocol an application should implement to be aware of the high-level happenings of the SDK.
 
 NOTE: only one AASDKObserver can exist at a time.
*/
@protocol AASDKObserver <NSObject>

/** 
 \brief SDK init complete.
 
 \param notification with userInfo NSDictionary that includes info about the SDK start.
 
 Notifies the applicaion that the SDK has initialized correctly.  Information in the `userInfo` param.  Keys: `zoneCount` : NSNumber* and `zoneIds` : NSArray*.
*/
- (void)aaSDKInitComplete:(nonnull NSNotification*)notification;

/** 
\brief callback when errors are detected by AASDK.
 
 \param error notification with userIndo NSDictionary that includes error messaging.
 
 Error message is in the `userInfo` param with the keys `message` and `recoverySuggestion` included.
 
 See \ref errorhandling for more detailed usage.
*/
- (void)aaSDKError:(nonnull NSNotification*)error;

/** optional elements */
@optional

/**
 \brief SDK is online.  Helpfull if you desire to know when SDK is back online after disconnect.
 
 \param notification with userInfo NSDictionary that includes info about the SDK start.
 
 Notifies the applicaion that the SDK has initialized correctly.  Information in the `userInfo` param.  Keys: AASDK_KEY_ZONE_COUNT : NSNumber* and AASDK_KEY_ZONE_IDS : NSArray* (of NSString*).
 */
- (void)aaSDKOnline:(nonnull NSNotification*)notification;


/**
 \brief Keyword Intercepts have loaded - a list of assets to cache is available
 
 \param notification with userInfo NSDictionary that includes info about the assets to cache
 
 Notifies the applicaion that the SDK has initialized Keyword Intercept terms.  Information in the `userInfo` param.  Keys: AASDK_KEY_ASSET_URL_LIST : NSArray* (of NSString*) is an array of URL images you may cache.
 
 \ref keywordintercept Keyword Intercept for more details
 */
- (void)aaSDKKeywordInterceptInitComplete:(nonnull NSNotification*)notification;


@end


/** \brief Content delivery delegate.
 
 Supports the delivery of content to the clien application.
 
 See: \ref ad_content for more details.
 */

@protocol AASDKContentDelegate <NSObject>

@optional
/** \brief Ad-based Content delivery notification
 
 \param notification The NSNotification has a `userInfo` dictionary with `AASDK_KEY_TYPE` and `AASDK_KEY_PAYLOAD` keys.
 
 e.g. The data can be retrieved with: `NSDictionary *payload = [[notification userInfo] objectForKey:AASDK_KEY_PAYLOAD];`
 
 See: \ref ad_content for more details.
 */
- (void)aaContentNotification:(nonnull NSNotification*)notification;


/** \brief Payload Service Content Delivery
 
 \param notification The NSNotification has a `userInfo` dictionary with `AASDK_KEY_CONTENT_PAYLOADS` key.
 
 e.g. The data can be retrieved with: `NSDictionary *payload = [[notification userInfo] objectForKey:AASDK_KEY_CONTENT_PAYLOADS];`
 
 See: \ref payload_content for more details.
 */
- (void)aaPayloadNotification:(nonnull NSNotification*)notification;

@end

/** \brief Debugging messaging from SDK.
 
 Provides the developer with detailed messaging around what the SDK is doing.
 
 NOTE: only one AASDKDebugObserver can exist at a time.
*/
@protocol AASDKDebugObserver <NSObject>

/** \brief Debugging notification
 
 \param notification The NSNotification has a `userInfo` dictionary with `AASDK_KEY_MESSAGE` and `AASDK_KEY_TYPE` keys.
 
e.g. Message can be retrieved with: `NSString *message = [[notification userInfo] objectForKey:AASDK_KEY_MESSAGE];`
*/
- (void)aaDebugNotification:(nonnull NSNotification*)notification;

@end


/**
\brief static accessors that are the primary public interface to AASDK and it's components
 
The AdAdapted iOS SDK's top-level class.  Most interactions with AdAdapted (and MoPub) are through this class,
 or the AAZoneView sub-classes.
 
A static access pattern is used, so client developers don't have to create or manange instances.  It's design is to
 "stay out of the developers way" as much as possible.
*/
@interface AASDK : NSObject

#pragma mark - notfications
/** \brief add listeners to conforming object

 \param observer the object to add listeners to
 
 add listeners most commonly used for a loading screen to wait for caching to complete, if wanted, and highest-level of error reporting */
+ (void)registerListenersFor:(nonnull id<AASDKObserver>)observer;

/** \brief removes all AASDKObserver notifications for object
 
 \param observer the object to remove listeners from
 
 does not remove debug listener, only ones added by registerListenersFor:
 */
+ (void)removeListenersFor:(nonnull id<AASDKObserver>)observer;


/** \brief add listeners to conforming object
 
 \param delegate the object to delegate to when content is received
 */
+ (void)registerContentListenersFor:(nonnull id<AASDKContentDelegate>)delegate;

/** \brief remove listeners for AASDKContentDelegate notifications
 
 \param delegate object to remove listeners from
 */
+ (void)removeContentListenersFor:(nonnull id<AASDKContentDelegate>)delegate;


#pragma mark - set user properties to be reported back
/** \brief user properties are stored between sessions.
 
 Use this to reset properties of the user if they logout, or if properties change.*/
+ (void)resetUserProperties;

/** \brief set aribitratry property for the user.
 \param value a string value to associate with the user
 \param name a string the value is keyed off of */
+ (void)setUserProperty:(nonnull NSString*)value withKey:(nonnull NSString*)name;

/** \brief set facebookid property for the user 
 \param value user's facebook id */
+ (void)setFacebookID:(nonnull NSString*)value;

/** \brief set twitter handle property for the user
 \param value user's twitter id */
+ (void)setTwitterID:(nonnull NSString*)value;

/** \brief set linkedin id property for the user
 \param value user's linkedin id */
+ (void)setLinkedInID:(nonnull NSString*)value;

#pragma mark - questions
/** \brief is SDK connected to internet 
 
 must be called a bit after making the first static AASDK call - it takes time for it to notice it's online.
 
 \return YES if SDK is online and can reach API.
 */
+ (BOOL)isOnline;

/** \brief convieniance method to let you know if sdk has initialized cache and ready to go 
 
 must be called after aaSDKInitComplete: is called
 
 \return YES if SDK is ready (init call to API is done)
 
 */
+ (BOOL)isReadyForUse;

#pragma mark - start session w/ server
/** \brief start session
 
Starts a session by contacting the AdAdapted API.  See \ref bootstrapping for more details.
 
 \param appID required, provided by AdAdapted
 \param observer optional, will let you know when cache has loaded (if used), and about errors.
 \param opDic optional, all values optional as well.  use the constants (top of this file) as keys.
 
\code{.m}
AASDK_OPTION_USE_CACHED_IMAGES - defaults to NO.  By default images are loaded Just In Time.
                                 YES loads image cache first, so ads render instantly.
AASDK_OPTION_DISABLE_WEBVIEW_CLOSE_BUTTON - defaults to NO.  
                                            YES means the popup ad themselves must use web hook to dismiss the ad.
AASDK_OPTION_SHOW_NAVIGATION_INSIDE_POPUP - defaults to NO. 
                                            YES will forward and back buttons to all the pop-up webview.
AASDK_OPTION_TARGET_ENVIRONMENT - AASDK_PRODUCTION is default
AASDK_OPTION_INIT_PARAMS - NSDictionary of JSON-complaint key/value pairs
\endcode
 */
+ (void)startSessionWithAppID:(nonnull NSString*)appID
         registerListenersFor:(nullable id<AASDKObserver>)observer
                      options:(nullable NSDictionary*)opDic;

/** \brief supports targeted ads
 
 \param location location to pass to API for more targeted ads
 
 note - this is not reset when init is called.  to set to unknown pass in nil */
+ (void)setDeviceLocation:(nonnull CLLocation*)location;

#pragma mark - Public event reporting
/** \brief not currently implemented - NOOP 
 
 \param name the name of the event
 \param payload optional hash of data to send along
 
 
 */
+ (void)reportEventNamed:(nonnull NSString*)name withPayload:(nullable NSDictionary*)payload;

/** \brief report item added to an optional list
 
 \param itemName the name of the item
 \param list the name of the list (optional)
 */
+ (void)reportItem:(nonnull NSString*)itemName addedToList:(nullable NSString*)list;

/** \brief report item crossed off an optional list
 
 In the case your app only has one removal state, use this one.  If your app has two states,
 use this one to mean "the user purchased this thing or completed this task".
 
 \param itemName the name of the item
 \param list the name of the list (optional)
 */
+ (void)reportItem:(nonnull NSString*)itemName crossedOffList:(nullable NSString*)list;


/** \brief report item crossed off an optional list
 
 In the case your app only has one removal state, DO NOT use this one.  If your app has two states,
 use this one to mean "the user deleted an item in a non-completed mannner".
 
 \param itemName the name of the item
 \param list the name of the list (optional)
 */
+ (void)reportItem:(nonnull NSString*)itemName deletedFromList:(nullable NSString*)list;

/** \brief report items added to an optional list
 
 \param items an array of NSStrings
 \param list the name of the list (optional)
 */
+ (void)reportItems:(nonnull NSArray<NSString*>*)items addedToList:(nullable NSString*)list;

/** \brief report items crossed off an optional list
 
 In the case your app only has one removal state, use this one.  If your app has two states,
 use this one to mean "the user purchased this thing or completed this task".
 
 \param items an array of NSStrings
 \param list the name of the list (optional)
 */
+ (void)reportItems:(nonnull NSArray<NSString*>*)items crossedOffList:(nullable NSString*)list;

/** \brief report items crossed off an optional list
 
 In the case your app only has one removal state, DO NOT use this one.  If your app has two states,
 use this one to mean "the user deleted an item in a non-completed mannner".
 
 \param items an array of NSStrings
 \param list the name of the list (optional)
 */
+ (void)reportItems:(nonnull NSArray<NSString*>*)items deletedFromList:(nullable NSString*)list;


#pragma mark - programatic layout conveniences
/** \brief the size of the zone in the CURRENT ORIENTATION
 
 \returns CGSizeMake(0, 0) if zone is unknown, or if not supported
 */
+ (CGSize)sizeOfZone:(nonnull NSString*)zoneId;

/** \brief the size of the zone
 
 \returns CGSizeMake(0, 0) if zone is unknown, or if not supported
 */
+ (CGSize)sizeOfZone:(nonnull NSString*)zoneId forOrientation:(UIInterfaceOrientation)orientation;

/** \brief a CGRect with origin 0,0 and the size of the zone 
 
  \returns CGRectMake(0, 0, 0, 0) if zone is unknown, or if not supported
 */
+ (CGRect)boundsOfZone:(nonnull NSString*)zoneId forOrientation:(UIInterfaceOrientation)orientation;

/** \brief supported orientations for a given zone
 
 Use UIInterfaceOrientationIsLandscape(value) and UIInterfaceOrientationIsLandscape(value) for testing the return value
 
 \returns mask with UIInterfaceOrientationMaskPortrait and/or UIInterfaceOrientationMaskLandscape
 */
+ (UIInterfaceOrientationMask)supportedInterfaceOrientationsForZone:(nonnull NSString*)zoneId;

#pragma mark - cacheInfo
/** \brief convenience mechanism
 
 \returns array of strings representing the AdAdapted zones the SDK is aware of.  
 
 If AASDK_OPTION_USE_CACHED_IMAGES:YES is set when starting the SDK, then these 
 images are already downloaded.  If NO, then these images are ready to render JIT.
  */
+ (nonnull NSArray*)availableZoneIDs;

/** \brief convenience mechanism
 
 \param zoneId a string value provided by AdAdapted staff.
 
 \returns YES if the SDK is ready to render the given AdAdapted ZoneId
 */
+ (BOOL)zoneAvailable:(nonnull NSString*)zoneId;

/** \brief convenience mechanism
 
 \returns array of strings representing the AdAdapted Keyword Intercept zones the SDK is aware of.
 
 MUST WAIT for KI_INIT_COMPLETE for this to return a non-empty list
 */
+ (nonnull NSArray*)availableKIZoneIDs;


/** \brief convenience mechanism
 
 \param zoneId a string value provided by AdAdapted staff.
 
 \returns YES if the SDK is ready to render the given AdAdapted Keyword Intercept ZoneId
 
 MUST WAIT for KI_INIT_COMPLETE for this to ever return true
 */
+ (BOOL)kiZoneAvailable:(nonnull NSString*)zoneId;


#pragma mark - debugging
/** \brief debugging support

 see \ref debugging for more details
 
 \param observer object implementing AASDKDebugObserver protocol
 \param types an array of debug messages you'd like to receive.
 */
+ (void)registerDebugListenersFor:(nonnull id<AASDKDebugObserver>)observer forMessageTypes:(nonnull NSArray*)types;

/** \brief removes only the debug observer */
+ (void)removeDebugListener;


#pragma mark - version info
/** \brief returns "X.Y.Z" */
+ (nonnull NSString*)buildVersion;

/** \brief returns "123456" */
+ (nonnull NSString*)buildSha;

#pragma mark - Keyword Intercept
/** \brief Submit term Keyword Intercept
 
 see \ref keywordintercept for more details
 
 \param userInput the characters the user has typed in.
 
 This is generally called for each character the user enters in the "onChange" handler of a text field or 
 area.  
 
 If a term is matched, results will be returned in an NSDictionary.  
 
 If no match is present, this returns nil.
 */
+ (nullable NSDictionary*)keywordInterceptFor:(nonnull NSString*)userInput;

/** \brief Record impression event
 
 see \ref keywordintercept for more details
 
 Call this when you present a result provided to you to the user.
 */

+ (void)keywordInterceptPresented;

/** \brief Record interaction event
 
 see \ref keywordintercept for more details
 
 Call this when the user interacts with a result provided to you.
 */
+ (void)keywordInterceptSelected;


@end
