#import <UIKit/UIKit.h>
#import "ZBarReaderController.h"

@class RootViewController;

@interface AppController : NSObject <ZBarReaderDelegate, UIApplicationDelegate> {
    UIWindow *window;
    RootViewController    *viewController;
}

@end

