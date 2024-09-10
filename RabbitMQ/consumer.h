#ifndef CONSUMER_H
#define CONSUMER_H

void consume_messages();


void amqp_basic_consume(amqp_connection_state_t conn, int channel, amqp_bytes_t queue, amqp_bytes_t amqp_bytes, int no_local, int no_ack,
                       int exclusive);

void amqp_queue_declare(amqp_connection_state_t conn, int channel, amqp_bytes_t queue, int passive, int durable, int exclusive,
                       amqp_table_t amqp_table);

#endif // CONSUMER_H
