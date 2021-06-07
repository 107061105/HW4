#include "mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"
#include "string.h"

Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(D1,D0); //tx,rx

InterruptIn button(USER_BUTTON); //Interrupt on digital pushbutton input SW2
bool c = false;

BBCar car(pin5, pin6, servo_ticker);

void confirm() {
    c = true;
}

int main(){
    uart.set_baud(9600);
    
    car.stop();
    ThisThread::sleep_for(2000ms);
    car.follow(20, 20);
    while(1){
        if(uart.readable()) break;
    }

    while(1){
        if(uart.readable()) {
            int value = 0;
            int sign = 0;
            char recv[1];
        
            uart.read(recv, sizeof(recv));
            if (recv[0] == 's') {
                car.stop();
            } else if (recv[0] == 'f') {
                car.follow(20, 20);
            } else if (recv[0] == 'r') {
                do {
                    uart.read(recv, sizeof(recv));
                    if (recv[0] == '-') {
                        sign = 1;
                    } else if (recv[0] != '\r') {
                        value = 10 * value + (((int)(recv[0])) - ((int)('0')));
                    }
                } while (recv[0] != '\r');

                if (value > 10) {
                    if (sign) value = value * -1;
                    car.follow(20.0 + value / 10, -(20.0 - value / 10));
                } else {
                    if (sign) value = value * -1;
                    car.follow(30.0 + value, -(30.0 - value));
                }
            }
        }
    }
}

