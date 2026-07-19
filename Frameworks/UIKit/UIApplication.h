#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, UIUserInterfaceLayoutDirection) {
    UIUserInterfaceLayoutDirectionLeftToRight,
    UIUserInterfaceLayoutDirectionRightToLeft,
};

typedef NS_ENUM(NSInteger, UIStatusBarStyle) {
    UIStatusBarStyleDefault,
    UIStatusBarStyleLightContent,
};

typedef NS_ENUM(NSInteger, UIStatusBarAnimation) {
    UIStatusBarAnimationNone,
    UIStatusBarAnimationFade,
    UIStatusBarAnimationSlide,
};

typedef NS_ENUM(NSInteger, UIBackgroundFetchResult) {
    UIBackgroundFetchResultNewData,
    UIBackgroundFetchResultNoData,
    UIBackgroundFetchResultFailed,
};

typedef NS_ENUM(NSInteger, UIUserInterfaceIdiom) {
    UIUserInterfaceIdiomPhone,
    UIUserInterfaceIdiomPad,
    UIUserInterfaceIdiomTV,
    UIUserInterfaceIdiomCarPlay,
};

typedef NS_ENUM(NSInteger, UIApplicationState) {
    UIApplicationStateActive,
    UIApplicationStateInactive,
    UIApplicationStateBackground,
};

typedef NSUInteger UIInterfaceOrientationMask;

@class UIWindow, UIApplication, UIView, UIViewController, UIImage, NSURL, UIScene;

@protocol UIApplicationDelegate <NSObject>
    @optional
    @property (nonatomic, strong) UIWindow *window;
    - (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions;
    - (void)applicationWillResignActive:(UIApplication *)application;
    - (void)applicationDidEnterBackground:(UIApplication *)application;
    - (void)applicationWillEnterForeground:(UIApplication *)application;
    - (void)applicationDidBecomeActive:(UIApplication *)application;
    - (void)applicationWillTerminate:(UIApplication *)application;
    - (BOOL)application:(UIApplication *)application openURL:(NSURL *)url options:(NSDictionary *)options;
@end

@interface UIApplication : UIResponder
    {
        id<UIApplicationDelegate> _delegate;
        UIWindow *_keyWindow;
        NSMutableArray *_windows;
        BOOL _statusBarHidden;
        UIStatusBarStyle _statusBarStyle;
        UIApplicationState _applicationState;
    }

    + (UIApplication *)sharedApplication;
    + (BOOL)isRunningInStoreDemoMode;

    @property (nonatomic, assign) id<UIApplicationDelegate> delegate;
    @property (nonatomic, readonly) UIWindow *keyWindow;
    @property (nonatomic, readonly) NSArray *windows;
    @property (nonatomic, readonly) UIApplicationState applicationState;
    @property (nonatomic, readonly) NSTimeInterval backgroundTimeRemaining;

    - (void)setStatusBarHidden:(BOOL)hidden withAnimation:(UIStatusBarAnimation)animation;
    - (BOOL)isStatusBarHidden;
    - (UIStatusBarStyle)statusBarStyle;

    - (BOOL)canOpenURL:(NSURL *)url;
    - (BOOL)openURL:(NSURL *)url;

    - (void)beginBackgroundTaskWithExpirationHandler:(void(^)(void))handler;
    - (void)endBackgroundTask:(NSUInteger)identifier;

    - (BOOL)sendAction:(SEL)action to:(id)target from:(id)sender forEvent:(UIEvent *)event;

    - (void)setBadgeNumber:(NSInteger)number;
    - (NSInteger)badgeNumber;

    - (UIUserInterfaceIdiom)userInterfaceIdiom;
@end

extern NSString *const UIApplicationDidFinishLaunchingNotification;
extern NSString *const UIApplicationWillResignActiveNotification;
extern NSString *const UIApplicationDidEnterBackgroundNotification;
extern NSString *const UIApplicationWillEnterForegroundNotification;
extern NSString *const UIApplicationDidBecomeActiveNotification;
extern NSString *const UIApplicationWillTerminateNotification;
extern NSString *const UIApplicationDidReceiveMemoryWarningNotification;

extern NSString *const UIApplicationLaunchOptionsURLKey;
extern NSString *const UIApplicationLaunchOptionsSourceApplicationKey;
