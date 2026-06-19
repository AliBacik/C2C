cmake_minimum_required(VERSION 3.20)
project(C2CSolution VERSION 1.0.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform check
if(NOT WIN32)
    message(FATAL_ERROR "This project is designed for Windows only")
endif()

# Configuration types
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/built_dbg")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/built")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/built_dbg/cache")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/built/cache")

# Enable solution folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#############################################
# C2C Main Application
#############################################

# Include directories for main app
set(C2C_INCLUDES
    Libs/json
    OS-ImGui/imgui
    OS-ImGui/stb
    ${CMAKE_SOURCE_DIR}
)

# Source files for main app
set(C2C_SOURCES
    # Main
    main.cpp

    # Config
    Config/ConfigMenu.cpp
    Config/ConfigSaver.cpp

    # Core
    Core/Cheats.cpp
    Core/GlobalVars.cpp
    Core/MemoryMgr.cpp

    # Features
    Features/Misc.cpp
    Features/Radar.cpp

    # Game
    Game/Bone.cpp
    Game/Entity.cpp
    Game/Game.cpp

    # Helpers
    Helpers/Mouse.cpp
    Helpers/UIAccess.cpp

    # Offsets
    Offsets/Offsets.cpp

    # ImGui
    OS-ImGui/imgui/imgui.cpp
    OS-ImGui/imgui/imgui_demo.cpp
    OS-ImGui/imgui/imgui_draw.cpp
    OS-ImGui/imgui/imgui_impl_dx11.cpp
    OS-ImGui/imgui/imgui_impl_win32.cpp
    OS-ImGui/imgui/imgui_tables.cpp
    OS-ImGui/imgui/imgui_widgets.cpp

    # OS-ImGui
    OS-ImGui/OS-ImGui.cpp
    OS-ImGui/OS-ImGui_Base.cpp
    OS-ImGui/OS-ImGui_External.cpp
)

# Header files for main app
set(C2C_HEADERS
    # Config
    Config/ConfigMenu.h
    Config/ConfigSaver.h

    # Core
    Core/Config.h
    Core/Cheats.h
    Core/Globals.h
    Core/GlobalVars.h
    Core/GUI.h
    Core/Init.h
    Core/MemoryMgr.h
    Core/Render.h

    # Features
    Features/ESP.h
    Features/Misc.h
    Features/Radar.h

    # Game
    Game/Bone.h
    Game/Entity.h
    Game/Game.h
    Game/View.h

    # Helpers
    Helpers/Format.h
    Helpers/KeyManager.h
    Helpers/Logger.h
    Helpers/Mouse.h
    Helpers/StorageMgr.h
    Helpers/UIAccess.h
    Helpers/WebApi.h

    # Offsets
    Offsets/Offsets.h

    # ImGui
    OS-ImGui/imgui/imconfig.h
    OS-ImGui/imgui/imgui.h
    OS-ImGui/imgui/imgui_impl_dx11.h
    OS-ImGui/imgui/imgui_impl_win32.h
    OS-ImGui/imgui/imgui_internal.h
    OS-ImGui/imgui/imstb_rectpack.h
    OS-ImGui/imgui/imstb_textedit.h
    OS-ImGui/imgui/imstb_truetype.h

    # OS-ImGui
    OS-ImGui/OS-ImGui.h
    OS-ImGui/OS-ImGui_Base.h
    OS-ImGui/OS-ImGui_Exception.hpp
    OS-ImGui/OS-ImGui_External.h
    OS-ImGui/OS-ImGui_Struct.h

    # STB
    OS-ImGui/stb/stb_image.h

    # Resources
    Resources/resource.h
    Resources/Font.hpp
    Resources/Images.h
    Resources/Language.h
    Resources/Sounds.h
)

# Resource files for main app
set(C2C_RESOURCES
    Resources/Resource.rc
    Resources/icon.ico
)

# Create the main executable
add_executable(C2C WIN32 ${C2C_SOURCES} ${C2C_HEADERS} ${C2C_RESOURCES})

