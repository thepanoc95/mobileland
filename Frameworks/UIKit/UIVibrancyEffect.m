#import "UIVibrancyEffect.h"
#import "UIBlurEffect.h"

@implementation UIVibrancyEffect

@synthesize tintColor = _tintColor;

+ (UIVibrancyEffect *)effectForBlurEffect:(UIBlurEffect *)blurEffect {
    return [self effectForBlurEffect:blurEffect tintColor:nil];
}

+ (UIVibrancyEffect *)effectForBlurEffect:(UIBlurEffect *)blurEffect tintColor:(UIColor *)tintColor {
    UIVibrancyEffect *effect = [[[self alloc] init] autorelease];
    effect->_tintColor = [tintColor retain];
    return effect;
}

- (void)dealloc {
    [_tintColor release];
    [super dealloc];
}

- (id)copyWithZone:(NSZone *)zone {
    UIVibrancyEffect *copy = [super copyWithZone:zone];
    copy->_tintColor = [_tintColor retain];
    return copy;
}

@end
