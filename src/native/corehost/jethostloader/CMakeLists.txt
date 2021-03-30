# Licensed to the .NET Foundation under one or more agreements.
# The .NET Foundation licenses this file to you under the MIT license.

project(jethostloader)
set(DOTNET_PROJECT_NAME "jethostloader")

set(SOURCES
    ./jethostloader.cpp
)

set(HEADERS
)

include(./exe.cmake)

# Disable manifest generation into the file .exe on Windows
if(CLR_CMAKE_TARGET_WIN32)
    set_property(TARGET ${PROJECT_NAME} PROPERTY
            LINK_FLAGS "/MANIFEST:NO"
        )
endif()

# Specify non-default Windows libs to be used for Arm/Arm64 builds
if (CLR_CMAKE_TARGET_WIN32 AND (CLR_CMAKE_TARGET_ARCH_ARM OR CLR_CMAKE_TARGET_ARCH_ARM64))
    target_link_libraries(apphost Advapi32.lib shell32.lib)
endif()
