#ifndef PRODUCER_H
#define PRODUCER_H

void send_message(const char* message);void amqp_queue_declare(amqp_connection_state_t conn, int channel, amqp_bytes_t queue, int passive, int durable, int exclusive,
                       amqp_table_t amqp_table);

void amqp_queue_declare(amqp_connection_state_t conn, int channel, amqp_bytes_t queue, int passive, int durable, int exclusive,
                       amqp_table_t amqp_table);

#endif // PRODUCER_H
