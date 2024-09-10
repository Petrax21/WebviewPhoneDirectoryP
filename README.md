# PostgreSQL Webview Phone Directory C++

**├── application  
│ └── service.hpp  
├── cmake-build-debug  
├── domain  
│   └── contact  
│       ├── contact.hpp  
│       ├── contact-factory.hpp  
│       ├── contact-repository.hpp  
│       └── contact-service.hpp  
├── json  
├── libs  
│   └── webview  
│       └── webview2  
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
#include "embedded_files.h"
#include "service.hpp"

int main() {

    webview::webview w(true, nullptr);

    w.set_title("Telefon Rehberi");
    w.set_size(800, 900, WEBVIEW_HINT_NONE);
    w.set_html(index_html);

    w.bind("add_contact", [](const std::string& seq, const std::string& req, void*) {
        ApplicationService::addContact(req);
        return "";
    }, nullptr);

    w.bind("delete_contact", [](const std::string& seq, const std::string& req, void*) {
        ApplicationService::deleteContact(req);
        return "";
    }, nullptr);

    w.bind("get_contacts", [](const std::string& seq, const std::string& req, void*) {
        return ApplicationService::getContactList();
    }, nullptr);

    w.run();

    ApplicationService::cleanup();
    return 0;
}
```

### application/service.hpp
```c++
#ifndef SERVICE_HPP
#define SERVICE_HPP

#include <iostream>
#include <pqxx/pqxx>
#include <string>
#include "nlohmann/json.hpp"

#include "contact-service.hpp"

using json = nlohmann::json;

namespace ApplicationService
{
    pqxx::connection* conn = nullptr;

    inline void connectToDatabase() {
        try {
            conn = new pqxx::connection("dbname=rehber user=mustafa password=sitil.2134 hostaddr=127.0.0.1 port=5432");
        } catch (const std::exception& e) {
            std::cerr << "Connection failed: " << e.what() << std::endl;
            exit(EXIT_FAILURE);
        }
    }

   inline void addContact(const std::string& req) {
         ApplicationService::connectToDatabase();

    json j = json::parse(req);
    const std::string firstName = j["ad"];
    const std::string lastName = j["soyad"];
    const std::string phone = j["telefon"];

    pqxx::work txn(*conn);
    ContactService::addContact(txn, firstName, lastName, phone);
    txn.commit();
}

inline void deleteContact(const std::string& req) {
      ApplicationService::connectToDatabase();

    json j = json::parse(req);
    const std::string firstName = j["ad"];
    const std::string lastName = j["soyad"];
    const std::string phone = j["telefon"];

    pqxx::work txn(*conn);
    ContactService::deleteContact(txn, firstName, lastName, phone);
    txn.commit();
}

inline std::string getContactList() {
      ApplicationService::connectToDatabase();

    pqxx::work txn(*conn);
    std::string contactList = ContactService::getContactList(txn);
    txn.commit();
    return contactList;
}


inline void cleanup() {
          ApplicationService::connectToDatabase();

        delete conn;
    }
}

#endif //SERVICE_HPP
```

### domain/contact/contact-factory.hpp
```c++
#ifndef CONTACT_FACTORY_HPP
#define CONTACT_FACTORY_HPP

#include <pqxx/pqxx>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace ContactFactory
{
    inline json generateGetContactList(const pqxx::result& res) {
        json jArray = json::array();
        for (auto row : res) {
            json jObject;
            jObject["ad"] = row[0].c_str();
            jObject["soyad"] = row[1].c_str();
            jObject["telefon"] = row[2].c_str();
            jArray.push_back(jObject);
        }
        return jArray.dump();
    }
}

#endif //CONTACT_FACTORY_HPP
```

### domain/contact/contact-repository.hpp
```c++
#ifndef CONTACT_REPOSITORY_HPP
#define CONTACT_REPOSITORY_HPP

#include <pqxx/pqxx>
#include "contact-factory.hpp"

namespace ContactRepository
{
    inline std::string getContactList(pqxx::work& txn) {
        const pqxx::result res = txn.exec("SELECT first_name, last_name, phone FROM contacts");
        return ContactFactory::generateGetContactList(res).dump();
    }

    inline void addContact(pqxx::work& txn, const std::string& firstName, const std::string& lastName, const std::string& phone) {
        txn.exec_params("INSERT INTO contacts (first_name, last_name, phone) VALUES ($1, $2, $3)", firstName, lastName, phone);
    }

    inline void deleteContact(pqxx::work& txn, const std::string& firstName, const std::string& lastName, const std::string& phone) {
        txn.exec_params("DELETE FROM contacts WHERE first_name = $1 AND last_name = $2 AND phone = $3", firstName, lastName, phone);
    }
}

#endif //CONTACT_REPOSITORY_HPP
```

### domain/contact/contact-service.hpp
```c++
#ifndef CONTACT_REPOSITORY_HPP
#define CONTACT_REPOSITORY_HPP

#include <pqxx/pqxx>
#include "contact-factory.hpp"

namespace ContactRepository
{
    inline std::string getContactList(pqxx::work& txn) {
        const pqxx::result res = txn.exec("SELECT first_name, last_name, phone FROM contacts");
        return ContactFactory::generateGetContactList(res).dump();
    }

    inline void addContact(pqxx::work& txn, const std::string& firstName, const std::string& lastName, const std::string& phone) {
        txn.exec_params("INSERT INTO contacts (first_name, last_name, phone) VALUES ($1, $2, $3)", firstName, lastName, phone);
    }

    inline void deleteContact(pqxx::work& txn, const std::string& firstName, const std::string& lastName, const std::string& phone) {
        txn.exec_params("DELETE FROM contacts WHERE first_name = $1 AND last_name = $2 AND phone = $3", firstName, lastName, phone);
    }
}

#endif //CONTACT_REPOSITORY_HPP
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

find_package(PostgreSQL REQUIRED)
find_package(libpqxx REQUIRED)
find_package(nlohmann_json REQUIRED)

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
        nlohmann_json::nlohmann_json
        libpqxx::pqxx
)
```


