#
# Copyright (c) 2010-2019, Gilles Caulier, <caulier dot gilles at gmail dot com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

include_directories(
    $<TARGET_PROPERTY:Qt5::Core,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:Qt5::Widgets,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:Qt5::Gui,INTERFACE_INCLUDE_DIRECTORIES>
)

set(multicorerawtopng_SRCS
    main.cpp
    processordlg.cpp
    myactionthread.cpp
)

add_executable(multicorerawtopng ${multicorerawtopng_SRCS})

target_link_libraries(multicorerawtopng
                      digikamcore

                      Qt5::Widgets
                      Qt5::Gui
                      Qt5::Core
)
