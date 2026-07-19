#import "UIView.h"

@class UIWindow, UIScreen, UIViewController;

@interface UIWindow : UIView {
    UIScreen *_screen;
    UIViewController *_rootViewController;
    BOOL _isKeyWindow;
}

@property (nonatomic, strong) UIScreen *screen;
@property (nonatomic, strong) UIViewController *rootViewController;
@property (nonatomic, readonly, getter=isKeyWindow) BOOL keyWindow;

- (void)makeKeyAndVisible;
- (void)makeKeyWindow;
- (void)resignKeyWindow;
- (void)bringSubviewToFront:(UIView *)view;

@end
