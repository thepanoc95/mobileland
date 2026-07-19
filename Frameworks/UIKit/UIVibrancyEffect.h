#import "UIVisualEffect.h"
#import "UIColor.h"

@interface UIVibrancyEffect : UIVisualEffect {
    UIColor *_tintColor;
}

@property (nonatomic, readonly) UIColor *tintColor;

+ (UIVibrancyEffect *)effectForBlurEffect:(UIBlurEffect *)blurEffect;
+ (UIVibrancyEffect *)effectForBlurEffect:(UIBlurEffect *)blurEffect tintColor:(UIColor *)tintColor;

@end
