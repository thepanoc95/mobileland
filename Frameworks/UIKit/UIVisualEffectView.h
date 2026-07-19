#import "UIView.h"

@class UIVisualEffect, UIVisualEffectView;

@interface UIVisualEffectView : UIView {
    UIVisualEffect *_effect;
    UIView *_contentView;
    BOOL _shouldRasterizeForEffects;
}

@property (nonatomic, copy) UIVisualEffect *effect;
@property (nonatomic, readonly) UIView *contentView;

- (instancetype)initWithEffect:(UIVisualEffect *)effect;

@end
