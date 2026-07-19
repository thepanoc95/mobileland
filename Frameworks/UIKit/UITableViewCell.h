#import "UIView.h"

@class UILabel, UIImageView, UIFont, UIColor;

typedef NS_ENUM(NSInteger, UITableViewCellStyle) {
    UITableViewCellStyleDefault,
    UITableViewCellStyleValue1,
    UITableViewCellStyleValue2,
    UITableViewCellStyleSubtitle,
};

typedef NS_ENUM(NSInteger, UITableViewCellSelectionStyle) {
    UITableViewCellSelectionStyleNone,
    UITableViewCellSelectionStyleBlue,
    UITableViewCellSelectionStyleGray,
    UITableViewCellSelectionStyleDefault,
};

typedef NS_ENUM(NSInteger, UITableViewCellAccessoryType) {
    UITableViewCellAccessoryNone,
    UITableViewCellAccessoryDisclosureIndicator,
    UITableViewCellAccessoryDetailDisclosureButton,
    UITableViewCellAccessoryCheckmark,
    UITableViewCellAccessoryDetailButton,
};

typedef NS_ENUM(NSInteger, UITableViewCellEditingStyle) {
    UITableViewCellEditingStyleNone,
    UITableViewCellEditingStyleDelete,
    UITableViewCellEditingStyleInsert,
};

@interface UITableViewCell : UIView {
    UILabel *_textLabel;
    UILabel *_detailTextLabel;
    UIImageView *_imageView;
    UIView *_contentView;
    UIView *_accessoryView;
    NSString *_reuseIdentifier;
    UITableViewCellSelectionStyle _selectionStyle;
    UITableViewCellAccessoryType _accessoryType;
    BOOL _selected;
    BOOL _highlighted;
}

@property (nonatomic, readonly) UILabel *textLabel;
@property (nonatomic, readonly) UILabel *detailTextLabel;
@property (nonatomic, readonly) UIImageView *imageView;
@property (nonatomic, readonly) UIView *contentView;
@property (nonatomic, strong) UIView *accessoryView;
@property (nonatomic, readonly, copy) NSString *reuseIdentifier;
@property (nonatomic) UITableViewCellSelectionStyle selectionStyle;
@property (nonatomic) UITableViewCellAccessoryType accessoryType;
@property (nonatomic, getter=isSelected) BOOL selected;
@property (nonatomic, getter=isHighlighted) BOOL highlighted;

- (instancetype)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier;
- (void)prepareForReuse;
- (void)setSelected:(BOOL)selected animated:(BOOL)animated;
- (void)setHighlighted:(BOOL)highlighted animated:(BOOL)animated;

@end
