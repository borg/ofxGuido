#######################################
# CMAKE guidolib
#######################################
project(guidolib)
cmake_minimum_required(VERSION 2.6)

set (CMAKE_CONFIGURATION_TYPES Debug Release)
set (target GUIDOEngine)
set (statictarget SGUIDOEngine)
#######################################
# versions management
set (VERSION 1.52)
set (STRVERS "v.1.52")
set (GUIDOFWKVERS C)
set (SOVERS 1)

if(UNIX)
	add_definitions(-Wall -DGCC)
endif(UNIX)


#######################################
# Options disabled by default
option ( IOS 	"Generates project for iOS" off )
option ( DIST 	"Compiles universal binaries (x86_64 i386) on MacOSX" off )
option ( MIDIEXPORT "MIDI export support (requires MidiShareLight)" off)
option ( STATICLIB  "Generates a static library (default for iOS)" off)

if(APPLE)

#######################################
#   iOS support
	if ( IOS )
		message (STATUS "Generates project for iOS - Use -DIOS=no to change.")
		set (CMAKE_XCODE_EFFECTIVE_PLATFORMS "iPhoneOS")
		set (CMAKE_OSX_ARCHITECTURES "armv6 armv7")
		set (CMAKE_OSX_SYSROOT "iphoneos")
		set (libtype STATIC)
	else ()
		message (STATUS "Generates Mac OS project- Use -DIOS=yes to change.")
	 	#######################################
		if (DIST)
			message (STATUS "All architectures will be compiled - Use -DDIST=no to change.")
			set (CMAKE_OSX_ARCHITECTURES "x86_64 i386")
		else ()
			message (STATUS "Only the current architecture will be compiled - Use -DDIST=yes to change.")
		endif ()
	set (CMAKE_OSX_ARCHITECTURES "i386")
	add_definitions(-D_USE_QD_COORDINATES_ -compatibility_version 1.0.0)
	set (CMAKE_C++_FLAGS -mmacosx-version-min=10.5)
	set (CMAKE_LDFLAGS -mmacosx-version-min=10.5)
	endif ()
endif(APPLE)

if(WIN32)
 add_definitions(-DWINVER=0x0400 -DWIN32)
 if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 6")
  add_definitions(-DVC6)
 elseif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 8 2005")
  add_definitions(-DVC2005)
 elseif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 9 2008")
  add_definitions(-DVC2005)
 elseif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 10")
  add_definitions(-DVC2005)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
 elseif(${CMAKE_GENERATOR} STREQUAL "MSYS Makefiles")
  add_definitions(-DGCC)
 endif()

 if (${CMAKE_GENERATOR} MATCHES ".*Win64")
  add_definitions(-DVC2005)
  set (WIN64 1)
  set (target GUIDOEngine64)
  set (statictarget SGUIDOEngine64)
 endif ()

endif(WIN32)
add_definitions(-DGUIDOExport)

#######################################
# set directories, src and headers.
set (GUIDO_PATH ${CMAKE_CURRENT_SOURCE_DIR}/..)
set (SRC_PATH	${GUIDO_PATH}/src)
set (SRCFOLDERS  lib graphic abstract parser tools misc include devices maps)
set (HEADERS_PATH ${SRC_PATH}/include)
if(WIN32)
	set(VGDEV_PATH ${GUIDO_PATH}/win32/src)
elseif(APPLE)
	set(VGDEV_PATH ${GUIDO_PATH}/macosx/src)
elseif(UNIX)
	set(VGDEV_PATH ${GUIDO_PATH}/linux/src)
endif(WIN32)

#######################################
# midi export support
if (MIDIEXPORT STREQUAL "yes")
	message (STATUS "MIDI export will be generated (requires MidiShareLight) - Use -DMIDIEXPORT=no to change.")
	set (SRCFOLDERS  ${SRCFOLDERS} midisharelight)
	add_definitions(-DMIDIEXPORT)
	if (WIN32)
		if (${CMAKE_CL_64})
			set(LINK ${LINK} "${SRC_PATH}/midisharelight/win64/midisharelight64.lib")
		else()
			set(LINK ${LINK} "${SRC_PATH}/midisharelight/win32/midisharelight.lib")
		endif()
	elseif(APPLE)
		set(LINK ${LINK} "-L${SRC_PATH}/midisharelight/macos -lmidisharelight")
	elseif(UNIX)
		set(LINK ${LINK} "-L/usr/local/lib -lmidisharelight")
	endif (WIN32)
else()
	message (STATUS "MIDI export will not be generated - Use -DMIDIEXPORT=yes to change (requires MidiShareLight).")
endif()


foreach(folder ${SRCFOLDERS})
	set (SRC ${SRC} "${SRC_PATH}/${folder}/*.cpp")					# add source files
