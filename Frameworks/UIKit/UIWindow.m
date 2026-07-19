#import "UIWindow.h"
#import "UIScreen.h"
#import "UIViewController.h"
#import <QuartzCore/CALayer.h>
#import <QuartzCore/CALayerContext.h>
#import <QuartzCore/CARenderer.h>

@implementation UIWindow

@synthesize screen = _screen;
@synthesize rootViewController = _rootViewController;

- (instancetype)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        _screen = [[UIScreen mainScreen] retain];
        _isKeyWindow = NO;
        _layer.anchorPoint = CGPointMake(0, 0);
        _layer.position = CGPointMake(frame.origin.x, frame.origin.y);
    }
    return self;
}

- (void)dealloc {
    [_screen release];
    [_rootViewController release];
    [super dealloc];
}

- (UIScreen *)screen {
    return _screen;
}

- (void)setScreen:(UIScreen *)screen {
    if (_screen != screen) {
        [_screen release];
        _screen = [screen retain];
    }
}

- (UIViewController *)rootViewController {
    return _rootViewController;
}

- (void)setRootViewController:(UIViewController *)rootViewController {
    if (_rootViewController != rootViewController) {
        [_rootViewController release];
        _rootViewController = [rootViewController retain];
        if (_rootViewController) {
            [self addSubView:[_rootViewController view]];
        }
    }
}

- (BOOL)isKeyWindow {
    return _isKeyWindow;
}

- (void)makeKeyAndVisible {
    [self makeKeyWindow];
    [self setHidden:NO];
}

- (void)makeKeyWindow {
    _isKeyWindow = YES;
}

- (void)resignKeyWindow {
    _isKeyWindow = NO;
}

- (void)bringSubviewToFront:(UIView *)view {
    [super bringSubviewToFront:view];
}

- (UIWindow *)window {
    return self;
}

- (void)setFrame:(CGRect)frame {
    [super setFrame:frame];
    [_rootViewController view].frame = CGRectMake(0, 0, frame.size.width, frame.size.height);
}

- (void)setBounds:(CGRect)bounds {
    [super setBounds:bounds];
    [_rootViewController view].frame = CGRectMake(0, 0, bounds.size.width, bounds.size.height);
}

@end
