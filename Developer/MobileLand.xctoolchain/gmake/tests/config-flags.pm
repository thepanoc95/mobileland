# This is a -*-perl-*- script
#
# Set variables that were defined by configure, in case we need them
# during the tests.

%CONFIG_FLAGS = (
    AM_LDFLAGS      => '-Wl,--export-dynamic',
    AR              => '/usr/bin/ar',
    CC              => '/usr/bin/clang',
    CFLAGS          => '-g -O2',
    CPP             => '/usr/bin/clang -E',
    CPPFLAGS        => '',
    GUILE_CFLAGS    => '',
    GUILE_LIBS      => '',
    LDFLAGS         => '',
    LIBS            => '',
    USE_SYSTEM_GLOB => 'yes'
);

1;
