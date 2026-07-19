#import "UIControl.h"
#import "UIColor.h"

typedef NS_ENUM(NSInteger, UIButtonType) {
    UIButtonTypeCustom = 0,
    UIButtonTypeRoundedRect,
    UIButtonTypeDetailDisclosure,
    UIButtonTypeInfoLight,
    UIButtonTypeInfoDark,
    UIButtonTypeContactAdd,
};

@interface UIButton : UIControl {
    UILabel *_titleLabel;
    UIColor *_titleLabelColor;
    UIColor *_titleLabelHighlightColor;
    UIImage *_currentImage;
    UIImage *_currentBackgroundImage;
    NSString *_currentTitle;
    BOOL _adjustsImageWhenHighlighted;
    BOOL _adjustsImageWhenDisabled;
    UIEdgeInsets _imageEdgeInsets;
    UIEdgeInsets _titleEdgeInsets;
    UIEdgeInsets _contentEdgeInsets;
}

@property (nonatomic, readonly) UILabel *titleLabel;
@property (nonatomic) BOOL adjustsImageWhenHighlighted;
@property (nonatomic) BOOL adjustsImageWhenDisabled;
@property (nonatomic) UIEdgeInsets imageEdgeInsets;
@property (nonatomic) UIEdgeInsets titleEdgeInsets;
@property (nonatomic) UIEdgeInsets contentEdgeInsets;

+ (UIButton *)buttonWithType:(UIButtonType)buttonType;

- (void)setTitle:(NSString *)title forState:(NSUInteger)state;
- (void)setTitleColor:(UIColor *)color forState:(NSUInteger)state;
- (void)setTitleShadowColor:(UIColor *)color forState:(NSUInteger)state;
- (void)setImage:(UIImage *)image forState:(NSUInteger)state;
- (void)setBackgroundImage:(UIImage *)image forState:(NSUInteger)state;

- (NSString *)titleForState:(NSUInteger)state;
- (UIColor *)titleColorForState:(NSUInteger)state;
- (UIImage *)imageForState:(NSUInteger)state;
- (UIImage *)backgroundImageForState:(NSUInteger)state;

- (CGSize)intrinsicContentSize;

- (void)sizeToFit;

@end
