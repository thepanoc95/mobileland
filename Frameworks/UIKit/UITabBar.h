#import "UIView.h"

@class UIColor, UIImage, UITabBarItem;

@interface UITabBar : UIView {
    NSArray *_items;
    UITabBarItem *_selectedItem;
    UIColor *_tintColor;
    UIColor *_barTintColor;
    BOOL _translucent;
}

@property (nonatomic, copy) NSArray *items;
@property (nonatomic, strong) UITabBarItem *selectedItem;
@property (nonatomic, strong) UIColor *tintColor;
@property (nonatomic, strong) UIColor *barTintColor;
@property (nonatomic) BOOL translucent;

@end

@interface UITabBarItem : NSObject {
    NSString *_title;
    UIImage *_image;
    NSInteger _tag;
    BOOL _enabled;
}

@property (nonatomic, copy) NSString *title;
@property (nonatomic, strong) UIImage *image;
@property (nonatomic) NSInteger tag;
@property (nonatomic, getter=isEnabled) BOOL enabled;

- (instancetype)initWithTitle:(NSString *)title image:(UIImage *)image tag:(NSInteger)tag;
- (instancetype)initWithTabBarSystemItem:(NSInteger)systemItem tag:(NSInteger)tag;

@end

extern const NSInteger UITabBarSystemItemMore;
extern const NSInteger UITabBarSystemItemFavorites;
extern const NSInteger UITabBarSystemItemFeatured;
extern const NSInteger UITabBarSystemItemHistory;
extern const NSInteger UITabBarSystemItemDownloads;
extern const NSInteger UITabBarSystemItemMostRecent;
extern const NSInteger UITabBarSystemItemMostViewed;
