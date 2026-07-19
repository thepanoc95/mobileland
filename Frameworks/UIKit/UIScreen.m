#import "UIScreen.h"

@implementation UIScreen

+ (UIScreen *)mainScreen {
    static UIScreen * mainScreen = nil;
    if (!mainScreen) {
        mainScreen = [[UIScreen alloc] init];
    }
    return mainScreen;
}

+ (UIScreen *)screens {
    return [NSArray arrayWithObject:[self mainScreen]];
}

- (instancetype)init {
    self = [super init];
    if (self) {
        _bounds = CGRectMake(0, 0, 375, 812);
        _scale = 2.0f;
    }
    return self;
}

- (CGRect)bounds {
    return _bounds;
}

- (CGFloat)scale {
    return _scale;
}

- (UIScreenMode *)currentMode {
    return _currentMode;
}

- (void)setCurrentMode:(UIScreenMode *)mode {
    if (_currentMode != mode) {
        [_currentMode release];
        _currentMode = [mode retain];
    }
}

- (CGRect)nativeBounds {
    return CGRectMake(0, 0, _bounds.size.width * _scale, _bounds.size.height * _scale);
}

@end

@implementation UIScreenMode

@synthesize size = _size;
@synthesize pixelAspectRatio = _pixelAspectRatio;

+ (UIScreenMode *)currentMode {
    static UIScreenMode * currentMode = nil;
    if (!currentMode) {
        currentMode = [[UIScreenMode alloc] init];
        currentMode->_size = CGSizeMake(375, 812);
        currentMode->_pixelAspectRatio = 1.0f;
    }
    return currentMode;
}

+ (NSArray *)availableModes {
    return [NSArray arrayWithObject:[self currentMode]];
}

@end