# Set target name
set_target_properties(C2C PROPERTIES OUTPUT_NAME "C2C")

# Include directories for main app
target_include_directories(C2C PRIVATE ${C2C_INCLUDES})

# Compiler-specific options for main app
if(MSVC)
    target_compile_options(C2C PRIVATE /W3 /sdl-)

    target_compile_definitions(C2C PRIVATE
        $<$<CONFIG:Debug>:DBDEBUG>
        $<$<CONFIG:Debug>:_DEBUG>
        $<$<CONFIG:Release>:NDEBUG>
        _CONSOLE
        UNICODE
        _UNICODE
    )

    set_property(TARGET C2C PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
    )

    set_target_properties(C2C PROPERTIES
        LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE /DEBUG"
        LINK_FLAGS_RELEASE "/SUBSYSTEM:CONSOLE /OPT:REF /OPT:ICF"
    )

    set_target_properties(C2C PROPERTIES
        LINK_FLAGS "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\""
    )
endif()

# Platform-specific libraries for main app
target_link_libraries(C2C PRIVATE
    d3d11
    dxgi
    user32
    kernel32
    gdi32
    winspool
    comdlg32
    advapi32
    shell32
    ole32
    oleaut32
    uuid
    odbc32
    odbccp32
)

#############################################
# C2C Kernel Project
#############################################

# Check if kernel project directory exists
if(EXISTS "${CMAKE_SOURCE_DIR}/kernel")
    set(KERNEL_DIR "${CMAKE_SOURCE_DIR}/kernel")

    if(KERNEL_DIR)
        # Source files for kernel project
        set(KERNEL_SOURCES
            ${KERNEL_DIR}/intel_driver.cpp
            ${KERNEL_DIR}/main.cpp
            ${KERNEL_DIR}/kdmapper.cpp
            ${KERNEL_DIR}/portable_executable.cpp
            ${KERNEL_DIR}/service.cpp
            ${KERNEL_DIR}/utils.cpp
        )

        # Header files for kernel project
        set(KERNEL_HEADERS
            ${KERNEL_DIR}/include/cfg.h
            ${KERNEL_DIR}/include/intel_driver.h
            ${KERNEL_DIR}/include/intel_driver_resource.h
            ${KERNEL_DIR}/include/kdmapper.h
            ${KERNEL_DIR}/include/logger.h
            ${KERNEL_DIR}/include/nt.h
            ${KERNEL_DIR}/include/portable_executable.h
            ${KERNEL_DIR}/include/service.h
            ${KERNEL_DIR}/include/utils.h
            ${KERNEL_DIR}/include/web_api.h
            ${KERNEL_DIR}/resource.h
        )

        # Resource files for kernel project
        set(KERNEL_RESOURCES
            ${KERNEL_DIR}/Resource.rc
            ${KERNEL_DIR}/icon.ico
        )

        # Create the kernel executable
        add_executable(C2C-kernel WIN32 ${KERNEL_SOURCES} ${KERNEL_HEADERS} ${KERNEL_RESOURCES})

        # Set target name
        set_target_properties(C2C-kernel PROPERTIES OUTPUT_NAME "C2C-kernel")

        # Include directories for kernel project
        target_include_directories(C2C-kernel PRIVATE ${KERNEL_DIR}/include)

        # Compiler-specific options for kernel project
        if(MSVC)
            target_compile_options(C2C-kernel PRIVATE
                $<$<CONFIG:Debug>:/W4>
                $<$<CONFIG:Debug>:/Od>
                $<$<CONFIG:Release>:/W3>
                $<$<CONFIG:Release>:/O2>
                $<$<CONFIG:Release>:/Oi>
                $<$<CONFIG:Release>:/GL>
                $<$<CONFIG:Release>:/MP>
                /sdl
            )

            target_compile_definitions(C2C-kernel PRIVATE
                $<$<CONFIG:Debug>:_DEBUG>
                $<$<CONFIG:Release>:NDEBUG>
                UNICODE
                _UNICODE
            )

            set_target_properties(C2C-kernel PROPERTIES
                LINK_FLAGS_DEBUG "/DEBUG"
                LINK_FLAGS_RELEASE "/OPT:REF /OPT:ICF /LTCG"
            )

            set_target_properties(C2C-kernel PROPERTIES
                LINK_FLAGS "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\""
            )
        endif()

        # Libraries for kernel project
        target_link_libraries(C2C-kernel PRIVATE
            version
            kernel32
            user32
            gdi32
            winspool
            comdlg32
            advapi32
            shell32
            ole32
            oleaut32
            uuid
            odbc32
            odbccp32
        )

        set_target_properties(C2C-kernel PROPERTIES FOLDER "Kernel")

    endif()
