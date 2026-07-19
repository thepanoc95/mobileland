#import "UIView.h"

@class UIImage;

typedef NS_ENUM(NSInteger, UIViewContentMode);

@interface UIImageView : UIView {
    UIImage *_image;
    UIViewContentMode _contentMode;
    BOOL _highlighted;
}

@property (nonatomic, strong) UIImage *image;
@property (nonatomic) UIViewContentMode contentMode;
@property (nonatomic, getter=isHighlighted) BOOL highlighted;

- (instancetype)initWithImage:(UIImage *)image;

@end
