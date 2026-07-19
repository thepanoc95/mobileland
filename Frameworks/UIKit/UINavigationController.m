#import "UINavigationController.h"
#import "UINavigationBar.h"
#import "UIViewController.h"
#import "UIColor.h"
#import <CoreGraphics/CGContext.h>

@implementation UINavigationController

@synthesize navigationBar = _navigationBar;
@synthesize navigationBarHidden = _navigationBarHidden;
@synthesize toolbarHidden = _toolbarHidden;

- (instancetype)initWithRootViewController:(UIViewController *)rootViewController {
    self = [super init];
    if (self) {
        _navigationBar = [[UINavigationBar alloc] initWithFrame:CGRectMake(0, 0, 320, 44)];
        _viewControllers = [[NSMutableArray alloc] init];
        _navigationBarHidden = NO;

        if (rootViewController) {
            [_viewControllers addObject:rootViewController];
            rootViewController->_navigationController = self;
        }
    }
    return self;
}

- (instancetype)initWithNavigationBarClass:(Class)navigationBarClass toolbarClass:(Class)toolbarClass {
    return [self initWithRootViewController:nil];
}

- (void)dealloc {
    [_navigationBar release];
    [_viewControllers release];
    [super dealloc];
}

- (NSArray *)viewControllers {
    return [[_viewControllers copy] autorelease];
}

- (void)setViewControllers:(NSArray *)viewControllers {
    [self setViewControllers:viewControllers animated:NO];
}

- (void)setViewControllers:(NSArray *)viewControllers animated:(BOOL)animated {
    [_viewControllers release];
    _viewControllers = [viewControllers mutableCopy];
    for (UIViewController *vc in _viewControllers) {
        vc->_navigationController = self;
    }
    [self setNeedsLayout];
}

- (UIViewController *)topViewController {
    return [_viewControllers lastObject];
}

- (UIViewController *)visibleViewController {
    return [self topViewController];
}

#pragma mark - Push/Pop

- (void)pushViewController:(UIViewController *)viewController animated:(BOOL)animated {
    if (!viewController) return;

    [_viewControllers addObject:viewController];
    viewController->_navigationController = self;

    UINavigationItem *item = [[[UINavigationItem alloc] initWithTitle:viewController.title] autorelease];
    [_navigationBar pushNavigationItem:item animated:animated];

    UIView *parentView = self.view;
    UIView *newView = viewController.view;

    if (animated) {
        newView.frame = CGRectMake(parentView.bounds.size.width, 0,
                                   parentView.bounds.size.width, parentView.bounds.size.height);
        [parentView addSubView:newView];

        [UIView animateWithDuration:0.35f animations:^{
            newView.frame = parentView.bounds;
            UIView *oldView = [self topViewController].view;
            oldView.frame = CGRectMake(-parentView.bounds.size.width / 3.0f, 0,
                                       parentView.bounds.size.width, parentView.bounds.size.height);
        } completion:^(BOOL finished) {
            // Remove old view
        }];
    } else {
        newView.frame = parentView.bounds;
        [parentView addSubView:newView];
    }
}

- (UIViewController *)popViewControllerAnimated:(BOOL)animated {
    if ([_viewControllers count] <= 1) return nil;

    UIViewController *popped = [[_viewControllers lastObject] autorelease];
    [_viewControllers removeLastObject];

    [_navigationBar popNavigationItemAnimated:animated];

    UIView *parentView = self.view;
    UIView *poppedView = popped.view;

    if (animated && [_viewControllers count] > 0) {
        UIViewController *previous = [_viewControllers lastObject];
        UIView *previousView = previous.view;

        previousView.frame = CGRectMake(-parentView.bounds.size.width / 3.0f, 0,
                                        parentView.bounds.size.width, parentView.bounds.size.height);
        [parentView insertSubview:previousView belowSubview:poppedView];

        [UIView animateWithDuration:0.35f animations:^{
            poppedView.frame = CGRectMake(parentView.bounds.size.width, 0,
                                          parentView.bounds.size.width, parentView.bounds.size.height);
            previousView.frame = parentView.bounds;
        } completion:^(BOOL finished) {
            [poppedView removeFromSuperview];
        }];
    } else {
        [poppedView removeFromSuperview];
    }

    return popped;
}

- (NSArray *)popToRootViewControllerAnimated:(BOOL)animated {
    if ([_viewControllers count] <= 1) return [NSArray array];

    UIViewController *root = [_viewControllers firstObject];
    NSMutableArray *popped = [NSMutableArray array];

    while ([_viewControllers count] > 1) {
        UIViewController *vc = [_viewControllers lastObject];
        [vc.view removeFromSuperview];
        [popped addObject:vc];
        [_viewControllers removeLastObject];
    }

    [_navigationBar popToRootViewControllerAnimated:animated];
    return popped;
}

- (NSArray *)popToViewController:(UIViewController *)viewController animated:(BOOL)animated {
    NSMutableArray *popped = [NSMutableArray array];
    while ([_viewControllers count] > 0 && [_viewControllers lastObject] != viewController) {
        UIViewController *vc = [_viewControllers lastObject];
        [vc.view removeFromSuperview];
        [popped addObject:vc];
        [_viewControllers removeLastObject];
    }
    return popped;
}

- (void)setNavigationBarHidden:(BOOL)navigationBarHidden animated:(BOOL)animated {
    _navigationBarHidden = navigationBarHidden;
    if (animated) {
        [UIView animateWithDuration:0.25f animations:^{
            self->_navigationBar.alpha = navigationBarHidden ? 0 : 1;
        }];
    } else {
        _navigationBar.hidden = navigationBarHidden;
    }
}

- (void)loadView {
    UIView *view = [[UIView alloc] initWithFrame:CGRectZero];
    self.view = view;
    [view release];

    if (!_navigationBarHidden) {
        [self.view addSubView:_navigationBar];
    }
}

- (void)viewDidLayoutSubviews {
    [super viewDidLayoutSubviews];
    CGRect bounds = self.view.bounds;

    if (!_navigationBarHidden) {
        _navigationBar.frame = CGRectMake(0, 0, bounds.size.width, 44);
        for (UIViewController *vc in _viewControllers) {
            if (vc.isViewLoaded) {
                vc.view.frame = CGRectMake(0, 44, bounds.size.width, bounds.size.height - 44);
            }
        }
    } else {
        for (UIViewController *vc in _viewControllers) {
            if (vc.isViewLoaded) {
                vc.view.frame = bounds;
            }
        }
    }
}

@end