endif()

#############################################
# IDE Organization
#############################################

source_group("C2C\\Config" FILES
    Config/ConfigMenu.cpp
    Config/ConfigSaver.cpp
    Config/ConfigMenu.h
    Config/ConfigSaver.h
)

source_group("C2C\\Core" FILES
    Core/Cheats.cpp
    Core/GlobalVars.cpp
    Core/MemoryMgr.cpp
    Core/Config.h
    Core/Cheats.h
    Core/Globals.h
    Core/GlobalVars.h
    Core/GUI.h
    Core/Init.h
    Core/MemoryMgr.h
    Core/Render.h
)

source_group("C2C\\Features" FILES
    Features/Misc.cpp
    Features/Radar.cpp
    Features/ESP.h
    Features/Misc.h
    Features/Radar.h
)

source_group("C2C\\Game" FILES
    Game/Bone.cpp
    Game/Entity.cpp
    Game/Game.cpp
    Game/Bone.h
    Game/Entity.h
    Game/Game.h
    Game/View.h
)

source_group("C2C\\Helpers" FILES
    Helpers/Mouse.cpp
    Helpers/UIAccess.cpp
    Helpers/Format.h
    Helpers/KeyManager.h
    Helpers/Logger.h
    Helpers/Mouse.h
    Helpers/StorageMgr.h
    Helpers/UIAccess.h
    Helpers/WebApi.h
)

source_group("C2C\\ImGui" FILES
    OS-ImGui/imgui/imgui.cpp
    OS-ImGui/imgui/imgui_demo.cpp
    OS-ImGui/imgui/imgui_draw.cpp
    OS-ImGui/imgui/imgui_impl_dx11.cpp
    OS-ImGui/imgui/imgui_impl_win32.cpp
    OS-ImGui/imgui/imgui_tables.cpp
    OS-ImGui/imgui/imgui_widgets.cpp
    OS-ImGui/imgui/imconfig.h
    OS-ImGui/imgui/imgui.h
    OS-ImGui/imgui/imgui_impl_dx11.h
    OS-ImGui/imgui/imgui_impl_win32.h
    OS-ImGui/imgui/imgui_internal.h
    OS-ImGui/imgui/imstb_rectpack.h
    OS-ImGui/imgui/imstb_textedit.h
    OS-ImGui/imgui/imstb_truetype.h
)

source_group("C2C\\OS-ImGui" FILES
    OS-ImGui/OS-ImGui.cpp
    OS-ImGui/OS-ImGui_Base.cpp
    OS-ImGui/OS-ImGui_External.cpp
    OS-ImGui/OS-ImGui.h
    OS-ImGui/OS-ImGui_Base.h
    OS-ImGui/OS-ImGui_Exception.hpp
    OS-ImGui/OS-ImGui_External.h
    OS-ImGui/OS-ImGui_Struct.h
    OS-ImGui/stb/stb_image.h
)

source_group("C2C\\Resources" FILES
    Resources/Resource.rc
    Resources/icon.ico
    Resources/resource.h
    Resources/Font.hpp
    Resources/Images.h
    Resources/Language.h
    Resources/Sounds.h
)

source_group("C2C\\Offsets" FILES
    Offsets/Offsets.cpp
    Offsets/Offsets.h
)

# Set main app as startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT C2C)

# Set folder for main project in IDE
set_target_properties(C2C PROPERTIES FOLDER "Main")
