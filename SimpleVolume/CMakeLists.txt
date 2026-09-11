# SimpleVolume\CMakeLists.txt
set(PLUGIN_NAME SimpleVolume)
set(PLUGIN_VERSION "00.02.01")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Debug build")
    cmake_minimum_required(VERSION 3.24)

    #change to your project name
    project(${PLUGIN_NAME} VERSION ${PLUGIN_VERSION})

    set(CMAKE_CXX_STANDARD 20)
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

    cmake_path(GET CMAKE_CURRENT_SOURCE_DIR PARENT_PATH PARENT_DIR)
    set(INCLUDE_DIR "${PARENT_DIR}/include")

    #Source files to include
    set(SourceFiles
            Source/PluginEditor.cpp
            Source/PluginProcessor.cpp
            Source/PluginEditor.h
            Source/PluginProcessor.h
    )

    set(IncludeFiles
            ${INCLUDE_DIR}/LeoButtons.h
            ${INCLUDE_DIR}/LeoCanvas.h
            ${INCLUDE_DIR}/LeoEvents.h
            ${INCLUDE_DIR}/LeoExceptions.h
            ${INCLUDE_DIR}/LeoMacros.h
            ${INCLUDE_DIR}/LeoSlider.cpp
            ${INCLUDE_DIR}/LeoSlider.h
    )

    #Plugin Details
    juce_add_plugin(${PLUGIN_NAME}
            COMPANY_NAME LeoPlugins
            IS_SYNTH FALSE
            NEEDS_MIDI_INPUT TRUE
            NEEDS_MIDI_OUTPUT FALSE
            IS_MIDI_EFFECT FALSE
            EDITOR_WANTS_KEYBOARD_FOCUS TRUE
            JUCE_VST3_CAN_REPLACE_VST2 FALSE
            COPY_PLUGIN_AFTER_BUILD TRUE
            PLUGIN_MANUFACTURER_CODE Leop
            PLUGIN_CODE Sivo
            FORMATS VST3
            PRODUCT_NAME "Simple Volume"
            #---
            VERSION "${PLUGIN_VERSION}"
            VST3_CATEGORIES Dynamics
    )

    target_include_directories(${PLUGIN_NAME}
            PRIVATE
            ${INCLUDE_DIR}
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

    source_group(TREE ${INCLUDE_DIR} FILES ${IncludeFiles})

    #Make SourceFiles buildable
    target_sources(${PLUGIN_NAME} PRIVATE ${SourceFiles} ${IncludeFiles})

    #toggleable options from JUCE Cmake API
    target_compile_definitions(${PLUGIN_NAME}
            PUBLIC
            JUCE_WEB_BROWSER=0
            JUCE_USE_CURL=0
            JUCE_VST3_CAN_REPLACE_VST2=0
    )

    #JUCE libraries to bringt into project
    target_link_libraries(${PLUGIN_NAME}
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

    juce_generate_juce_header(${PLUGIN_NAME})
    #==================================================================================================================
else()

    message(STATUS "Release build")
    cmake_path(GET CMAKE_CURRENT_SOURCE_DIR PARENT_PATH PARENT_DIR)
    message("${PARENT_DIR}")
    set(INCLUDE_DIR "${PARENT_DIR}/include")
    message("${INCLUDE_DIR}")
    set(SourceFiles
            Source/PluginEditor.cpp
            Source/PluginProcessor.cpp
            Source/PluginEditor.h
            Source/PluginProcessor.h
    )

    set(IncludeFiles
            ${INCLUDE_DIR}/LeoButtons.h
            ${INCLUDE_DIR}/LeoCanvas.h
            ${INCLUDE_DIR}/LeoEvents.h
            ${INCLUDE_DIR}/LeoExceptions.h
            ${INCLUDE_DIR}/LeoMacros.h
            ${INCLUDE_DIR}/LeoSlider.cpp
            ${INCLUDE_DIR}/LeoSlider.h
    )

    juce_add_plugin(${PLUGIN_NAME}
        COMPANY_NAME "LeoPlugins"
        IS_SYNTH FALSE
        NEEDS_MIDI_INPUT FALSE
        NEEDS_MIDI_OUTPUT FALSE
        IS_MIDI_EFFECT FALSE
        EDITOR_WANTS_KEYBOARD_FOCUS TRUE
        JUCE_VST3_CAN_REPLACE_VST2 FALSE

        # In CI lieber AUS (Actions hat keinen sinnvollen "Copy after build" Ort)
        COPY_PLUGIN_AFTER_BUILD FALSE

        PLUGIN_MANUFACTURER_CODE Leop
        PLUGIN_CODE Sivo
        FORMATS VST3 Standalone
        PRODUCT_NAME "Simple Volume"
        VERSION "${PLUGIN_VERSION}"
        VST3_CATEGORIES Dynamics
    )

    target_include_directories(${PLUGIN_NAME}
            PRIVATE
            ${INCLUDE_DIR}
    )

    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SourceFiles})
    source_group(TREE ${INCLUDE_DIR} FILES ${IncludeFiles})
    target_sources(${PLUGIN_NAME} PRIVATE ${SourceFiles} ${IncludeFiles})

    target_compile_definitions(${PLUGIN_NAME}
        PUBLIC
            JUCE_WEB_BROWSER=0
            JUCE_USE_CURL=0
            JUCE_VST3_CAN_REPLACE_VST2=0
    )

    juce_add_binary_data(SimpleVolume_BinaryData SOURCES
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

    target_link_libraries(${PLUGIN_NAME}
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
            SimpleVolume_BinaryData
    )

    juce_generate_juce_header(${PLUGIN_NAME})

    # Optional: Install-Regeln (damit CPack im Root was "einsammeln" kann)
    # WICHTIG: JUCE erstellt pro Format Targets wie SimpleVolume_VST3 und SimpleVolume_Standalone
    if(TARGET ${PLUGIN_NAME}_VST3)
        if(WIN32)
            install(TARGETS ${PLUGIN_NAME}_VST3 DESTINATION "." COMPONENT "VST3")
        elseif(APPLE)
            install(TARGETS ${PLUGIN_NAME}_VST3 DESTINATION ".")
        else()
            install(TARGETS ${PLUGIN_NAME}_VST3 DESTINATION ".")
        endif()
    endif()
endif()
