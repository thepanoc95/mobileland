#import "UIColor.h"
#import <CoreGraphics/CGColor.h>
#import <CoreGraphics/CGColorSpace.h>

@implementation UIColor

+ (UIColor *)whiteColor {
    return [[[self alloc] initWithWhite:1.0 alpha:1.0] autorelease];
}

+ (UIColor *)blackColor {
    return [[[self alloc] initWithWhite:0.0 alpha:1.0] autorelease];
}

+ (UIColor *)clearColor {
    return [[[self alloc] initWithWhite:0.0 alpha:0.0] autorelease];
}

+ (UIColor *)redColor {
    return [[[self alloc] initWithRed:1.0 green:0.0 blue:0.0 alpha:1.0] autorelease];
}

+ (UIColor *)greenColor {
    return [[[self alloc] initWithRed:0.0 green:1.0 blue:0.0 alpha:1.0] autorelease];
}

+ (UIColor *)blueColor {
    return [[[self alloc] initWithRed:0.0 green:0.0 blue:1.0 alpha:1.0] autorelease];
}

+ (UIColor *)grayColor {
    return [[[self alloc] initWithWhite:0.5 alpha:1.0] autorelease];
}

+ (UIColor *)lightGrayColor {
    return [[[self alloc] initWithWhite:0.667 alpha:1.0] autorelease];
}

+ (UIColor *)darkGrayColor {
    return [[[self alloc] initWithWhite:0.333 alpha:1.0] autorelease];
}

+ (UIColor *)orangeColor {
    return [[[self alloc] initWithRed:1.0 green:0.5 blue:0.0 alpha:1.0] autorelease];
}

+ (UIColor *)yellowColor {
    return [[[self alloc] initWithRed:1.0 green:1.0 blue:0.0 alpha:1.0] autorelease];
}

+ (UIColor *)cyanColor {
    return [[[self alloc] initWithRed:0.0 green:1.0 blue:1.0 alpha:1.0] autorelease];
}

+ (UIColor *)magentaColor {
    return [[[self alloc] initWithRed:1.0 green:0.0 blue:1.0 alpha:1.0] autorelease];
}

+ (UIColor *)brownColor {
    return [[[self alloc] initWithRed:0.6 green:0.4 blue:0.2 alpha:1.0] autorelease];
}

+ (UIColor *)purpleColor {
    return [[[self alloc] initWithRed:0.5 green:0.0 blue:0.5 alpha:1.0] autorelease];
}

+ (UIColor *)colorWithWhite:(CGFloat)white alpha:(CGFloat)alpha {
    return [[[self alloc] initWithWhite:white alpha:alpha] autorelease];
}

+ (UIColor *)colorWithRed:(CGFloat)red green:(CGFloat)green blue:(CGFloat)blue alpha:(CGFloat)alpha {
    return [[[self alloc] initWithRed:red green:green blue:blue alpha:alpha] autorelease];
}

+ (UIColor *)colorWithHue:(CGFloat)hue saturation:(CGFloat)saturation brightness:(CGFloat)brightness alpha:(CGFloat)alpha {
    return [[[self alloc] initWithHue:hue saturation:saturation brightness:brightness alpha:alpha] autorelease];
}

+ (UIColor *)colorWithCGColor:(CGColorRef)CGColor {
    return [[[self alloc] initWithCGColor:CGColor] autorelease];
}

- initWithWhite:(CGFloat)white alpha:(CGFloat)alpha {
    self = [super init];
    if (self) {
        CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
        CGFloat components[] = {white, white, white, alpha};
        _CGColor = CGColorCreate(colorSpace, components);
        CGColorSpaceRelease(colorSpace);
    }
    return self;
}

- initWithRed:(CGFloat)red green:(CGFloat)green blue:(CGFloat)blue alpha:(CGFloat)alpha {
    self = [super init];
    if (self) {
        CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
        CGFloat components[] = {red, green, blue, alpha};
        _CGColor = CGColorCreate(colorSpace, components);
        CGColorSpaceRelease(colorSpace);
    }
    return self;
}

- initWithHue:(CGFloat)hue saturation:(CGFloat)saturation brightness:(CGFloat)brightness alpha:(CGFloat)alpha {
    self = [super init];
    if (self) {
        CGFloat r, g, b;
        CGFloat h = hue * 360.0f;
        CGFloat s = saturation;
        CGFloat v = brightness;

        CGFloat c = v * s;
        CGFloat x = c * (1.0f - fabsf(fmodf(h / 60.0f, 2.0f) - 1.0f));
        CGFloat m = v - c;

        if (h < 60)       { r = c; g = x; b = 0; }
        else if (h < 120) { r = x; g = c; b = 0; }
        else if (h < 180) { r = 0; g = c; b = x; }
        else if (h < 240) { r = 0; g = x; b = c; }
        else if (h < 300) { r = x; g = 0; b = c; }
        else              { r = c; g = 0; b = x; }

        CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
        CGFloat components[] = {r + m, g + m, b + m, alpha};
        _CGColor = CGColorCreate(colorSpace, components);
        CGColorSpaceRelease(colorSpace);
    }
    return self;
}

