#import "UIApplication.h"
#import "UIWindow.h"
#import "UIScreen.h"
#import "UIViewController.h"

#if defined(__GNUSTEP__)
    #error "GNUstep is not supported for UIKit! Sorry."
#endif

NSString *const UIApplicationDidFinishLaunchingNotification = @"UIApplicationDidFinishLaunchingNotification";
NSString *const UIApplicationWillResignActiveNotification = @"UIApplicationWillResignActiveNotification";
NSString *const UIApplicationDidEnterBackgroundNotification = @"UIApplicationDidEnterBackgroundNotification";
NSString *const UIApplicationWillEnterForegroundNotification = @"UIApplicationWillEnterForegroundNotification";
NSString *const UIApplicationDidBecomeActiveNotification = @"UIApplicationDidBecomeActiveNotification";
NSString *const UIApplicationWillTerminateNotification = @"UIApplicationWillTerminateNotification";
NSString *const UIApplicationDidReceiveMemoryWarningNotification = @"UIApplicationDidReceiveMemoryWarningNotification";

NSString *const UIApplicationLaunchOptionsURLKey = @"UIApplicationLaunchOptionsURLKey";
NSString *const UIApplicationLaunchOptionsSourceApplicationKey = @"UIApplicationLaunchOptionsSourceApplicationKey";

@implementation UIApplication
    + (UIApplication *)sharedApplication {
        static UIApplication *sharedApp = nil;
        return sharedApp;
    }

    + (void)initialize {
        if (self == [UIApplication class]) {
            // Will be set during launch
        }
    }

    + (BOOL)isRunningInStoreDemoMode {
        return NO;
    }

    - (instancetype)init {
        self = [super init];
        if (self) {
            _windows = [[NSMutableArray alloc] init];
            _statusBarHidden = NO;
            _statusBarStyle = UIStatusBarStyleDefault;
            _applicationState = UIApplicationStateActive;
        }
        return self;
    }

    - (void)dealloc {
        [_windows release];
        [super dealloc];
    }

    - (id<UIApplicationDelegate>)delegate {
        return _delegate;
    }

    - (void)setDelegate:(id<UIApplicationDelegate>)delegate {
        _delegate = delegate;
    }

    - (UIWindow *)keyWindow {
        for (UIWindow *window in _windows) {
            if ([window isKeyWindow]) {
                return window;
            }
        }
        return _keyWindow;
    }

    - (NSArray *)windows {
        return [[_windows copy] autorelease];
    }

    - (UIApplicationState)applicationState {
        return _applicationState;
    }

    - (NSTimeInterval)backgroundTimeRemaining {
        return 0;
    }

    #pragma mark - Status Bar

    - (void)setStatusBarHidden:(BOOL)hidden withAnimation:(UIStatusBarAnimation)animation {
        _statusBarHidden = hidden;
    }

    - (BOOL)isStatusBarHidden {
        return _statusBarHidden;
    }

    - (UIStatusBarStyle)statusBarStyle {
        return _statusBarStyle;
    }

    #pragma mark - URL Handling

    - (BOOL)canOpenURL:(NSURL *)url {
        return url != nil;
    }

    - (BOOL)openURL:(NSURL *)url {
        if (url) {
            return YES;
        }
        return NO;
    }

    #pragma mark - Background Tasks

    - (void)beginBackgroundTaskWithExpirationHandler:(void(^)(void))handler {
        // Stub for now
    }

    - (void)endBackgroundTask:(NSUInteger)identifier {
        // Stub for now
    }

    #pragma mark - Actions

    - (BOOL)sendAction:(SEL)action to:(id)target from:(id)sender forEvent:(UIEvent *)event {
        if (target) {
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Warc-performSelector-leaks"
            [target performSelector:action withObject:sender];
    #pragma clang diagnostic pop
            return YES;
        }
        return NO;
    }

    #pragma mark - Badge

    - (void)setBadgeNumber:(NSInteger)number {
        // Stub for now
    }

    - (NSInteger)badgeNumber {
        return 0;
    }

    #pragma mark - Device

    - (UIUserInterfaceIdiom)userInterfaceIdiom {
        return UIUserInterfaceIdiomPhone;
    }
@end
