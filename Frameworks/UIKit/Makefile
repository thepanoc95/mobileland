MK_WERROR = no

FRAMEWORK = UIKit
FMWK_VERSION = A
INFO_PLIST = Info.plist

HEADERS = UIKit.h UIColor.h UIFont.h UIResponder.h UIView.h UIControl.h \
	UIApplication.h UIWindow.h UIScreen.h UIViewController.h \
	UIImage.h UIImageView.h UIGraphics.h UIEvent.h \
	UILabel.h UIButton.h UITextField.h \
	UIScrollView.h UITableView.h UITableViewCell.h \
	UINavigationBar.h UINavigationController.h \
	UITabBar.h UITabBarController.h \
	UIVisualEffect.h UIBlurEffect.h UIVibrancyEffect.h \
	UIVisualEffectView.h UIMaterialView.h

PRIVATE_HEADERS =

SLF = /System/Library/Frameworks
INSTALL_NAME_DIR = ${SLF}/${FRAMEWORK}.framework/Versions/${FMWK_VERSION}

SRCS = UIColor.m \
    UIFont.m \
    UIResponder.m \
    UIView.m \
    UIControl.m \
    UIApplication.m \
    UIWindow.m \
    UIScreen.m \
    UIViewController.m \
    UIImage.m \
    UIImageView.m \
    UIGraphics.m \
    UIEvent.m \
    UILabel.m \
    UIButton.m \
    UITextField.m \
    UIScrollView.m \
    UITableView.m \
    UITableViewCell.m \
    UINavigationBar.m \
    UINavigationController.m \
    UITabBar.m \
    UITabBarController.m \
    UIVisualEffect.m \
    UIBlurEffect.m \
    UIVibrancyEffect.m \
    UIVisualEffectView.m \
    UIMaterialView.m

CFLAGS += -I${.CURDIR}/.. -I${BUILDROOT}/usr/include \
    -I${.CURDIR} -I${MAKEOBJDIR} \
    -I${SRCTOP}/Frameworks/CoreGraphics/Headers \
    -I${SRCTOP}/Frameworks/QuartzCore/Headers \
    -I${SRCTOP}/Frameworks/Foundation/Headers \
    -D_BuildingFramework

LDFLAGS += -Wl,-R/${SLF}/Foundation.framework/Versions/A \
    -Wl,-R/${SLF}/CoreFoundation.framework/Versions/A \
    -Wl,-R/${SLF}/CoreGraphics.framework/Versions/A \
    -Wl,-R/${SLF}/QuartzCore.framework/Versions/A \
    -L${OBJTOP}/Frameworks/Foundation/Foundation.framework \
    -L${OBJTOP}/Frameworks/CoreFoundation/CoreFoundation.framework \
    -L${OBJTOP}/Frameworks/CoreGraphics/CoreGraphics.framework \
    -L${OBJTOP}/Frameworks/QuartzCore/QuartzCore.framework \
    -lFoundation -lCoreFoundation -lCoreGraphics -lQuartzCore \
    -lobjc -lm

.include <rvn.framework.mk>

CFLAGS += ${FMWK_CFLAGS}
