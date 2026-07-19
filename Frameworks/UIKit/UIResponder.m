#import "UIResponder.h"

@implementation UIResponder {
    BOOL _isFirstResponder;
}

- (UIResponder *)nextResponder {
    return nil;
}

- (BOOL)canBecomeFirstResponder {
    return NO;
}

- (BOOL)becomeFirstResponder {
    if ([self canBecomeFirstResponder]) {
        _isFirstResponder = YES;
        return YES;
    }
    return NO;
}

- (BOOL)canResignFirstResponder {
    return YES;
}

- (BOOL)resignFirstResponder {
    if ([self canResignFirstResponder]) {
        _isFirstResponder = NO;
        return YES;
    }
    return NO;
}

- (BOOL)isFirstResponder {
    return _isFirstResponder;
}

- (void)motionBegan:(UIEventSubtype)motion withEvent:(UIEvent *)event {
    [[self nextResponder] motionBegan:motion withEvent:event];
}

- (void)motionEnded:(UIEventSubtype)motion withEvent:(UIEvent *)event {
    [[self nextResponder] motionEnded:motion withEvent:event];
}

- (void)motionCancelled:(UIEventSubtype)motion withEvent:(UIEvent *)event {
    [[self nextResponder] motionCancelled:motion withEvent:event];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    [[self nextResponder] touchesBegan:touches withEvent:event];
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
    [[self nextResponder] touchesMoved:touches withEvent:event];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    [[self nextResponder] touchesEnded:touches withEvent:event];
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
    [[self nextResponder] touchesCancelled:touches withEvent:event];
}

- (void)pressesBegan:(NSSet *)presses withEvent:(UIPressesEvent *)event {
    [[self nextResponder] pressesBegan:presses withEvent:event];
}

- (void)pressesEnded:(NSSet *)presses withEvent:(UIPressesEvent *)event {
    [[self nextResponder] pressesEnded:presses withEvent:event];
}

- (void)pressesCancelled:(NSSet *)presses withEvent:(UIPressesEvent *)event {
    [[self nextResponder] pressesCancelled:presses withEvent:event];
}

- (void)pressesChanged:(NSSet *)presses withEvent:(UIPressesEvent *)event {
    [[self nextResponder] pressesChanged:presses withEvent:event];
}

@end
