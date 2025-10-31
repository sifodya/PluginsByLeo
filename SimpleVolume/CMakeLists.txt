cmake_minimum_required(VERSION 3.24)

#change to your project name
project(SIMPLE_VOLUME VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_XCODE_GENERATE_SCHEME OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

#CPM check if installed, if not install it
set(CPM_DOWNLOAD_VERSION 0.34.0)
set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")

if (NOT EXISTS $[{CPM_DOWNLOAD_LOCATION})
    message(STATUS "Downloading CPM.cmake to ${CPM_DOWNLOAD_LOCATION}")
    file(DOWNLOAD https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
endif()

include(${CPM_DOWNLOAD_LOCATION})

#Bring in JUCE locally
CPMAddPackage(
    NAME juce
    GITHUB_REPOSITORY juce-framework/JUCE
    GIT_TAG origin/master
)

#Source files to include
set(SourceFiles
    Source/PluginEditor.cpp
    Source/PluginProcessor.cpp
    Source/PluginEditor.h
    Source/PluginProcessor.h
        Source/LeoBackground.cpp
        Source/SimpleSliderLookAndFeel.cpp
        Source/LeoPluginTitle.cpp
        Source/SimpleImageButton.cpp
        Source/SimpleHyperlinkButton.cpp
)

#Plugin Details
juce_add_plugin(${PROJECT_NAME}
        COMPANY_NAME LeoPlugins
        IS_SYNTH FALSE
        NEEDS_MIDI_INPUT FALSE
        NEEDS_MIDI_OUTPUT FALSE
        IS_MIDI_EFFECT FALSE
        EDITOR_WANTS_KEYBOARD_FOCUS FALSE
        JUCE_VST3_CAN_REPLACE_VST2 FALSE
        COPY_PLUGIN_AFTER_BUILD TRUE
        PLUGIN_MANUFACTURER_CODE Leop
        PLUGIN_CODE Sivo
        FORMATS VST3 Standalone
        PRODUCT_NAME "Simple Volume"
        #---
        VERSION "00.01.00"
        VST3_CATEGORIES Dynamics
)

# Set macOS-specific settings for the plugin (only for macOS build)
if(APPLE)
    set_target_properties(MyPlugin PROPERTIES
            MACOSX_BUNDLE TRUE
            BUNDLE_EXTENSION ".vst3"
    )
endif()

#How the SourceFiles appear in IDE
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SourceFiles})

#Make SourceFiles buildable
target_sources(${PROJECT_NAME} PRIVATE ${SourceFiles})

#toggleable options from JUCE Cmake API
target_compile_definitions(${PROJECT_NAME}
        PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)

#JUCE libraries to bringt into project
target_link_libraries(${PROJECT_NAME}
        PUBLIC
        juce::juce_analytics
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_core
        juce::juce_data_structures
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
        juce::juce_audio_utils
        juce::juce_dsp
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
        PRIVATE
        BinaryData
)

#add Resources
juce_add_binary_data(BinaryData SOURCES
        resources/bg.png
        resources/fader.png
        resources/fader_bg.png
        resources/full.png
        resources/learn_normal.png
        resources/learn_plain_red.png
        resources/learn_plain_yellow.png
        resources/learn_red.png
        resources/learn_yellow.png
        resources/learn_plain.png
        resources/ueberschrift.png
        resources/NeueHaasDisplayLight.ttf
        resources/NeueHaasDisplayMediu.ttf
)

juce_generate_juce_header(${PROJECT_NAME})