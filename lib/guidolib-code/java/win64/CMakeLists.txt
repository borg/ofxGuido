#######################################
# CMAKE guidolib
#######################################
project(guidojni)
cmake_minimum_required(VERSION 2.6)

set (target jniGUIDOEngine)
#######################################
# versions management
set (VERSION 1.21)
set (STRVERS "v.1.21")
set (SSTRVERS "1.21")

FIND_PACKAGE(JNI REQUIRED)
#FIND_PACKAGE(Java REQUIRED)
#message ( "JNI_INCLUDE_DIRS		 ${JNI_Iset_target_properties (${target} PROPERTIES
#message ( "JAVA_AWT_LIBRARY		 ${JAVA_AWT_LIBRARY}")
#message ( "JAVA_JVM_LIBRARY		 ${JAVA_JVM_LIBRARY}")
#message ( "JAVA_INCLUDE_PATH	 ${JAVA_INCLUDE_PATH}")
#message ( "JAVA_INCLUDE_PATH2	 ${JAVA_INCLUDE_PATH2}")
#message ( "JAVA_AWT_INCLUDE_PATH ${JAVA_AWT_INCLUDE_PATH}")


if(APPLE)
	set (CMAKE_OSX_ARCHITECTURES "x86_64 i386 ppc")
	set (CMAKE_OSX_SYSROOT /Developer/SDKs/MacOSX10.5.sdk)
	set (CMAKE_C++_FLAGS -mmacosx-version-min=10.4)
endif(APPLE)

if(WIN32)
 add_definitions(-DWINVER=0x0400 -DWIN32)
 if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 6")
  add_definitions(-DVC6)
 elseif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 8 2005")
  add_definitions(-DVC2005)
 elseif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 8 2005 Win64")
  add_definitions(-DVC2005)
 elseif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 9 2008")
  add_definitions(-DVC2005)
 endif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 6")
 
 if (${CMAKE_GENERATOR} MATCHES ".*Win64")
  set (WIN64 1)
  set (target jniGUIDOEngine64)
 endif (${CMAKE_GENERATOR} MATCHES ".*Win64")

endif(WIN32)

#######################################
# set directories, src and headers.
set (JAVA_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../guidoengine)
set (JNI_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../jni)
set (GUIDO_SRC ${CMAKE_CURRENT_SOURCE_DIR}/../../src)

set (SRC "${JNI_PATH}/*.cpp")				# add source files
set (HEADERS "${JNI_PATH}/*.h")				# add header files
set (INCL ${JNI_INCLUDE_DIRS} ${GUIDO_SRC}/include ${GUIDO_SRC}/tools)

if(APPLE)
#	set (SRC ${SRC} ${JNI_PATH}/macosx/guidoengine_native_paint.mm)
	set (INCL ${INCL} ${GUIDO_SRC}/include ${CMAKE_CURRENT_SOURCE_DIR}/../../macosx/src)
elseif(WIN32)
#	set (SRC ${SRC} ${JNI_PATH}/win32/guidoengine_native_paint.cpp)
	set (INCL ${INCL} ${GUIDO_SRC}/include ${CMAKE_CURRENT_SOURCE_DIR}/../../win32/src)
elseif(UNIX)
	set (SRC ${SRC} )
	set (INCL ${INCL} ${GUIDO_SRC}/include ${CMAKE_CURRENT_SOURCE_DIR}/../../linux/src)
	EXECUTE_PROCESS ( COMMAND "pkg-config"  "--cflags" "cairo"
                OUTPUT_VARIABLE TMP
                ERROR_VARIABLE TMP
        )
	string (STRIP ${TMP} UINCL)
	add_definitions(${UINCL})
else(UNIX)
	message (FATAL_ERROR "Unknown target platform")
endif(APPLE)
#endif(WIN32)

#######################################
# set includes
include_directories( ${INCL})

#######################################
# set library target
file (GLOB JAVA RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${JAVA_PATH}/*.java")
file (GLOB LIBSRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${SRC})
file (GLOB LIBH RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${HEADERS})
if (WIN64)
 set (GUIDO_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../win64)
 find_library (GUIDO GUIDOEngine64 PATHS ${GUIDO_PATH}/Release NO_DEFAULT_PATH)
else(WIN64)
 set (GUIDO_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)
 find_library (GUIDO GUIDOEngine PATHS ${GUIDO_PATH}/Release NO_DEFAULT_PATH)
endif(WIN64)

set (LIBCONTENT ${LIBSRC} ${LIBH})
#set (LINK ${JNI_LIBRARIES} ${GUIDO})
set (LINK  ${GUIDO})

if(WIN32)
	enable_language(RC)
	set(LIBCONTENT ${LIBCONTENT} ../jni/win32/jniGUIDOEngine.rc)
	set(LINK ${LINK} Gdiplus)
elseif(APPLE)
	set(LINK ${LINK}  "-framework Cocoa  -framework CoreFoundation")
endif(WIN32)

add_library(${target} SHARED ${LIBCONTENT})
set_target_properties (${target} PROPERTIES VERSION ${VERSION})
target_link_libraries (${target} ${LINK})

#######################################
# install settings
if(!WIN32)
set (LIBDEST /usr/lib)
if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
  set (LIBDEST /usr/lib64)
  message (STATUS "64 bits system detected : setting install dir to ${LIBDEST}")
endif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")

install ( TARGETS ${target}	LIBRARY DESTINATION ${LIBDEST})
endif(!WIN32)