endforeach(folder)
if(WIN32)
	set (VGDEV_SRC "${VGDEV_PATH}/GDeviceWin32.cpp" "${VGDEV_PATH}/GFontWin32.cpp" "${VGDEV_PATH}/GSystemWin32.cpp"
		"${VGDEV_PATH}/GDeviceWin32GDIPlus.cpp" "${VGDEV_PATH}/GFontWin32GDIPlus.cpp" "${VGDEV_PATH}/GSystemWin32GDIPlus.cpp")
	string (REPLACE .cpp .h VGDEV_H ${VGDEV_SRC})
elseif(APPLE)
	set (VGDEV_SRC "${VGDEV_PATH}/*.cpp")
	string (REPLACE .cpp .h VGDEV_H ${VGDEV_SRC})
elseif(UNIX)
	set (VGDEV_SRC "${VGDEV_PATH}/*Cairo*.cpp")
	string (REPLACE .cpp .h VGDEV_H ${VGDEV_SRC})
	EXECUTE_PROCESS ( COMMAND "pkg-config"  "--cflags" "cairo"
                OUTPUT_VARIABLE TMP
                ERROR_VARIABLE TMP
        )
	string (STRIP ${TMP} UINCL)
	add_definitions(${UINCL})
endif(WIN32)
file (GLOB LIBSRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${SRC} ${VGDEV_SRC})

foreach(folder ${SRCFOLDERS})
	set(HEADERS ${HEADERS} "${SRC_PATH}/${folder}/*.h")		# add header files
endforeach(folder)
file (GLOB LIBH RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${HEADERS} ${VGDEV_H})
file (GLOB PUBLICHEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${HEADERS_PATH}/*.h" "${VGDEV_H}" "${SRC_PATH}/devices/*H")

foreach(folder ${SRCFOLDERS})
	set(INCL ${INCL} "${SRC_PATH}/${folder}")			# add include folders
endforeach(folder)
set(INCL ${INCL} "${VGDEV_PATH}")						# add include folders

#######################################
# set includes
include_directories( ${INCL})
set_source_files_properties (${INCL} PROPERTIES HEADER_FILE_ONLY TRUE)

#######################################
# set library target
if(WIN32)
	enable_language(RC)
	set(LIBCONTENT ${LIBSRC} ${LIBH} ${GUIDO_PATH}/win32/GUIDOEngine.rc)
	set(LINK ${LINK} "msimg32.lib" "gdiplus.lib")
else(WIN32)
	set(LIBCONTENT ${LIBSRC} ${LIBH})
endif(WIN32)

if(APPLE)
	set(LINK ${LINK} "-framework Carbon -compatibility_version 1.0.0")
elseif (UNIX)
      EXECUTE_PROCESS ( COMMAND "pkg-config"  "--libs" "cairo"
                OUTPUT_VARIABLE TMP
                ERROR_VARIABLE TMP
        )
	string (STRIP ${TMP} CAIRO)
	set(LINK ${LINK} ${CAIRO})
endif(APPLE)

#######################################
# library generation
if (IOS STREQUAL "yes")
	add_library(${target} STATIC ${LIBCONTENT})

else()

	add_library(${target} SHARED ${LIBCONTENT})
	set_target_properties (${target} PROPERTIES
				FRAMEWORK TRUE
				FRAMEWORK_VERSION ${GUIDOFWKVERS}
				PUBLIC_HEADER "${PUBLICHEADERS}"
				DEFINE_SYMBOL LIBMUSICXML_EXPORTS
				VERSION ${VERSION}
				SOVERSION ${SOVERS}
				MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${STRVERS}
				MACOSX_FRAMEWORK_BUNDLE_VERSION ${VERSION}
				MACOSX_FRAMEWORK_BUNDLE_VERSION "A")

	if (STATICLIB)
		message (STATUS "Static Guido lib will be generated - Use -DSTATICLIB=no to change.")
		add_library(${statictarget} STATIC ${LIBCONTENT})
	else ()
		message (STATUS "Static Guido lib will not be generated - Use -DSTATICLIB=yes to change.")
	endif ()

	set (LIBDEST /usr/lib)
	target_link_libraries (${target} ${LINK})
endif()

if(WIN32)
	set_target_properties (${target} PROPERTIES
		DEBUG_OUTPUT_NAME ${target}D
		RELEASE_OUTPUT_NAME ${target})
endif()

#######################################
# install setup
if(APPLE)
set(FRMWK  FRAMEWORK DESTINATION "/Library/Frameworks" CONFIGURATIONS Release)
endif(APPLE)

if (NOT WIN32 AND NOT IOS)
 if (DEFINED LIBINSTALL)
	set(LIBPATH ${LIBINSTALL})
 else (DEFINED LIBINSTALL)
	set(LIBPATH "/usr/lib")
 endif (DEFINED LIBINSTALL)
 message (STATUS "Library install path set to ${LIBPATH} - Use -DLIBINSTALL=path to change.")

install ( TARGETS ${target}
    	LIBRARY DESTINATION ${LIBPATH}
    	${FRMWK}
    	PUBLIC_HEADER DESTINATION /usr/include/guido
)
endif()


