#import "UIView.h"
#import "UIColor.h"
#import <QuartzCore/CALayer.h>
#import <QuartzCore/CATransaction.h>
#import <CoreGraphics/CGContext.h>

const UIEdgeInsets UIEdgeInsetsZero = {0, 0, 0, 0};

@implementation UIView
    + (UIView *)appearance {
        return nil;
    }

    + (void)animateWithDuration:(NSTimeInterval)duration animations:(void (^)(void))animations {
        [self animateWithDuration:duration animations:animations completion:nil];
    }

    + (void)animateWithDuration:(NSTimeInterval)duration animations:(void (^)(void))animations completion:(void (^)(BOOL finished))completion {
        [self animateWithDuration:duration delay:0 options:0 animations:animations completion:completion];
    }

    + (void)animateWithDuration:(NSTimeInterval)duration delay:(NSTimeInterval)delay options:(UIViewAnimationOptions)options animations:(void (^)(void))animations completion:(void (^)(BOOL finished))completion {
        [CATransaction begin];
        [CATransaction setAnimationDuration:duration];

        if (animations) {
            animations();
        }

        [CATransaction commit];

        if (completion) {
            completion(YES);
        }
    }

    - (instancetype)initWithFrame:(CGRect)frame {
        self = [super init];
        if (self) {
            _frame = frame;
            _bounds = CGRectMake(0, 0, frame.size.width, frame.size.height);
            _subviews = [[NSMutableArray alloc] init];
            _alpha = 1.0;
            _hidden = NO;
            _userInteractionEnabled = YES;
            _clipsToBounds = NO;
            _autoresizingMask = UIViewAutoresizingNone;

            _layer = [[CALayer alloc] init];
            [_layer setFrame:frame];
            [_layer setBounds:_bounds];
            [_layer setAnchorPoint:CGPointMake(0, 0)];
            [_layer setPosition:CGPointMake(frame.origin.x, frame.origin.y)];
        }
        return self;
    }

    - (void)dealloc {
        [_layer release];
        [_subviews release];
        [_backgroundColor release];
        [super dealloc];
    }

    - (void)setFrame:(CGRect)frame {
        _frame = frame;
        _bounds = CGRectMake(0, 0, frame.size.width, frame.size.height);
        [_layer setFrame:frame];
        [_layer setBounds:_bounds];
    }

    - (CGRect)frame {
        return _frame;
    }

    - (void)setBounds:(CGRect)bounds {
        _bounds = bounds;
        _frame.size = bounds.size;
        [_layer setBounds:bounds];
    }

    - (CGRect)bounds {
        return _bounds;
    }

    - (void)setCenter:(CGPoint)center {
        _frame.origin.x = center.x - _frame.size.width / 2.0f;
        _frame.origin.y = center.y - _frame.size.height / 2.0f;
        [_layer setPosition:CGPointMake(center.x, center.y)];
    }

    - (CGPoint)center {
        return CGPointMake(_frame.origin.x + _frame.size.width / 2.0f,
                        _frame.origin.y + _frame.size.height / 2.0f);
    }

    - (CALayer *)layer {
        return _layer;
    }

    - (NSArray *)subviews {
        return [[_subviews copy] autorelease];
    }

    - (UIView *)superview {
        return _superview;
    }

    - (void)setAlpha:(CGFloat)alpha {
        _alpha = alpha;
        [_layer setOpacity:(float)alpha];
    }

    - (CGFloat)alpha {
        return _alpha;
    }

    - (void)setHidden:(BOOL)hidden {
        _hidden = hidden;
        [_layer setHidden:hidden];
    }

    - (BOOL)isHidden {
        return _hidden;
    }

    - (void)setBackgroundColor:(UIColor *)backgroundColor {
        if (_backgroundColor != backgroundColor) {
            [_backgroundColor release];
            _backgroundColor = [backgroundColor retain];
            [_layer setNeedsDisplay];
        }
    }

    - (UIColor *)backgroundColor {
        return _backgroundColor;
    }

    - (void)setAutoresizingMask:(UIViewAutoresizing)autoresizingMask {
        _autoresizingMask = autoresizingMask;
    }

    - (UIViewAutoresizing)autoresizingMask {
        return _autoresizingMask;
    }

    - (void)setClipsToBounds:(BOOL)clipsToBounds {
        _clipsToBounds = clipsToBounds;
        [_layer setMasksToBounds:clipsToBounds];
    }

    - (BOOL)clipsToBounds {
        return _clipsToBounds;
    }

    - (void)setUserInteractionEnabled:(BOOL)userInteractionEnabled {
        _userInteractionEnabled = userInteractionEnabled;
    }

    - (BOOL)isUserInteractionEnabled {
        return _userInteractionEnabled;
    }

    - (UIWindow *)window {
        if ([_superview isKindOfClass:[UIWindow class]]) {
            return (UIWindow *)_superview;
        }
        return [_superview window];
    }

    - (UIScreen *)screen {
        return [[self window] screen];
    }

    #pragma mark Subview Management

    - (void)addSubView:(UIView *)view {
        if (!view) return;
        [view removeFromSuperview];
        view->_superview = self;
        [_subviews addObject:view];
        [_layer addSublayer:view.layer];
        [self setNeedsLayout];
    }

    - (void)removeFromSuperview {
        if (_superview) {
            [_superview->_subviews removeObjectIdenticalTo:self];
            [_layer removeFromSuperlayer];
            _superview = nil;
        }
    }

    - (void)bringSubviewToFront:(UIView *)view {
        [_subviews removeObjectIdenticalTo:view];
        [_subviews addObject:view];
        [_layer insertSublayer:view.layer atIndex:(unsigned)[_subviews count] - 1];
    }

    - (void)sendSubviewToBack:(UIView *)view {
        [_subviews removeObjectIdenticalTo:view];
        [_subviews insertObject:view atIndex:0];
        [_layer insertSublayer:view.layer atIndex:0];
    }

    - (void)insertSubview:(UIView *)view atIndex:(NSInteger)index {
        if (!view) return;
        [view removeFromSuperview];
        view->_superview = self;
        [_subviews insertObject:view atIndex:index];
        [_layer insertSublayer:view.layer atIndex:(unsigned)index];
    }

    - (void)insertSubview:(UIView *)view belowSubview:(UIView *)siblingSubview {
        NSInteger index = [_subviews indexOfObjectIdenticalTo:siblingSubview];
        if (index != NSNotFound) {
            [self insertSubview:view atIndex:index];
        } else {
            [self addSubView:view];
        }
    }

    - (void)insertSubview:(UIView *)view aboveSubview:(UIView *)siblingSubview {
        NSInteger index = [_subviews indexOfObjectIdenticalTo:siblingSubview];
        if (index != NSNotFound) {
            [self insertSubview:view atIndex:index + 1];
        } else {
            [self addSubView:view];
        }
    }

    - (void)exchangeSubviewAtIndex:(NSInteger)index1 withSubviewAtIndex:(NSInteger)index2 {
        if (index1 >= 0 && index2 >= 0 && index1 < (NSInteger)[_subviews count] && index2 < (NSInteger)[_subviews count]) {
            [_subviews exchangeObjectAtIndex:index1 withObjectAtIndex:index2];
        }
    }

    - (UIView *)viewWithTag:(NSInteger)tag {
        if (self.tag == tag) return self;
        for (UIView *subview in _subviews) {
            UIView *found = [subview viewWithTag:tag];
            if (found) return found;
        }
        return nil;
    }

    - (void)removeAllSubviews {
        NSArray *subviewsCopy = [[_subviews copy] autorelease];
        for (UIView *subview in subviewsCopy) {
            [subview removeFromSuperview];
        }
    }

    #pragma mark - Layout

    - (void)setNeedsLayout {
        _needsLayout = YES;
    }

    - (void)layoutSubviews {
        for (UIView *subview in _subviews) {
            [subview layoutSubviews];
        }
    }

    - (void)layoutIfNeeded {
        if (_needsLayout) {
            _needsLayout = NO;
            [self layoutSubviews];
        }
        for (UIView *subview in _subviews) {
            [subview layoutIfNeeded];
        }
    }

    - (void)sizeToFit {
        // Default: no-op for base UIView
    }

    #pragma mark - Drawing

    - (void)setNeedsDisplay {
        _needsDisplay = YES;
        [_layer setNeedsDisplay];
    }

    - (void)drawRect:(CGRect)rect {
        if (_backgroundColor) {
            CGContextRef context = UIGraphicsGetCurrentContext();
            if (context) {
                CGContextSetFillColorWithColor(context, [_backgroundColor CGColor]);
                CGContextFillRect(context, rect);
            }
        }
    }

    #pragma mark - Hit Testing

    - (BOOL)pointInside:(CGPoint)point withEvent:(UIEvent *)event {
        if (_hidden || !_userInteractionEnabled || _alpha < 0.01f) return NO;
        return CGRectContainsPoint(_bounds, point);
    }

    - (UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event {
        if (_hidden || !_userInteractionEnabled || _alpha < 0.01f) return nil;

        for (UIView *subview in [_subviews reverseObjectEnumerator]) {
            CGPoint subPoint = [subview convertPoint:point fromView:self];
            UIView *hitView = [subview hitTest:subPoint withEvent:event];
            if (hitView) return hitView;
        }

        if ([self pointInside:point withEvent:event]) {
            return self;
        }
        return nil;
    }

    #pragma mark - Coordinate Conversion

    - (CGPoint)convertPoint:(CGPoint)point toView:(UIView *)view {
        if (view == nil) {
            view = [[self window] screen].coordinateSpace;
        }
        CGPoint result = point;
        result.x += _frame.origin.x;
        result.y += _frame.origin.y;
        return result;
    }

    - (CGPoint)convertPoint:(CGPoint)point fromView:(UIView *)view {
        CGPoint result = point;
        result.x -= _frame.origin.x;
        result.y -= _frame.origin.y;
        return result;
    }

    - (CGRect)convertRect:(CGRect)rect toView:(UIView *)view {
        CGPoint origin = [self convertPoint:rect.origin toView:view];
        return CGRectMake(origin.x, origin.y, rect.size.width, rect.size.height);
    }

    - (CGRect)convertRect:(CGRect)rect fromView:(UIView *)view {
        CGPoint origin = [self convertPoint:rect.origin fromView:view];
        return CGRectMake(origin.x, origin.y, rect.size.width, rect.size.height);
    }

    #pragma mark - UIResponder

    - (BOOL)canBecomeFirstResponder {
        return _userInteractionEnabled;
    }

    - (UIResponder *)nextResponder {
        return _superview;
    }
@end
