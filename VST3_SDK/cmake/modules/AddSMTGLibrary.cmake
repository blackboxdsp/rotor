
include(CMakePrintHelpers)
include(UniversalBinary)

# Returns the windows architecture.
#
# This name will be used as a folder name inside the plug-in package.
# The variable WIN_ARCHITECTURE_NAME will be set.
function(smtg_set_vst_win_architecture_name)
    if(SMTG_WIN AND CMAKE_SIZEOF_VOID_P EQUAL 8)
        if(${CMAKE_GENERATOR} MATCHES "ARM")
            set(WIN_ARCHITECTURE_NAME "arm_64-win")
        else()
            set(WIN_ARCHITECTURE_NAME "x86_64-win")
        endif()
    else()
        if(${CMAKE_GENERATOR} MATCHES "ARM")
            set(WIN_ARCHITECTURE_NAME "arm-win")
        else()
            set(WIN_ARCHITECTURE_NAME "x86-win")
        endif()
    endif()

    set_target_properties(${target}
        PROPERTIES
            SMTG_WIN_ARCHITECTURE_NAME ${WIN_ARCHITECTURE_NAME}
    )
endfunction()

# Prints out all relevant properties of a target for debugging.
#
# @param target The target whose properties will be printed.
function(smtg_dump_plugin_package_variables target)
    cmake_print_properties(
        TARGETS
            ${target}
        PROPERTIES
            SMTG_PLUGIN_PACKAGE_NAME
            SMTG_PLUGIN_BINARY_DIR
            SMTG_PLUGIN_EXTENSION
            LIBRARY_OUTPUT_DIRECTORY
            SMTG_PLUGIN_PACKAGE_CONTENTS
            SMTG_PLUGIN_PACKAGE_RESOURCES
            SMTG_PLUGIN_PACKAGE_SNAPSHOTS
            SMTG_PLUGIN_USER_DEFINED_TARGET
            SMTG_WIN_ARCHITECTURE_NAME
    )
endfunction()

# Strips all symbols on linux
#
# @param target The target whose build symbols will be stripped
function (smtg_strip_symbols target)
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND_STRIP} $ENV{CROSS_COMPILE}strip --strip-debug --strip-unneeded $<TARGET_FILE:${target}>
    )
endfunction()

#! smtg_strip_symbols : Strips all symbols on and creates debug file on Linux 
#
# @param target The target whose build symbols will be stripped
function (smtg_strip_symbols_with_dbg target)
    add_custom_command(TARGET ${target} POST_BUILD
        # Create a target.so.dbg file with debug information
        COMMAND ${CMAKE_COMMAND_OBJECT_COPY} $ENV{CROSS_COMPILE}objcopy --only-keep-debug $<TARGET_FILE:${target}> $<TARGET_FILE:${target}>.dbg
        COMMAND ${CMAKE_COMMAND_STRIP} $ENV{CROSS_COMPILE}strip --strip-debug --strip-unneeded $<TARGET_FILE:${target}>
        COMMAND ${CMAKE_COMMAND_OBJECT_COPY} $ENV{CROSS_COMPILE}objcopy --add-gnu-debuglink=$<TARGET_FILE:${target}>.dbg $<TARGET_FILE:${target}>
    )
endfunction()

