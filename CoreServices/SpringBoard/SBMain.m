#if defined(__GNUSTEP__)
    #import <AppKit>
    #pragma mark @if (isAvailable): <UIKit/UIKit.h> @else: goto 2
    // TODO:     ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

    @interface SBWindow : NSWindow
        @property (nonatomic, strong) NSImageView *wallpaperView;
        @property (nonatomic, strong) NSCollectionView *appGrid;
    @end

    @implementation SBWindow
        - (instancetype)initWithContentRect:(NSRect)rect {
            self = [super initWithContentRect:rect
                styleMask:NSWindowStyleMaskBorderless
                backing:NSBackingStoreBuffered
                defer:NO
            ];

            self.wallpaperView = [[NSImageView alloc] initWithFrame:rect];
            self.wallpaperView.image = [NSImage imageThemed:@"default"];
            self.wallpaperView.imageScaling = NSImageScaleProportionallyUpOrDown;
            [self.contentView addSubView:self.wallpaperView];

            return self;
        }
    @end
#endif

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@implementation SBWindow
        - (instancetype)initWithContentRect:(CGRect)rect {
            self = [super initWithContentRect:rect];
        }
@end

int main(int argv, const *argc) {
    // WIP
}