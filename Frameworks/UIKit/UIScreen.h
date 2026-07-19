#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>

@class CALayer, UIScreenMode;

@interface UIScreen : NSObject {
    CGRect _bounds;
    CGFloat _scale;
    UIScreenMode *_currentMode;
    CALayer *_displayLayer;
}

+ (UIScreen *)mainScreen;
+ (UIScreen *)screens;

- (CGRect)bounds;
- (CGFloat)scale;
- (UIScreenMode *)currentMode;
- (void)setCurrentMode:(UIScreenMode *)mode;

- (CGRect)nativeBounds;

@end

@interface UIScreenMode : NSObject {
    CGSize _size;
    CGFloat _pixelAspectRatio;
}

@property (nonatomic, readonly) CGSize size;
@property (nonatomic, readonly) CGFloat pixelAspectRatio;

+ (UIScreenMode *)currentMode;
+ (NSArray *)availableModes;

@end