# Creates a symlink to the targets output resp plug-in.
#
# A symlink to the output plug-in will be created as a post build step.
#
# @param target The target whose output is the symlink's source.
function (smtg_create_link_to_plugin target)
    if(${SMTG_PLUGIN_TARGET_PATH} STREQUAL "")
        message(FATAL_ERROR "Define a proper value for SMTG_PLUGIN_TARGET_PATH")
    endif()

    get_target_property(TARGET_SOURCE       ${target} SMTG_PLUGIN_PACKAGE_PATH)
    get_target_property(TARGET_DESTINATION  ${target} SMTG_PLUGIN_USER_DEFINED_TARGET)
    if(SMTG_WIN)
        get_target_property(PLUGIN_BINARY_DIR   ${target} SMTG_PLUGIN_BINARY_DIR)
        get_target_property(PLUGIN_PACKAGE_NAME ${target} SMTG_PLUGIN_PACKAGE_NAME)

        file(TO_NATIVE_PATH "${TARGET_DESTINATION}/${PLUGIN_PACKAGE_NAME}" SRC_NATIVE_PATH)
        file(TO_NATIVE_PATH "${PLUGIN_BINARY_DIR}/Debug/${PLUGIN_PACKAGE_NAME}" TARGET_DESTINATION_DEBUG)
        file(TO_NATIVE_PATH "${PLUGIN_BINARY_DIR}/Release/${PLUGIN_PACKAGE_NAME}" TARGET_DESTINATION_RELEASE)

        add_custom_command(
            TARGET ${target} POST_BUILD
            COMMAND del "${SRC_NATIVE_PATH}"
            COMMAND mklink 
                ${SRC_NATIVE_PATH}
                $<$<CONFIG:Debug>:${TARGET_DESTINATION_DEBUG}>
                $<$<CONFIG:Release>:${TARGET_DESTINATION_RELEASE}>
        )
    else()
        add_custom_command(
            TARGET ${target} POST_BUILD
            COMMAND mkdir -p "${TARGET_DESTINATION}"
            COMMAND ln -svfF "${TARGET_SOURCE}" "${TARGET_DESTINATION}"
        )
    endif()
endfunction()

set(SMTG_DESKTOP_INI_PATH ${CMAKE_CURRENT_LIST_DIR}/../templates/desktop.ini.in)
get_directory_property(hasParent PARENT_DIRECTORY)
if(hasParent)
    set(SMTG_DESKTOP_INI_PATH ${SMTG_DESKTOP_INI_PATH} PARENT_SCOPE)
endif()
# Customizes folder icon on windows
#
# Customizes folder icon on windows by copying desktop.ini and PlugIn.ico into the package.
#
# @param target The target whose folder icon will be customized.
function(smtg_add_folder_icon target icon)
    get_target_property(PLUGIN_PACKAGE_PATH ${target} SMTG_PLUGIN_PACKAGE_PATH)
    add_custom_command(TARGET ${target}
        COMMENT "Copy PlugIn.ico and desktop.ini and change their attributes."   
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            ${icon}
            ${PLUGIN_PACKAGE_PATH}/PlugIn.ico
        COMMAND ${CMAKE_COMMAND} -E copy
            ${SMTG_DESKTOP_INI_PATH}
            ${PLUGIN_PACKAGE_PATH}/desktop.ini
        COMMAND attrib +s ${PLUGIN_PACKAGE_PATH}/desktop.ini
        COMMAND attrib +s ${PLUGIN_PACKAGE_PATH}/PlugIn.ico
        COMMAND attrib +s ${PLUGIN_PACKAGE_PATH}
    )
endfunction()

# Adds the plug-in's main entry point.
#
# The variable public_sdk_SOURCE_DIR needs to be specified.
#
# @param target The target to which the main entry point will be added.
function(smtg_add_library_main target)
    if(public_sdk_SOURCE_DIR)
        if(SMTG_MAC)
            target_sources (${target} 
                PRIVATE 
                    ${public_sdk_SOURCE_DIR}/source/main/macmain.cpp
            )
            smtg_set_exported_symbols(${target} ${public_sdk_SOURCE_DIR}/source/main/macexport.exp)
        elseif(SMTG_WIN)
            target_sources (${target} 
                PRIVATE 
                    ${public_sdk_SOURCE_DIR}/source/main/dllmain.cpp
            )
        elseif(SMTG_LINUX)
            target_sources (${target} 
                PRIVATE 
                    ${public_sdk_SOURCE_DIR}/source/main/linuxmain.cpp
            )
        endif()
    endif()
endfunction()

