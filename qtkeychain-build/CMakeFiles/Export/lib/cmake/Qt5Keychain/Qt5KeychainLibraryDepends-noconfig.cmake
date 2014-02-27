#----------------------------------------------------------------
# Generated CMake target import file for configuration "".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "qt5keychain" for configuration ""
set_property(TARGET qt5keychain APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(qt5keychain PROPERTIES
  IMPORTED_LINK_INTERFACE_LIBRARIES_NOCONFIG "Qt5::Core;/System/Library/Frameworks/CoreFoundation.framework;/System/Library/Frameworks/Security.framework"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libqt5keychain.0.2.0.dylib"
  IMPORTED_SONAME_NOCONFIG "libqt5keychain.0.dylib"
  )

list(APPEND _IMPORT_CHECK_TARGETS qt5keychain )
list(APPEND _IMPORT_CHECK_FILES_FOR_qt5keychain "${_IMPORT_PREFIX}/lib/libqt5keychain.0.2.0.dylib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
