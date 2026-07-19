#import "UIFont.h"

NSString *const UIFontTextStyleHeadline = @"UIFontTextStyleHeadline";
NSString *const UIFontTextStyleBody = @"UIFontTextStyleBody";
NSString *const UIFontTextStyleCallout = @"UIFontTextStyleCallout";
NSString *const UIFontTextStyleSubheadline = @"UIFontTextStyleSubheadline";
NSString *const UIFontTextStyleFootnote = @"UIFontTextStyleFootnote";
NSString *const UIFontTextStyleCaption1 = @"UIFontTextStyleCaption1";
NSString *const UIFontTextStyleCaption2 = @"UIFontTextStyleCaption2";

@implementation UIFont

+ (UIFont *)systemFontOfSize:(CGFloat)fontSize {
    return [[[self alloc] initWithName:@"Helvetica" size:fontSize] autorelease];
}

+ (UIFont *)boldSystemFontOfSize:(CGFloat)fontSize {
    return [[[self alloc] initWithName:@"Helvetica-Bold" size:fontSize] autorelease];
}

+ (UIFont *)italicSystemFontOfSize:(CGFloat)fontSize {
    return [[[self alloc] initWithName:@"Helvetica-Oblique" size:fontSize] autorelease];
}

+ (UIFont *)fontWithName:(NSString *)fontName size:(CGFloat)fontSize {
    return [[[self alloc] initWithName:fontName size:fontSize] autorelease];
}

+ (UIFont *)preferredFontForTextStyle:(NSString *)style {
    CGFloat size = 17.0f;
    if ([style isEqualToString:UIFontTextStyleHeadline]) {
        size = 17.0f;
    } else if ([style isEqualToString:UIFontTextStyleBody]) {
        size = 17.0f;
    } else if ([style isEqualToString:UIFontTextStyleCallout]) {
        size = 16.0f;
    } else if ([style isEqualToString:UIFontTextStyleSubheadline]) {
        size = 15.0f;
    } else if ([style isEqualToString:UIFontTextStyleFootnote]) {
        size = 13.0f;
    } else if ([style isEqualToString:UIFontTextStyleCaption1]) {
        size = 12.0f;
    } else if ([style isEqualToString:UIFontTextStyleCaption2]) {
        size = 11.0f;
    }
    return [self systemFontOfSize:size];
}

- (instancetype)initWithName:(NSString *)fontName size:(CGFloat)fontSize {
    self = [super init];
    if (self) {
        _fontName = [fontName copy];
        _pointSize = fontSize;
    }
    return self;
}

- (void)dealloc {
    [_fontName release];
    [super dealloc];
}

- (NSString *)fontName {
    return _fontName;
}

- (CGFloat)pointSize {
    return _pointSize;
}

- (CGFloat)ascender {
    return _pointSize * 0.8f;
}

- (CGFloat)descender {
    return -_pointSize * 0.2f;
}

- (CGFloat)capHeight {
    return _pointSize * 0.7f;
}

- (CGFloat)xHeight {
    return _pointSize * 0.5f;
}

- (CGFloat)lineHeight {
    return _pointSize * 1.2f;
}

- (UIFont *)fontWithSize:(CGFloat)fontSize {
    return [UIFont fontWithName:_fontName size:fontSize];
}

@end
