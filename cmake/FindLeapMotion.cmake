#[=======================================================================[.rst:
FindLeapMotion
--------------

Find the Leap Motion SDK components.

Imported Targets
^^^^^^^^^^^^^^^^

Several :ref:`imported targets <Imported targets>` are provided
if the SDK is found:

``LeapMotion::Leap``
  Main library for interacting with Leap Motion API.

Result Variables
^^^^^^^^^^^^^^^^

This module defines the following variables:

``LEAPMOTION_FOUND``
  True if the SDK was found, false otherwise.
``LEAPMOTION_INCLUDE_DIRS``
  Include directories needed to find Leap headers.
``LEAPMOTION_LIBRARIES``
  The list of all Leap Motion libraries.

It is recommended to link against the imported targets, rather than using the
latter two variables above.

Cache Variables
^^^^^^^^^^^^^^^

This module uses the following cache variables:

``LEAPMOTION_LEAP_LIBRARY_RELEASE``
  The location of the Leap Motion API driver library, release build.
``LEAPMOTION_LEAP_LIBRARY_DEBUG``
  The location of the Leap Motion API driver library, debug build.
``LEAPMOTION_INCLUDE_DIR``
  The location of the Leap Motion SDK include directory containing ``Leap.h``.
``LEAPMOTION_ROOT_DIR``
  An optional root to start looking for the Leap Motion SDK, if it``s not specified
  with the ``LEAP_SDK`` environment variable or in a so-called conventional
  location (x86 Program Files directory + ``Leap Motion/LeapSDK/`` on Windows)

The cache variables should not be used by project code.
They may be set by end users to point at Leap SDK components, though the script
can find the current (as of this writing) version of the SDK in its default
location.

#]=======================================================================]


set(LEAPMOTION_ROOT_DIR
    "${LEAPMOTION_ROOT_DIR}"
    CACHE
    PATH
    "Directory to search for Leap Motion API SDK")

# Their docs suggest setting a LEAP_SDK environment variable. We'll use it if we can find it.
if(NOT LEAPMOTION_ROOT_DIR)
    get_filename_component(_leapmotion_env_var "$ENV{LEAP_SDK}" ABSOLUTE)
    if(NOT "${_leapmotion_env_var}" STREQUAL "")
        set(LEAPMOTION_ROOT_DIR "${_leapmotion_env_var}")
    endif()
endif()

if(LEAPMOTION_ROOT_DIR AND EXISTS "${LEAPMOTION_ROOT_DIR}")
    if(EXISTS "${LEAPMOTION_ROOT_DIR}/LeapSDK")
        # Drop right away into LeapSDK if we can.
        set(LEAPMOTION_ROOT_DIR "${LEAPMOTION_ROOT_DIR}/LeapSDK")
    endif()
endif()

# TODO: handle the libc++ vs libstdc++ variants on Mac OS X
set(LEAPMOTION_LIBRARY_PATH_SUFFIX lib)

if(WIN32)
    # Test 32/64 bits
    if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
        set(LEAPMOTION_LIBRARY_PATH_SUFFIX "lib/x64")
    else()
        set(LEAPMOTION_LIBRARY_PATH_SUFFIX "lib/x86")
    endif()

    # Get desired program files directory
    set(_PF86_ENV "ProgramFiles(x86)")
    set(_PF86 $ENV{${_PF86_ENV}})

    if(NOT "$ENV{${_PF86}}" STREQUAL "")
        # 32-bit dir: only set on win64
        file(TO_CMAKE_PATH "$ENV{_PF86}" _progfiles)
    else()
        # 32-bit dir on win32
        file(TO_CMAKE_PATH "$ENV{ProgramFiles}" _progfiles)
    endif()
endif()

# Try to find the header first, since it's simplest (no 32-64 nonsense in the path)
# and can provide a root directory for finding the rest of the SDK.
find_path(LEAPMOTION_INCLUDE_DIR
    NAMES
    Leap.h
    PATHS
    "${LEAPMOTION_ROOT_DIR}"
    "${_progfiles}/Leap Motion/LeapSDK/include"
    PATH_SUFFIXES
    include
    LeapSDK
    LeapSDK/include)

if(LEAPMOTION_INCLUDE_DIR)
    get_filename_component(_leapmotion_root "${LEAPMOTION_INCLUDE_DIR}" DIRECTORY)
endif()

find_library(LEAPMOTION_LEAP_LIBRARY_RELEASE
    NAMES
    Leap
    libLeap
    HINTS
    "${_leapmotion_root}"
    PATHS
    "${LEAPMOTION_ROOT_DIR}"
    "${_progfiles}/Leap Motion/LeapSDK/"
    PATH_SUFFIXES
    ${LEAPMOTION_LIBRARY_PATH_SUFFIX})

