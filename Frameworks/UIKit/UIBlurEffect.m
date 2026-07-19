#import "UIBlurEffect.h"

@implementation UIBlurEffect

@synthesize style = _style;

+ (UIBlurEffect *)effectWithStyle:(UIBlurEffectStyle)style {
    UIBlurEffect *effect = [[[self alloc] init] autorelease];
    effect->_style = style;
    return effect;
}

- (instancetype)init {
    self = [super init];
    if (self) {
        _style = UIBlurEffectStyleRegular;
        _blurRadius = 20.0f;
    }
    return self;
}

- (id)copyWithZone:(NSZone *)zone {
    UIBlurEffect *copy = [[UIBlurEffect allocWithZone:zone] init];
    copy->_style = _style;
    copy->_blurRadius = _blurRadius;
    return copy;
}

- (CGFloat)_blurRadius {
    return _blurRadius;
}

@end
