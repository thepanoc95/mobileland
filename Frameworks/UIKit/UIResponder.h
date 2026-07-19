#import <Foundation/Foundation.h>

@interface UIResponder : NSObject

- (UIResponder *)nextResponder;

- (BOOL)canBecomeFirstResponder;
- (BOOL)becomeFirstResponder;
- (BOOL)canResignFirstResponder;
- (BOOL)resignFirstResponder;
- (BOOL)isFirstResponder;

- (void)motionBegan:(UIEventSubtype)motion withEvent:(UIEvent *)event;
- (void)motionEnded:(UIEventSubtype)motion withEvent:(UIEvent *)event;
- (void)motionCancelled:(UIEventSubtype)motion withEvent:(UIEvent *)event;

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event;

- (void)pressesBegan:(NSSet *)presses withEvent:(UIPressesEvent *)event;
- (void)pressesEnded:(NSSet *)presses withEvent:(UIPressesEvent *)event;
- (void)pressesCancelled:(NSSet *)presses withEvent:(UIPressesEvent *)event;
- (void)pressesChanged:(NSSet *)presses withEvent:(UIPressesEvent *)event;

@end
