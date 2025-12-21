# SimpleVolume\CMakeLists.txt

set(PLUGIN_NAME SimpleVolume)
set(PLUGIN_VERSION "00.02.01")

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

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SourceFiles})
target_sources(${PLUGIN_NAME} PRIVATE ${SourceFiles})

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
        install(TARGETS ${PLUGIN_NAME}_VST3 DESTINATION "Common Files/VST3")
    elseif(APPLE)
        install(TARGETS ${PLUGIN_NAME}_VST3 DESTINATION "Library/Audio/Plug-Ins/VST3")
    else()
        install(TARGETS ${PLUGIN_NAME}_VST3 DESTINATION "lib/vst3")
    endif()
endif()
