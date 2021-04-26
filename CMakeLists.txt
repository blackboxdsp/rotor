# Checks that the installed version of CMake can understand the following commands
cmake_minimum_required(VERSION 3.15)

# Declaring targets and describing source/ binary directories
project(Rotor VERSION 1.0.0)

# Declare dependency on JUCE (as installed on the local system)
add_subdirectory(juce)

juce_add_plugin(Rotor
        VERSION "1.0.0"
        COMPANY_NAME "Black Box DSP"
        IS_SYNTH FALSE
        IS_MIDI_EFFECT FALSE
        NEEDS_MIDI_INPUT FALSE
        NEEDS_MIDI_OUTPUT FALSE
        PLUGIN_MANUFACTURER_CODE BKBX
        PLUGIN_CODE ROTR
        FORMATS AU VST3
        PRODUCT_NAME "Rotor")

# Generate the JUCE header file for our source code
juce_generate_juce_header(Rotor)

# Declare necessary source files to include into the target
target_sources(Rotor PRIVATE
        src/RotorAnalyzer.cpp
        src/RotorEditor.cpp
        src/RotorLookAndFeel.cpp
        src/RotorProcessor.cpp)

# Preprocessor definitions for our target
target_compile_definitions(Rotor PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_DISPLAY_SPLASH_SCREEN=0)

# Add binary data from resources folder
juce_add_binary_data(RotorData SOURCES
        resources/background.png
        resources/montserrat-light.ttf
        resources/montserrat.ttf
        resources/louis-george-cafe-light.ttf
        resources/louis-george-cafe.ttf)

# Link libraries to the plugin's source code and other resources (i.e. binary data)
target_link_libraries(Rotor PRIVATE
        # Link-time optimization (LTO) once applicable
        RotorData
        juce::juce_audio_utils
        juce::juce_dsp
        juce::juce_opengl)
