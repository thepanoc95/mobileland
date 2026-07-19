#import "UIView.h"
#import "UIColor.h"

@class UIBarButtonItem, UINavigationItem;

@interface UINavigationBar : UIView {
    NSMutableArray *_items;
    UINavigationItem *_topItem;
    UINavigationItem *_backItem;
    UIColor *_barTintColor;
    BOOL _translucent;
    NSInteger _barStyle;
}

@property (nonatomic, copy) NSArray *items;
@property (nonatomic, readonly) UINavigationItem *topItem;
@property (nonatomic, readonly) UINavigationItem *backItem;
@property (nonatomic, strong) UIColor *barTintColor;
@property (nonatomic, getter=isTranslucent) BOOL translucent;
@property (nonatomic) NSInteger barStyle;

- (void)pushNavigationItem:(UINavigationItem *)item animated:(BOOL)animated;
- (UINavigationItem *)popNavigationItemAnimated:(BOOL)animated;
- (void)setItems:(NSArray *)items animated:(BOOL)animated;

@end

@interface UINavigationItem : NSObject {
    NSString *_title;
    UIView *_titleView;
    NSArray *_leftBarButtonItems;
    NSArray *_rightBarButtonItems;
    UIBarButtonItem *_backBarButtonItem;
    BOOL _hidesBackButton;
}

@property (nonatomic, copy) NSString *title;
@property (nonatomic, strong) UIView *titleView;
@property (nonatomic, copy) NSArray *leftBarButtonItems;
@property (nonatomic, copy) NSArray *rightBarButtonItems;
@property (nonatomic, strong) UIBarButtonItem *backBarButtonItem;
@property (nonatomic) BOOL hidesBackButton;

- (instancetype)initWithTitle:(NSString *)title;

@end

@interface UIBarButtonItem : NSObject {
    NSString *_title;
    NSInteger _style;
    id _target;
    SEL _action;
    BOOL _enabled;
    UIImage *_image;
}

@property (nonatomic, copy) NSString *title;
@property (nonatomic) NSInteger style;
@property (nonatomic, assign) id target;
@property (nonatomic) SEL action;
@property (nonatomic, getter=isEnabled) BOOL enabled;
@property (nonatomic, strong) UIImage *image;

+ (instancetype)barButtonSystemItem:(NSInteger)systemItem target:(id)target action:(SEL)action;
+ (instancetype)barButtonItemWithCustomView:(UIView *)customView;
- (instancetype)initWithTitle:(NSString *)title style:(NSInteger)style target:(id)target action:(SEL)action;
- (instancetype)initWithBarButtonSystemItem:(NSInteger)systemItem target:(id)target action:(SEL)action;

@end
