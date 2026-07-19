#import "UIViewController.h"

@class UITabBar, UITabBarItem;

@interface UITabBarController : UIViewController {
    NSArray *_viewControllers;
    UIViewController *_selectedViewController;
    UITabBar *_tabBar;
    NSInteger _selectedIndex;
}

@property (nonatomic, copy) NSArray *viewControllers;
@property (nonatomic, strong) UIViewController *selectedViewController;
@property (nonatomic, readonly) UITabBar *tabBar;
@property (nonatomic) NSInteger selectedIndex;

- (void)setViewControllers:(NSArray *)viewControllers animated:(BOOL)animated;

@end

@protocol UITabBarControllerDelegate <NSObject>
@optional
- (BOOL)tabBarController:(UITabBarController *)tabBarController shouldSelectViewController:(UIViewController *)viewController;
- (void)tabBarController:(UITabBarController *)tabBarController didSelectViewController:(UIViewController *)viewController;
@end
