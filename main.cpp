#include <gtk/gtk.h>
#include "libs/webview/webview.h"
#include <string>
#include <iostream>
#include <curl/curl.h>
#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>

#include "embedded_files.h"


void listenToRabbitMQ() {
    amqp_connection_state_t conn;
    conn = amqp_new_connection();

    amqp_socket_t *socket = amqp_tcp_socket_new(conn);
    if (!socket) {
        std::cerr << "RabbitMQ socket oluşturulamadı.\n";
        return;
    }

    int status = amqp_socket_open(socket, "localhost", 5672);
    if (status) {
        std::cerr << "RabbitMQ bağlantısı açılamadı.\n";
        return;
    }

    amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, "guest", "guest");
    amqp_channel_open(conn, 1);
    amqp_get_rpc_reply(conn);

    amqp_basic_consume(conn, 1, amqp_cstring_bytes("queue_name"), amqp_empty_bytes, 0, 1, 0, amqp_empty_table);

    while (true) {
        amqp_rpc_reply_t res;
        amqp_envelope_t envelope;

        amqp_maybe_release_buffers(conn);

        res = amqp_consume_message(conn, &envelope, nullptr, 0);

        if (res.reply_type != AMQP_RESPONSE_NORMAL) {
            break;
        }

        std::string message((char*) envelope.message.body.bytes, envelope.message.body.len);
        std::cout << "RabbitMQ mesajı alındı: " << message << std::endl;

        amqp_destroy_envelope(&envelope);
    }

    amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS);
    amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(conn);
}

// Web sayfasındaki bir API'ye HTTP GET isteği gönderme fonksiyonu
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string performHTTPRequest(const std::string& url) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return readBuffer;
}

int main() {

    webview::webview w(true, nullptr);
    w.set_title("Telefon Rehberi");
    w.set_size(800, 900, WEBVIEW_HINT_NONE);
    w.set_html(index_html);


    std::thread rabbitMQThread(listenToRabbitMQ);
    rabbitMQThread.detach();


    w.bind("getContactInfo", [](const std::string &url) -> std::string {
        return performHTTPRequest(url);
    });

    w.run();
}
