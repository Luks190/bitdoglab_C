#ifndef MQTT_H
#define MQTT_H
#include <stdbool.h> 

// Function prototypes for MQTT operations
void mqtt_setup(const char *client_id, const char *broker_ip, const char *user, const char *pass, char *text_buffe);

// Function to publish data to a specific topic
void mqtt_comm_publish(const char *topic, const uint8_t *data, size_t len, int retain);

// Function to subscribe to a topic
void mqtt_comm_subscribe(const char *topico, char *rec_data);

// Function to check if the MQTT connection is established
bool mqtt_is_connected();

#endif