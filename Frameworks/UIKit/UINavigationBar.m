#import "UINavigationBar.h"
#import "UIColor.h"
#import "UIBarButtonItem.h"
#import <CoreGraphics/CGContext.h>

@implementation UINavigationBar

@synthesize barTintColor = _barTintColor;
@synthesize translucent = _translucent;
@synthesize barStyle = _barStyle;

- (instancetype)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        _items = [[NSMutableArray alloc] init];
        _translucent = YES;
        _barStyle = 0;
        self.backgroundColor = [UIColor colorWithRed:0.97f green:0.97f blue:0.97f alpha:0.94f];
    }
    return self;
}

- (void)dealloc {
    [_items release];
    [_barTintColor release];
    [super dealloc];
}

- (NSArray *)items {
    return [[_items copy] autorelease];
}

- (void)setItems:(NSArray *)items {
    [self setItems:items animated:NO];
}

- (void)setItems:(NSArray *)items animated:(BOOL)animated {
    [_items release];
    _items = [items mutableCopy];
    _topItem = [_items lastObject];
    if ([_items count] >= 2) {
        _backItem = [_items objectAtIndex:[_items count] - 2];
    }
    [self setNeedsDisplay];
}

- (void)pushNavigationItem:(UINavigationItem *)item animated:(BOOL)animated {
    [_items addObject:item];
    _topItem = item;
    if ([_items count] >= 2) {
        _backItem = [_items objectAtIndex:[_items count] - 2];
    }
    [self setNeedsDisplay];
}

- (UINavigationItem *)popNavigationItemAnimated:(BOOL)animated {
    UINavigationItem *item = [[_items lastObject] retain];
    [_items removeLastObject];
    _topItem = [_items lastObject];
    if ([_items count] >= 2) {
        _backItem = [_items objectAtIndex:[_items count] - 2];
    } else {
        _backItem = nil;
    }
    [self setNeedsDisplay];
    return [item autorelease];
}

- (UINavigationItem *)topItem {
    return _topItem;
}

- (UINavigationItem *)backItem {
    return _backItem;
}

- (void)layoutSubviews {
    [super layoutSubviews];
}

- (void)drawRect:(CGRect)rect {
    CGContextRef context = UIGraphicsGetCurrentContext();
    if (!context) return;

    // Background
    UIColor *bgColor = _barTintColor ?: [UIColor colorWithRed:0.97f green:0.97f blue:0.97f alpha:0.94f];
    CGContextSetFillColorWithColor(context, [bgColor CGColor]);
    CGContextFillRect(context, rect);

    // Bottom border line
    CGContextSetGrayStrokeColor(context, 0.78f, 1.0f);
    CGContextSetLineWidth(context, 0.5f);
    CGContextMoveToPoint(context, 0, rect.size.height);
    CGContextAddLineToPoint(context, rect.size.width, rect.size.height);
    CGContextStrokePath(context);

    // Title
    if (_topItem.title) {
        CGContextSetRGBFillColor(context, 0.0f, 0.0f, 0.0f, 1.0f);
        CGSize titleSize = [_topItem.title sizeWithAttributes:nil];
        CGFloat titleX = (rect.size.width - titleSize.width) / 2.0f;
        CGFloat titleY = (rect.size.height - titleSize.height) / 2.0f + titleSize.height * 0.75f;
        CGContextShowTextAtPoint(context, titleX, titleY, [_topItem.title UTF8String], strlen([_topItem.title UTF8String]));
    }

    // Back button
    if (_backItem && !_backItem.hidesBackButton) {
        NSString *backTitle = _backItem.title ?: @"";
        if ([backTitle length] == 0 && _backItem.backBarButtonItem) {
            backTitle = _backItem.backBarButtonItem.title ?: @"";
        }
        if ([backTitle length] > 0) {
            CGContextSetRGBFillColor(context, 0.0f, 0.478f, 1.0f, 1.0f);
            NSString *display = [@"< " stringByAppendingString:backTitle];
            CGContextShowTextAtPoint(context, 10, rect.size.height * 0.75f, [display UTF8String], strlen([display UTF8String]));
        }
    }
}

@end

@implementation UINavigationItem

@synthesize title = _title;
@synthesize titleView = _titleView;
@synthesize leftBarButtonItems = _leftBarButtonItems;
@synthesize rightBarButtonItems = _rightBarButtonItems;
@synthesize backBarButtonItem = _backBarButtonItem;
@synthesize hidesBackButton = _hidesBackButton;

- (instancetype)initWithTitle:(NSString *)title {
    self = [super init];
    if (self) {
        _title = [title copy];
    }
    return self;
}

- (void)dealloc {
    [_title release];
    [_titleView release];
    [_leftBarButtonItems release];
    [_rightBarButtonItems release];
    [_backBarButtonItem release];
    [super dealloc];
}

@end

@implementation UIBarButtonItem

@synthesize title = _title;
@synthesize style = _style;
@synthesize target = _target;
@synthesize action = _action;
@synthesize enabled = _enabled;
@synthesize image = _image;

+ (instancetype)barButtonSystemItem:(NSInteger)systemItem target:(id)target action:(SEL)action {
    UIBarButtonItem *item = [[[self alloc] init] autorelease];
    item.target = target;
    item.action = action;
    item.enabled = YES;
    return item;
}

+ (instancetype)barButtonItemWithCustomView:(UIView *)customView {
    UIBarButtonItem *item = [[[self alloc] init] autorelease];
    return item;
}

- (instancetype)initWithTitle:(NSString *)title style:(NSInteger)style target:(id)target action:(SEL)action {
    self = [super init];
    if (self) {
        _title = [title copy];
        _style = style;
        _target = target;
        _action = action;
        _enabled = YES;
    }
    return self;
}

- (instancetype)initWithBarButtonSystemItem:(NSInteger)systemItem target:(id)target action:(SEL)action {
    return [self initWithTitle:@"" style:0 target:target action:action];
}

- (void)dealloc {
    [_title release];
    [_image release];
    [super dealloc];
}

@end
