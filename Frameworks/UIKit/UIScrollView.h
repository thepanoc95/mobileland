#import "UIView.h"

@class UIColor, UIFont;

@protocol UIScrollViewDelegate <NSObject>
@optional
- (void)scrollViewDidScroll:(UIScrollView *)scrollView;
- (void)scrollViewDidZoom:(UIScrollView *)scrollView;
- (void)scrollViewWillBeginDragging:(UIScrollView *)scrollView;
- (void)scrollViewDidEndDragging:(UIScrollView *)scrollView willDecelerate:(BOOL)decelerate;
- (void)scrollViewWillBeginDecelerating:(UIScrollView *)scrollView;
- (void)scrollViewDidEndDecelerating:(UIScrollView *)scrollView;
- (void)scrollViewDidEndScrollingAnimation:(UIScrollView *)scrollView;
@end

@interface UIScrollView : UIView {
    CGPoint _contentOffset;
    CGSize _contentSize;
    UIEdgeInsets _contentInset;
    UIEdgeInsets _scrollIndicatorInsets;
    BOOL _bounces;
    BOOL _alwaysBounceVertical;
    BOOL _alwaysBounceHorizontal;
    BOOL _scrollEnabled;
    BOOL _pagingEnabled;
    BOOL _showsVerticalScrollIndicator;
    BOOL _showsHorizontalScrollIndicator;
    CGFloat _minimumZoomScale;
    CGFloat _maximumZoomScale;
    CGFloat _zoomScale;
    BOOL _decelerating;
    id _delegate;
}

@property (nonatomic) CGPoint contentOffset;
@property (nonatomic) CGSize contentSize;
@property (nonatomic) UIEdgeInsets contentInset;
@property (nonatomic) UIEdgeInsets scrollIndicatorInsets;
@property (nonatomic) BOOL bounces;
@property (nonatomic) BOOL alwaysBounceVertical;
@property (nonatomic) BOOL alwaysBounceHorizontal;
@property (nonatomic, getter=isScrollEnabled) BOOL scrollEnabled;
@property (nonatomic, getter=isPagingEnabled) BOOL pagingEnabled;
@property (nonatomic) BOOL showsVerticalScrollIndicator;
@property (nonatomic) BOOL showsHorizontalScrollIndicator;
@property (nonatomic) CGFloat minimumZoomScale;
@property (nonatomic) CGFloat maximumZoomScale;
@property (nonatomic) CGFloat zoomScale;
@property (nonatomic, readonly, getter=isDecelerating) BOOL decelerating;
@property (nonatomic, assign) id<UIScrollViewDelegate> delegate;

- (void)setContentOffset:(CGPoint)contentOffset animated:(BOOL)animated;
- (void)scrollRectToVisible:(CGRect)rect animated:(BOOL)animated;
- (void)zoomToRect:(CGRect)rect animated:(BOOL)animated;

@end
