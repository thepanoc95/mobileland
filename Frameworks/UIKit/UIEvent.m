#import "UIEvent.h"

@implementation UIEvent

@synthesize type = _type;
@synthesize subtype = _subtype;
@synthesize timestamp = _timestamp;

- (NSSet *)allTouches {
    return [NSSet set];
}

- (NSSet *)touchesForView:(UIView *)view {
    return [NSSet set];
}

- (NSSet *)touchesForWindow:(UIWindow *)window {
    return [NSSet set];
}

@end

@implementation UITouch

@synthesize view = _view;
@synthesize window = _window;
@synthesize tapCount = _tapCount;
@synthesize timestamp = _timestamp;

- (CGPoint)locationInView:(UIView *)view {
    return _locationInWindow;
}

- (CGPoint)previousLocationInView:(UIView *)view {
    return _previousLocationInWindow;
}

- (UITouchPhase)phase {
    return (UITouchPhase)_phase;
}

@end

@implementation UIPressesEvent

@end
