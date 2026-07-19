#import "UIVisualEffectView.h"
#import "UIVisualEffect.h"
#import "UIBlurEffect.h"
#import "UIColor.h"
#import <CoreGraphics/CGContext.h>
#import <QuartzCore/CALayer.h>

@implementation UIVisualEffectView
    @synthesize effect = _effect;
    - (instancetype)initWithEffect:(UIVisualEffect *)effect {
        self = [super initWithFrame:CGRectZero];
        if (self) {
            _effect = [effect copy];
            _contentView = [[UIView alloc] initWithFrame:CGRectZero];
            _contentView.clipsToBounds = YES;
            [super addSubView:_contentView];
        }
        return self;
    }

    - (void)dealloc {
        [_effect release];
        [_contentView release];
        [super dealloc];
    }

    - (UIView *)contentView {
        return _contentView;
    }

    - (void)setEffect:(UIVisualEffect *)effect {
        if (_effect != effect) {
            [_effect release];
            _effect = [effect copy];
            [self setNeedsDisplay];
        }
    }

    - (void)layoutSubviews {
        [super layoutSubviews];
        _contentView.frame = self.bounds;
    }

    - (void)addSubView:(UIView *)view {
        [_contentView addSubView:view];
    }

    - (void)drawRect:(CGRect)rect {
        CGContextRef context = UIGraphicsGetCurrentContext();
        if (!context) return;

        if ([_effect isKindOfClass:[UIBlurEffect class]]) {
            UIBlurEffect *blur = (UIBlurEffect *)_effect;
            CGFloat radius = [blur _blurRadius];

            switch (blur.style) {
                case UIBlurEffectStyleExtraLight:
                case UIBlurEffectStyleSystemUltraThinMaterial:
                    CGContextSetRGBFillColor(context, 1.0f, 1.0f, 1.0f, 0.75f);
                    break;
                case UIBlurEffectStyleLight:
                case UIBlurEffectStyleSystemThinMaterial:
                case UIBlurEffectStyleSystemMaterial:
                case UIBlurEffectStyleRegular:
                case UIBlurEffectStyleProminent:
                    CGContextSetRGBFillColor(context, 1.0f, 1.0f, 1.0f, 0.65f);
                    break;
                case UIBlurEffectStyleDark:
                case UIBlurEffectStyleSystemThickMaterial:
                case UIBlurEffectStyleSystemChromeMaterial:
                    CGContextSetRGBFillColor(context, 0.1f, 0.1f, 0.1f, 0.7f);
                    break;
            }

            CGContextFillRect(context, rect);

            // Specular highlight along the top edge (Liquid --ass effect)
            CGFloat highlightHeight = radius * 0.3f;
            CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
            CGFloat components[] = {1.0f, 1.0f, 1.0f, 0.25f};
            CGColorRef highlightColor = CGColorCreate(colorSpace, components);

            CGContextSetFillColorWithColor(context, highlightColor);
            CGRect highlightRect = CGRectMake(0, 0, rect.size.width, highlightHeight);
            CGContextFillRect(context, highlightRect);

            CGColorRelease(highlightColor);
            CGColorSpaceRelease(colorSpace);

            // Subtle border glow
            CGContextSetRGBStrokeColor(context, 1.0f, 1.0f, 1.0f, 0.15f);
            CGContextSetLineWidth(context, 0.5f);
            CGRect borderRect = CGRectInset(rect, 0.25f, 0.25f);
            CGContextStrokeRect(context, borderRect);
        }
    }
@end
