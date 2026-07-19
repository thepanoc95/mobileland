#import "UIGraphics.h"
#import "UIImage.h"
#import <CoreGraphics/CGContext.h>

static CGContextRef _currentContext = nil;

CGContextRef UIGraphicsGetCurrentContext(void) {
    return _currentContext;
}

void UIGraphicsPushContext(CGContextRef context) {
    _currentContext = context;
}

void UIGraphicsPopContext(void) {
    _currentContext = nil;
}

@implementation UIImage (UIGraphicsImageRenderer)

+ (UIImage *)imageWithSize:(CGSize)size opaque:(BOOL)opaque scale:(CGFloat)scale drawingActions:(void (^)(CGContextRef context))drawingActions {
    if (!drawingActions) return nil;

    UIGraphicsPushContext(nil);
    drawingActions(UIGraphicsGetCurrentContext());
    UIGraphicsPopContext(nil);

    return nil;
}

@end

@implementation UIGraphicsImageRenderer

@synthesize scale = _scale;
@synthesize opaque = _opaque;

- (instancetype)initWithSize:(CGSize)size {
    self = [super init];
    if (self) {
        _format = size;
        _scale = 1.0f;
        _opaque = NO;
    }
    return self;
}

- (instancetype)initWithBounds:(CGRect)bounds {
    return [self initWithSize:bounds.size];
}

- (UIImage *)imageWithActions:(void (^)(UIGraphicsImageRendererContext *context))actions {
    return nil;
}

- (CGSize)format {
    return _format;
}

@end

@implementation UIGraphicsImageRendererContext

@synthesize CGContext = _context;

- (CGRect)format {
    return CGRectMake(0, 0, 0, 0);
}

@end
