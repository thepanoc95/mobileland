#import <CoreGraphics/CoreGraphics.h>
#import <Foundation/Foundation.h>
#import <QuartzCore/CALayer.h>

@class UIColor, UIViewController, UIWindow, UIScreen, UIMotionEvent;

typedef NS_ENUM(NSInteger, UIViewContentMode) {
    UIViewContentModeScaleToFill,
    UIViewContentModeScaleAspectFit,
    UIViewContentModeScaleAspectFill,
    UIViewContentModeRedraw,
    UIViewContentModeCenter,
    UIViewContentModeTop,
    UIViewContentModeBottom,
    UIViewContentModeLeft,
    UIViewContentModeRight,
    UIViewContentModeTopLeft,
    UIViewContentModeTopRight,
    UIViewContentModeBottomLeft,
    UIViewContentModeBottomRight,
};

typedef NS_ENUM(NSInteger, UIViewAutoresizing) {
    UIViewAutoresizingNone                 = 0,
    UIViewAutoresizingFlexibleLeftMargin   = 1 << 0,
    UIViewAutoresizingFlexibleWidth        = 1 << 1,
    UIViewAutoresizingFlexibleRightMargin  = 1 << 2,
    UIViewAutoresizingFlexibleTopMargin    = 1 << 3,
    UIViewAutoresizingFlexibleHeight       = 1 << 4,
    UIViewAutoresizingFlexibleBottomMargin = 1 << 5,
};

typedef NS_ENUM(NSInteger, UILineBreakMode) {
    UILineBreakModeWordWrap = 0,
    UILineBreakModeCharacterWrap,
    UILineBreakModeClip,
    UILineBreakModeHeadTruncation,
    UILineBreakModeTailTruncation,
    UILineBreakModeMiddleTruncation,
};

typedef struct UIEdgeInsets {
    CGFloat top, left, bottom, right;
} UIEdgeInsets;

static inline UIEdgeInsets UIEdgeInsetsMake(CGFloat top, CGFloat left, CGFloat bottom, CGFloat right) {
    UIEdgeInsets insets = {top, left, bottom, right};
    return insets;
}

extern const UIEdgeInsets UIEdgeInsetsZero;

@interface UIView : UIResponder {
    CGRect _frame;
    CGRect _bounds;
    CALayer *_layer;
    NSMutableArray *_subviews;
    UIView *_superview;
    BOOL _hidden;
    CGFloat _alpha;
    UIColor *_backgroundColor;
    UIViewAutoresizing _autoresizingMask;
    BOOL _clipsToBounds;
    BOOL _userInteractionEnabled;
    BOOL _needsLayout;
    BOOL _needsDisplay;
    UIViewController *_viewController;
}

+ (UIView *)appearance;
+ (void)animateWithDuration:(NSTimeInterval)duration animations:(void (^)(void))animations;
+ (void)animateWithDuration:(NSTimeInterval)duration animations:(void (^)(void))animations completion:(void (^)(BOOL finished))completion;
+ (void)animateWithDuration:(NSTimeInterval)duration delay:(NSTimeInterval)delay options:(UIViewAnimationOptions)options animations:(void (^)(void))animations completion:(void (^)(BOOL finished))completion;

- (instancetype)initWithFrame:(CGRect)frame;

@property (nonatomic) CGRect frame;
@property (nonatomic) CGRect bounds;
@property (nonatomic) CGPoint center;
@property (nonatomic, readonly) CALayer *layer;
@property (nonatomic, readonly) NSArray *subviews;
@property (nonatomic, weak) UIView *superview;
@property (nonatomic) CGFloat alpha;
@property (nonatomic, getter=isHidden) BOOL hidden;
@property (nonatomic, strong) UIColor *backgroundColor;
@property (nonatomic) UIViewAutoresizing autoresizingMask;
@property (nonatomic) BOOL clipsToBounds;
@property (nonatomic) BOOL userInteractionEnabled;
@property (nonatomic, getter=isMultipleTouchEnabled) BOOL multipleTouchEnabled;
@property (nonatomic, readonly) UIWindow *window;
@property (nonatomic, readonly) UIScreen *screen;
@property (nonatomic, weak) UIViewController *nextResponder;

- (void)addSubView:(UIView *)view;
- (void)removeFromSuperview;
- (void)bringSubviewToFront:(UIView *)view;
- (void)sendSubviewToBack:(UIView *)view;
- (void)insertSubview:(UIView *)view atIndex:(NSInteger)index;
- (void)insertSubview:(UIView *)view belowSubview:(UIView *)siblingSubview;
- (void)insertSubview:(UIView *)view aboveSubview:(UIView *)siblingSubview;
- (void)exchangeSubviewAtIndex:(NSInteger)index1 withSubviewAtIndex:(NSInteger)index2;
- (UIView *)viewWithTag:(NSInteger)tag;

- (void)setNeedsDisplay;
- (void)layoutSubviews;
- (void)setNeedsLayout;
- (void)sizeToFit;

- (void)drawRect:(CGRect)rect;
- (void)layoutIfNeeded;

- (BOOL)pointInside:(CGPoint)point withEvent:(UIEvent *)event;
- (UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event;

- (CGPoint)convertPoint:(CGPoint)point toView:(UIView *)view;
- (CGPoint)convertPoint:(CGPoint)point fromView:(UIView *)view;
- (CGRect)convertRect:(CGRect)rect toView:(UIView *)view;
- (CGRect)convertRect:(CGRect)rect fromView:(UIView *)view;

- (void)removeAllSubviews;

@end
