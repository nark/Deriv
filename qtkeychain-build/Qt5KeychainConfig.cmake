# - Config file for the QtKeychain package
# It defines the following variables
#  QTKEYCHAIN_INCLUDE_DIRS - include directories for QtKeychain
#  QTKEYCHAIN_LIBRARIES    - libraries to link against

# Compute paths
get_filename_component(QTKEYCHAIN_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
if(EXISTS "${QTKEYCHAIN_CMAKE_DIR}/CMakeCache.txt")
   # In build tree
   include("${QTKEYCHAIN_CMAKE_DIR}/Qt5KeychainBuildTreeSettings.cmake")
else()
   set(QTKEYCHAIN_INCLUDE_DIRS "/usr/local/include")
endif()

# Our library dependencies (contains definitions for IMPORTED targets)
include("${QTKEYCHAIN_CMAKE_DIR}/Qt5KeychainLibraryDepends.cmake")

# These are IMPORTED targets created by FooBarLibraryDepends.cmake
set(QTKEYCHAIN_LIBRARIES "qt5keychain")

set(QTKEYCHAIN_FOUND TRUE)
