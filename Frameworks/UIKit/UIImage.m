#import "UIImage.h"
#import <CoreGraphics/CGImage.h>
#import <CoreGraphics/CGDataProvider.h>
#import <CoreGraphics/CGContext.h>

@implementation UIImage

+ (UIImage *)imageNamed:(NSString *)name {
    NSString *path = [[NSBundle mainBundle] pathForResource:name ofType:@"png"];
    if (!path) {
        path = [[NSBundle mainBundle] pathForResource:name ofType:@"jpg"];
    }
    if (path) {
        return [[[self alloc] initWithContentsOfFile:path] autorelease];
    }
    return nil;
}

+ (UIImage *)imageWithData:(NSData *)data {
    return [[[self alloc] initWithData:data] autorelease];
}

+ (UIImage *)imageWithData:(NSData *)data scale:(CGFloat)scale {
    return [[[self alloc] initWithData:data scale:scale] autorelease];
}

+ (UIImage *)imageWithContentsOfFile:(NSString *)path {
    return [[[self alloc] initWithContentsOfFile:path] autorelease];
}

+ (UIImage *)imageWithCGImage:(CGImageRef)CGImage {
    return [[[self alloc] initWithCGImage:CGImage] autorelease];
}

+ (UIImage *)imageWithCGImage:(CGImageRef)CGImage scale:(CGFloat)scale orientation:(UIImageOrientation)orientation {
    return [[[self alloc] initWithCGImage:CGImage scale:scale orientation:orientation] autorelease];
}

- (instancetype)initWithData:(NSData *)data {
    return [self initWithData:data scale:1.0f];
}

- (instancetype)initWithData:(NSData *)data scale:(CGFloat)scale {
    self = [super init];
    if (self) {
        _scale = scale;
        _imageOrientation = UIImageOrientationUp;

        CGDataProviderRef provider = CGDataProviderCreateWithCFData((CFDataRef)data);
        if (provider) {
            _CGImage = CGImageCreateWithPNGDataProvider(provider, NULL, true, kCGRenderingIntentDefault);
            if (!_CGImage) {
                _CGImage = CGImageCreateWithJPEGDataProvider(provider, NULL, true, kCGRenderingIntentDefault);
            }
            CGDataProviderRelease(provider);
        }
    }
    return self;
}

- (instancetype)initWithContentsOfFile:(NSString *)path {
    self = [super init];
    if (self) {
        _scale = 1.0f;
        _imageOrientation = UIImageOrientationUp;
        NSData *data = [NSData dataWithContentsOfFile:path];
        if (data) {
            CGDataProviderRef provider = CGDataProviderCreateWithCFData((CFDataRef)data);
            if (provider) {
                _CGImage = CGImageCreateWithPNGDataProvider(provider, NULL, true, kCGRenderingIntentDefault);
                if (!_CGImage) {
                    _CGImage = CGImageCreateWithJPEGDataProvider(provider, NULL, true, kCGRenderingIntentDefault);
                }
                CGDataProviderRelease(provider);
            }
        }
    }
    return self;
}

- (instancetype)initWithCGImage:(CGImageRef)CGImage {
    return [self initWithCGImage:CGImage scale:1.0f orientation:UIImageOrientationUp];
}

- (instancetype)initWithCGImage:(CGImageRef)CGImage scale:(CGFloat)scale orientation:(UIImageOrientation)orientation {
    self = [super init];
    if (self) {
        _CGImage = CGImageRetain(CGImage);
        _scale = scale;
        _imageOrientation = orientation;
    }
    return self;
}

- (void)dealloc {
    if (_CGImage) {
        CGImageRelease(_CGImage);
    }
    [super dealloc];
}

- (id)copyWithZone:(NSZone *)zone {
    UIImage *copy = [[UIImage allocWithZone:zone] initWithCGImage:_CGImage scale:_scale orientation:_imageOrientation];
    return copy;
}

- (CGSize)size {
    if (_CGImage) {
        return CGSizeMake(CGImageGetWidth(_CGImage) / _scale,
                          CGImageGetHeight(_CGImage) / _scale);
    }
    return CGSizeZero;
}

- (CGFloat)scale {
    return _scale;
}

- (UIImageOrientation)imageOrientation {
    return _imageOrientation;
}

- (CGImageRef)CGImage {
    return _CGImage;
}

- (NSData *)PNGRepresentation {
    if (!_CGImage) return nil;

    CFMutableDataRef data = CFDataCreateMutable(NULL, 0);
    CGDataProviderRef provider = CGImageGetDataProvider(_CGImage);
    CFDataRef imageData = CGDataProviderCopyData(provider);

    if (imageData) {
        [(NSData *)data release];
        data = (CFMutableDataRef)[(NSData *)imageData mutableCopy];
        CFRelease(imageData);
    }

    return [(NSData *)data autorelease];
}

- (NSData *)JPEGRepresentationWithQuality:(CGFloat)compressionQuality {
    if (!_CGImage) return nil;
    CFMutableDataRef data = CFDataCreateMutable(NULL, 0);
    CGDataProviderRef provider = CGImageGetDataProvider(_CGImage);
    CFDataRef imageData = CGDataProviderCopyData(provider);

    if (imageData) {
        [(NSData *)data release];
        data = (CFMutableDataRef)[(NSData *)imageData mutableCopy];
        CFRelease(imageData);
    }

    return [(NSData *)data autorelease];
}

- (BOOL)isEqual:(id)object {
    if (![object isKindOfClass:[UIImage class]]) return NO;
    UIImage *other = (UIImage *)object;
    return _CGImage == other->_CGImage;
}

- (UIImage *)stretchableImageWithLeftCapWidth:(NSInteger)leftCapWidth topCapHeight:(NSInteger)topCapHeight {
    UIImage *copy = [self copy];
    copy->_capInsets = UIEdgeInsetsMake(topCapHeight, leftCapWidth, 0, 0);
    copy->_resizingMode = UIImageResizingModeStretch;
    return [copy autorelease];
}

- (UIImage *)resizableImageWithCapInsets:(UIEdgeInsets)capInsets {
    return [self resizableImageWithCapInsets:capInsets resizingMode:UIImageResizingModeStretch];
}

- (UIImage *)resizableImageWithCapInsets:(UIEdgeInsets)capInsets resizingMode:(UIImageResizingMode)resizingMode {
    UIImage *copy = [self copy];
    copy->_capInsets = capInsets;
    copy->_resizingMode = resizingMode;
    return [copy autorelease];
}

- (void)drawAtPoint:(CGPoint)point {
    if (!_CGImage) return;
    CGContextRef context = UIGraphicsGetCurrentContext();
    if (!context) return;

    CGContextSaveGState(context);
    CGContextTranslateCTM(context, point.x, point.y);
    CGContextDrawImage(context, CGRectMake(0, 0, CGImageGetWidth(_CGImage) / _scale, CGImageGetHeight(_CGImage) / _scale), _CGImage);
    CGContextRestoreGState(context);
}

- (void)drawInRect:(CGRect)rect {
    if (!_CGImage) return;
    CGContextRef context = UIGraphicsGetCurrentContext();
    if (!context) return;

    CGContextSaveGState(context);
    CGContextDrawImage(context, rect, _CGImage);
    CGContextRestoreGState(context);
}

- (void)drawAsPatternInRect:(CGRect)rect {
    [self drawInRect:rect];
}

@end
