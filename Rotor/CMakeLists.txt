juce_add_plugin(Rotor
        VERSION "0.0.1"
        COMPANY_NAME "Maxwell Audio"
        IS_SYNTH FALSE
        IS_MIDI_EFFECT FALSE
        NEEDS_MIDI_INPUT FALSE
        NEEDS_MIDI_OUTPUT FALSE
        PLUGIN_MANUFACTURER_CODE Matt
        PLUGIN_CODE Roto
        FORMATS AU VST3
        PRODUCT_NAME "Rotor")

# Generate the JUCE header file for our source code
juce_generate_juce_header(Rotor)

# Declare necessary source files to include into the target
target_sources(Rotor PRIVATE
        Source/RotorAnalyzer.cpp
        Source/RotorEditor.cpp
        Source/RotorLookAndFeel.cpp
        Source/RotorProcessor.cpp)

# Preprocessor definitions for our target
target_compile_definitions(Rotor PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_DISPLAY_SPLASH_SCREEN=0)

# Add binary data from resources folder
juce_add_binary_data(RotorData SOURCES
        Resources/background.png
        Resources/montserrat-light.ttf
        Resources/montserrat.ttf
        Resources/louis-george-cafe-light.ttf
        Resources/louis-george-cafe.ttf)

# Link libraries to the plugin's source code and other resources (i.e. binary data)
target_link_libraries(Rotor PRIVATE
        # Link-time optimization (LTO) once applicable
        RotorData
        juce::juce_audio_utils
        juce::juce_dsp
        juce::juce_opengl)
