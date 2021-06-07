#include "mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"
#include "string.h"

Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
BufferedSerial xbee(D1, D0);
void park(Arguments *in, Reply *out);
RPCFunction rpcpark(&park, "park");

BBCar car(pin5, pin6, servo_ticker);

int main() {
   char buf[256], outbuf[256];
   FILE *devin = fdopen(&xbee, "r");
   FILE *devout = fdopen(&xbee, "w");
   while (1) {
      memset(buf, 0, 256);
      for( int i = 0; ; i++ ) {
         char recv = fgetc(devin);
         if(recv == '\n') {
            printf("\r\n");
            break;
         }
         buf[i] = fputc(recv, devout);
      }
   RPC::call(buf, outbuf);
   }
}

void park(Arguments *in, Reply *out) {
    int d1 = in->getArg<double>();
    int d2 = in->getArg<double>();
    const char *direction = in->getArg<const char*>();

    /*car.goStraight(-100);
    ThisThread::sleep_for(1000ms);
    car.stop();*/

    double timed1 = (d1 + 10) / 16.5 * 1000;
    double timed2 = (d2 + 2) / 16.5 * 1000;

    if (strcmp(direction, "west") == 0) {
        car.goStraight(-100);
        ThisThread::sleep_for(timed1);
        car.stop();
        ThisThread::sleep_for(1000ms);
        car.turn(100,-0.01);
        ThisThread::sleep_for(1000ms);
        car.stop();
        ThisThread::sleep_for(1000ms);
        car.goStraight(-100);
        ThisThread::sleep_for(timed2);
        car.stop();
        return;
    } else if (strcmp(direction, "east") == 0) {
        car.goStraight(-100);
        ThisThread::sleep_for(timed1);
        car.stop();
        ThisThread::sleep_for(1000ms);
        car.turn(100,0.01);
        ThisThread::sleep_for(1000ms);
        car.stop();
        ThisThread::sleep_for(1000ms);
        car.goStraight(-100);
        ThisThread::sleep_for(timed2);
        car.stop();
        return;
    } /*else if (strcmp(direction, "north") == 0) {
        car.goStraight(-100);
        ThisThread::sleep_for(timed1);
        car.stop();
        car.turn(100,-0.01);
        ThisThread::sleep_for(1000ms);
        car.stop();
        car.goStraight(-100);
        ThisThread::sleep_for(timed2);
        car.stop();
        return;
    } else if (strcmp(direction, "south") == 0) {
        car.goStraight(-100);
        ThisThread::sleep_for(timed1);
        car.stop();
        car.turn(100,-0.01);
        ThisThread::sleep_for(1000ms);
        car.stop();
        car.goStraight(-100);
        ThisThread::sleep_for(timed2);
        car.stop();
        return;
    }*/
}
