
include(AddSMTGLibrary)

# Returns the windows architecture.
#
# This name will be used as a folder name inside the plug-in package.
# The variable WIN_ARCHITECTURE_NAME will be set.
function(smtg_set_aax_win_architecture_name)
    if(SMTG_WIN AND CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(WIN_ARCHITECTURE_NAME "x64")
    else()
        set(WIN_ARCHITECTURE_NAME "win32")
    endif()

    set_target_properties(${target}
        PROPERTIES
        SMTG_WIN_ARCHITECTURE_NAME ${WIN_ARCHITECTURE_NAME}           
    )
endfunction()

# Adds a AAX target.
#
# @param target The target to which a aax plug-in will be added. 
function(smtg_add_aaxplugin target)
    add_library(${target} MODULE ${ARGN})
    smtg_set_aax_win_architecture_name(${target})
    smtg_make_plugin_package(${target} aaxplugin)
    # smtg_dump_plugin_package_variables(${target})
endfunction()