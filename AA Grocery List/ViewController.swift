//
//  ViewController.swift
//  AA Grocery List
//
//  Created by Mike Pedersen on 7/25/17.
//  Copyright © 2017 Joshua Truitt. All rights reserved.
//

import UIKit

class ViewController: UIViewController, UITextFieldDelegate {
    
    @IBOutlet weak var itemNameTextField: UITextField!
    @IBOutlet weak var itemQuantityTextField: UITextField!
    @IBOutlet weak var itemScrollView: UIScrollView!
    @IBOutlet weak var showRemovedToggle: UISegmentedControl!
    @IBOutlet weak var zoneView: AAAdAdaptedZoneView!
    
    var numItemScrollViewDefaultSubviews: Int = 0
    
    var allItems = [UIItemButton]()
    var showRemoved: Bool = false;
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        // Do any additional setup after loading the view, typically from a nib.
        zoneView.zoneOwner = self; // AASDK
        AASDK.registerContentListeners(for: self)
        
        self.itemNameTextField.delegate = self
        self.itemQuantityTextField.delegate = self
        
        numItemScrollViewDefaultSubviews = itemScrollView.subviews.count
        
        itemScrollView.contentSize = CGSize(width: itemScrollView.frame.width, height: 0)
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    func textFieldShouldReturn(_ textField: UITextField) -> Bool
    {
        self.view.endEditing(true)
        return false
    }

    @IBAction func btn_addItem(_ sender: UIButton)
    {
        let itemName = itemNameTextField.text
        let itemQuantity = Int(itemQuantityTextField.text!)
        
        addItem(itemName: itemName!, itemQuantity: itemQuantity!)
    }
    
    func addItem(itemName: String, itemQuantity: Int)
    {
        // Create the new item button.
        let newItemButton = UIItemButton(frame: CGRect(x: 0, y: 0, width: Int(itemScrollView.contentSize.width), height: 50), itemName: itemName, itemQuantity: itemQuantity)
        
        // Add the button to the interface
        addItemToScrollView(item: newItemButton)
        addToList(button: newItemButton) // Register with app's list.
        
        // Clear the text fields
        itemNameTextField.text = ""
        itemQuantityTextField.text = ""
        
        // Add the on click listener for the new item button.
        newItemButton.addTarget(self, action: #selector(ViewController.btn_item(_:)), for: .touchUpInside)
    }

    @IBAction func btn_clearList(_ sender: UIButton)
    {
        clearList()
    }
    
    func clearList()
    {
        // Remove the views from the screen.
        for item in allItems
        {
            item.removeFromSuperview()
            
            // Register every removal with AA
            AASDK.reportItem(item.itemName, deletedFromList: "grocery list")
        }
        
        // Clear the list.
        allItems.removeAll()
    }
    
    @IBAction func btn_removedItemsToggle(_ sender: UISegmentedControl)
    {
        showRemoved = sender.selectedSegmentIndex == 1 ? true : false
        refreshScrollView()
    }
    
    /*
        Registers the button with the list.
    */
    func addToList(button: UIItemButton)
    {
        allItems.append(button)
    }
    
    /*
         The click listener for the UI Item Buttons. Put it here so that the contents
         of the ViewController are available to change, since doing it from the UIItemButton
         script is needlessly complex.
    */
    func btn_item(_ sender: UIItemButton)
    {
        // Make the removed state the opposite of whatever it is.
        sender.isRemoved = !sender.isRemoved
        
        if (sender.isRemoved)
        {
            sender.backgroundColor = sender.removedColor_bkg
            sender.setTitleColor(sender.removedColor_txt, for: .normal)
            
            // Register as crossed off with AA
            AASDK.reportItem(sender.itemName, crossedOffList: "grocery list")
        }
        else
        {
            sender.backgroundColor = sender.activeColor_bkg
            sender.setTitleColor(sender.removedColor_txt, for: .normal)
        }
        
        refreshScrollView()
    }
    
    func addItemToScrollView(item: UIItemButton)
    {
        item.frame = CGRect(x: 0, y: 60 * (itemScrollView.subviews.count - numItemScrollViewDefaultSubviews), width: Int(itemScrollView.contentSize.width), height: 50)
        
        itemScrollView.contentSize = CGSize(width: itemScrollView.frame.width, height: CGFloat(60 * (itemScrollView.subviews.count - numItemScrollViewDefaultSubviews + 1)))
        
        itemScrollView.addSubview(item)
    }
    
    /*
        Refreshes the item scroll view by analyzing which items should be showing up
        and removing those that shouldn't and adding those that should.
    */
    func refreshScrollView()
    {
        // Clear the list first.
        for item in allItems
        {
            item.removeFromSuperview()
        }
        
        // Now display items that should be displayed.
        for item in allItems
        {
            if (!item.isRemoved || (item.isRemoved && showRemoved))
            {
                addItemToScrollView(item: item)
            }
        }
    }
}

extension ViewController : AAZoneViewOwner
{
    func viewControllerForPresentingModalView() -> UIViewController! {
        return self
    }
}

/*
    Allows access to the payloads.
 */
extension ViewController : AASDKContentDelegate
{

    // see: http://dev.adadapted.com/ios/4.0.2_e810178/html_docs/ad_content.html
    func aaContentNotification(_ notification: Notification) {
        guard let userinfo = notification.userInfo else { return }
        guard let payload = userinfo[AASDK_KEY_PAYLOAD] as? [AnyHashable:Any] else { return }
        guard let items   = payload["list-items"] as? [String] else { return }
        
        for item in items
        {
            addItem(itemName: item, itemQuantity: 1)
        }
    }
    
    // see: http://dev.adadapted.com/ios/4.0.2_e810178/html_docs/payload_content.html
    func aaPayloadNotification(_ notification: Notification)
    {
        guard let userinfo = notification.userInfo else { return }
        guard let payloads = userinfo[AASDK_KEY_CONTENT_PAYLOADS] as? [AAContentPayload] else { return }
        
        for payload in payloads
        {
            for detailedItem in payload.detailedListItems
            {
                // App-specific handling
                addItem(itemName: detailedItem.productTitle, itemQuantity: 1)
                
                // Report items added (or crossed off, or deleted) to/from list.
                // For this app, the item is registered as crossed off in the btn_item method. The item is registered as deleted when the clearList method is called.
                AASDK.reportItem(detailedItem.productTitle, addedToList: "grocery list")
            }
            
            // Accept or reject the payload
            payload.reportReceivedOntoList("grocery list")
        }
    }
}