# Returns the linux architecture name.
#
# This name will be used as a folder name inside the plug-in package.
# The variable LINUX_ARCHITECTURE_NAME will be set.
function(smtg_get_linux_architecture_name)
    if(ANDROID)
        set(ARCHITECTURE ${CMAKE_ANDROID_ARCH_ABI})
        set(LINUX_ARCHITECTURE_NAME ${ARCHITECTURE}-android PARENT_SCOPE)
    else()
        # Check if we are cross-compiling for a different architecture
        if(NOT $ENV{CROSS_COMPILE} STREQUAL "")
            string(REGEX MATCH "^[A-Z|a-z|0-9|_]*-" CROSS_COMPILE_PREFIX $ENV{CROSS_COMPILE})
            # Strip trailing - from regex result
            string(LENGTH ${CROSS_COMPILE_PREFIX} CROSS_COMPILE_PREFIX_LEN)
            math(EXPR CROSS_COMPILE_PREFIX_LEN "${CROSS_COMPILE_PREFIX_LEN} -1")
            string(SUBSTRING ${CROSS_COMPILE_PREFIX} 0 ${CROSS_COMPILE_PREFIX_LEN} ARCHITECTURE)
        else()
            EXECUTE_PROCESS(
                COMMAND uname -m
                COMMAND tr -d '\n'
                OUTPUT_VARIABLE ARCHITECTURE
            )
        endif()
        set(LINUX_ARCHITECTURE_NAME ${ARCHITECTURE}-linux PARENT_SCOPE)
    endif()
endfunction()

