#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>

@class UIColor, UIImage, CGImage;

typedef NS_ENUM(NSInteger, UIImageOrientation) {
    UIImageOrientationUp,
    UIImageOrientationDown,
    UIImageOrientationLeft,
    UIImageOrientationRight,
    UIImageOrientationUpMirrored,
    UIImageOrientationDownMirrored,
    UIImageOrientationLeftMirrored,
    UIImageOrientationRightMirrored,
};

typedef NS_ENUM(NSInteger, UIImageResizingMode) {
    UIImageResizingModeTile,
    UIImageResizingModeStretch,
};

@interface UIImage : NSObject <NSCopying> {
    CGImageRef _CGImage;
    UIImageOrientation _imageOrientation;
    UIEdgeInsets _capInsets;
    UIImageResizingMode _resizingMode;
    CGFloat _scale;
}

+ (UIImage *)imageNamed:(NSString *)name;
+ (UIImage *)imageWithData:(NSData *)data;
+ (UIImage *)imageWithData:(NSData *)data scale:(CGFloat)scale;
+ (UIImage *)imageWithContentsOfFile:(NSString *)path;
+ (UIImage *)imageWithCGImage:(CGImageRef)CGImage;
+ (UIImage *)imageWithCGImage:(CGImageRef)CGImage scale:(CGFloat)scale orientation:(UIImageOrientation)orientation;

- (instancetype)initWithData:(NSData *)data;
- (instancetype)initWithData:(NSData *)data scale:(CGFloat)scale;
- (instancetype)initWithContentsOfFile:(NSString *)path;
- (instancetype)initWithCGImage:(CGImageRef)CGImage;
- (instancetype)initWithCGImage:(CGImageRef)CGImage scale:(CGFloat)scale orientation:(UIImageOrientation)orientation;

- (CGSize)size;
- (CGFloat)scale;
- (UIImageOrientation)imageOrientation;
- (CGImageRef)CGImage;
- (NSData *)PNGRepresentation;
- (NSData *)JPEGRepresentationWithQuality:(CGFloat)compressionQuality;
- (BOOL)isEqual:(id)object;

- (UIImage *)stretchableImageWithLeftCapWidth:(NSInteger)leftCapWidth topCapHeight:(NSInteger)topCapHeight;
- (UIImage *)resizableImageWithCapInsets:(UIEdgeInsets)capInsets;
- (UIImage *)resizableImageWithCapInsets:(UIEdgeInsets)capInsets resizingMode:(UIImageResizingMode)resizingMode;

- (void)drawAtPoint:(CGPoint)point;
- (void)drawInRect:(CGRect)rect;
- (void)drawAsPatternInRect:(CGRect)rect;

@end
