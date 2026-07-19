#import "UIVisualEffect.h"

typedef NS_ENUM(NSInteger, UIBlurEffectStyle) {
    UIBlurEffectStyleExtraLight,
    UIBlurEffectStyleLight,
    UIBlurEffectStyleDark,
    UIBlurEffectStyleRegular,
    UIBlurEffectStyleProminent,
    UIBlurEffectStyleSystemUltraThinMaterial,
    UIBlurEffectStyleSystemThinMaterial,
    UIBlurEffectStyleSystemMaterial,
    UIBlurEffectStyleSystemThickMaterial,
    UIBlurEffectStyleSystemChromeMaterial,
};

@interface UIBlurEffect : UIVisualEffect {
    UIBlurEffectStyle _style;
    CGFloat _blurRadius;
}

@property (nonatomic, readonly) UIBlurEffectStyle style;

+ (UIBlurEffect *)effectWithStyle:(UIBlurEffectStyle)style;

@end
