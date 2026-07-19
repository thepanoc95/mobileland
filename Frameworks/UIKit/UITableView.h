#import "UIView.h"

@class UITableViewCell;

typedef NS_ENUM(NSInteger, UITableViewStyle) {
    UITableViewStylePlain,
    UITableViewStyleGrouped,
};

typedef NS_ENUM(NSInteger, UITableViewCellSeparatorStyle) {
    UITableViewCellSeparatorStyleNone,
    UITableViewCellSeparatorStyleSingleLine,
    UITableViewCellSeparatorStyleSingleLineEtched,
};

typedef NS_ENUM(NSInteger, UITableViewScrollPosition) {
    UITableViewScrollPositionNone,
    UITableViewScrollPositionTop,
    UITableViewScrollPositionMiddle,
    UITableViewScrollPositionBottom,
};

typedef NS_ENUM(NSInteger, UITableViewRowAnimation) {
    UITableViewRowAnimationNone,
    UITableViewRowAnimationFade,
    UITableViewRowAnimationRight,
    UITableViewRowAnimationLeft,
    UITableViewRowAnimationTop,
    UITableViewRowAnimationBottom,
    UITableViewRowAnimationAutomatic,
};

@protocol UITableViewDataSource <NSObject>
@required
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section;
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath;

@optional
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView;
- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section;
- (NSString *)tableView:(UITableView *)tableView titleForFooterInSection:(NSInteger)section;
- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath;
- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath;
@end

@protocol UITableViewDelegate <NSObject>
@optional
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath;
- (void)tableView:(UITableView *)tableView didDeselectRowAtIndexPath:(NSIndexPath *)indexPath;
- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath;
- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section;
- (CGFloat)tableView:(UITableView *)tableView heightForFooterInSection:(NSInteger)section;
- (UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section;
- (UIView *)tableView:(UITableView *)tableView viewForFooterInSection:(NSInteger)section;
- (BOOL)tableView:(UITableView *)tableView shouldHighlightRowAtIndexPath:(NSIndexPath *)indexPath;
@end

@interface UITableView : UIView {
    id<UITableViewDataSource> _dataSource;
    id<UITableViewDelegate> _delegate;
    UITableViewStyle _style;
    NSArray *_sectionHeaders;
    NSArray *_sectionFooters;
    CGFloat _rowHeight;
    CGFloat _sectionHeaderHeight;
    CGFloat _sectionFooterHeight;
    NSInteger _numberOfSections;
    BOOL _editing;
    BOOL _allowsSelection;
    BOOL _allowsMultipleSelection;
    NSInteger _separatorStyle;
}

@property (nonatomic, assign) id<UITableViewDataSource> dataSource;
@property (nonatomic, assign) id<UITableViewDelegate> delegate;
@property (nonatomic, readonly) UITableViewStyle style;
@property (nonatomic) CGFloat rowHeight;
@property (nonatomic) CGFloat sectionHeaderHeight;
@property (nonatomic) CGFloat sectionFooterHeight;
@property (nonatomic, getter=isEditing) BOOL editing;
@property (nonatomic) BOOL allowsSelection;
@property (nonatomic) BOOL allowsMultipleSelection;

- (instancetype)initWithFrame:(CGRect)frame style:(UITableViewStyle)style;

- (void)reloadData;
- (void)reloadSections:(NSIndexSet *)sections withRowAnimation:(UITableViewRowAnimation)animation;

- (UITableViewCell *)dequeueReusableCellWithIdentifier:(NSString *)identifier;
- (UITableViewCell *)cellForRowAtIndexPath:(NSIndexPath *)indexPath;
- (NSIndexPath *)indexPathForSelectedRow;
- (NSArray *)indexPathsForVisibleRows;

- (void)selectRowAtIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated scrollPosition:(UITableViewScrollPosition)scrollPosition;
- (void)deselectRowAtIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated;

- (void)registerClass:(Class)cellClass forCellReuseIdentifier:(NSString *)identifier;
- (void)registerNib:(id)nib forCellReuseIdentifier:(NSString *)identifier;

- (NSInteger)numberOfSections;
- (NSInteger)numberOfRowsInSection:(NSInteger)section;
- (CGRect)rectForSection:(NSInteger)section;
- (CGRect)rectForRowAtIndexPath:(NSIndexPath *)indexPath;

@end
