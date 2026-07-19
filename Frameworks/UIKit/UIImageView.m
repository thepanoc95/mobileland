#import "UIImageView.h"
#import "UIImage.h"
#import "UIColor.h"
#import <CoreGraphics/CGContext.h>

@implementation UIImageView
    - (instancetype)initWithImage:(UIImage *)image {
        self = [super initWithFrame:CGRectZero];
        if (self) {
            _image = [image retain];
            _contentMode = UIViewContentModeCenter;
            _highlighted = NO;
            if (image) {
                CGSize size = [image size];
                CGRect frame = self.frame;
                frame.size = size;
                self.frame = frame;
            }
        }
        return self;
    }

    - (void)dealloc {
        [_image release];
        [super dealloc];
    }

    - (void)setImage:(UIImage *)image {
        if (_image != image) {
            [_image release];
            _image = [image retain];
            [self setNeedsDisplay];
        }
    }

    - (UIImage *)image {
        return _image;
    }

    - (void)setContentMode:(UIViewContentMode)contentMode {
        _contentMode = contentMode;
        [self setNeedsDisplay];
    }

    - (void)setHighlighted:(BOOL)highlighted {
        _highlighted = highlighted;
        [self setNeedsDisplay];
    }

    - (void)drawRect:(CGRect)rect {
        if (!_image) return;

        CGRect drawRect;
        CGSize imageSize = [_image size];
        CGSize boundsSize = _bounds.size;

        switch (_contentMode) {
            case UIViewContentModeScaleToFill: {
                drawRect = _bounds;
                break;
            }
            case UIViewContentModeScaleAspectFit: {
                CGFloat imageAspect = imageSize.width / imageSize.height;
                CGFloat viewAspect = boundsSize.width / boundsSize.height;
                if (imageAspect > viewAspect) {
                    CGFloat newHeight = boundsSize.width / imageAspect;
                    drawRect = CGRectMake(0, (boundsSize.height - newHeight) / 2.0f,
                                        boundsSize.width, newHeight);
                } else {
                    CGFloat newWidth = boundsSize.height * imageAspect;
                    drawRect = CGRectMake((boundsSize.width - newWidth) / 2.0f, 0,
                                        newWidth, boundsSize.height);
                }
                break;
            }
            case UIViewContentModeScaleAspectFill: {
                CGFloat imageAspect = imageSize.width / imageSize.height;
                CGFloat viewAspect = boundsSize.width / boundsSize.height;
                if (imageAspect > viewAspect) {
                    CGFloat newHeight = boundsSize.width / imageAspect;
                    drawRect = CGRectMake(0, (boundsSize.height - newHeight) / 2.0f,
                                        boundsSize.width, newHeight);
                } else {
                    CGFloat newWidth = boundsSize.height * imageAspect;
                    drawRect = CGRectMake((boundsSize.width - newWidth) / 2.0f, 0,
                                        newWidth, boundsSize.height);
                }
                break;
            }
            case UIViewContentModeCenter: {
                drawRect = CGRectMake((boundsSize.width - imageSize.width) / 2.0f,
                                    (boundsSize.height - imageSize.height) / 2.0f,
                                    imageSize.width, imageSize.height);
                break;
            }
            case UIViewContentModeTop: {
                drawRect = CGRectMake((boundsSize.width - imageSize.width) / 2.0f, 0,
                                    imageSize.width, imageSize.height);
                break;
            }
            case UIViewContentModeBottom: {
                drawRect = CGRectMake((boundsSize.width - imageSize.width) / 2.0f,
                                    boundsSize.height - imageSize.height,
                                    imageSize.width, imageSize.height);
                break;
            }
            case UIViewContentModeLeft: {
                drawRect = CGRectMake(0,
                                    (boundsSize.height - imageSize.height) / 2.0f,
                                    imageSize.width, imageSize.height);
                break;
            }
            case UIViewContentModeRight: {
                drawRect = CGRectMake(boundsSize.width - imageSize.width,
                                    (boundsSize.height - imageSize.height) / 2.0f,
                                    imageSize.width, imageSize.height);
                break;
            }
            case UIViewContentModeTopLeft: {
                drawRect = CGRectMake(0, 0, imageSize.width, imageSize.height);
                break;
            }
            case UIViewContentModeTopRight: {
                drawRect = CGRectMake(boundsSize.width - imageSize.width, 0,
                                    imageSize.width, imageSize.height);
                break;
            }
            case UIViewContentModeBottomLeft: {
                drawRect = CGRectMake(0, boundsSize.height - imageSize.height,
                                    imageSize.width, imageSize.height);
                break;
            }
            case UIViewContentModeBottomRight: {
                drawRect = CGRectMake(boundsSize.width - imageSize.width,
                                    boundsSize.height - imageSize.height,
                                    imageSize.width, imageSize.height);
                break;
            }
            default:
                drawRect = _bounds;
                break;
        }

        [_image drawInRect:drawRect];
    }
@end
