//
//  AppDelegate.swift
//  AA Grocery List
//
//  Created by Mike Pedersen on 7/25/17.
//  Copyright © 2017 Joshua Truitt. All rights reserved.
//

import UIKit
import CoreTelephony
import CoreLocation
import Security
import SystemConfiguration
import MobileCoreServices
import AdSupport

@UIApplicationMain
class AppDelegate: UIResponder, UIApplicationDelegate {

    var window: UIWindow?


    func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplicationLaunchOptionsKey: Any]?) -> Bool {
        // Override point for customization after application launch.
        
        // START FOR AASDK
        // see: http://dev.adadapted.com/ios/4.0.2_e810178/html_docs/bootstrapping.html
        let options = [
            AASDK_OPTION_TEST_MODE: true // DO NOT ship to PROD with test mode on
            //AASDK_OPTION_INIT_PARAMS: ["example_store_id":"KNOWNATRUNTIME"]
            ] as [String : Any]
        
        // we recommend this setting when starting integration, or debugging issues.
        // DO NOT ship to PROD with debugging on
        AASDK.registerDebugListeners(for: self as! AASDKDebugObserver, forMessageTypes: [AASDK_DEBUG_GENERAL, AASDK_DEBUG_NETWORK_DETAILED])
        
        // AdAdapted will provide you with an AppId to pass in here
        AASDK.startSession(withAppID: "IOSRECIPEDEMOAPP", registerListenersFor: self, options: options)
        return true
    }

    func applicationWillResignActive(_ application: UIApplication) {
        // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
        // Use this method to pause ongoing tasks, disable timers, and invalidate graphics rendering callbacks. Games should use this method to pause the game.
    }

    func applicationDidEnterBackground(_ application: UIApplication) {
        // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
        // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
    }

    func applicationWillEnterForeground(_ application: UIApplication) {
        // Called as part of the transition from the background to the active state; here you can undo many of the changes made on entering the background.
    }

    func applicationDidBecomeActive(_ application: UIApplication) {
        // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
    }

    func applicationWillTerminate(_ application: UIApplication) {
        // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
        
        
    }
}

extension AppDelegate: AASDKObserver {
    func aaSDKInitComplete(_ notification: Notification) {
        print("init complete")
    }
    
    func aaSDKError(_ error: Notification) {
        print("error")
    }
}

// see: http://dev.adadapted.com/ios/4.0.2_e810178/html_docs/debugging.html
extension AppDelegate: AASDKDebugObserver {
    func aaDebugNotification(_ notification: Notification) {
        if let message = notification.userInfo?[AASDK_KEY_MESSAGE] as? String
        {
            //print(message)
        }
    }
}

