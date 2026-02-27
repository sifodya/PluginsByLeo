cmake_minimum_required(VERSION 3.24)
project(PluginsByLeo VERSION 00.02.01)

# 1. JUCE nichts installieren lassen
set(JUCE_INSTALL_TARGETS OFF CACHE BOOL "Don't install JUCE targets" FORCE)
# optional:
set(JUCE_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(JUCE_BUILD_EXTRAS OFF CACHE BOOL "" FORCE)

# add JUCE directory if pushed to repo
add_subdirectory(JUCE)

# --- Packaging info (common) ---
set(CPACK_PACKAGE_NAME "PluginsByLeo")
set(CPACK_PACKAGE_VENDOR "Leo Plugins")
set(CPACK_PACKAGE_CONTACT "leo@example.com")          # <- Pflicht für DEB
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Collection of Leo Audio Plugins")

# optional (falls vorhanden)
# set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")

# --- Debian-spezifisch ---
if(UNIX AND NOT APPLE)
  set(CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "" FORCE)
  set(CPACK_PACKAGING_INSTALL_PREFIX "/")  # sorgt dafür, dass /usr/... im Paket landet
endif()

if(APPLE)
  set(CPACK_GENERATOR "DragNDrop")
  set(CPACK_DMG_VOLUME_NAME "Leo Audio Plugins")
  # Name der DMG-Datei (ohne .dmg)
  set(CPACK_PACKAGE_FILE_NAME "LeoPlugins-macOS")
  # Universal Binary: Intel + Apple Silicon
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "" FORCE)
endif()

# --- Windows NSIS Configuration ---
if(WIN32)
  set(CPACK_GENERATOR "NSIS")
  set(CPACK_NSIS_INSTALL_ROOT "$COMMONFILES64")
  set(CPACK_PACKAGE_INSTALL_DIRECTORY "VST3/LeoPlugins")
  set(CPACK_COMPONENTS_ALL VST3) # Cpack only installs things marked with VST3 component
  # set(CPACK_NSIS_COMPONENT_INSTALL OFF) #Hide Component selection Page
  set(CPACK_NSIS_UNINSTALL_NAME "Leo Plugins Uninstall")
  set(CPACK_NSIS_PACKAGE_NAME "Leo Plugins")
  
  # NSIS specific settings
  #  (CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/path/to/icon.ico") set
  set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
  set(CPACK_NSIS_DISPLAY_NAME "Leo Audio Plugins ${PROJECT_VERSION}")
endif()

# List of all available plugins
set(ALL_PLUGINS
  # SeriousBrassEnhancer
  # SeriousPercussionEnhancer
  #SeriousStringEnhancer
  #SimpleCinematicPower
  #SimpleColoration
  #SimpleEmotion
  SimpleVolume
)

# Build all plugins by default, or specify with: cmake -DPLUGINS_TO_BUILD="SimpleVolume;SimpleColoration"
set(PLUGINS_TO_BUILD "${ALL_PLUGINS}" CACHE STRING "List of plugins to build")

# Add plugin subdirectories
foreach(PLUGIN ${PLUGINS_TO_BUILD})
  if(EXISTS "${CMAKE_SOURCE_DIR}/${PLUGIN}/CMakeLists.txt")
    add_subdirectory(${PLUGIN})
    message(STATUS "Added plugin: ${PLUGIN}")
  else()
    message(WARNING "Plugin not found: ${PLUGIN}")
  endif()
endforeach()

# Set package name based on which plugins are included
string(REPLACE ";" "_" PLUGIN_SUFFIX "${PLUGINS_TO_BUILD}")
if(PLUGINS_TO_BUILD STREQUAL ALL_PLUGINS)
  set(CPACK_PACKAGE_NAME "PluginsByLeo-AllPlugins")
  set(CPACK_PACKAGE_FILE_NAME "PluginsByLeo-AllPlugins-${PROJECT_VERSION}-win64")
  set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Complete Collection of Leo Audio Plugins")
else()
  set(CPACK_PACKAGE_NAME "PluginsByLeo-${PLUGIN_SUFFIX}")
  set(CPACK_PACKAGE_FILE_NAME "PluginsByLeo-${PLUGIN_SUFFIX}-${PROJECT_VERSION}-win64")
  set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Leo Audio Plugins: ${PLUGIN_SUFFIX}")
endif()

include(CPack)

