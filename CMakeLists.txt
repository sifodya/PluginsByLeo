cmake_minimum_required(VERSION 3.24)
project(PluginsByLeo VERSION 1.0.0)

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
  set(CPACK_GENERATOR "DEB")
  set(CPACK_DEBIAN_PACKAGE_SECTION "sound")
  set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6, libstdc++6, libx11-6, libasound2")
endif()

if(APPLE)
  set(CPACK_GENERATOR "DragNDrop")
  set(CPACK_DMG_VOLUME_NAME "Leo Audio Plugins")
  # Name der DMG-Datei (ohne .dmg)
  set(CPACK_PACKAGE_FILE_NAME "LeoPlugins-macOS")
  # Universal Binary: Intel + Apple Silicon
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "" FORCE)
endif()

include(CPack)

# build all Plugins

# add_subdirectory(SeriousBrassEnhancer)
# add_subdirectory(SeriousPercussionEnhancer)
# add_subdirectory(SeriousStringEnhancer)
# add_subdirectory(SimpleCinematicPower)
# add_subdirectory(SimpleColoration)
# add_subdirectory(SimpleEmotion)
add_subdirectory(SimpleVolume)