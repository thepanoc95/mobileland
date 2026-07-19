#import "UITableView.h"
#import "UITableViewCell.h"
#import "UIColor.h"
#import <CoreGraphics/CGContext.h>

@implementation UITableView

@synthesize dataSource = _dataSource;
@synthesize delegate = _delegate;
@synthesize style = _style;
@synthesize rowHeight = _rowHeight;
@synthesize sectionHeaderHeight = _sectionHeaderHeight;
@synthesize sectionFooterHeight = _sectionFooterHeight;
@synthesize editing = _editing;
@synthesize allowsSelection = _allowsSelection;
@synthesize allowsMultipleSelection = _allowsMultipleSelection;

- (instancetype)initWithFrame:(CGRect)frame {
    return [self initWithFrame:frame style:UITableViewStylePlain];
}

- (instancetype)initWithFrame:(CGRect)frame style:(UITableViewStyle)style {
    self = [super initWithFrame:frame];
    if (self) {
        _style = style;
        _rowHeight = 44.0f;
        _sectionHeaderHeight = 22.0f;
        _sectionFooterHeight = 22.0f;
        _numberOfSections = 1;
        _editing = NO;
        _allowsSelection = YES;
        _allowsMultipleSelection = NO;
        _separatorStyle = UITableViewCellSeparatorStyleSingleLine;
        self.backgroundColor = [UIColor whiteColor];
    }
    return self;
}

- (void)reloadData {
    if (_dataSource && [_dataSource respondsToSelector:@selector(numberOfSectionsInTableView:)]) {
        _numberOfSections = [_dataSource numberOfSectionsInTableView:self];
    }
    [self setNeedsLayout];
    [self setNeedsDisplay];
}

- (void)reloadSections:(NSIndexSet *)sections withRowAnimation:(UITableViewRowAnimation)animation {
    [self reloadData];
}

- (UITableViewCell *)dequeueReusableCellWithIdentifier:(NSString *)identifier {
    return [[[UITableViewCell alloc] initWithStyle:0 reuseIdentifier:identifier] autorelease];
}

- (UITableViewCell *)cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    return nil;
}

- (NSIndexPath *)indexPathForSelectedRow {
    return nil;
}

- (NSArray *)indexPathsForVisibleRows {
    return nil;
}

- (void)selectRowAtIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated scrollPosition:(UITableViewScrollPosition)scrollPosition {
}

- (void)deselectRowAtIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated {
}

- (void)registerClass:(Class)cellClass forCellReuseIdentifier:(NSString *)identifier {
}

- (void)registerNib:(id)nib forCellReuseIdentifier:(NSString *)identifier {
}

- (NSInteger)numberOfSections {
    if (_dataSource && [_dataSource respondsToSelector:@selector(numberOfSectionsInTableView:)]) {
        return [_dataSource numberOfSectionsInTableView:self];
    }
    return 1;
}

- (NSInteger)numberOfRowsInSection:(NSInteger)section {
    if (_dataSource) {
        return [_dataSource tableView:self numberOfRowsInSection:section];
    }
    return 0;
}

- (CGRect)rectForSection:(NSInteger)section {
    CGFloat y = section * (_sectionHeaderHeight + _numberOfSections * _rowHeight + _sectionFooterHeight);
    return CGRectMake(0, y, self.bounds.size.width, _sectionHeaderHeight + [self numberOfRowsInSection:section] * _rowHeight + _sectionFooterHeight);
}

- (CGRect)rectForRowAtIndexPath:(NSIndexPath *)indexPath {
    CGRect sectionRect = [self rectForSection:indexPath.section];
    CGFloat y = sectionRect.origin.y + _sectionHeaderHeight + indexPath.row * _rowHeight;
    return CGRectMake(0, y, self.bounds.size.width, _rowHeight);
}

- (void)layoutSubviews {
    [super layoutSubviews];
}

- (void)drawRect:(CGRect)rect {
    CGContextRef context = UIGraphicsGetCurrentContext();
    if (!context) return;

    CGFloat currentY = 0;

    for (NSInteger section = 0; section < [self numberOfSections]; section++) {
        // Section header
        if (_sectionHeaderHeight > 0) {
            UIColor *headerBg = (_style == UITableViewStyleGrouped) ?
                [UIColor colorWithRed:0.92f green:0.92f blue:0.95f alpha:1.0f] : [UIColor clearColor];
            CGContextSetFillColorWithColor(context, [headerBg CGColor]);
            CGContextFillRect(context, CGRectMake(0, currentY, rect.size.width, _sectionHeaderHeight));

            if ([_delegate respondsToSelector:@selector(tableView:heightForHeaderInSection:)]) {
                // Use delegate height
            }
            currentY += _sectionHeaderHeight;
        }

        // Rows
        NSInteger rows = [self numberOfRowsInSection:section];
        for (NSInteger row = 0; row < rows; row++) {
            // Cell background
            CGContextSetFillColorWithColor(context, [UIColor whiteColor].CGColor);
            CGContextFillRect(context, CGRectMake(0, currentY, rect.size.width, _rowHeight));

            // Separator line
            if (_separatorStyle == UITableViewCellSeparatorStyleSingleLine && row < rows - 1) {
                CGContextSetGrayStrokeColor(context, 0.85f, 1.0f);
                CGContextSetLineWidth(context, 0.5f);
                CGContextMoveToPoint(context, 15.0f, currentY + _rowHeight);
                CGContextAddLineToPoint(context, rect.size.width, currentY + _rowHeight);
                CGContextStrokePath(context);
            }

            // Draw cell content via data source
            if (_dataSource && [_dataSource respondsToSelector:@selector(tableView:cellForRowAtIndexPath:)]) {
                NSIndexPath *indexPath = [NSIndexPath indexPathForRow:row inSection:section];
                UITableViewCell *cell = [_dataSource tableView:self cellForRowAtIndexPath:indexPath];
                if (cell) {
                    cell.frame = CGRectMake(0, currentY, rect.size.width, _rowHeight);
                    [cell drawRect:cell.frame];
                }
            }

            currentY += _rowHeight;
        }

        // Section footer
        currentY += _sectionFooterHeight;
    }
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    if (!_allowsSelection) return;
    [super touchesBegan:touches withEvent:event];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    if (!_allowsSelection) return;

    UITouch *touch = [touches anyObject];
    CGPoint point = [touch locationInView:self];

    for (NSInteger section = 0; section < [self numberOfSections]; section++) {
        NSInteger rows = [self numberOfRowsInSection:section];
        for (NSInteger row = 0; row < rows; row++) {
            NSIndexPath *indexPath = [NSIndexPath indexPathForRow:row inSection:section];
            CGRect cellRect = [self rectForRowAtIndexPath:indexPath];
            if (CGRectContainsPoint(cellRect, point)) {
                if ([_delegate respondsToSelector:@selector(tableView:didSelectRowAtIndexPath:)]) {
                    [_delegate tableView:self didSelectRowAtIndexPath:indexPath];
                }
                break;
            }
        }
    }

    [super touchesEnded:touches withEvent:event];
}

@end
