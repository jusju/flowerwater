
#include <GPRS_Shield_Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>

#define PIN_TX    7
#define PIN_RX    8
#define BAUDRATE  9600
#define PHONE_NUMBER "0405209879"
#define MESSAGE  "starting automatic water system"

#define MESSAGE_LENGTH 160
char message[MESSAGE_LENGTH];
int messageIndex = 0;

char phone[16];
char datetime[24];

GPRS gprs(PIN_TX, PIN_RX, BAUDRATE); //RX,TX,BaudRate

void setup() {
    gprs.checkPowerUp();
    Serial.begin(9600);

    while (!gprs.init()) {
        delay(1000);
        Serial.println("Initialization failed!");
    }

    while (!gprs.isNetworkRegistered()) {
        delay(1000);
        Serial.println("Network has not registered yet!");
    }

    Serial.println("gprs initialize done!");
    Serial.println("start to send message ...");

    if (gprs.sendSMS(PHONE_NUMBER, MESSAGE)) { //define phone number and text
        Serial.print("Send SMS Succeed!\r\n");
    } else {
        Serial.print("Send SMS failed!\r\n");
    }
}

void loop() {
    messageIndex = gprs.isSMSunread();
    if (messageIndex > 0) { //At least, there is one UNREAD SMS
        gprs.readSMS(messageIndex, message, MESSAGE_LENGTH, phone, datetime);
        //In order not to full SIM Memory, is better to delete it
        gprs.deleteSMS(messageIndex);
        Serial.print("From number: ");
        Serial.println(phone);
        Serial.print("Datetime: ");
        Serial.println(datetime);
        Serial.print("Recieved Message: ");
        Serial.println(message);
    }
}
