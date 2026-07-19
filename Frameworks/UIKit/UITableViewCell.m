#import "UITableViewCell.h"
#import "UILabel.h"
#import "UIImageView.h"
#import "UIColor.h"
#import <CoreGraphics/CGContext.h>

@implementation UITableViewCell

@synthesize textLabel = _textLabel;
@synthesize detailTextLabel = _detailTextLabel;
@synthesize imageView = _imageView;
@synthesize contentView = _contentView;
@synthesize accessoryView = _accessoryView;
@synthesize reuseIdentifier = _reuseIdentifier;
@synthesize selectionStyle = _selectionStyle;
@synthesize accessoryType = _accessoryType;

- (instancetype)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier {
    self = [super initWithFrame:CGRectZero];
    if (self) {
        _reuseIdentifier = [reuseIdentifier copy];
        _selectionStyle = UITableViewCellSelectionStyleDefault;
        _accessoryType = UITableViewCellAccessoryNone;

        _contentView = [[UIView alloc] initWithFrame:CGRectZero];
        [super addSubView:_contentView];

        _textLabel = [[UILabel alloc] initWithFrame:CGRectZero];
        _textLabel.font = [UIFont systemFontOfSize:17.0f];
        _textLabel.textColor = [UIColor blackColor];
        [_contentView addSubView:_textLabel];

        _detailTextLabel = [[UILabel alloc] initWithFrame:CGRectZero];
        _detailTextLabel.font = [UIFont systemFontOfSize:12.0f];
        _detailTextLabel.textColor = [UIColor grayColor];
        [_contentView addSubView:_detailTextLabel];

        _imageView = [[UIImageView alloc] initWithFrame:CGRectZero];
        [_contentView addSubView:_imageView];
    }
    return self;
}

- (void)dealloc {
    [_textLabel release];
    [_detailTextLabel release];
    [_imageView release];
    [_contentView release];
    [_accessoryView release];
    [_reuseIdentifier release];
    [super dealloc];
}

- (void)addSubView:(UIView *)view {
    [_contentView addSubView:view];
}

- (void)prepareForReuse {
    self.selected = NO;
    self.highlighted = NO;
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated {
    _selected = selected;
    [self setNeedsDisplay];
}

- (void)setHighlighted:(BOOL)highlighted animated:(BOOL)animated {
    _highlighted = highlighted;
    [self setNeedsDisplay];
}

- (void)layoutSubviews {
    [super layoutSubviews];
    CGRect bounds = self.bounds;

    _contentView.frame = bounds;

    // Layout based on cell style
    CGFloat imageWidth = 0;
    if (_imageView.image) {
        imageWidth = 40.0f;
        _imageView.frame = CGRectMake(15, (bounds.size.height - 30) / 2.0f, 30, 30);
    }

    CGFloat textX = 15 + imageWidth;
    _textLabel.frame = CGRectMake(textX, 8, bounds.size.width - textX - 15, 22);
    _detailTextLabel.frame = CGRectMake(textX, 32, bounds.size.width - textX - 15, 16);
}

- (void)drawRect:(CGRect)rect {
    CGContextRef context = UIGraphicsGetCurrentContext();
    if (!context) return;

    // Selection highlight
    if (_selected || _highlighted) {
        CGContextSetRGBFillColor(context, 0.85f, 0.85f, 0.85f, 1.0f);
        CGContextFillRect(context, rect);
    }

    // Accessory indicator
    if (_accessoryType == UITableViewCellAccessoryDisclosureIndicator) {
        CGContextSetGrayStrokeColor(context, 0.6f, 1.0f);
        CGContextSetLineWidth(context, 1.5f);
        CGFloat x = rect.size.width - 20;
        CGFloat y = rect.size.height / 2.0f;
        CGContextMoveToPoint(context, x, y - 5);
        CGContextAddLineToPoint(context, x + 5, y);
        CGContextAddLineToPoint(context, x, y + 5);
        CGContextStrokePath(context);
    } else if (_accessoryType == UITableViewCellAccessoryCheckmark) {
        CGContextSetRGBStrokeColor(context, 0.0f, 0.478f, 1.0f, 1.0f);
        CGContextSetLineWidth(context, 2.0f);
        CGFloat x = rect.size.width - 25;
        CGFloat y = rect.size.height / 2.0f;
        CGContextMoveToPoint(context, x - 4, y);
        CGContextAddLineToPoint(context, x - 1, y + 4);
        CGContextAddLineToPoint(context, x + 5, y - 4);
        CGContextStrokePath(context);
    }
}

@end
