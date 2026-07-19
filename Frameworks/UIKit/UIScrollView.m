#import "UIScrollView.h"
#import <QuartzCore/CALayer.h>

@implementation UIScrollView

@synthesize contentOffset = _contentOffset;
@synthesize contentSize = _contentSize;
@synthesize contentInset = _contentInset;
@synthesize scrollIndicatorInsets = _scrollIndicatorInsets;
@synthesize bounces = _bounces;
@synthesize alwaysBounceVertical = _alwaysBounceVertical;
@synthesize alwaysBounceHorizontal = _alwaysBounceHorizontal;
@synthesize pagingEnabled = _pagingEnabled;
@synthesize showsVerticalScrollIndicator = _showsVerticalScrollIndicator;
@synthesize showsHorizontalScrollIndicator = _showsHorizontalScrollIndicator;
@synthesize minimumZoomScale = _minimumZoomScale;
@synthesize maximumZoomScale = _maximumZoomScale;
@synthesize zoomScale = _zoomScale;
@synthesize decelerating = _decelerating;
@synthesize delegate = _delegate;

- (instancetype)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        _bounces = YES;
        _scrollEnabled = YES;
        _showsVerticalScrollIndicator = YES;
        _showsHorizontalScrollIndicator = NO;
        _minimumZoomScale = 1.0f;
        _maximumZoomScale = 1.0f;
        _zoomScale = 1.0f;
        _decelerating = NO;
    }
    return self;
}

- (void)setContentOffset:(CGPoint)contentOffset animated:(BOOL)animated {
    _contentOffset = contentOffset;
    if (animated) {
        [UIView animateWithDuration:0.25f animations:^{
            self->_layer.position = CGPointMake(contentOffset.x, contentOffset.y);
        }];
    } else {
        [_layer setPosition:contentOffset];
    }
    if ([_delegate respondsToSelector:@selector(scrollViewDidScroll:)]) {
        [_delegate scrollViewDidScroll:self];
    }
}

- (void)scrollRectToVisible:(CGRect)rect animated:(BOOL)animated {
    [self setContentOffset:rect.origin animated:animated];
}

- (void)zoomToRect:(CGRect)rect animated:(BOOL)animated {
}

- (void)setScrollEnabled:(BOOL)scrollEnabled {
    _scrollEnabled = scrollEnabled;
}

- (BOOL)isScrollEnabled {
    return _scrollEnabled;
}

- (void)layoutSubviews {
    [super layoutSubviews];

    for (UIView *subview in self.subviews) {
        CGRect frame = subview.frame;
        frame.origin.x -= _contentOffset.x;
        frame.origin.y -= _contentOffset.y;
        subview.frame = frame;
    }
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    if (_scrollEnabled) {
        [super touchesBegan:touches withEvent:event];
    }
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
    if (_scrollEnabled) {
        UITouch *touch = [touches anyObject];
        CGPoint current = [touch locationInView:self];
        CGPoint previous = [touch previousLocationInView:self];
        CGPoint delta = CGPointMake(current.x - previous.x, current.y - previous.y);

        _contentOffset.x -= delta.x;
        _contentOffset.y -= delta.y;

        if ([_delegate respondsToSelector:@selector(scrollViewDidScroll:)]) {
            [_delegate scrollViewDidScroll:self];
        }
        [self setNeedsLayout];
    }
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    if (_scrollEnabled && [_delegate respondsToSelector:@selector(scrollViewDidEndDragging:willDecelerate:)]) {
        [_delegate scrollViewDidEndDragging:self willDecelerate:NO];
    }
    [super touchesEnded:touches withEvent:event];
}

@end
