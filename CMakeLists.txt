cmake_minimum_required(VERSION 3.7.2)
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "" FORCE)
set(AU_COPY_PATH "~/Library/Audio/Plug-Ins/Components")
project(Fuzz VERSION 0.0.1)

include(FetchContent)
set(FETCH_CONTENT_QUIET OFF)
FetchContent_Declare(
        JUCE
        GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
        GIT_TAG origin/master
        GIT_SHALLOW ON
        FIND_PACKAGE_ARGS 7.0.3 GLOBAL
)

juce_add_plugin(
        Fuzz
        PLUGIN_MANUFACTURER_CODE SMPI
        PLUGIN_CODE SM02
        FORMATS AU VST3 Standalone
        PRODUCT_NAME "Fuzz"
)
juce_add_module(${CMAKE_CURRENT_SOURCE_DIR}/Modules/SDSP)
juce_add_module(${CMAKE_CURRENT_SOURCE_DIR}/Modules/SUX)
juce_generate_juce_header(Fuzz)

add_subdirectory(source)
target_sources(Fuzz PRIVATE
        ${PROJECT_SOURCE}
        )

target_include_directories(Fuzz PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Asio
        ${CMAKE_CURRENT_SOURCE_DIR}/source)


target_compile_definitions(Fuzz PUBLIC
        JUCE_ASIO=1
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

juce_add_binary_data(BinaryData SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/UnderwaterConv.wav
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/Thermometer.png
        )
#juce_add_binary_data(SprinklerData SOURCES
# Resources/NotoSansJP-Regular.otf
# Resources/SprinklerSliderDark.png
#)
target_link_libraries(Fuzz PRIVATE
        BinaryData
        juce::juce_audio_utils
        juce::juce_dsp
        SDSP
        PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)