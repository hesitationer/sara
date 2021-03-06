# DO++ modified version of LuaDist version of CMakeLists.txt
# Copyright (C) 2013 David OK.
# Redistribution and use of this file is allowed according to the terms of the 
# MPLv2 license.
# For details see the COPYRIGHT file distributed with DO++.
# Please note that the package source code is licensed under its own license.

# Copyright (C) 2007-2012 LuaDist.
# Created by Peter Kapec, David Manura
# Redistribution and use of this file is allowed according to the terms of the MIT license.
# For details see the COPYRIGHT file distributed with LuaDist.
# Please note that the package source code is licensed under its own license.

project (libjpeg C)
cmake_minimum_required (VERSION 2.8)

include (CheckIncludeFile)
check_include_file (stddef.h HAVE_STDDEF_H)
check_include_file (stdlib.h HAVE_STDLIB_H)
if (WIN32 AND NOT CYGWIN)
  set (TWO_FILE_COMMANDLINE true)
endif ()
configure_file (jconfig.h.cmake ${CMAKE_CURRENT_SOURCE_DIR}/jconfig.h)

set (HEADERS jerror.h jmorecfg.h jpeglib.h ${CMAKE_CURRENT_SOURCE_DIR}/jconfig.h)

set (SRC jmemnobs.c jaricom.c jcapimin.c jcapistd.c jcarith.c jccoefct.c jccolor.c 
  jcdctmgr.c jchuff.c jcinit.c jcmainct.c jcmarker.c jcmaster.c jcomapi.c jcparam.c 
  jcprepct.c jcsample.c jctrans.c jdapimin.c jdapistd.c jdarith.c jdatadst.c jdatasrc.c 
  jdcoefct.c jdcolor.c jddctmgr.c jdhuff.c jdinput.c jdmainct.c jdmarker.c jdmaster.c 
  jdmerge.c jdpostct.c jdsample.c jdtrans.c jerror.c jfdctflt.c jfdctfst.c jfdctint.c 
  jidctflt.c jidctfst.c jidctint.c jquant1.c jquant2.c jutils.c jmemmgr.c jdct.h 
  jinclude.h jmemsys.h jpegint.h jversion.h)

add_library(jpeg STATIC ${SRC} ${HEADERS})
set_target_properties(jpeg
  PROPERTIES
  OUTPUT_NAME_DEBUG jpeg-d)
if (WIN32)
  install(TARGETS jpeg
          LIBRARY DESTINATION lib
          ARCHIVE DESTINATION lib
          COMPONENT ThirdParty)
endif ()
