#include <gtk/gtk.h>
#include <mysql/mysql.h>
#include "libs/webview/webview.h"
#include <string>
#include <functional>
#include <future>
#include "embedded_files.h"

// MySQL bağlantısı
MYSQL* connect_to_db() {
    MYSQL *conn = mysql_init(nullptr);
    if (conn == nullptr) {
        fprintf(stderr, "mysql_init() failed\n");
        return nullptr;
    }

    if (mysql_real_connect(conn, "localhost", "mustafa", "sitil.2134", "telefon_rehberi", 0, nullptr, 0) == nullptr) {
        fprintf(stderr, "mysql_real_connect() failed\n");
        mysql_close(conn);
        return nullptr;
    }

    return conn;
}

// Yeni kişi ekleme
void add_contact_to_db(const std::string& ad, const std::string& soyad, const std::string& telefon) {
    MYSQL *conn = connect_to_db();
    if (conn) {
        std::string query = "INSERT INTO rehber (ad, soyad, telefon) VALUES ('" + ad + "', '" + soyad + "', '" + telefon + "')";
        if (mysql_query(conn, query.c_str())) {
            fprintf(stderr, "INSERT failed: %s\n", mysql_error(conn));
        }
        mysql_close(conn);
    }
}

// Kişi silme
void delete_contact_from_db(const std::string& ad, const std::string& soyad, const std::string& telefon) {
    MYSQL *conn = connect_to_db();
    if (conn) {
        std::string query = "DELETE FROM rehber WHERE ad='" + ad + "' AND soyad='" + soyad + "' AND telefon='" + telefon + "'";
        if (mysql_query(conn, query.c_str())) {
            fprintf(stderr, "DELETE failed: %s\n", mysql_error(conn));
        }
        mysql_close(conn);
    }
}

// Kişi listesini okuma
std::string get_contacts_from_db() {
    MYSQL *conn = connect_to_db();
    std::string contacts_json = "[";
    if (conn) {
        if (mysql_query(conn, "SELECT ad, soyad, telefon FROM rehber")) {
            fprintf(stderr, "SELECT failed: %s\n", mysql_error(conn));
        } else {
            MYSQL_RES *res = mysql_store_result(conn);
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res))) {
                contacts_json += "{\"ad\":\"" + std::string(row[0]) + "\",\"soyad\":\"" + std::string(row[1]) + "\",\"telefon\":\"" + std::string(row[2]) + "\"},";
            }
            mysql_free_result(res);
        }
        mysql_close(conn);
    }
    if (contacts_json.back() == ',') {
        contacts_json.pop_back();
    }
    contacts_json += "]";
    return contacts_json;
}

int main() {
    webview::webview w(true, nullptr);
    w.set_title("Telefon Rehberi");
    w.set_size(800, 600, WEBVIEW_HINT_NONE);
    w.set_html(index_html);

    // Webview'e C++ işlevlerini bağlama
    w.bind("add_contact", [](const std::string& seq, const std::string& req, void*) {
        auto ad = webview::json_parse(req, "ad", 0);
        auto soyad = webview::json_parse(req, "soyad", 0);
        auto telefon = webview::json_parse(req, "telefon", 0);
        add_contact_to_db(ad, soyad, telefon);
        return "";
    }, nullptr);

    w.bind("delete_contact", [](const std::string& seq, const std::string& req, void*) {
        auto ad = webview::json_parse(req, "ad", 0);
        auto soyad = webview::json_parse(req, "soyad", 0);
        auto telefon = webview::json_parse(req, "telefon", 0);
        delete_contact_from_db(ad, soyad, telefon);
        return "";
    }, nullptr);

    w.bind("get_contacts", [](const std::string& seq, const std::string& req, void*) {
        return get_contacts_from_db();
    }, nullptr);

    w.run();
    return 0;
}
