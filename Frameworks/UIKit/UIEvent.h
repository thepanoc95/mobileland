#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>

@class UIWindow, UIView;

typedef NS_ENUM(NSInteger, UIEventType) {
    UIEventTypeTouches,
    UIEventTypeMotion,
    UIEventTypeRemoteControl,
    UIEventTypePresses,
};

typedef NS_ENUM(NSInteger, UIEventSubtype) {
    UIEventSubtypeNone,
    UIEventSubtypeMotionShake,
    UIEventSubtypeRemoteControlPlay,
    UIEventSubtypeRemoteControlPause,
    UIEventSubtypeRemoteControlStop,
    UIEventSubtypeRemoteControlTogglePlayPause,
    UIEventSubtypeRemoteControlNextTrack,
    UIEventSubtypeRemoteControlPreviousTrack,
    UIEventSubtypeRemoteControlBeginSeekingBackward,
    UIEventSubtypeRemoteControlEndSeekingBackward,
    UIEventSubtypeRemoteControlBeginSeekingForward,
    UIEventSubtypeRemoteControlEndSeekingForward,
};

@interface UIEvent : NSObject {
    UIEventType _type;
    UIEventSubtype _subtype;
    NSTimeInterval _timestamp;
}

@property (nonatomic, readonly) UIEventType type;
@property (nonatomic, readonly) UIEventSubtype subtype;
@property (nonatomic, readonly) NSTimeInterval timestamp;

- (NSSet *)allTouches;
- (NSSet *)touchesForView:(UIView *)view;
- (NSSet *)touchesForWindow:(UIWindow *)window;

@end

@interface UITouch : NSObject {
    UIView *_view;
    UIWindow *_window;
    CGPoint _locationInWindow;
    CGPoint _previousLocationInWindow;
    NSTimeInterval _timestamp;
    NSUInteger _tapCount;
    NSUInteger _phase;
}

@property (nonatomic, readonly) UIView *view;
@property (nonatomic, readonly) UIWindow *window;
@property (nonatomic, readonly) NSUInteger tapCount;
@property (nonatomic, readonly) NSTimeInterval timestamp;

typedef NS_ENUM(NSUInteger, UITouchPhase) {
    UITouchPhaseBegan,
    UITouchPhaseMoved,
    UITouchPhaseStationary,
    UITouchPhaseEnded,
    UITouchPhaseCancelled,
};

- (CGPoint)locationInView:(UIView *)view;
- (CGPoint)previousLocationInView:(UIView *)view;
- (UITouchPhase)phase;

@end

@interface UIPressesEvent : UIEvent

@end
