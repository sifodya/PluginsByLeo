cmake_minimum_required(VERSION 3.24)

#change to your project name
project(SIMPLE_VOLUME VERSION 0.1.0)

set(PLUGIN_VERSION "00.02.01")

# To get started on a new plugin, copy this entire folder (containing this file and C++ sources) to
# a convenient location, and then start making modifications.

# The first line of any CMake project should be a call to `cmake_minimum_required`, which checks
# that the installed CMake will be able to understand the following CMakeLists, and ensures that
# CMake's behaviour is compatible with the named version. This is a standard CMake command, so more
# information can be found in the CMa

# If you've installed JUCE somehow (via a package manager, or directly using the CMake install
# target), you'll need to tell this project that it depends on the installed copy of JUCE. If you've
# included JUCE directly in your source tree (perhaps as a submodule), you'll need to tell CMake to
# include that subdirectory as part of the build.

# find_package(JUCE CONFIG REQUIRED)                  # If you've installed JUCE to your system
# or
# add_subdirectory("../JUCE" JUCE)                    # If you've put JUCE in a subdirectory called JUCE

# If you are building a VST2 or AAX plugin, CMake needs to be told where to find these SDKs on your
# system. This setup should be done before calling `juce_add_plugin`.

# juce_set_vst2_sdk_path(...)
# juce_set_aax_sdk_path(...)

# `juce_add_plugin` adds a static library target with the name passed as the first argument
# (AudioPluginExample here). This target is a normal CMake target, but has a lot of extra properties set
# up by default. As well as this shared code static library, this function adds targets for each of
# the formats specified by the FORMATS arguments. This function accepts many optional arguments.
# Check the readme at `docs/CMake API.md` in the JUCE repo for the full li
    # VERSION ...                               # Set this if the plugin version is different to the project version
    # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
    # ICON_SMALL ...                 # Specify the name of the plugin's author
    # IS_SYNTH TRUE/FALSE                       # Is this a synth or an effect?
    # NEEDS_MIDI_INPUT TRUE/FALSE               # Does the plugin need midi input?
    # NEEDS_MIDI_OUTPUT TRUE/FALSE              # Does the plugin need midi output?
    # IS_MIDI_EFFECT TRUE/FALSE                 # Is this plugin a MIDI effect?
    # EDITOR_WANTS_KEYBOARD_FOCUS TRUE/FALSE    # Does the editor need keyboard focus?
    # COPY_PLUGIN_AFTER_BUILD TRUE/FALSE        # Should the plugin be installed to a default location after building?
             # A four-character manufacturer id with at least one upper-case characte                         # A unique four-character plugin id with exactly one upper-case character
                                                # GarageBand 10.3 requires the first letter to be upper-case, and the               # The name of the final executable, which can differ from the target name

# `juce_generate_juce_header` will create a JuceHeader.h for a given target, which will be generated
# into your build tree. This should be included with `#include <JuceHeader.h>`. The include path for
# this header will be automatically added to the target. The main function of the JuceHeader is to
# include all your JUCE module headers; if you're happy to include module headers directly, you
# probably don't need to call this.

# link freetype package for Linux build ============================
if(UNIX AND NOT APPLE)  # nur Linux
  find_package(Freetype REQUIRED)
  # dein JUCE/Projekt-Target hier einsetzen:
  target_link_libraries(simpleVolume PRIVATE Freetype::Freetype)
  # Optional, falls dein FindFreetype alt ist:
  # target_include_directories(juce_add_plugin PRIVATE ${FREETYPE_INCLUDE_DIRS})
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

# Set macOS-specific settings for the plugin (only for macOS build)
if(APPLE)
    set_target_properties(MyPlugin PROPERTIES
            MACOSX_BUNDLE TRUE
            BUNDLE_EXTENSION ".vst3"
    )
endif()
# ==================================================================

# juce_generate_juce_header(simpleVolume)

# `target_sources` adds source files to a target. We pass the target that needs the sources as the
# first argument, then a visibility parameter for the sources which should normally be PRIVATE.
# Finally, we supply a list of source files that will be built into the target. This is a standard
# CMake command.

target_sources(simpleVolume
    PRIVATE
        Source/PluginEditor.cpp
        Source/PluginProcessor.cpp)

# `target_compile_definitions` adds some preprocessor definitions to our target. In a Projucer
# project, these might be passed in the 'Preprocessor Definitions' field. JUCE modules also make use
# of compile definitions to switch certain features on/off, so if there's a particular feature you
# need that's not on by default, check the module header for the correct flag to set here. These
# definitions will be visible both to your code, and also the JUCE module code, so for new
# definitions, pick unique names that are unlikely to collide! This is a standard CMake command.

target_compile_definitions(simpleVolume
    PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0)

# If your target needs extra binary assets, you can add them here. The first argument is the name of
# a new static library target that will include all the binary resources. There is an optional
# `NAMESPACE` argument that can specify the namespace of the generated binary data class. Finally,
# the SOURCES argument should be followed by a list of source files that should be built into the
# static library. These source files can be of any kind (wav data, images, fonts, icons etc.).
# Conversion to binary-data will happen when your target is built.

# Assets (generate BinaryData.h)
juce_add_binary_data(SimpleVolume_BinaryData
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/bg.png 
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/fader_bg.png 
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/fader.png 
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/full.png 
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/learn_normal.png 
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/learn_plain_red.png 
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/learn_plain_yellow.png 
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/learn_plain.png 
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/learn_red.png 
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/learn_yellow.png 
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/NeueHaasDisplayLight.ttf 
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/NeueHaasDisplayMediu.ttf 
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/ueberschrift.png)

# `target_link_libraries` links libraries and JUCE modules to other libraries or executables. Here,
# we're linking our executable target to the `juce::juce_audio_utils` module. Inter-module
# dependencies are resolved automatically, so `juce_core`, `juce_events` and so on will also be
# linked automatically. If we'd generated a binary data target above, we would need to link to it
# here too. This is a standard CMake command.

target_link_libraries(simpleVolume
    PRIVATE
        SimpleVolume_BinaryData  # If we'd created a binary data target, we'd link to it here
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

# --------------------------------------------------
# Install-Rules for the Plugin (for CPack / Installer)
# --------------------------------------------------

if(WIN32)
    set(JUCE_VST3_INSTALL_PATH
        "C:/Program Files/Common Files/VST3"
        CACHE STRING "VST3 install path"
    )      
    install(TARGETS simpleVolume_VST3
        BUNDLE DESTINATION "."
        LIBRARY DESTINATION "."
    )
elseif(APPLE)
    install(TARGETS simpleVolume_VST3
        BUNDLE DESTINATION "."
        LIBRARY DESTINATION "."
    )
elseif(UNIX)
  install(TARGETS simpleVolume_VST3
    BUNDLE DESTINATION "lib/vst3"
    LIBRARY DESTINATION "lib/vst3"
  )
endif()