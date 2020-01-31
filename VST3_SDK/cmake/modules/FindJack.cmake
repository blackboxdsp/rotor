
# - Try to find Jack Audio Libraries
# libjack; libjackserver;
# Once done this will define
#  LIBJACK_FOUND - System has libjack.so
#  LIBJACK_INCLUDE_DIRS - The jack.h include directories
#  LIBJACK_LIBRARIES - The libraries needed to use jack

# Install jack on Ubuntu: apt-get install libjack-jackd2-dev
# Add user to audio group: usermod -a -G audio <yourUserName>

if(SMTG_WIN)
    if(NOT SMTG_JACK_SDK_PATH)
        if(EXISTS "${SDK_ROOT}/../external.jackaudio/lib")
            set(SMTG_JACK_SDK_PATH "${SDK_ROOT}/../external.jackaudio" CACHE PATH "Here you can define where to find the JACK SDK")
        elseif(EXISTS "${SDK_ROOT}/external.jackaudio/lib")
            set(SMTG_JACK_SDK_PATH "${SDK_ROOT}/external.jackaudio" CACHE PATH "Here you can define where to find the JACK SDK")
        endif()
    else()
        if(NOT IS_ABSOLUTE ${SMTG_JACK_SDK_PATH})
            get_filename_component(SMTG_JACK_SDK_PATH "${SDK_ROOT}/${SMTG_JACK_SDK_PATH}" ABSOLUTE)
        endif()
        if(NOT EXISTS "${SMTG_JACK_SDK_PATH}/include/jack/jack.h")
            message(FATAL_ERROR "SMTG_JACK_SDK_PATH is set but does not point to an expected location!")
        endif()
    endif() 
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(EXTENSION 64) 
        set(LOCAL_JACK_LIB "${SMTG_JACK_SDK_PATH}/lib/win64")
    else ()
        set(LOCAL_JACK_LIB "${SMTG_JACK_SDK_PATH}/lib/win32")
    endif()
    
    set(PROGRAM_FILE_JACK_INCLUDE "C:/Program Files (x86)/Jack/includes")
    set(PROGRAM_FILE_JACK_LIB "C:/Program Files (x86)/Jack/lib")
    set(LOCAL_JACK_INCLUDE "${SMTG_JACK_SDK_PATH}/include")
endif()

find_path(LIBJACK_INCLUDE_DIR
    NAMES
        jack/jack.h
    PATHS
        /usr/include
        /usr/local/include
        /opt/local/include
        ${PROGRAM_FILE_JACK_INCLUDE}
        ${LOCAL_JACK_INCLUDE}
)

find_library(LIBJACK_libjack_LIBRARY
    NAMES
        libjack${EXTENSION} jack
    PATHS
        /usr/lib
        /usr/local/lib
        /opt/local/lib
        ${PROGRAM_FILE_JACK_LIB}
        ${LOCAL_JACK_LIB}
)

find_library(LIBJACK_libjackserver_LIBRARY
    NAMES
        libjackserver${EXTENSION} jackserver
    PATHS
        /usr/lib
        /usr/local/lib
        /opt/local/lib
        ${PROGRAM_FILE_JACK_LIB}
        ${LOCAL_JACK_LIB}   
)

set(LIBJACK_LIBRARIES ${LIBJACK_libjack_LIBRARY})
set(LIBJACK_LIBRARY ${LIBJACK_LIBRARIES})
set(LIBJACK_INCLUDE_DIRS ${LIBJACK_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBJACK_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(LIBJACK  DEFAULT_MSG
                                  LIBJACK_LIBRARY LIBJACK_INCLUDE_DIR)

if(LIBJACK_FOUND)
    message(STATUS "LIBJACK_FOUND at: " ${LIBJACK_INCLUDE_DIR})
elseif(SMTG_LINUX)
    message(STATUS "LIBJACK_FOUND is not set. If you need it, install it (for example with: apt-get install libjack-jackd2-dev) or check Jack Audio from http://www.jackaudio.org!")
else()
    message(STATUS "LIBJACK_FOUND is not set. If you need it, please install Jack Audio from http://www.jackaudio.org!")
endif()