- initWithCGColor:(CGColorRef)CGColor {
    self = [super init];
    if (self) {
        _CGColor = CGColorRetain(CGColor);
    }
    return self;
}

- (void)dealloc {
    if (_CGColor) {
        CGColorRelease(_CGColor);
    }
    [super dealloc];
}

- (id)copyWithZone:(NSZone *)zone {
    UIColor *copy = [[UIColor allocWithZone:zone] initWithCGColor:_CGColor];
    return copy;
}

- (CGColorRef)CGColor {
    return _CGColor;
}

- (CGFloat)redComponent {
    const CGFloat *components = CGColorGetComponents(_CGColor);
    return components[0];
}

- (CGFloat)greenComponent {
    const CGFloat *components = CGColorGetComponents(_CGColor);
    return components[1];
}

- (CGFloat)blueComponent {
    const CGFloat *components = CGColorGetComponents(_CGColor);
    return components[2];
}

- (CGFloat)alphaComponent {
    return CGColorGetAlpha(_CGColor);
}

- (CGFloat)whiteComponent {
    const CGFloat *components = CGColorGetComponents(_CGColor);
    return components[0];
}

- (CGFloat)hueComponent {
    CGFloat r = [self redComponent];
    CGFloat g = [self greenComponent];
    CGFloat b = [self blueComponent];
    CGFloat max = fmaxf(r, fmaxf(g, b));
    CGFloat min = fminf(r, fminf(g, b));
    CGFloat d = max - min;
    if (d == 0) return 0;
    CGFloat h = 0;
    if (max == r)      h = fmodf((g - b) / d, 6.0f);
    else if (max == g) h = (b - r) / d + 2.0f;
    else               h = (r - g) / d + 4.0f;
    h *= 60.0f;
    if (h < 0) h += 360.0f;
    return h / 360.0f;
}

- (CGFloat)saturationComponent {
    CGFloat r = [self redComponent];
    CGFloat g = [self greenComponent];
    CGFloat b = [self blueComponent];
    CGFloat max = fmaxf(r, fmaxf(g, b));
    CGFloat min = fminf(r, fminf(g, b));
    CGFloat d = max - min;
    if (max == 0) return 0;
    return d / max;
}

- (CGFloat)brightnessComponent {
    CGFloat r = [self redComponent];
    CGFloat g = [self greenComponent];
    CGFloat b = [self blueComponent];
    return fmaxf(r, fmaxf(g, b));
}

- (UIColor *)colorWithAlphaComponent:(CGFloat)alpha {
    const CGFloat *components = CGColorGetComponents(_CGColor);
    size_t count = CGColorGetNumberOfComponents(_CGColor);
    CGColorSpaceRef colorSpace = CGColorGetColorSpace(_CGColor);

    if (count == 4) {
        CGFloat newComponents[] = {components[0], components[1], components[2], alpha};
        CGColorRef newColor = CGColorCreate(colorSpace, newComponents);
        UIColor *result = [UIColor colorWithCGColor:newColor];
        CGColorRelease(newColor);
        return result;
    } else if (count == 2) {
        CGFloat newComponents[] = {components[0], alpha};
        CGColorRef newColor = CGColorCreate(colorSpace, newComponents);
        UIColor *result = [UIColor colorWithCGColor:newColor];
        CGColorRelease(newColor);
        return result;
    }
    return self;
}

- (void)setFill {
    CGContextRef context = UIGraphicsGetCurrentContext();
    if (context) {
        CGContextSetFillColorWithColor(context, _CGColor);
    }
}

- (void)setStroke {
    CGContextRef context = UIGraphicsGetCurrentContext();
    if (context) {
        CGContextSetStrokeColorWithColor(context, _CGColor);
    }
}

- (BOOL)isEqual:(id)object {
    if (![object isKindOfClass:[UIColor class]]) return NO;
    UIColor *other = (UIColor *)object;
    return CGColorEqualToColor(_CGColor, [other CGColor]);
}

- (NSUInteger)hash {
    return (NSUInteger)(uintptr_t)_CGColor;
}

- (NSString *)description {
    const CGFloat *c = CGColorGetComponents(_CGColor);
    size_t count = CGColorGetNumberOfComponents(_CGColor);
    if (count == 4) {
        return [NSString stringWithFormat:@"UIColor %p: rgba(%.2f, %.2f, %.2f, %.2f)", self, c[0], c[1], c[2], c[3]];
    } else if (count == 2) {
        return [NSString stringWithFormat:@"UIColor %p: wa(%.2f, %.2f)", self, c[0], c[1]];
    }
    return [NSString stringWithFormat:@"UIColor %p: <unknown color space>", self];
}

@end
