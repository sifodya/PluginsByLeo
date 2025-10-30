cmake_minimum_required(VERSION 3.24)

#change to your project name
project(SIMPLE_VOLUME VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_XCODE_GENERATE_SCHEME OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

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

#How the SourceFiles appear in IDE
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SourceFiles})

#Make SourceFiles buildable
target_sources(${PROJECT_NAME} PRIVATE ${SourceFiles})

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL$<$<CONFIG:Debug>:Debug>")

set_target_properties(MyPlugin_VST3 PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/dist"
)

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
        #BinaryData
)

#add Resources
juce_add_binary_data(BinaryData SOURCES
        resources/bg.png
        resources/fader.png
        resources/full.png
        resources/learn_normal.png
        resources/learn_plain red.png
        resources/learn_plain yellow.png
        resources/learn_red.png
        resources/learn_yellow.png
        resources/learn_plain.png
        resources/ueberschrift.png
)

juce_generate_juce_header(${PROJECT_NAME})

# -------------------------------
# Packaging with CPack
# -------------------------------
include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_NAME "Simple Volume")
set(CPACK_PACKAGE_VENDOR "LeoPlugins")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Apply's Gain to all the channels")
set(CPACK_PACKAGE_VERSION "0.1.0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "LeoPlugins/SimpleVolume")

# This is the default VST3 folder for all users
set(VST3_INSTALL_DIR "C:/Program Files/Common Files/VST3")

# Install your plugin
install(DIRECTORY "${CMAKE_BINARY_DIR}/dist/SimpleVolume.vst3"
        DESTINATION "${VST3_INSTALL_DIR}")

# Optional: include MSVC redistributable (see below)
set(CPACK_GENERATOR "NSIS")  # Creates a .exe installer
set(CPACK_NSIS_DISPLAY_NAME "Simple Volume")
#set(CPACK_NSIS_HELP_LINK "https://mycompany.com/help")
#set(CPACK_NSIS_URL_INFO_ABOUT "https://mycompany.com")
#set(CPACK_NSIS_CONTACT "support@mycompany.com")
set(CPACK_NSIS_MODIFY_PATH OFF)

# MSVC runtime detection (optional but recommended)
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
#set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/icon.ico")

include(CPack)

# Add the redistributable to the installer
install(FILES "${CMAKE_SOURCE_DIR}/installer/vc_redist.x64.exe" DESTINATION .)

# Tell NSIS to run it silently before main install
set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
  ExecWait '\\\"$INSTDIR\\\\vc_redist.x64.exe\\\" /install /quiet /norestart'
")