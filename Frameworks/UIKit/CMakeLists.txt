add_framework_bundle(UIKit
    VERSION A
    INFO_PLIST Info.plist
    HEADERS UIKit.h UIColor.h UIFont.h UIResponder.h UIView.h UIControl.h
            UIApplication.h UIWindow.h UIScreen.h UIViewController.h
            UIImage.h UIImageView.h UIGraphics.h UIEvent.h
            UILabel.h UIButton.h UITextField.h
            UIScrollView.h UITableView.h UITableViewCell.h
            UINavigationBar.h UINavigationController.h
            UITabBar.h UITabBarController.h
            UIVisualEffect.h UIBlurEffect.h UIVibrancyEffect.h
            UIVisualEffectView.h UIMaterialView.h
    PRIVATE_HEADERS
    INSTALL_NAME_DIR /System/Library/Frameworks/UIKit.framework/Versions/A
)

target_sources(UIKit PRIVATE
    UIColor.m
    UIFont.m
    UIResponder.m
    UIView.m
    UIControl.m
    UIApplication.m
    UIWindow.m
    UIScreen.m
    UIViewController.m
    UIImage.m
    UIImageView.m
    UIGraphics.m
    UIEvent.m
    UILabel.m
    UIButton.m
    UITextField.m
    UIScrollView.m
    UITableView.m
    UITableViewCell.m
    UINavigationBar.m
    UINavigationController.m
    UITabBar.m
    UITabBarController.m
    UIVisualEffect.m
    UIBlurEffect.m
    UIVibrancyEffect.m
    UIVisualEffectView.m
    UIMaterialView.m
)

target_include_directories(UIKit PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${RAVYN_SDKROOT}/System/Library/Frameworks/CoreGraphics.framework/Headers
    ${RAVYN_SDKROOT}/System/Library/Frameworks/QuartzCore.framework/Headers
    ${RAVYN_SDKROOT}/System/Library/Frameworks/Foundation.framework/Headers
)

target_compile_options(UIKit PRIVATE
    -fblocks
    -fobjc-exceptions
    -Wno-deprecated-declarations
)

target_link_options(UIKit PRIVATE
    -F${RAVYN_SDKROOT}/System/Library/Frameworks
    -framework CoreGraphics
    -framework QuartzCore
    -framework CoreFoundation
    -framework Foundation
)

add_dependencies(UIKit CoreGraphics QuartzCore Foundation)