# Debug build came with some earlier SDKs but not 2.3.1
find_library(LEAPMOTION_LEAP_LIBRARY_DEBUG
    NAMES
    Leapd
    libLeapd
    HINTS
    "${_leapmotion_root}"
    PATHS
    "${LEAPMOTION_ROOT_DIR}"
    "${_progfiles}/Leap Motion/LeapSDK/"
    PATH_SUFFIXES
    ${LEAPMOTION_LIBRARY_PATH_SUFFIX})

include(SelectLibraryConfigurations)
select_library_configurations(LEAPMOTION_LEAP)

if(WIN32)
    if(LEAPMOTION_LEAP_LIBRARY_RELEASE)
        get_filename_component(_leapmotion_libdir "${LEAPMOTION_LEAP_LIBRARY_RELEASE}" DIRECTORY)
        find_file(LEAPMOTION_LEAP_RUNTIME_LIBRARY_RELEASE
            NAMES
            Leap.dll
            HINTS
            "${_leapmotion_libdir}")
    endif()
    if(LEAPMOTION_LEAP_LIBRARY_DEBUG)
        get_filename_component(_leapmotion_libdir "${LEAPMOTION_LEAP_LIBRARY_DEBUG}" DIRECTORY)
        find_file(LEAPMOTION_LEAP_RUNTIME_LIBRARY_DEBUG
            NAMES
            Leapd.dll
            HINTS
            "${_leapmotion_libdir}")
    endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LeapMotion FOUND_VAR LEAPMOTION_FOUND
    REQUIRED_VARS
    LEAPMOTION_LEAP_LIBRARY_RELEASE
    LEAPMOTION_INCLUDE_DIR)

if(LEAPMOTION_FOUND)
    set(LEAPMOTION_LIBRARIES ${LEAPMOTION_LEAP_LIBRARIES})
    set(LEAPMOTION_INCLUDE_DIRS "${LEAPMOTION_INCLUDE_DIR}")
    mark_as_advanced(LEAPMOTION_ROOT_DIR)
    if(NOT TARGET LeapMotion::Leap)
        set(_leapmotion_dll OFF)
        if(NOT WIN32)
            set(_leapmotion_libtype SHARED)
        elseif(WIN32 AND LEAPMOTION_LEAP_RUNTIME_LIBRARY_RELEASE)
            set(_leapmotion_dll ON)
            set(_leapmotion_libtype SHARED)
        else()
            set(_leapmotion_libtype UNKNOWN)
        endif()
        add_library(LeapMotion::Leap ${_leapmotion_libtype} IMPORTED)
        set_target_properties(LeapMotion::Leap PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${LEAPMOTION_INCLUDE_DIRS}"
            IMPORTED_LINK_INTERFACE_LANGUAGES "C++")
        if(_leapmotion_dll)
            set_target_properties(LeapMotion::Leap PROPERTIES
                IMPORTED_LOCATION "${LEAPMOTION_LEAP_RUNTIME_LIBRARY_RELEASE}"
                IMPORTED_IMPLIB "${LEAPMOTION_LEAP_LIBRARY_RELEASE}")
        else()
            set_target_properties(LeapMotion::Leap PROPERTIES
                IMPORTED_LOCATION "${LEAPMOTION_LEAP_LIBRARY_RELEASE}")
        endif()

        # TODO decide if we leave this in or not
        if(LEAPMOTION_LEAP_LIBRARY_DEBUG)
            if(_leapmotion_dll)
                if(LEAPMOTION_LEAP_RUNTIME_LIBRARY_DEBUG)
                    # Only add the debug variant if we have both parts lest we confuse CMake.
                    set_target_properties(LeapMotion::Leap PROPERTIES
                        IMPORTED_LOCATION_DEBUG "${LEAPMOTION_LEAP_RUNTIME_LIBRARY_DEBUG}"
                        IMPORTED_IMPLIB_DEBUG "${LEAPMOTION_LEAP_LIBRARY_DEBUG}")
                endif()
            else()
                set_target_properties(LeapMotion::Leap PROPERTIES
                IMPORTED_LOCATION_DEBUG "${LEAPMOTION_LEAP_LIBRARY_DEBUG}")
            endif()

        endif()
    endif()
endif()

mark_as_advanced(LEAPMOTION_INCLUDE_DIR
    LEAPMOTION_LEAP_LIBRARY_RELEASE
    LEAPMOTION_LEAP_LIBRARY_DEBUG
    LEAPMOTION_LEAP_RUNTIME_LIBRARY_RELEASE
    LEAPMOTION_LEAP_RUNTIME_LIBRARY_DEBUG)
