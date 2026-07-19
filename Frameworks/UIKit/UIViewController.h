#import <Foundation/Foundation.h>
#import "UIView.h"

@class UIApplication, UINavigationController, UIWindow;

@interface UIViewController : UIResponder {
    UIView *_view;
    UIWindow *_viewWindow;
    NSString *_title;
    UIViewController *_parentViewController;
    UINavigationController *_navigationController;
    BOOL _isViewLoaded;
    BOOL _wantsFullScreenLayout;
    NSInteger _interfaceOrientation;
}

@property (nonatomic, strong) UIView *view;
@property (nonatomic, copy) NSString *title;
@property (nonatomic, readonly) UIViewController *parentViewController;
@property (nonatomic, readonly) UINavigationController *navigationController;
@property (nonatomic, readonly) UIWindow *viewWindow;
@property (nonatomic) BOOL wantsFullScreenLayout;
@property (nonatomic, readonly) BOOL isViewLoaded;

- (void)viewDidLoad;
- (void)viewWillAppear:(BOOL)animated;
- (void)viewDidAppear:(BOOL)animated;
- (void)viewWillDisappear:(BOOL)animated;
- (void)viewDidDisappear:(BOOL)animated;
- (void)viewDidUnload;
- (void)didReceiveMemoryWarning;

- (void)loadView;
- (BOOL)isViewLoaded;

- (void)presentViewController:(UIViewController *)viewControllerToPresent animated:(BOOL)flag completion:(void (^)(void))completion;
- (void)dismissViewControllerAnimated:(BOOL)flag completion:(void (^)(void))completion;

- (void)addChildViewController:(UIViewController *)childController;
- (void)removeFromParentViewController;
- (void)willMoveToParentViewController:(UIViewController *)parent;
- (void)didMoveToParentViewController:(UIViewController *)parent;

- (BOOL)shouldAutorotateToInterfaceOrientation:(NSInteger)interfaceOrientation;

- (UINavigationController *)navigationController;

@end
