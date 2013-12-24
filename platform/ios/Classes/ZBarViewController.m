//
//  ViewController.m
//  QR code
//
//  Created by 斌 on 12-8-2.
//  Copyright (c) 2012年 斌. All rights reserved.
//

#import "ZBarViewController.h"
#import "ZBarSDK.h"
#import "QRCodeGenerator.h"
#import "AppController.h"

@interface ViewController ()

@end

UIWindow *window;
AppController *controller;
@implementation ViewController
ViewController *s_controller;

- (void)pick
{
    /*扫描二维码部分：
    导入ZBarSDK文件并引入一下框架
    AVFoundation.framework
    CoreMedia.framework
    CoreVideo.framework
    QuartzCore.framework
    libiconv.dylib
    引入头文件#import “ZBarSDK.h” 即可使用
    当找到条形码时，会执行代理方法
    
    - (void) imagePickerController: (UIImagePickerController*) reader didFinishPickingMediaWithInfo: (NSDictionary*) info
    
    最后读取并显示了条形码的图片和内容。*/
    
    ZBarReaderViewController *reader = [ZBarReaderViewController new];
    reader.readerDelegate = controller;
//	reader.readerDelegate = self;
    reader.supportedOrientationsMask = ZBarOrientationMaskAll;
    
    ZBarImageScanner *scanner = reader.scanner;
    
    [scanner setSymbology: ZBAR_I25
                   config: ZBAR_CFG_ENABLE
                       to: 0];

	UIViewController *controller = [window rootViewController];
    [controller presentModalViewController: reader
                            animated: YES];
    [reader release];
}

- (void)setRootWindow:(UIWindow*)win
{
	window = win;
}
- (void)setRootController:(AppController*)control
{
	controller = control;
}
+ (ViewController *)sharedViewController
{
	if (s_controller == NULL)
	{
	  s_controller = [ViewController alloc];
	}
	return	s_controller;
}
- (void) imagePickerController: (UIImagePickerController*) reader
 didFinishPickingMediaWithInfo: (NSDictionary*) info
{
//    id<NSFastEnumeration> results =
//    [info objectForKey: ZBarReaderControllerResults];
//    ZBarSymbol *symbol = nil;
//    for(symbol in results)
//        break;
//    
//    [reader dismissModalViewControllerAnimated: YES];
//    
//    //判断是否包含 头'http:'
//    NSString *regex = @"http+:[^\\s]*";
//    NSPredicate *predicate = [NSPredicate predicateWithFormat:@"SELF MATCHES %@",regex];
//    
//    //判断是否包含 头'ssid:'
//    NSString *ssid = @"ssid+:[^\\s]*";;
//    NSPredicate *ssidPre = [NSPredicate predicateWithFormat:@"SELF MATCHES %@",ssid];
//	
//	NSLog(@"getString %@", symbol.data);
//	
//	const char* temp = [symbol.data UTF8String];
//	
//	
}

- (void)dealloc {
    [super dealloc];
}
@end
