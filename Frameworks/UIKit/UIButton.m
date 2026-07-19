#import "UIButton.h"
#import "UILabel.h"
#import "UIColor.h"
#import "UIImage.h"
#import <CoreGraphics/CGContext.h>
#import <QuartzCore/CALayer.h>

@implementation UIButton

@synthesize titleLabel = _titleLabel;
@synthesize adjustsImageWhenHighlighted = _adjustsImageWhenHighlighted;
@synthesize adjustsImageWhenDisabled = _adjustsImageWhenDisabled;
@synthesize imageEdgeInsets = _imageEdgeInsets;
@synthesize titleEdgeInsets = _titleEdgeInsets;
@synthesize contentEdgeInsets = _contentEdgeInsets;

+ (UIButton *)buttonWithType:(UIButtonType)buttonType {
    UIButton *button = [[[self alloc] initWithFrame:CGRectZero] autorelease];
    return button;
}

- (instancetype)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        _titleLabel = [[UILabel alloc] initWithFrame:CGRectZero];
        _titleLabel.backgroundColor = [UIColor clearColor];
        _titleLabel.textAlignment = 1; // center
        [super addSubView:_titleLabel];

        _adjustsImageWhenHighlighted = YES;
        _adjustsImageWhenDisabled = YES;

        _titleLabelColor = [[UIColor whiteColor] retain];
        _titleLabelHighlightColor = [[UIColor lightGrayColor] retain];

        self.backgroundColor = [UIColor colorWithRed:0.0f green:0.478f blue:1.0f alpha:1.0f];
    }
    return self;
}

- (void)dealloc {
    [_titleLabel release];
    [_titleLabelColor release];
    [_titleLabelHighlightColor release];
    [_currentImage release];
    [_currentBackgroundImage release];
    [_currentTitle release];
    [super dealloc];
}

#pragma mark - Title

- (void)setTitle:(NSString *)title forState:(NSUInteger)state {
    [_currentTitle release];
    _currentTitle = [title copy];
    _titleLabel.text = _currentTitle;
    [self setNeedsLayout];
    [self setNeedsDisplay];
}

- (NSString *)titleForState:(NSUInteger)state {
    return _currentTitle;
}

- (void)setTitleColor:(UIColor *)color forState:(NSUInteger)state {
    [_titleLabelColor release];
    _titleLabelColor = [color retain];
    _titleLabel.textColor = color;
}

- (UIColor *)titleColorForState:(NSUInteger)state {
    return _titleLabelColor;
}

- (void)setTitleShadowColor:(UIColor *)color forState:(NSUInteger)state {
}

#pragma mark - Image

- (void)setImage:(UIImage *)image forState:(NSUInteger)state {
    [_currentImage release];
    _currentImage = [image retain];
    [self setNeedsDisplay];
}

- (UIImage *)imageForState:(NSUInteger)state {
    return _currentImage;
}

- (void)setBackgroundImage:(UIImage *)image forState:(NSUInteger)state {
    [_currentBackgroundImage release];
    _currentBackgroundImage = [image retain];
    [self setNeedsDisplay];
}

- (UIImage *)backgroundImageForState:(NSUInteger)state {
    return _currentBackgroundImage;
}

#pragma mark - Layout

- (CGSize)intrinsicContentSize {
    CGSize titleSize = [_titleLabel.text sizeWithAttributes:nil];
    return CGSizeMake(titleSize.width + 20.0f, titleSize.height + 10.0f);
}

- (void)sizeToFit {
    CGSize size = [self intrinsicContentSize];
    CGRect frame = self.frame;
    frame.size = size;
    self.frame = frame;
}

- (void)layoutSubviews {
    [super layoutSubviews];

    CGRect bounds = self.bounds;

    // Center the label
    CGSize titleSize = [_titleLabel.text sizeWithAttributes:nil];
    _titleLabel.frame = CGRectMake((bounds.size.width - titleSize.width) / 2.0f,
                                   (bounds.size.height - titleSize.height) / 2.0f,
                                   titleSize.width,
                                   titleSize.height);
}

- (void)drawRect:(CGRect)rect {
    CGContextRef context = UIGraphicsGetCurrentContext();
    if (!context) return;

    // Background image or default background
    if (_currentBackgroundImage) {
        [_currentBackgroundImage drawInRect:rect];
    } else {
        // Default rounded rect button appearance
        UIColor *bgColor = self.backgroundColor ?: [UIColor colorWithRed:0.0f green:0.478f blue:1.0f alpha:1.0f];
        CGContextSetFillColorWithColor(context, [bgColor CGColor]);

        CGFloat radius = 8.0f;
        CGContextMoveToPoint(context, CGRectGetMinX(rect) + radius, CGRectGetMinY(rect));
        CGContextAddLineToPoint(context, CGRectGetMaxX(rect) - radius, CGRectGetMinY(rect));
        CGContextAddArcToPoint(context, CGRectGetMaxX(rect), CGRectGetMinY(rect), CGRectGetMaxX(rect), CGRectGetMinY(rect) + radius, radius);
        CGContextAddLineToPoint(context, CGRectGetMaxX(rect), CGRectGetMaxY(rect) - radius);
        CGContextAddArcToPoint(context, CGRectGetMaxX(rect), CGRectGetMaxY(rect), CGRectGetMaxX(rect) - radius, CGRectGetMaxY(rect), radius);
        CGContextAddLineToPoint(context, CGRectGetMinX(rect) + radius, CGRectGetMaxY(rect));
        CGContextAddArcToPoint(context, CGRectGetMinX(rect), CGRectGetMaxY(rect), CGRectGetMinX(rect), CGRectGetMaxY(rect) - radius, radius);
        CGContextAddLineToPoint(context, CGRectGetMinX(rect), CGRectGetMinY(rect) + radius);
        CGContextAddArcToPoint(context, CGRectGetMinX(rect), CGRectGetMinY(rect), CGRectGetMinX(rect) + radius, CGRectGetMinY(rect), radius);
        CGContextClosePath(context);
        CGContextFillPath(context);
    }

    // Draw title
    if (_currentTitle) {
        UIColor *titleColor = _titleLabelColor ?: [UIColor whiteColor];
        CGContextSetFillColorWithColor(context, [titleColor CGColor]);

        CGSize titleSize = [_currentTitle sizeWithAttributes:nil];
        CGPoint titlePoint = CGPointMake((rect.size.width - titleSize.width) / 2.0f,
                                         (rect.size.height - titleSize.height) / 2.0f + titleSize.height * 0.75f);
        CGContextShowTextAtPoint(context, titlePoint.x, titlePoint.y, [_currentTitle UTF8String], strlen([_currentTitle UTF8String]));
    }
}

#pragma mark - Touch Handling

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    _highlighted = YES;
    if (_adjustsImageWhenHighlighted) {
        self.alpha = 0.7f;
    }
    [self setNeedsDisplay];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    _highlighted = NO;
    self.alpha = 1.0f;
    [self setNeedsDisplay];
    [self sendActionsForControlEvents:UIControlEventTouchUpInside];
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
    _highlighted = NO;
    self.alpha = 1.0f;
    [self setNeedsDisplay];
    [self sendActionsForControlEvents:UIControlEventTouchUpOutside];
}

@end