# Prepare the target to build a plug-in package.
#
# @param target The target whose output will be put into a package.
# @param extension The package's extension
function(smtg_make_plugin_package target extension)
    set(pkg_extension ${extension})
    if(${extension} STREQUAL "ski")
        if(SMTG_WIN)
            set(extension dll)
            if(SMTG_CREATE_BUNDLE_FOR_WINDOWS)
                set(pkg_extension bundle)
            else()
                set(pkg_extension ${extension})
            endif()
        elseif(SMTG_MAC)
            set(extension bundle)
        endif()
    endif()
    string(TOUPPER ${extension} PLUGIN_EXTENSION_UPPER)

    if(SMTG_CUSTOM_BINARY_LOCATION)
        set(SMTG_PLUGIN_BINARY_LOCATION ${SMTG_CUSTOM_BINARY_LOCATION})
    else()
        set(SMTG_PLUGIN_BINARY_LOCATION ${CMAKE_BINARY_DIR})
    endif()

    set_target_properties(${target} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY        ${SMTG_PLUGIN_BINARY_LOCATION}/${PLUGIN_EXTENSION_UPPER}
        SMTG_PLUGIN_BINARY_DIR          ${SMTG_PLUGIN_BINARY_LOCATION}/${PLUGIN_EXTENSION_UPPER}
        SMTG_PLUGIN_EXTENSION           ${extension}
        SMTG_PLUGIN_PACKAGE_NAME        ${target}.${pkg_extension}
        SMTG_PLUGIN_PACKAGE_CONTENTS    Contents
        SMTG_PLUGIN_PACKAGE_RESOURCES   Contents/Resources
        SMTG_PLUGIN_PACKAGE_SNAPSHOTS   Snapshots
        SMTG_PLUGIN_USER_DEFINED_TARGET ${SMTG_PLUGIN_TARGET_PATH}/${PLUGIN_PACKAGE_NAME}
    )

    get_target_property(PLUGIN_BINARY_DIR   ${target} SMTG_PLUGIN_BINARY_DIR)
    get_target_property(PLUGIN_EXTENSION    ${target} SMTG_PLUGIN_EXTENSION)
    get_target_property(PLUGIN_PACKAGE_NAME ${target} SMTG_PLUGIN_PACKAGE_NAME)

    smtg_add_library_main(${target})

    if(SMTG_MAC)
        set_target_properties(${target} PROPERTIES
            BUNDLE TRUE
        )
        if(XCODE)
            set_target_properties(${target} PROPERTIES 
                XCODE_ATTRIBUTE_GENERATE_PKGINFO_FILE   YES
                XCODE_ATTRIBUTE_WRAPPER_EXTENSION       ${PLUGIN_EXTENSION}
                SMTG_PLUGIN_PACKAGE_PATH      
                    ${PLUGIN_BINARY_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/${PLUGIN_PACKAGE_NAME}
            )
        else()
            set_target_properties(${target} PROPERTIES 
                BUNDLE_EXTENSION            ${PLUGIN_EXTENSION}
                LIBRARY_OUTPUT_DIRECTORY    ${PLUGIN_BINARY_DIR}/${CMAKE_BUILD_TYPE}
                SMTG_PLUGIN_PACKAGE_PATH    ${PLUGIN_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PLUGIN_PACKAGE_NAME}
            )
        endif()

        target_link_libraries(${target} PRIVATE "-framework CoreFoundation")
        smtg_setup_universal_binary(${target})

    elseif(SMTG_WIN)
        if(SMTG_CUSTOM_BINARY_LOCATION)
            set(PLUGIN_PACKAGE_PATH ${PLUGIN_BINARY_DIR}/${PLUGIN_PACKAGE_NAME})
        else()
            set(PLUGIN_PACKAGE_PATH ${PLUGIN_BINARY_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/${PLUGIN_PACKAGE_NAME})
        endif()
        set_target_properties(${target} PROPERTIES 
            SUFFIX                      .${PLUGIN_EXTENSION}
            LINK_FLAGS                  /EXPORT:GetPluginFactory
            SMTG_PLUGIN_PACKAGE_PATH    ${PLUGIN_PACKAGE_PATH}
        )
        
        # In order not to have the PDB inside the plug-in package in release builds, we specify a different location.
        if(CMAKE_SIZEOF_VOID_P EQUAL 4)
            set(WIN_PDB WIN_PDB32)
        else()
            set(WIN_PDB WIN_PDB64)
        endif()
        set_target_properties(${target} PROPERTIES
            PDB_OUTPUT_DIRECTORY        ${PROJECT_BINARY_DIR}/${WIN_PDB}
        )

        # Create Bundle on Windows
        if(SMTG_CREATE_BUNDLE_FOR_WINDOWS)
            get_target_property(WIN_ARCHITECTURE_NAME ${target} SMTG_WIN_ARCHITECTURE_NAME)

            # When using LIBRARY_OUTPUT_DIRECTORY, cmake adds another /Debug resp. /Release folder at the end of the path.
            # In order to prevent cmake from doing that we set LIBRARY_OUTPUT_DIRECTORY_DEBUG and LIBRARY_OUTPUT_DIRECTORY_RELEASE
            # (or LIBRARY_OUTPUT_DIRECTORY_${CONFIG_UPPERCASE}) directly.
            get_target_property(PLUGIN_PACKAGE_CONTENTS ${target} SMTG_PLUGIN_PACKAGE_CONTENTS)
            foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
                string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
                if(SMTG_CUSTOM_BINARY_LOCATION)
                    set_target_properties(${target} PROPERTIES 
                        LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER}
                            ${PLUGIN_BINARY_DIR}/${PLUGIN_PACKAGE_NAME}/${PLUGIN_PACKAGE_CONTENTS}/${WIN_ARCHITECTURE_NAME}
                        )
                else()
                    set_target_properties(${target} PROPERTIES 
                        LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER}
                            ${PLUGIN_BINARY_DIR}/${OUTPUTCONFIG}/${PLUGIN_PACKAGE_NAME}/${PLUGIN_PACKAGE_CONTENTS}/${WIN_ARCHITECTURE_NAME}
                        )
                endif()
            endforeach()
            if(EXISTS ${SMTG_PACKAGE_ICON_PATH})
                smtg_add_folder_icon(${target} ${SMTG_PACKAGE_ICON_PATH})
            endif()
        endif()
        # Disable warning LNK4221: "This object file does not define any previously undefined public symbols...".
        # Enable "Generate Debug Information" in release config by setting "/Zi" and "/DEBUG" flags.
        if(MSVC)
            target_compile_options(${target} 
                PRIVATE 
                    /wd4221
                    $<$<CONFIG:RELEASE>:/Zi>
            )
            set_property(TARGET ${target} 
                APPEND PROPERTY 
                    LINK_FLAGS_RELEASE /DEBUG
            )
        endif()
    elseif(SMTG_LINUX)
        smtg_get_linux_architecture_name() # Sets var LINUX_ARCHITECTURE_NAME
        message(STATUS "Linux architecture name is ${LINUX_ARCHITECTURE_NAME}.")

        get_target_property(PLUGIN_PACKAGE_CONTENTS ${target} SMTG_PLUGIN_PACKAGE_CONTENTS)
        set_target_properties(${target} PROPERTIES
            PREFIX                   ""
            LIBRARY_OUTPUT_DIRECTORY ${PLUGIN_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PLUGIN_PACKAGE_NAME}/${PLUGIN_PACKAGE_CONTENTS}/${LINUX_ARCHITECTURE_NAME}
            SMTG_PLUGIN_PACKAGE_PATH ${PLUGIN_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PLUGIN_PACKAGE_NAME}
        )

        # Strip symbols in Release config
        if(${CMAKE_BUILD_TYPE} MATCHES Release)
            smtg_strip_symbols(${target})
        elseif(${CMAKE_BUILD_TYPE} MATCHES RelWithDebInfo)
            smtg_strip_symbols_with_dbg(${target})
        endif()
    endif()
