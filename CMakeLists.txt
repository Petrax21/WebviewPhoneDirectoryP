cmake_minimum_required(VERSION 3.28)
project(PhoneDirectoryDBP)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_TOOLCHAIN_FILE "/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")


find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)
pkg_check_modules(WEBKIT REQUIRED webkit2gtk-4.0)

find_package(libpqxx REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(Crow REQUIRED)
find_package(CURL REQUIRED)

add_subdirectory(RabbitMQ)

include_directories(${GTK_INCLUDE_DIRS}
        ${WEBKIT_INCLUDE_DIRS}
        ${PostgreSQL_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/libs/webview
        ${CMAKE_SOURCE_DIR}/json/include
        ${CMAKE_SOURCE_DIR}/json/include/nlohmann
        ${CMAKE_SOURCE_DIR}/domain/contact
        ${CMAKE_SOURCE_DIR}/application)  # Include path for nlohmann/json

add_executable(${PROJECT_NAME} main.cpp embedded_files.h
        domain/contact/contact.hpp
        domain/contact/contact-factory.hpp
        domain/contact/contact-repository.hpp
        domain/contact/contact-service.hpp
        application/service.hpp
        main.cpp
)

target_link_libraries(${PROJECT_NAME} ${GTK_LIBRARIES}
        ${WEBKIT_LIBRARIES}
        ${PostgreSQL_LIBRARIES}
        curl-demo ${CURL_LIBRARIES}
        nlohmann_json::nlohmann_json
        libpqxx::pqxx
        Crow::Crow
)
