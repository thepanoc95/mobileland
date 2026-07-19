#import "UIViewController.h"
#import "UIWindow.h"
#import "UINavigationController.h"

@implementation UIViewController

@synthesize title = _title;
@synthesize parentViewController = _parentViewController;
@synthesize wantsFullScreenLayout = _wantsFullScreenLayout;

- (instancetype)init {
    self = [super init];
    if (self) {
        _isViewLoaded = NO;
        _wantsFullScreenLayout = YES;
    }
    return self;
}

- (void)dealloc {
    [_view release];
    [_title release];
    [super dealloc];
}

- (UIView *)view {
    if (!_isViewLoaded) {
        [self loadView];
        [self viewDidLoad];
        _isViewLoaded = YES;
    }
    return _view;
}

- (void)setView:(UIView *)view {
    if (_view != view) {
        [_view release];
        _view = [view retain];
        _isViewLoaded = (view != nil);
    }
}

- (BOOL)isViewLoaded {
    return _isViewLoaded;
}

- (void)loadView {
    UIView *view = [[UIView alloc] initWithFrame:CGRectZero];
    self.view = view;
    [view release];
}

- (void)viewDidLoad {
}

- (void)viewWillAppear:(BOOL)animated {
}

- (void)viewDidAppear:(BOOL)animated {
}

- (void)viewWillDisappear:(BOOL)animated {
}

- (void)viewDidDisappear:(BOOL)animated {
}

- (void)viewDidUnload {
    self.view = nil;
    _isViewLoaded = NO;
}

- (void)didReceiveMemoryWarning {
    if (_isViewLoaded && !_view.window) {
        [self viewDidUnload];
    }
}

- (UIWindow *)viewWindow {
    if (_view) {
        return [_view window];
    }
    return nil;
}

#pragma mark - Navigation

- (UINavigationController *)navigationController {
    return _navigationController;
}

#pragma mark - Presentation

- (void)presentViewController:(UIViewController *)viewControllerToPresent animated:(BOOL)flag completion:(void (^)(void))completion {
    if (!viewControllerToPresent) return;

    UIView *presentedView = [viewControllerToPresent view];

    UIWindow *keyWindow = [[UIApplication sharedApplication] keyWindow];
    if (keyWindow) {
        presentedView.frame = keyWindow.bounds;
        [keyWindow addSubView:presentedView];
    }

    [viewControllerToPresent viewWillAppear:flag];

    if (completion) {
        completion();
    }
}

- (void)dismissViewControllerAnimated:(BOOL)flag completion:(void (^)(void))completion {
    [self.view removeFromSuperview];
    [self viewWillDisappear:flag];

    if (_parentViewController) {
        [_parentViewController viewDidAppear:flag];
    }

    if (completion) {
        completion();
    }
}

#pragma mark - Child View Controllers

- (void)addChildViewController:(UIViewController *)childController {
    [childController willMoveToParentViewController:self];
    childController->_parentViewController = self;
}

- (void)removeFromParentViewController {
    [self willMoveToParentViewController:nil];
    _parentViewController = nil;
}

- (void)willMoveToParentViewController:(UIViewController *)parent {
}

- (void)didMoveToParentViewController:(UIViewController *)parent {
}

#pragma mark - Rotation

- (BOOL)shouldAutorotateToInterfaceOrientation:(NSInteger)interfaceOrientation {
    return YES;
}

@end
