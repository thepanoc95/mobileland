#import "UIViewController.h"

@class UINavigationBar, UIBarButtonItem, UINavigationItem;

@interface UINavigationController : UIViewController {
    UINavigationBar *_navigationBar;
    NSMutableArray *_viewControllers;
    BOOL _navigationBarHidden;
    BOOL _toolbarHidden;
}

@property (nonatomic, readonly) UINavigationBar *navigationBar;
@property (nonatomic, copy) NSArray *viewControllers;
@property (nonatomic, readonly) UIViewController *topViewController;
@property (nonatomic, readonly) UIViewController *visibleViewController;
@property (nonatomic, getter=isNavigationBarHidden) BOOL navigationBarHidden;
@property (nonatomic) BOOL toolbarHidden;

- (instancetype)initWithRootViewController:(UIViewController *)rootViewController;
- (instancetype)initWithNavigationBarClass:(Class)navigationBarClass toolbarClass:(Class)toolbarClass;

- (void)pushViewController:(UIViewController *)viewController animated:(BOOL)animated;
- (UIViewController *)popViewControllerAnimated:(BOOL)animated;
- (NSArray *)popToRootViewControllerAnimated:(BOOL)animated;
- (NSArray *)popToViewController:(UIViewController *)viewController animated:(BOOL)animated;

- (void)setViewControllers:(NSArray *)viewControllers animated:(BOOL)animated;
- (void)setNavigationBarHidden:(BOOL)navigationBarHidden animated:(BOOL)animated;

@end
