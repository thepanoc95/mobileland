#import "UIView.h"
#import "UIColor.h"

@interface UILabel : UIView {
    NSString *_text;
    UIColor *_textColor;
    UIColor *_shadowColor;
    UIFont *_font;
    NSInteger _textAlignment;
    NSInteger _lineBreakMode;
    NSInteger _numberOfLines;
    CGSize _shadowOffset;
    BOOL _adjustsFontSizeToFitWidth;
    CGFloat _minimumScaleFactor;
    BOOL _enabled;
    BOOL _highlighted;
    UIColor *_highlightedTextColor;
}

@property (nonatomic, copy) NSString *text;
@property (nonatomic, strong) UIColor *textColor;
@property (nonatomic, strong) UIColor *shadowColor;
@property (nonatomic, strong) UIFont *font;
@property (nonatomic) NSInteger textAlignment;
@property (nonatomic) NSInteger lineBreakMode;
@property (nonatomic) NSInteger numberOfLines;
@property (nonatomic) CGSize shadowOffset;
@property (nonatomic) BOOL adjustsFontSizeToFitWidth;
@property (nonatomic) CGFloat minimumScaleFactor;
@property (nonatomic, getter=isEnabled) BOOL enabled;
@property (nonatomic, getter=isHighlighted) BOOL highlighted;
@property (nonatomic, strong) UIColor *highlightedTextColor;

- (CGSize)textSize;
- (CGSize)intrinsicContentSize;

@end
