#import "AppController.h"
#import "EAGLView.h"
#import "cocos2d.h"
#import "AppDelegate.h"
#import "RootViewController.h"
#import "ZBarSDK.h"
#import "ZBarViewController.h"
#import "ZBarInterface.h"
#include "SceneHelper.h"


@implementation AppController

#pragma mark -
#pragma mark Application lifecycle

// cocos2d application instance
static AppDelegate s_sharedApplication;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {    
    
    // Override point for customization after application launch.

    // Add the view controller's view to the window and display.
    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
    CGRect rc = [window bounds];
    // Init the EAGLView
    EAGLView *__glView = [EAGLView viewWithFrame: [window bounds]
                                     pixelFormat: kEAGLColorFormatRGB565
                                     depthFormat: GL_DEPTH24_STENCIL8_OES
                              preserveBackbuffer: NO
                                      sharegroup: nil
                                   multiSampling: NO
                                 numberOfSamples: 0];

    // Use RootViewController manage EAGLView 
    viewController = [[RootViewController alloc] initWithNibName:nil bundle:nil];
    viewController.wantsFullScreenLayout = YES;
    viewController.view = __glView;

    // Set RootViewController to window
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        // warning: addSubView doesn't work on iOS6
        [window addSubview: viewController.view];
    }
    else
    {
        // use this method on ios6
        [window setRootViewController:viewController];
    }
    
    [window makeKeyAndVisible];
    
    [[UIApplication sharedApplication] setStatusBarHidden:true];
	
	//set root window
    [[ViewController sharedViewController] setRootWindow:window];
	[[ViewController sharedViewController] setRootController:self];
	rc = [__glView bounds];
    cocos2d::CCApplication::sharedApplication()->run();
    return YES;
}


- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
    cocos2d::CCDirector::sharedDirector()->pause();
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
    cocos2d::CCDirector::sharedDirector()->resume();
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
    cocos2d::CCApplication::sharedApplication()->applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
    cocos2d::CCApplication::sharedApplication()->applicationWillEnterForeground();
}

- (void)applicationWillTerminate:(UIApplication *)application {

    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
}
//DISRENDER
- (void) imagePickerController: (UIImagePickerController*) reader
 didFinishPickingMediaWithInfo: (NSDictionary*) info
{
    id<NSFastEnumeration> results =
    [info objectForKey: ZBarReaderControllerResults];
    ZBarSymbol *symbol = nil;
    for(symbol in results)
	{
        break;
	}
    
//    imageview.image =
    [info objectForKey: UIImagePickerControllerOriginalImage];
    
    [reader dismissModalViewControllerAnimated: YES];
    
	NSLog(@"getString %@", symbol.data);
    
    // connect
    NSString* temp = [NSString stringWithString:symbol.data];

    const char* c_storeIP = ZBarInterface::sharedZBarInterface()->getIP();
    NSString* storeIP = [NSString stringWithUTF8String:c_storeIP];
    if ([temp isEqualToString:storeIP] == NO)
    {
        ZBarInterface::sharedZBarInterface()->DisConnect();
        SceneHelper::sharedSceneHelper()->setSocketState(SOCKET_DISCONNECT);
    }
    
    NSArray* array = [temp componentsSeparatedByString:NSLocalizedString(@":", nil)];
    for (NSString* log in array)
    {
        NSLog(@"%@", log);
    }
    NSString* nsIP = [array objectAtIndex:0];
    NSString* nsPort = [array objectAtIndex:1];
    const char* ip = [nsIP UTF8String];
    int port = [nsPort intValue];
    NSLog(@"ip = %s", ip);
    NSLog(@"port = %i", port);
    NSLog(@"temp = %@", temp);
    ZBarInterface::sharedZBarInterface()->setIP([temp UTF8String]);
    ZBarInterface::sharedZBarInterface()->CreateConnect(ip, port);
    
    /*
	NSString *temp = [[NSString alloc]initWithString:symbol.data];
//NSString *temp = [[NSString alloc]initWithString:@"192.168.22.89:1819"];
//	NSString* string2 = [[NSString alloc]initWithString:@"@"];
	NSArray *arrWriterPos =[temp componentsSeparatedByString:NSLocalizedString(@" ", nil)];
	for(int i = 0; i < [arrWriterPos count]; i++)
	{
		NSLog(@"%@",[arrWriterPos objectAtIndex:i]);
	}
	
	NSString* data[5];
	const char* c_data[5];
	NSString* tag = [[NSString alloc]initWithString:@"@"];
	for (int i = 0; i < 5; i++)
	{
		NSRange range = [[arrWriterPos objectAtIndex:i] rangeOfString:tag];
		int location = range.location;
		data[i] = [[arrWriterPos objectAtIndex:i] substringFromIndex:location+1];
		c_data[i] = [data[i] UTF8String];
		NSLog(@"data: %@", data[i]);
		printf("char %s\n", c_data[i]);
	}    
	ZBarInterface::sharedZBarInterface()->DownLoadZip(c_data[0], c_data[1], c_data[2], c_data[3], c_data[4]);

//	NSRange range = [temp rangeOfString:string2];
//	int location = range.location;
//	
//	NSString *zip = [temp substringToIndex:location];
//	NSString *version = [temp substringFromIndex:location+1];
//	NSString *project = [temp substringFromIndex:location+3];
//	NSString *width = [temp substringFromIndex:location+4];
//	NSString *height = [temp substringFromIndex:location+5];
//	NSLog(@"%@\n %@\n %@\n %@\n %@\n", zip, version, project, width,height);

//	const char* c_ip = [ip UTF8String];
//	const char* c_port = [port UTF8String];
	//连接
//	bool bConnect = Connect_CreateClient(g_sArg, "192.168.22.254", 1818, 0, 0, enTcp);
//	ZBarInterface::sharedZBarInterface()->CreateConnet(c_ip, c_port);
     */
}


#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
}


- (void)dealloc {
    [window release];
    [super dealloc];
}


@end
