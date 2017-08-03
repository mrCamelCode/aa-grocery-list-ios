//
//  UIItemButton.swift
//  AA Grocery List
//
//  Created by Mike Pedersen on 7/27/17.
//  Copyright © 2017 Joshua Truitt. All rights reserved.
//

import Foundation
import UIKit
/*
    A custom button class that represents grocery list items. Override
    UIButton because the behaviour of a list item is similar to that
    of a button. With a custom button class, we can create our own unique
    fields.
 */
class UIItemButton : UIButton
{
    var isRemoved: Bool = false
    var text: String?
    {
        get { return titleLabel?.text }
        set(value)
        {
            setTitle(value, for: .normal)
        }
    }
    
    var itemName: String = ""
    var itemQuantity: Int = 0
    
    let activeColor_bkg: UIColor = UIColor(red: 0, green: 0.3, blue: 1, alpha: 0.7)
    let removedColor_bkg: UIColor = UIColor(red: 1, green: 0, blue: 0, alpha: 0.7)
    let activeColor_txt: UIColor = UIColor.white
    let removedColor_txt: UIColor = UIColor.white
    
    override init(frame: CGRect)
    {
        super.init(frame: frame)
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    // Use this initializer
    init (frame: CGRect, itemName: String, itemQuantity: Int)
    {
        super.init(frame: frame)
        
        self.itemName = itemName
        // Default quantity of 1
        self.itemQuantity = itemQuantity > 0 ? itemQuantity : 1
        
        // Initialize the appearance of the button.
        backgroundColor = activeColor_bkg
        setTitleColor(activeColor_txt, for: .normal)
        titleLabel?.textRect(forBounds: CGRect.init(x: 0, y: 0, width: bounds.width, height: bounds.height), limitedToNumberOfLines: 1)
        titleLabel?.textAlignment = NSTextAlignment.center
        
        setTitle("\(itemName) (\(itemQuantity))", for: .normal)
    }
 
    func set(itemName: String!, itemQuantity: Int)
    {
        self.itemName = itemName
        self.itemQuantity = itemQuantity
        
        setTitle("\(itemName!) (\(itemQuantity))", for: .normal)
    }
}
