#import "UITabBarController.h"
#import "UITabBar.h"
#import "UIViewController.h"
#import <CoreGraphics/CGContext.h>

@implementation UITabBarController

@synthesize viewControllers = _viewControllers;
@synthesize selectedViewController = _selectedViewController;
@synthesize tabBar = _tabBar;
@synthesize selectedIndex = _selectedIndex;

- (instancetype)init {
    self = [super init];
    if (self) {
        _tabBar = [[UITabBar alloc] initWithFrame:CGRectZero];
        _selectedIndex = 0;
    }
    return self;
}

- (void)dealloc {
    [_viewControllers release];
    [_selectedViewController release];
    [_tabBar release];
    [super dealloc];
}

- (void)setViewControllers:(NSArray *)viewControllers {
    [self setViewControllers:viewControllers animated:NO];
}

- (void)setViewControllers:(NSArray *)viewControllers animated:(BOOL)animated {
    [_viewControllers release];
    _viewControllers = [viewControllers copy];

    NSMutableArray *items = [NSMutableArray array];
    for (UIViewController *vc in _viewControllers) {
        UITabBarItem *item = [[[UITabBarItem alloc] initWithTitle:vc.title image:nil tag:0] autorelease];
        [items addObject:item];
    }
    _tabBar.items = items;

    if ([_viewControllers count] > 0 && _selectedIndex < (NSInteger)[_viewControllers count]) {
        self.selectedViewController = [_viewControllers objectAtIndex:_selectedIndex];
    }
}

- (void)setSelectedViewController:(UIViewController *)selectedViewController {
    if (_selectedViewController != selectedViewController) {
        [_selectedViewController.view removeFromSuperview];
        [_selectedViewController release];
        _selectedViewController = [selectedViewController retain];

        NSInteger index = [_viewControllers indexOfObjectIdenticalTo:selectedViewController];
        if (index != NSNotFound) {
            _selectedIndex = index;
            _tabBar.selectedItem = [_tabBar.items objectAtIndex:index];
        }
    }
}

- (void)setSelectedIndex:(NSInteger)selectedIndex {
    if (selectedIndex >= 0 && selectedIndex < (NSInteger)[_viewControllers count]) {
        self.selectedViewController = [_viewControllers objectAtIndex:selectedIndex];
    }
}

- (void)loadView {
    UIView *container = [[UIView alloc] initWithFrame:CGRectZero];
    self.view = container;
    [container release];

    [self.view addSubView:_tabBar];
}

- (void)viewDidLayoutSubviews {
    [super viewDidLayoutSubviews];
    CGRect bounds = self.view.bounds;

    CGFloat tabBarHeight = 49.0f;
    _tabBar.frame = CGRectMake(0, bounds.size.height - tabBarHeight, bounds.size.width, tabBarHeight);

    if (_selectedViewController && _selectedViewController.isViewLoaded) {
        _selectedViewController.view.frame = CGRectMake(0, 0, bounds.size.width, bounds.size.height - tabBarHeight);
    }
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    UITouch *touch = [touches anyObject];
    CGPoint point = [touch locationInView:_tabBar];

    NSUInteger count = [_tabBar.items count];
    if (count == 0) return;
    CGFloat tabWidth = _tabBar.bounds.size.width / count;
    NSInteger tappedIndex = (NSInteger)(point.x / tabWidth);

    if (tappedIndex >= 0 && tappedIndex < (NSInteger)count) {
        self.selectedIndex = tappedIndex;
    }
}

@end
