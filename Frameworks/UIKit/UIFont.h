#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>

@interface UIFont : NSObject {
    NSString *_fontName;
    CGFloat _pointSize;
}

+ (UIFont *)systemFontOfSize:(CGFloat)fontSize;
+ (UIFont *)boldSystemFontOfSize:(CGFloat)fontSize;
+ (UIFont *)italicSystemFontOfSize:(CGFloat)fontSize;
+ (UIFont *)fontWithName:(NSString *)fontName size:(CGFloat)fontSize;

+ (UIFont *)preferredFontForTextStyle:(NSString *)style;

- (instancetype)initWithName:(NSString *)fontName size:(CGFloat)fontSize;

- (NSString *)fontName;
- (CGFloat)pointSize;
- (CGFloat)ascender;
- (CGFloat)descender;
- (CGFloat)capHeight;
- (CGFloat)xHeight;
- (CGFloat)lineHeight;

- (UIFont *)fontWithSize:(CGFloat)fontSize;

@end

extern NSString *const UIFontTextStyleHeadline;
extern NSString *const UIFontTextStyleBody;
extern NSString *const UIFontTextStyleCallout;
extern NSString *const UIFontTextStyleSubheadline;
extern NSString *const UIFontTextStyleFootnote;
extern NSString *const UIFontTextStyleCaption1;
extern NSString *const UIFontTextStyleCaption2;
