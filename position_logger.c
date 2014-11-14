#include <stdio.h>
#include <stdlib.h>
#include <gps.h>
#include <string.h>
#include <MQTTClient.h>

#define ADDRESS     "tcp://192.168.1.73:1883"
#define CLIENTID    "ExampleClientPub"
#define TOPIC       "walter"
#define QOS         1
#define TIMEOUT     10000L

int main(void) {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;

    MQTTClient_create(&client, ADDRESS, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    }

    // Open
    gps_init();

    loc_t data;

    char* str;
    str = (char *)malloc(100*sizeof(char));

    // warn while 1
    while (1) {
        gps_location(&data);

        sprintf(str, "lat: %lf, lon: %lf", data.latitude, data.longitude);
        pubmsg.payload = str;
        pubmsg.payloadlen = strlen(str);
        pubmsg.qos = QOS;
        pubmsg.retained = 0;
        MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);

        printf("%lf %lf\n", data.latitude, data.longitude);
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);

    return EXIT_SUCCESS;
}
