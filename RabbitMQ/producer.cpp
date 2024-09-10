#include <iostream>
#include <amqp.h>
#include <amqp_tcp_socket.h>
#include "producer.h"



void send_message(const char* message) {
    const char* hostname = "localhost";
    int port = 5672;
    const char* queue_name = "hello";

    // RabbitMQ bağlantısı oluştur
    amqp_connection_state_t conn = amqp_new_connection();
    amqp_socket_t *socket = amqp_tcp_socket_new(conn);
    if (!socket) {
        std::cerr << "Error creating TCP socket" << std::endl;
        return;
    }

    if (amqp_socket_open(socket, hostname, port)) {
        std::cerr << "Error opening TCP socket" << std::endl;
        return;
    }

    // RabbitMQ'ya bağlan
    amqp_rpc_reply_t reply = amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, "guest", "guest");
    if (reply.reply_type != AMQP_RESPONSE_NORMAL) {
        std::cerr << "Error logging in to RabbitMQ" << std::endl;
        return;
    }

    amqp_channel_open(conn, 1);
    reply = amqp_get_rpc_reply(conn);
    if (reply.reply_type != AMQP_RESPONSE_NORMAL) {
        std::cerr << "Error opening channel" << std::endl;
        return;
    }

    // Kuyruğu oluştur
    amqp_queue_declare(conn, 1, amqp_cstring_bytes(queue_name), 0, 0, 0, amqp_empty_table);
    reply = amqp_get_rpc_reply(conn);
    if (reply.reply_type != AMQP_RESPONSE_NORMAL) {
        std::cerr << "Error declaring queue" << std::endl;
        return;
    }

    // Mesajı gönder
    amqp_basic_publish(conn, 1, amqp_empty_bytes, amqp_cstring_bytes(queue_name), 0, 0, NULL, amqp_cstring_bytes(message));

    amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS);
    amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(conn);
}
