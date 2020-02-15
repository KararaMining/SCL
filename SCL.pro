#-----------------------------------------------------------------------------------------------------------------------------------
#
# PROJECT:            Storage Class Library
# FILE:								SCL.pro
# SUBSYSTEM:          Project File
# LANGUAGE:						C++
# TARGET OS:          All
# LIBRARY DEPENDANCE:	GCL, MCL.
# NAMESPACE:          N/A
# AUTHOR:							Gavin Blakeman.
# LICENSE:            GPLv2
#
#                     Copyright 2010, 2013-2020 Gavin Blakeman.
#                     This file is part of the Storage Class Library (SCL)
#
#                     SCL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
#                     License as published by the Free Software Foundation, either version 2 of the License, or (at your option)
#                     any later version.
#
#                     SCL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
#                     warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
#                     more details.
#
#                     You should have received a copy of the GNU General Public License along with SCL.  If not,
#                     see <http://www.gnu.org/licenses/>.
#
# OVERVIEW:
#
# HISTORY:            2015-09-22 GGB - astroManager 2015.09 release
#                     2013-06-16 GGB - Project file created.
#
#-----------------------------------------------------------------------------------------------------------------------------------

TARGET          = SCL
TEMPLATE        = lib
CONFIG          += staticlib

QMAKE_CXXFLAGS  += -std=c++17 #-static -static-libgcc -save-temps

OBJECTS_DIR     = "objects"

win32:CONFIG(release, debug|release) {
  DESTDIR = "../Library/win32/release"
  OBJECTS_DIR = "../Library/win32/release/object/SCL"
}
else:win32:CONFIG(debug, debug|release) {
  DESTDIR = "../Library/win32/debug"
  OBJECTS_DIR = "../Library/win32/debug/object/SCL"
}
else:unix:CONFIG(release, debug|release) {
  DESTDIR = "../Library/unix/release"
  OBJECTS_DIR = "../Library/unix/release/object/SCL"
}
else:unix:CONFIG(debug, debug|release) {
  DESTDIR = ""
  OBJECTS_DIR = "objects"
}

INCLUDEPATH += \
  "../GCL" \
  "../MCL" \
  "/home/gavin/Documents/Projects/software/Library/Boost/boost_1_71_0"

SOURCES += \
    source/AVLTree.cpp \
    source/common.cpp \
    source/config.cpp \
    source/valarray2DP.cpp \
    source/array2DP.cpp \
    source/array1DP.cpp \
    source/error.cpp

HEADERS += \
    include/VectorSorted.hpp \
    include/Base.h \
    include/AVLTree.h \
    include/valarray_mt.hpp \
    include/any.h \
    include/common.h \
    include/config.h \
    include/valarray2DP.h \
    include/algorithm.hpp \
    include/array2DP.h \
    include/array1DP.h \
    include/binaryTree.h \
    include/circularbuffer.hpp \
    include/error.h \
    SCL \
    include/hierarchyBuilder.hpp \
    include/array2D.hpp

DISTFILES += \
    license.txt \
    Doxyfile \
    changelog.txt
