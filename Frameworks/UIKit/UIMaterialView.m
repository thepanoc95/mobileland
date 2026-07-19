#import "UIMaterialView.h"
#import "UIColor.h"
#import <CoreGraphics/CGContext.h>
#import <CoreGraphics/CGGradient.h>
#import <CoreGraphics/CGColorSpace.h>
#import <QuartzCore/CALayer.h>

@implementation UIMaterialView

+ (UIMaterialView *)materialViewWithStyle:(UIMaterialStyle)style {
    return [[[self alloc] initWithStyle:style] autorelease];
}

+ (UIMaterialView *)materialViewWithStyle:(UIMaterialStyle)style tintColor:(UIColor *)tintColor {
    UIMaterialView *view = [[[self alloc] initWithStyle:style] autorelease];
    view.tintColor = tintColor;
    return view;
}

- (instancetype)initWithStyle:(UIMaterialStyle)style {
    self = [super initWithFrame:CGRectZero];
    if (self) {
        _materialStyle = style;
        _blurRadius = 20.0f;
        _cornerRadius = 16.0f;
        _automaticallyUpdatesTint = YES;
        _specularColor = [[UIColor colorWithWhite:1.0f alpha:0.18f] retain];

        [self applyMaterialStyle:style];
        self.clipsToBounds = YES;
        [_layer setCornerRadius:_cornerRadius];
    }
    return self;
}

- (void)dealloc {
    [_tintColor release];
    [_specularColor release];
    [super dealloc];
}

- (void)setTintColor:(UIColor *)tintColor {
    if (_tintColor != tintColor) {
        [_tintColor release];
        _tintColor = [tintColor retain];
        [self setNeedsDisplay];
    }
}

- (UIColor *)tintColor {
    return _tintColor;
}

- (void)setCornerRadius:(CGFloat)cornerRadius {
    _cornerRadius = cornerRadius;
    [_layer setCornerRadius:cornerRadius];
}

- (void)setBlurRadius:(CGFloat)blurRadius {
    _blurRadius = blurRadius;
    [self setNeedsDisplay];
}

- (void)applyMaterialStyle:(UIMaterialStyle)style {
    _materialStyle = style;

    switch (style) {
        case UIMaterialStyleThin:
            _blurRadius = 12.0f;
            [_tintColor release];
            _tintColor = [[UIColor colorWithWhite:1.0f alpha:0.15f] retain];
            break;
        case UIMaterialStyleRegular:
            _blurRadius = 20.0f;
            [_tintColor release];
            _tintColor = [[UIColor colorWithWhite:1.0f alpha:0.25f] retain];
            break;
        case UIMaterialStyleThick:
            _blurRadius = 30.0f;
            [_tintColor release];
            _tintColor = [[UIColor colorWithWhite:1.0f alpha:0.35f] retain];
            break;
        case UIMaterialStyleProminent:
            _blurRadius = 40.0f;
            [_tintColor release];
            _tintColor = [[UIColor colorWithWhite:1.0f alpha:0.45f] retain];
            break;
    }
}

- (void)drawRect:(CGRect)rect {
    CGContextRef context = UIGraphicsGetCurrentContext();
    if (!context) return;

    // 1) Base translucent fill
    CGFloat r = 1.0f, g = 1.0f, b = 1.0f, a = 0.30f;
    if (_tintColor) {
        r = [_tintColor redComponent];
        g = [_tintColor greenComponent];
        b = [_tintColor blueComponent];
        a = [_tintColor alphaComponent];
    }
    CGContextSetRGBFillColor(context, r, g, b, a);
    CGContextFillRect(context, rect);

    // 2) Gaussian blur effect layer (simulated with semi-transparent overlay)
    CGFloat blurAlpha = _blurRadius / 100.0f;
    CGContextSetRGBFillColor(context, r, g, b, blurAlpha * 0.4f);
    CGContextFillRect(context, rect);

    // 3) Specular highlight at the top (Liquid Glass light refraction)
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();

    CGFloat locations[] = {0.0f, 0.12f, 1.0f};
    CGFloat components[] = {
        1.0f, 1.0f, 1.0f, 0.32f,   // bright top highlight
        1.0f, 1.0f, 1.0f, 0.08f,   // fading mid
        1.0f, 1.0f, 1.0f, 0.0f     // transparent bottom
    };
    CGGradientRef gradient = CGGradientCreateWithColorComponents(colorSpace, components, locations, 3);
    CGContextDrawLinearGradient(context, gradient,
                                CGPointMake(0, CGRectGetMinY(rect)),
                                CGPointMake(0, CGRectGetMaxY(rect)),
                                0);
    CGGradientRelease(gradient);

    // 4) Subtle edge highlight (bevel)
    if (_specularColor) {
        CGContextSetStrokeColorWithColor(context, [_specularColor CGColor]);
        CGContextSetLineWidth(context, 0.5f);
        CGRect borderRect = CGRectInset(rect, 0.25f, 0.25f);
        CGContextStrokeRect(context, borderRect);
    }

    // 5) Inner shadow at the bottom for depth
    CGFloat shadowComponents[] = {0.0f, 0.0f, 0.0f, 0.08f};
    CGColorRef shadowColor = CGColorCreate(colorSpace, shadowComponents);
    CGGradientRef shadowGradient = CGGradientCreateWithColorComponents(colorSpace,
        (CGFloat[]){0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.08f},
        (CGFloat[]){0.0f, 1.0f}, 2);
    CGContextDrawLinearGradient(context, shadowGradient,
                                CGPointMake(0, CGRectGetMaxY(rect) - 8.0f),
                                CGPointMake(0, CGRectGetMaxY(rect)),
                                0);
    CGGradientRelease(shadowGradient);
    CGColorRelease(shadowColor);
    CGColorSpaceRelease(colorSpace);
}

@end
