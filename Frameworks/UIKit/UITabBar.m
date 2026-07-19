#import "UITabBar.h"
#import "UIColor.h"
#import "UIImage.h"
#import <CoreGraphics/CGContext.h>

const NSInteger UITabBarSystemItemMore = 0;
const NSInteger UITabBarSystemItemFavorites = 1;
const NSInteger UITabBarSystemItemFeatured = 2;
const NSInteger UITabBarSystemItemHistory = 3;
const NSInteger UITabBarSystemItemDownloads = 4;
const NSInteger UITabBarSystemItemMostRecent = 5;
const NSInteger UITabBarSystemItemMostViewed = 6;

@implementation UITabBar

@synthesize tintColor = _tintColor;
@synthesize barTintColor = _barTintColor;
@synthesize translucent = _translucent;

- (instancetype)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        _translucent = YES;
        self.backgroundColor = [UIColor colorWithRed:0.97f green:0.97f blue:0.97f alpha:0.94f];
    }
    return self;
}

- (void)dealloc {
    [_items release];
    [_selectedItem release];
    [_tintColor release];
    [_barTintColor release];
    [super dealloc];
}

- (NSArray *)items {
    return [[_items copy] autorelease];
}

- (void)setItems:(NSArray *)items {
    [_items release];
    _items = [items copy];
    [self setNeedsDisplay];
}

- (void)setSelectedItem:(UITabBarItem *)selectedItem {
    if (_selectedItem != selectedItem) {
        [_selectedItem release];
        _selectedItem = [selectedItem retain];
        [self setNeedsDisplay];
    }
}

- (void)drawRect:(CGRect)rect {
    CGContextRef context = UIGraphicsGetCurrentContext();
    if (!context) return;

    // Background
    UIColor *bgColor = _barTintColor ?: [UIColor colorWithRed:0.97f green:0.97f blue:0.97f alpha:0.94f];
    CGContextSetFillColorWithColor(context, [bgColor CGColor]);
    CGContextFillRect(context, rect);

    // Top border
    CGContextSetGrayStrokeColor(context, 0.78f, 1.0f);
    CGContextSetLineWidth(context, 0.5f);
    CGContextMoveToPoint(context, 0, 0);
    CGContextAddLineToPoint(context, rect.size.width, 0);
    CGContextStrokePath(context);

    // Tab items
    NSUInteger count = [_items count];
    if (count == 0) return;
    CGFloat tabWidth = rect.size.width / count;

    for (NSUInteger i = 0; i < count; i++) {
        UITabBarItem *item = [_items objectAtIndex:i];
        CGFloat x = i * tabWidth;

        BOOL isSelected = (item == _selectedItem);
        UIColor *textColor = isSelected ? (_tintColor ?: [UIColor colorWithRed:0.0f green:0.478f blue:1.0f alpha:1.0f]) : [UIColor grayColor];
        CGContextSetFillColorWithColor(context, [textColor CGColor]);

        if (item.title) {
            CGFloat textX = x + (tabWidth - [item.title sizeWithAttributes:nil].width) / 2.0f;
            CGContextShowTextAtPoint(context, textX, rect.size.height * 0.85f, [item.title UTF8String], strlen([item.title UTF8String]));
        }
    }
}

@end

@implementation UITabBarItem

@synthesize title = _title;
@synthesize image = _image;
@synthesize tag = _tag;
@synthesize enabled = _enabled;

- (instancetype)initWithTitle:(NSString *)title image:(UIImage *)image tag:(NSInteger)tag {
    self = [super init];
    if (self) {
        _title = [title copy];
        _image = [image retain];
        _tag = tag;
        _enabled = YES;
    }
    return self;
}

- (instancetype)initWithTabBarSystemItem:(NSInteger)systemItem tag:(NSInteger)tag {
    return [self initWithTitle:@"" image:nil tag:tag];
}

- (void)dealloc {
    [_title release];
    [_image release];
    [super dealloc];
}

@end
