#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>

@interface UIColor : NSObject <NSCopying> {
    CGColorRef _CGColor;
}

+ (UIColor *)whiteColor;
+ (UIColor *)blackColor;
+ (UIColor *)clearColor;
+ (UIColor *)redColor;
+ (UIColor *)greenColor;
+ (UIColor *)blueColor;
+ (UIColor *)grayColor;
+ (UIColor *)lightGrayColor;
+ (UIColor *)darkGrayColor;
+ (UIColor *)orangeColor;
+ (UIColor *)yellowColor;
+ (UIColor *)cyanColor;
+ (UIColor *)magentaColor;
+ (UIColor *)brownColor;
+ (UIColor *)purpleColor;

+ (UIColor *)colorWithWhite:(CGFloat)white alpha:(CGFloat)alpha;
+ (UIColor *)colorWithRed:(CGFloat)red green:(CGFloat)green blue:(CGFloat)blue alpha:(CGFloat)alpha;
+ (UIColor *)colorWithHue:(CGFloat)hue saturation:(CGFloat)saturation brightness:(CGFloat)brightness alpha:(CGFloat)alpha;
+ (UIColor *)colorWithCGColor:(CGColorRef)CGColor;

- initWithWhite:(CGFloat)white alpha:(CGFloat)alpha;
- initWithRed:(CGFloat)red green:(CGFloat)green blue:(CGFloat)blue alpha:(CGFloat)alpha;
- initWithHue:(CGFloat)hue saturation:(CGFloat)saturation brightness:(CGFloat)brightness alpha:(CGFloat)alpha;
- initWithCGColor:(CGColorRef)CGColor;

- (CGColorRef)CGColor;
- (CGFloat)redComponent;
- (CGFloat)greenComponent;
- (CGFloat)blueComponent;
- (CGFloat)alphaComponent;
- (CGFloat)whiteComponent;
- (CGFloat)hueComponent;
- (CGFloat)saturationComponent;
- (CGFloat)brightnessComponent;

- (UIColor *)colorWithAlphaComponent:(CGFloat)alpha;

- (void)setFill;
- (void)setStroke;

@end
