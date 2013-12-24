//
//  ViewController.h
//  QR code
//
//  Created by 斌 on 12-8-2.
//  Copyright (c) 2012年 斌. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <UIKit/UIWindow.h>
#import "ZBarSDK.h"
#import "AppController.h"

@interface ViewController : UIViewController<ZBarReaderDelegate>

- (void)pick;
- (void)setRootWindow:(UIWindow*)window;
- (void)setRootController:(AppController*)controller;
+ (ViewController *)sharedViewController;
@end
