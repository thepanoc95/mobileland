#import "UIControl.h"
#import <Foundation/NSArray.h>

@interface UIControlTargetAction : NSObject {
    __weak id _target;
    SEL _action;
    UIControlEvents _controlEvents;
}

@property (nonatomic, weak) id target;
@property (nonatomic) SEL action;
@property (nonatomic) UIControlEvents controlEvents;

@end

@implementation UIControlTargetAction

@synthesize target = _target;
@synthesize action = _action;
@synthesize controlEvents = _controlEvents;

@end

@implementation UIControl {
    NSMutableArray *_targetActions;
}

- (instancetype)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        _enabled = YES;
        _selected = NO;
        _highlighted = NO;
        _state = UIControlStateNormal;
        _targetActions = [[NSMutableArray alloc] init];
    }
    return self;
}

- (void)dealloc {
    [_targetActions release];
    [super dealloc];
}

- (void)setEnabled:(BOOL)enabled {
    _enabled = enabled;
    [self setNeedsDisplay];
}

- (BOOL)isEnabled {
    return _enabled;
}

- (void)setSelected:(BOOL)selected {
    _selected = selected;
    [self setNeedsDisplay];
}

- (BOOL)isSelected {
    return _selected;
}

- (void)setHighlighted:(BOOL)highlighted {
    _highlighted = highlighted;
    [self setNeedsDisplay];
}

- (BOOL)isHighlighted {
    return _highlighted;
}

- (NSUInteger)state {
    NSUInteger state = UIControlStateNormal;
    if (_highlighted) state |= UIControlStateHighlighted;
    if (_enabled == NO) state |= UIControlStateDisabled;
    if (_selected) state |= UIControlStateSelected;
    return state;
}

- (void)addTarget:(id)target action:(SEL)action forControlEvents:(UIControlEvents)controlEvents {
    UIControlTargetAction *ta = [[[UIControlTargetAction alloc] init] autorelease];
    ta.target = target;
    ta.action = action;
    ta.controlEvents = controlEvents;
    [_targetActions addObject:ta];
}

- (void)removeTarget:(id)target action:(SEL)action forControlEvents:(UIControlEvents)controlEvents {
    NSMutableIndexSet *indexes = [NSMutableIndexSet indexSet];
    NSUInteger idx = 0;
    for (UIControlTargetAction *ta in _targetActions) {
        if (ta.target == target && ta.action == action && ta.controlEvents == controlEvents) {
            [indexes addIndex:idx];
        }
        idx++;
    }
    [_targetActions removeObjectsAtIndexes:indexes];
}

- (void)sendActionsForControlEvents:(UIControlEvents)controlEvents {
    for (UIControlTargetAction *ta in _targetActions) {
        if (ta.controlEvents & controlEvents) {
            if (ta.target && [ta.target respondsToSelector:ta.action]) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Warc-performSelector-leaks"
                [ta.target performSelector:ta.action withObject:self];
#pragma clang diagnostic pop
            }
        }
    }
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    _highlighted = YES;
    [self setNeedsDisplay];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    _highlighted = NO;
    [self setNeedsDisplay];
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
    _highlighted = NO;
    [self setNeedsDisplay];
}

@end
