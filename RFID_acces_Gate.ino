/* SMART AGRICULTURE PROJECT Applicatios (Arduino code)
# 3 phase detection
# soil moisture detection
# rfid access to enter the field (gate open and close)
# fensing security for entire field 
# security alarm for unauthorised access (for RFID and Fensing)
# Motor for water supply
*/

#include <RFID.h>

/*
* Read a card using a mfrc522 reader on your SPI interface
* Pin layout should be as follows (on Arduino Uno):
* MOSI: Pin 11 / ICSP-4
* MISO: Pin 12 / ICSP-1
* SCK: Pin 13 / ISCP-3
* SS/SDA: Pin 10
* RST: Pin 9
*/

#include <SPI.h>

#include <RFID.h>
#include <Servo.h>
#define SS_PIN 10
#define RST_PIN 9
// Add library  (Kütüphaneyi Ekleyin)

RFID rfid(SS_PIN,RST_PIN);

char uartdata;
int dcmotor = 7;
int buzzer = 5;
int fencing = 6;
int flag=0;
Servo name_servo;
int serNum[5];
int cards[][5] = {                  //Registered RFID cards
    {118,41,18,179,254},
    {64,181,48,28,217}

};

bool access = false;

void setup()
{
    name_servo.attach (3);          // Define the servo signal pins (PWM 3-5-6-9-10-11)
    Serial.begin(9600);

    SPI.begin();
    rfid.init();

    pinMode(dcmotor, OUTPUT);
    pinMode(fencing, INPUT);

    pinMode(buzzer, OUTPUT);
    digitalWrite(dcmotor, LOW);
}

void loop()
{

    if(rfid.isCard()) {

        if(rfid.readCardSerial()) {

            for(int x = 0; x < sizeof(cards); x++) {
                for(int i = 0; i < sizeof(rfid.serNum); i++ ) {
                    if(rfid.serNum[i] != cards[x][i]) {
                        access = false;
                        break;
                    } else {
                        access = true;
                    }
                }
                if(access) break;
            }
        }

        if(access) {                 // if authorised access
            Serial.print('s');
            name_servo.write (180);  // open the gate to enter the field
            delay(2000);
            name_servo.write (0);

        }
        if(access==false) {         // un authorised access
            Serial.print('n');
            digitalWrite(buzzer, HIGH);
        }
    }
    if (Serial.available()) { // Uart data reading
        uartdata = Serial.read();
        //Serial.println(uartdata);
        if(uartdata == 'a') { // for buzzer ON
            digitalWrite(buzzer, HIGH);
            flag =1;

        }
        if(uartdata == 'b') { // for buzzer OFF
            digitalWrite(buzzer, LOW);
            flag =0;
        }
        if(uartdata == 'c') { // for dcmotor ON
            digitalWrite(dcmotor, HIGH);
        }
        if(uartdata == 'd') { // for dcmotor OFF
            digitalWrite(dcmotor, LOW);
        }
    }

    if(flag == 0) {
        if(digitalRead(fencing)==0) { // fencing data
            digitalWrite(buzzer, HIGH); // security alarm
            Serial.print('f');          // sesnding the data to wizwiki W750SR via UART

        }
    }


}

    
    
    



