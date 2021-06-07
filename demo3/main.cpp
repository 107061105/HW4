#include "mbed.h"
#include "math.h"
#include "bbcar.h"
#include "bbcar_rpc.h"
#include "string.h"

#define PI 3.14159265

Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(D1,D0); //tx,rx

InterruptIn button(USER_BUTTON); //Interrupt on digital pushbutton input SW2

BBCar car(pin5, pin6, servo_ticker);

int main(){
    uart.set_baud(9600);

    while(1){
        if(uart.readable()) {
            double d1 = 0;
            double d2 = 0;
            int order = 0;
            int sign = 0;
            double distance = 0;
            double dx = 0;
            double dz = 0;
            double degree = 0;
            //double timed1 = d1 / 2 * 1000;
            //double timed2 = (d2 - 3) / 2 * 1000;
        
            char recv[1];

            uart.read(recv, sizeof(recv));
            if (recv[0] == 'd') {
                do {
                    uart.read(recv, sizeof(recv));
                    if (recv[0] == '-') {
                        sign = 1;
                    } else if (recv[0] != '.') {
                        d1 = 10 * d1 + (((int)(recv[0])) - ((int)('0')));
                    }
                } while (recv[0] != '.');
                do {
                    uart.read(recv, sizeof(recv));
                    if (recv[0] != '\r') {
                        order = order + 1;
                        double k = ((int)(recv[0])) - ((int)('0'));
                        for (int i = 0; i < order; i++) k = 0.1 * k;
                        d2 = d2 + k;
                    }
                } while (recv[0] != '\r');
                dx = d1 + d2;
                if (sign) dx = dx * -1;
                d1 = d2 = sign = order = 0;

                do {
                    uart.read(recv, sizeof(recv));
                    if (recv[0] == '-') {
                        sign = 1;
                    } else if (recv[0] != '.') {
                        d1 = 10 * d1 + (((int)(recv[0])) - ((int)('0')));
                    }
                } while (recv[0] != '.');
                do {
                    uart.read(recv, sizeof(recv));
                    if (recv[0] != '\r') {
                        order = order + 1;
                        double k = ((int)(recv[0])) - ((int)('0'));
                        for (int i = 0; i < order; i++) k = 0.1 * k;
                        d2 = d2 + k;
                    }
                } while (recv[0] != '\r');
                dz = d1 + d2;
                //dz = dz * -1;
                d1 = d2 = sign = order = 0;

                do {
                    uart.read(recv, sizeof(recv));
                    if (recv[0] == '-') {
                        sign = 1;
                    } else if (recv[0] != '.') {
                        d1 = 10 * d1 + (((int)(recv[0])) - ((int)('0')));
                    }
                } while (recv[0] != '.');
                do {
                    uart.read(recv, sizeof(recv));
                    if (recv[0] != '\r') {
                        order = order + 1;
                        double k = ((int)(recv[0])) - ((int)('0'));
                        for (int i = 0; i < order; i++) k = 0.1 * k;
                        d2 = d2 + k;
                    }
                } while (recv[0] != '\r');
                degree = d1 + d2;
                if (degree >= 270 && degree <= 360) degree = degree - 360;
                d1 = d2 = sign = order = 0;
                //printf("%f, %f, %f\r\n", dx, dz, degree);

                if (abs(dz) <= 6 && abs(dx) <= 0.8 && (abs(degree) <= 8)) {
                    car.stop();
                } else if (abs(dz) <= 5 && abs(dx) <= 0.8 && (abs(degree) <= 10)) {
                    car.stop();
                } else if (abs(degree) <= 6) {
                    if (abs(dx) > 0.2) {
                        if (dx > 0) car.follow(0, 8);
                        else car.follow(-8, 0);
                    } else if ((abs(dz) > 8) && (abs(dx) <= 0.2)) {
                        car.goStraight(30);
                    }
                } else {
                    if (dx >= 0) {
                        if (degree > 0) {
                            double speed = 6 * dx + 10 / dz + 1 * degree;
                            car.follow(30 + speed, -30);
                        } else {
                            double speed = -6 * dx + 10 / dz + 1 * degree;
                            car.follow(30, -30 - speed);
                        }
                    } else {
                        if (degree > 0) {
                            double speed = -6 * dx + 5 / dz + 1 * degree;
                            car.follow(30, -30 - speed);
                        } else {
                            double speed = 6 * dx + 5 / dz + 1 * degree;
                            car.follow(30 + speed, -30);
                        }
                    }
                }

                
            }
        }
    }

}