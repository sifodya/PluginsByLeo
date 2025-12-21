#change to your project name
project(SIMPLE_VOLUME VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_XCODE_GENERATE_SCHEME OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(PLUGIN_VERSION "00.02.01")

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
        EDITOR_WANTS_KEYBOARD_FOCUS TRUE
        JUCE_VST3_CAN_REPLACE_VST2 FALSE
        COPY_PLUGIN_AFTER_BUILD TRUE
        PLUGIN_MANUFACTURER_CODE Leop
        PLUGIN_CODE Sivo
        FORMATS VST3 Standalone
        PRODUCT_NAME "Simple Volume"
        #---
        VERSION "${PLUGIN_VERSION}"
        VST3_CATEGORIES Dynamics
)

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

#add Resources
juce_add_binary_data(BinaryData SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/bg.png
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/fader.png
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/fader_bg.png
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/full.png
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/learn_normal.png
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/learn_plain_red.png
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/learn_plain_yellow.png
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/learn_red.png
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/learn_yellow.png
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/learn_plain.png
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/ueberschrift.png
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/NeueHaasDisplayLight.ttf
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/NeueHaasDisplayMediu.ttf
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

juce_generate_juce_header(${PROJECT_NAME})

if(WINDOWS)
#Enable packaging
include(InstallRequiredSystemLibraries)
set(CPACK_GENERATOR "NSIS")

#set basic packaging options
set(CPACK_PACKAGE_NAME "Simple Volume")
set(CPACK_PACKAGE_VERSION "${PLUGIN_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION "Apply gain to all channels")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "C:/Program Files/Common Files/VST3")

#Set up installation rules
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

#include CPack module to generate installer
include(CPack)
endif ()

if(APPLE)
    # macOS packaging
    set(CPACK_GENERATOR "DragNDrop")
    set(CPACK_PACKAGE_NAME "Simple Volume")
    set(CPACK_PACKAGE_VERSION "${VERSION}")
    set(CPACK_PACKAGE_DESCRIPTION "JUCE Audio Plugin")
    set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-macOS")
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "Applications")

    # Set installation rules for macOS
    install(TARGETS ${PROJECT_NAME} DESTINATION bin)

    include(CPack)
endif ()

if(LINUX)
    # Linux packaging
    set(CPACK_GENERATOR "DEB")
    set(CPACK_PACKAGE_NAME "Simple Volume")
    set(CPACK_PACKAGE_VERSION "${VERSION}")
    set(CPACK_PACKAGE_DESCRIPTION "JUCE Audio Plugin")
    set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-linux")
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "/opt/YourPlugin")

    install(TARGETS ${PROJECT_NAME} DESTINATION bin)

    include(CPack)
endif ()