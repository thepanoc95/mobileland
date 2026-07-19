#import "UILabel.h"
#import "UIColor.h"
#import <CoreGraphics/CGContext.h>

@implementation UILabel

- (instancetype)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        _text = nil;
        _textColor = [[UIColor blackColor] retain];
        _font = nil;
        _textAlignment = 0;
        _lineBreakMode = UILineBreakModeTailTruncation;
        _numberOfLines = 1;
        _shadowOffset = CGSizeMake(0, -1);
        _adjustsFontSizeToFitWidth = NO;
        _minimumScaleFactor = 0.0f;
        _enabled = YES;
        _highlighted = NO;
    }
    return self;
}

- (void)dealloc {
    [_text release];
    [_textColor release];
    [_shadowColor release];
    [_font release];
    [_highlightedTextColor release];
    [super dealloc];
}

- (void)setText:(NSString *)text {
    if (_text != text) {
        [_text release];
        _text = [text copy];
        [self setNeedsDisplay];
        [self setNeedsLayout];
    }
}

- (NSString *)text {
    return _text;
}

- (void)setTextColor:(UIColor *)textColor {
    if (_textColor != textColor) {
        [_textColor release];
        _textColor = [textColor retain];
        [self setNeedsDisplay];
    }
}

- (void)setShadowColor:(UIColor *)shadowColor {
    if (_shadowColor != shadowColor) {
        [_shadowColor release];
        _shadowColor = [shadowColor retain];
        [self setNeedsDisplay];
    }
}

- (void)setFont:(UIFont *)font {
    if (_font != font) {
        [_font release];
        _font = [font retain];
        [self setNeedsDisplay];
        [self setNeedsLayout];
    }
}

- (void)setHighlightedTextColor:(UIColor *)highlightedTextColor {
    if (_highlightedTextColor != highlightedTextColor) {
        [_highlightedTextColor release];
        _highlightedTextColor = [highlightedTextColor retain];
        [self setNeedsDisplay];
    }
}

- (CGSize)textSize {
    if (!_text || [_text length] == 0) return CGSizeZero;
    // Approximate text size
    CGFloat charWidth = 8.0f;
    CGFloat lineHeight = 20.0f;
    NSUInteger length = [_text length];
    return CGSizeMake(length * charWidth, lineHeight);
}

- (CGSize)intrinsicContentSize {
    return [self textSize];
}

- (void)drawRect:(CGRect)rect {
    [super drawRect:rect];

    if (!_text || [_text length] == 0) return;

    CGContextRef context = UIGraphicsGetCurrentContext();
    if (!context) return;

    CGSize textSize = [self textSize];
    CGPoint textPoint;

    switch (_textAlignment) {
        case 0: // NSTextAlignmentLeft
            textPoint = CGPointMake(0, (rect.size.height - textSize.height) / 2.0f);
            break;
        case 1: // NSTextAlignmentCenter
            textPoint = CGPointMake((rect.size.width - textSize.width) / 2.0f,
                                   (rect.size.height - textSize.height) / 2.0f);
            break;
        case 2: // NSTextAlignmentRight
            textPoint = CGPointMake(rect.size.width - textSize.width,
                                   (rect.size.height - textSize.height) / 2.0f);
            break;
        default:
            textPoint = CGPointMake(0, (rect.size.height - textSize.height) / 2.0f);
            break;
    }

    // Shadow
    if (_shadowColor) {
        CGContextSetShadowWithColor(context, _shadowOffset, 1.0f, [_shadowColor CGColor]);
    }

    UIColor *drawColor = (_highlighted && _highlightedTextColor) ? _highlightedTextColor : _textColor;
    if (drawColor) {
        CGContextSetFillColorWithColor(context, [drawColor CGColor]);
    }

    // Draw each character (simplified text rendering)
    // In a real implementation this would use CoreText
    CGFloat x = textPoint.x;
    CGFloat y = textPoint.y + textSize.height * 0.75f;
    const char *str = [_text UTF8String];
    if (str) {
        CGContextShowTextAtPoint(context, x, y, str, strlen(str));
    }

    CGContextSetShadow(context, CGSizeZero, 0);
}

- (void)layoutSubviews {
    [super layoutSubviews];
}

@end
