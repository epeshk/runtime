# Licensed to the .NET Foundation under one or more agreements.
# The .NET Foundation licenses this file to you under the MIT license.

project(jethost)
set(DOTNET_PROJECT_NAME "jethost")

# Add RPATH to the apphost binary that allows using local copies of shared libraries
# dotnet core depends on for special scenarios when system wide installation of such
# dependencies is not possible for some reason.
# This cannot be enabled for MacOS (Darwin) since its RPATH works in a different way,
# doesn't apply to libraries loaded via dlopen and most importantly, it is not transitive.
if (NOT CLR_CMAKE_TARGET_OSX)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    set(CMAKE_INSTALL_RPATH "\$ORIGIN/netcoredeps")
endif()

set(SKIP_VERSIONING 1)

include_directories(..)
include_directories(../../json)

set(SOURCES
        ../jethost_properties.cpp
        ../jethost_runtime_config.cpp
        ../jethost_main.cpp
        ./hostfxr_resolver.cpp
        ./hostpolicy_resolver.cpp
        ../../hostpolicy/standalone/coreclr_resolver.cpp
        ../Exports.def
)

set(HEADERS
        ../jethost_properties.h
        ../jethost_runtime_config.h
        ../../hostfxr_resolver.h
)

add_definitions(-DHOSTPOLICY_EMBEDDED)

include(../../fxr/files.cmake)
include(../../hostpolicy/files.cmake)
include(../../hostcommon/files.cmake)

if(CLR_CMAKE_TARGET_WIN32)
    list(APPEND SOURCES
            ../../apphost/apphost.windows.cpp)

    list(APPEND HEADERS
            ../../apphost/apphost.windows.h)
endif()

include(../exe.cmake)
include(configure.cmake)

# add_definitions(-DFEATURE_APPHOST=1)
add_definitions(-DFEATURE_JETHOST=1)

# Disable manifest generation into the file .exe on Windows
if(CLR_CMAKE_TARGET_WIN32)
    set_property(TARGET ${PROJECT_NAME} PROPERTY
            LINK_FLAGS "/MANIFEST:NO"
        )
endif()

# Specify non-default Windows libs to be used for Arm/Arm64 builds
if (CLR_CMAKE_TARGET_WIN32 AND (CLR_CMAKE_TARGET_ARCH_ARM OR CLR_CMAKE_TARGET_ARCH_ARM64))
    target_link_libraries(jethost Advapi32.lib shell32.lib)
endif()
