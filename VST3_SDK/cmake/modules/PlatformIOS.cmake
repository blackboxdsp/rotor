
#-------------------------------------------------------------------------------
# Platform
#-------------------------------------------------------------------------------

function(smtg_set_platform_ios target)
    if(NOT SMTG_MAC)
        message(FATAL_ERROR "smtg_set_platform_ios only works on macOS, use it in an if(SMTG_MAC) block")
    endif()
    set_target_properties(${target} PROPERTIES XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET "9.0")
    set_target_properties(${target} PROPERTIES XCODE_ATTRIBUTE_SDKROOT "iphoneos")
    set_target_properties(${target} PROPERTIES XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2")
endfunction()
