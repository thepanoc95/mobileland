#import "UITextField.h"
#import "UIColor.h"
#import "UIFont.h"
#import <CoreGraphics/CGContext.h>
#import <QuartzCore/CALayer.h>

@implementation UITextField

- (instancetype)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        _text = nil;
        _placeholder = nil;
        _textColor = [[UIColor blackColor] retain];
        _font = nil;
        _textAlignment = 0;
        _secureTextEntry = NO;
        _editing = NO;
        _clearsOnBeginEditing = YES;
        _clearsOnInsertion = NO;
        _borderStyle = 0;
        _minimumFontSize = 17.0f;

        self.backgroundColor = [UIColor whiteColor];
        self.userInteractionEnabled = YES;
    }
    return self;
}

- (void)dealloc {
    [_text release];
    [_placeholder release];
    [_textColor release];
    [_tintColor release];
    [_font release];
    [_inputView release];
    [_inputAccessoryView release];
    [super dealloc];
}

#pragma mark - Text

- (void)setText:(NSString *)text {
    if (_text != text) {
        [_text release];
        _text = [text copy];
        [self setNeedsDisplay];
    }
}

- (NSString *)text {
    return _text;
}

- (void)setPlaceholder:(NSString *)placeholder {
    if (_placeholder != placeholder) {
        [_placeholder release];
        _placeholder = [placeholder copy];
        [self setNeedsDisplay];
    }
}

- (void)setTextColor:(UIColor *)textColor {
    if (_textColor != textColor) {
        [_textColor release];
        _textColor = [textColor retain];
        [self setNeedsDisplay];
    }
}

- (void)setTintColor:(UIColor *)tintColor {
    if (_tintColor != tintColor) {
        [_tintColor release];
        _tintColor = [tintColor retain];
        [self setNeedsDisplay];
    }
}

- (void)setFont:(UIFont *)font {
    if (_font != font) {
        [_font release];
        _font = [font retain];
        [self setNeedsDisplay];
    }
}

- (BOOL)hasText {
    return _text != nil && [_text length] > 0;
}

- (void)clearText {
    self.text = nil;
}

- (void)selectAllText {
}

#pragma mark - Drawing

- (void)drawRect:(CGRect)rect {
    CGContextRef context = UIGraphicsGetCurrentContext();
    if (!context) return;

    // Border style
    switch (_borderStyle) {
        case 1: // Line
            CGContextSetGrayStrokeColor(context, 0.7f, 1.0f);
            CGContextSetLineWidth(context, 1.0f);
            CGContextStrokeRect(context, rect);
            break;
        case 2: // Bezel
            CGContextSetGrayFillColor(context, 0.9f, 1.0f);
            CGContextFillRect(context, rect);
            CGContextSetGrayStrokeColor(context, 0.5f, 1.0f);
            CGContextSetLineWidth(context, 1.0f);
            CGContextStrokeRect(context, rect);
            break;
        case 3: // Rounded rect
        {
            CGFloat radius = 5.0f;
            CGContextSetGrayFillColor(context, 1.0f, 1.0f);
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

            CGContextSetGrayStrokeColor(context, 0.7f, 1.0f);
            CGContextSetLineWidth(context, 1.0f);
            CGContextStrokeRect(context, rect);
            break;
        }
        default:
            break;
    }

    // Text or placeholder
    NSString *displayText = (_text && [_text length] > 0) ? _text : _placeholder;
    if (displayText) {
        UIColor *drawColor = (_text && [_text length] > 0) ? _textColor : [UIColor grayColor];
        CGContextSetFillColorWithColor(context, [drawColor CGColor]);

        CGFloat textX = 8.0f;
        CGFloat textY = rect.size.height * 0.75f;
        const char *str = [displayText UTF8String];
        if (str) {
            CGContextShowTextAtPoint(context, textX, textY, str, strlen(str));
        }
    }

    // Cursor when editing
    if (_editing) {
        UIColor *cursorColor = _tintColor ?: [UIColor colorWithRed:0.0f green:0.478f blue:1.0f alpha:1.0f];
        CGContextSetFillColorWithColor(context, [cursorColor CGColor]);
        CGFloat cursorX = 8.0f;
        if (_text) {
            cursorX += [_text sizeWithAttributes:nil].width;
        }
        CGContextFillRect(context, CGRectMake(cursorX, 4.0f, 2.0f, rect.size.height - 8.0f));
    }
}

#pragma mark - Touch Handling

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    if (!_editing) {
        _editing = YES;
        if (_clearsOnBeginEditing && _text) {
            [self clearText];
        }
        [self setNeedsDisplay];
    }
}

- (BOOL)canBecomeFirstResponder {
    return YES;
}

@end
