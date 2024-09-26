# PhoneDirectoryDBP

**├── libcpp-http-client  
│   └── src
│       └── libcpp-http-client.hpp
├── libs  
│   └── webview   
├── main.cpp  
├── CMakeLists.txt  
├── embedded_files.h  
├── vcpkg.json**  

### main.cpp
```c++
#include <gtk/gtk.h>
#include "libs/webview/webview.h"
#include <string>
#include <iostream>
#include <curl/curl.h>
#include "embedded_files.h"

// CURL ile HTTP istekleri göndermek için yardımcı fonksiyonlar
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void sendHttpRequest(const std::string& url, const std::string& postData = "") {
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        if (!postData.empty()) {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
        }

        std::string readBuffer;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        curl_easy_cleanup(curl);
        std::cout << "Response: " << readBuffer << std::endl;
    }
}

int main() {

    webview::webview w(true, nullptr);

    w.set_title("Telefon Rehberi");
    w.set_size(800, 900, WEBVIEW_HINT_NONE);
    w.set_html(index_html);

    // Kişi ekleme fonksiyonu bağlandı
    w.bind("add_contact", [](const std::string& seq, const std::string& req, void*) {
        sendHttpRequest("http://localhost:8080/add_contact", req);
        return "";
    }, nullptr);

    // Kişi silme fonksiyonu bağlandı
    w.bind("delete_contact", [](const std::string& seq, const std::string& req, void*) {
        sendHttpRequest("http://localhost:8080/delete_contact", req);
        return "";
    }, nullptr);

    // Kişi listesini alma fonksiyonu bağlandı
    w.bind("get_contacts", [](const std::string& seq, const std::string& req, void*) {
        return sendHttpRequest("http://localhost:8080/get_contacts");
    }, nullptr);

    w.run();

    return 0;
}
```

### CMakeLists.txt
```cmake
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

include_directories(${GTK_INCLUDE_DIRS}
        ${WEBKIT_INCLUDE_DIRS}
        ${PostgreSQL_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/libs/webview
        ${CMAKE_SOURCE_DIR}/json/include
        ${CMAKE_SOURCE_DIR}/json/include/nlohmann

)  # Include path for nlohmann/json

add_executable(${PROJECT_NAME} main.cpp embedded_files.h
        libcpp-http-client/src/libcpp-http-client.hpp
)

target_link_libraries(${PROJECT_NAME} ${GTK_LIBRARIES}
        ${WEBKIT_LIBRARIES}
        ${PostgreSQL_LIBRARIES}
        nlohmann_json::nlohmann_json
        libpqxx::pqxx
        Crow::Crow
        CURL::libcurl
)
```


