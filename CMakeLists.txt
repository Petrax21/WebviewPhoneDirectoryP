cmake_minimum_required(VERSION 3.28)
project(webview-sample)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Ensure that we are using static libraries if desired
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")

if (WIN32)
    # For Windows: Static linking flags
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")

    # Create the executable for Windows
    add_executable(${PROJECT_NAME} WIN32 main.cpp)

    # Link against Windows libraries
    target_link_libraries(${PROJECT_NAME} PRIVATE advapi32 ole32 shell32 shlwapi user32 version)

else()
    # For non-Windows systems (e.g., Linux)

    # Set the path to vcpkg toolchain file if using vcpkg
    set(CMAKE_TOOLCHAIN_FILE "/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

    # Find GTK and WebKitGTK packages using pkg-config
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK REQUIRED gtk+-3.0)
    pkg_check_modules(WEBKIT REQUIRED webkit2gtk-4.1)

    # Find PostgreSQL package
    find_package(PostgreSQL REQUIRED)
    find_package(nlohmann-json REQUIRED)

    # Include directories
    include_directories(${GTK_INCLUDE_DIRS}
            ${WEBKIT_INCLUDE_DIRS}
            ${PostgreSQL_INCLUDE_DIRS}
            ${CMAKE_SOURCE_DIR}/libs/webview
            ${CMAKE_SOURCE_DIR}/json/include
            ${CMAKE_SOURCE_DIR}/json/include/nlohmann
            ${CMAKE_SOURCE_DIR}/domain/contact
            ${CMAKE_SOURCE_DIR}/application)  # Include path for nlohmann/json

    # Create the executable
    add_executable(${PROJECT_NAME} main.cpp embedded_files.h
            domain/contact/contact.hpp
            domain/contact/contact-factory.hpp
            domain/contact/contact-repository.hpp
            domain/contact/contact-service.hpp
            json/include/nlohmann/json.hpp
            application/service.hpp
    )

    # Link libraries
    target_link_libraries(${PROJECT_NAME} ${GTK_LIBRARIES}
            ${WEBKIT_LIBRARIES}
            ${PostgreSQL_LIBRARIES})
endif()
