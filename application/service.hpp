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