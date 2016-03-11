cmake_minimum_required(VERSION 2.8)

set(PROJ_NAME Thrift)

if(MSVC)
    set(MY_CSC "msbuild.exe")
else()
    set(MY_CSC "xbuild")
endif()
get_filename_component(MY_CSC "${MY_CSC}" PROGRAM)
if(NOT EXISTS "${MY_CSC}")
    if(MSVC)
        message(FATAL_ERROR "Cannot find msbuild.exe. Please install Visual Studio and run cmake within Visual Studio build command console.")
    else()
        message(FATAL_ERROR "Cannot find xbuild. Please install mono and xbuild.")
    endif()
endif()

set(MY_OUTPUT_PATH "${CMAKE_INSTALL_PREFIX}/lib")
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/${PROJ_NAME}.csproj.template" "${CMAKE_CURRENT_SOURCE_DIR}/${PROJ_NAME}.csproj")

execute_process(
    COMMAND ${MY_CSC} "${CMAKE_CURRENT_SOURCE_DIR}/${PROJ_NAME}.csproj" "/p:Configuration=${CMAKE_BUILD_TYPE}"
    )