endfunction()

# Adds a resource for a target 
#
# The resource which gets copied into the targets Resource Bundle directory.
#
# @param target cmake target
# @param input_file resource file
# @param ARGV2 destination subfolder
function(smtg_add_plugin_resource target input_file)
    if(SMTG_LINUX OR (SMTG_WIN AND SMTG_CREATE_BUNDLE_FOR_WINDOWS))
        get_target_property(PLUGIN_PACKAGE_PATH ${target} SMTG_PLUGIN_PACKAGE_PATH)
        get_target_property(PLUGIN_PACKAGE_RESOURCES ${target} SMTG_PLUGIN_PACKAGE_RESOURCES)
        set(destination_folder "${PLUGIN_PACKAGE_PATH}/${PLUGIN_PACKAGE_RESOURCES}")
        if(ARGV2)
            set(destination_folder "${destination_folder}/${ARGV2}")
        endif()
        if(NOT EXISTS ${destination_folder})
            add_custom_command(TARGET ${target} PRE_LINK
              COMMAND ${CMAKE_COMMAND} -E make_directory
              "${destination_folder}"
            )
        endif()
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_LIST_DIR}/${input_file}
            ${destination_folder}
        )
    elseif(SMTG_MAC)
        target_sources(${target} PRIVATE ${input_file})
        set(destination_folder "Resources")
        if(ARGV2)
            set(destination_folder "${destination_folder}/${ARGV2}")
        endif()

        set_source_files_properties(${input_file} 
            PROPERTIES 
            MACOSX_PACKAGE_LOCATION "${destination_folder}"
        )
    endif()
endfunction()

# Adds a snapshot for a target.
#
# Adds a snapshot for a target which gets copied into the targets Snapshot Bundle directory.
#
# @param target The target to which the snapshot will be added
# @param snapshot The snapshot to be added.
function(smtg_add_plugin_snapshot target snapshot)
    get_target_property(PLUGIN_PACKAGE_SNAPSHOTS ${target} SMTG_PLUGIN_PACKAGE_SNAPSHOTS)
    smtg_add_plugin_resource (${target} ${snapshot} ${PLUGIN_PACKAGE_SNAPSHOTS}) 
endfunction()

# Adds multiple resources to target.
#
# Usage:
#  smtg_add_vst3_resources (target
#    RESOURCES
#      bitmap0.png
#      bitmap1.png
#    OUTPUT_SUBDIRECTORY
#      Graphics
#  )
# This adds both bitmaps to <Bundle>/Resources/Graphics
#
# @param target The target to which the resources will be added. 
function(smtg_add_vst3_resources target)
    cmake_parse_arguments(
        PARSED_ARGS # Prefix of output variables e.g. PARSED_ARGS_RESOURCES
        ""          # List of names for boolean arguments
        "OUTPUT_SUBDIRECTORY" # List of names for mono-valued arguments
        "RESOURCES" # List of names for multi-valued arguments resp. lists
        ${ARGN}     # Arguments of the function to parse
    )

    foreach(rsrc ${PARSED_ARGS_RESOURCES})
        smtg_add_plugin_resource (${target} ${rsrc} ${PARSED_ARGS_OUTPUT_SUBDIRECTORY}) 
    endforeach()
endfunction()
