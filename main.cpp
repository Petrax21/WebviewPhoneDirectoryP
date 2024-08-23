#include <gtk/gtk.h>
#include "libs/webview/webview.h"
#include <string>
#include <iostream>
#include "embedded_files.h"
#include "service.hpp"

int main() {
    ApplicationService::connectToDatabase();

    webview::webview w(true, nullptr);
    w.set_title("Telefon Rehberi");
    w.set_size(800, 600, WEBVIEW_HINT_NONE);
    w.set_html(index_html);

    // Webview'e C++ işlevlerini bağlama
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