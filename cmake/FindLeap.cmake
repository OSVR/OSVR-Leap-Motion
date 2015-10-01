#[=======================================================================[.rst:
FindLeap
-----------

Find the Leap Motion SDK components.

Imported Targets
^^^^^^^^^^^^^^^^

Several :ref:`imported targets <Imported targets>` are provided
if the SDK is found:

``LEAP::Leap``
  Main library for interacting with Leap Motion API.

Result Variables
^^^^^^^^^^^^^^^^

This module defines the following variables:

``LEAP_FOUND``
  True if the SDK was found, false otherwise.
``LEAP_INCLUDE_DIRS``
  Include directories needed to find Leap headers.
``LEAP_LIBRARIES``
  The list of all Leap Motion libraries.

It is recommended to link against the imported targets, rather than using the
latter two variables above.

Cache Variables
^^^^^^^^^^^^^^^

This module uses the following cache variables:

``LEAP_LIBRARY``
  The location of the Leap Motion API driver library.
``LEAP_INCLUDE_DIR``
  The location of the Leap Motion SDK include directory containing ``Leap.h``.
``LEAP_ROOT_DIR``
  An optional root to start looking for the Leap Motion SDK, if it``s not in its
  conventional location (x86 Program Files directory +
  ``Leap Motion/LeapSDK/``)

The cache variables should not be used by project code.
They may be set by end users to point at Leap SDK components, though the script
can find the current (as of this writing) version of the SDK in its default
location.

#]=======================================================================]


set(LEAP_ROOT_DIR
	"${LEAP_ROOT_DIR}"
	CACHE
	PATH
	"Directory to search for Leap Motion API SDK")

if(WIN32)
    # Test 32/64 bits
        # Test 32/64 bits
    if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
        set(Leap_LIBRARY_PATH_SUFFIX "lib/x64")
    else()
        set(Leap_LIBRARY_PATH_SUFFIX "lib/x86")
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
    
    find_library(LEAP_LIBRARY
        NAMES
        Leap
        PATHS
        "${LEAP_ROOT_DIR}"
        "${_progfiles}/Leap Motion/LeapSDK/"
        PATH_SUFFIXES
        ${Leap_LIBRARY_PATH_SUFFIX})

    if(LEAP_LIBRARY)
        get_filename_component(_libdir "${LEAP_LIBRARY}" DIRECTORY)
    endif()

    find_path(LEAP_INCLUDE_DIR
        NAMES
        Leap.h
        HINTS
        "${_libdir}/../include"
        "${_libdir}/../../include"
        PATHS
        "${LEAP_ROOT_DIR}"
        "${_progfiles}/Leap Motion/LeapSDK/include")
endif() # WIN32

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Leap FOUND_VAR LEAP_FOUND
    REQUIRED_VARS
    LEAP_LIBRARY
    LEAP_INCLUDE_DIR)

if(LEAP_FOUND)
    set(APPEND LEAP_LIBRARIES ${LEAP_LIBRARY})
    set(LEAP_INCLUDE_DIR ${LEAP_INCLUDE_DIR})
    mark_as_advanced(LEAP_ROOT_DIR)
    if(NOT TARGET Leap::Leap)
        add_library(Leap::Leap UNKNOWN IMPORTED)
        set_target_properties(Leap::Leap PROPERTIES
            IMPORTED_LOCATION "${LEAP_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${LEAP_INCLUDE_DIRS}"
            IMPORTED_LINK_INTERFACE_LANGUAGES "C")
    endif()
endif()

mark_as_advanced(LEAP_INCLUDE_DIR
    LEAP_LIBRARY)
