#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>

@class UIView;

CGContextRef UIGraphicsGetCurrentContext(void);
void UIGraphicsPushContext(CGContextRef context);
void UIGraphicsPopContext(void);

@interface UIImage (UIGraphicsImageRenderer)

+ (UIImage *)imageWithSize:(CGSize)size opaque:(BOOL)opaque scale:(CGFloat)scale drawingActions:(void (^)(CGContextRef context))drawingActions;

@end

@interface UIGraphicsImageRenderer : NSObject {
    CGSize _format;
    CGFloat _scale;
    BOOL _opaque;
}

@property (nonatomic) CGSize format;
@property (nonatomic) CGFloat scale;
@property (nonatomic) BOOL opaque;

- (instancetype)initWithSize:(CGSize)size;
- (instancetype)initWithBounds:(CGRect)bounds;
- (UIImage *)imageWithActions:(void (^)(UIGraphicsImageRendererContext *context))actions;

@end

@interface UIGraphicsImageRendererContext : NSObject {
    CGContextRef _context;
    CGRect _bounds;
}

@property (nonatomic, readonly) CGContextRef CGContext;
@property (nonatomic, readonly) CGRect format;

@end
