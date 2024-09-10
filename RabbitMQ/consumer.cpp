#include <iostream>
#include <amqp.h>
#include <amqp_tcp_socket.h>
#include "consumer.h"




void consume_messages() {
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

    // Mesajları tüket
    amqp_basic_consume(conn, 1, amqp_cstring_bytes(queue_name), amqp_empty_bytes, 0, 1, 0);
    reply = amqp_get_rpc_reply(conn);
    if (reply.reply_type != AMQP_RESPONSE_NORMAL) {
        std::cerr << "Error consuming from queue" << std::endl;
        return;
    }

    while (true) {
        amqp_rpc_reply_t res = amqp_read_message(conn, 1, NULL, 0);
        if (res.reply_type == AMQP_RESPONSE_NORMAL) {
            std::cout << "Received message: " << (char*)res.reply.message.body.bytes << std::endl;
        } else {
            std::cerr << "Error reading message" << std::endl;
            break;
        }
    }

    amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS);
    amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(conn);
}
