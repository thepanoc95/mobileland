#import "UIView.h"
#import "UIColor.h"

typedef NS_ENUM(NSInteger, UIMaterialStyle) {
    UIMaterialStyleThin,
    UIMaterialStyleRegular,
    UIMaterialStyleThick,
    UIMaterialStyleProminent,
};

@interface UIMaterialView : UIView {
    UIMaterialStyle _materialStyle;
    UIColor *_tintColor;
    UIColor *_specularColor;
    CGFloat _blurRadius;
    CGFloat _cornerRadius;
    BOOL _automaticallyUpdatesTint;
    UIView *_backgroundCaptureView;
}

@property (nonatomic) UIMaterialStyle materialStyle;
@property (nonatomic, strong) UIColor *tintColor;
@property (nonatomic, strong) UIColor *specularColor;
@property (nonatomic) CGFloat blurRadius;
@property (nonatomic) CGFloat cornerRadius;
@property (nonatomic) BOOL automaticallyUpdatesTint;

+ (UIMaterialView *)materialViewWithStyle:(UIMaterialStyle)style;
+ (UIMaterialView *)materialViewWithStyle:(UIMaterialStyle)style tintColor:(UIColor *)tintColor;

- (instancetype)initWithStyle:(UIMaterialStyle)style;
- (void)applyMaterialStyle:(UIMaterialStyle)style;

@end
