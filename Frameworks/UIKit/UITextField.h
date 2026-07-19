#import "UIControl.h"
#import "UIColor.h"

@class UIFont, UITextRange;

@interface UITextField : UIControl {
    NSString *_text;
    NSString *_placeholder;
    UIColor *_textColor;
    UIColor *_tintColor;
    UIFont *_font;
    NSInteger _textAlignment;
    BOOL _secureTextEntry;
    BOOL _editing;
    BOOL _clearsOnBeginEditing;
    BOOL _clearsOnInsertion;
    NSInteger _borderStyle;
    CGFloat _minimumFontSize;
    id _delegate;
    UIView *_inputView;
    UIView *_inputAccessoryView;
}

@property (nonatomic, copy) NSString *text;
@property (nonatomic, copy) NSString *placeholder;
@property (nonatomic, strong) UIColor *textColor;
@property (nonatomic, strong) UIColor *tintColor;
@property (nonatomic, strong) UIFont *font;
@property (nonatomic) NSInteger textAlignment;
@property (nonatomic, getter=isSecureTextEntry) BOOL secureTextEntry;
@property (nonatomic, readonly, getter=isEditing) BOOL editing;
@property (nonatomic) BOOL clearsOnBeginEditing;
@property (nonatomic) BOOL clearsOnInsertion;
@property (nonatomic) NSInteger borderStyle;
@property (nonatomic) CGFloat minimumFontSize;
@property (nonatomic, assign) id delegate;
@property (nonatomic, strong) UIView *inputView;
@property (nonatomic, strong) UIView *inputAccessoryView;

- (BOOL)hasText;
- (void)clearText;
- (void)selectAllText;

@end

@protocol UITextFieldDelegate <NSObject>
@optional
- (BOOL)textFieldShouldBeginEditing:(UITextField *)textField;
- (BOOL)textFieldDidBeginEditing:(UITextField *)textField;
- (BOOL)textFieldShouldEndEditing:(UITextField *)textField;
- (void)textFieldDidEndEditing:(UITextField *)textField;
- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string;
- (BOOL)textFieldShouldClear:(UITextField *)textField;
- (BOOL)textFieldShouldReturn:(UITextField *)textField;
@end
