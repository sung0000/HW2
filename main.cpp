#include "mbed.h"
#include "math.h"
AnalogOut Aout(DAC0_OUT);
AnalogIn Ain(A0);
Serial pc( USBTX, USBRX );
BusOut display(D6, D7, D9, D10, D11, D5, D4, D8);
DigitalIn  Switch(SW3);
DigitalOut redLED(LED1);
DigitalOut greenLED(LED2);
char table[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
int sample = 1200;
float ADCdata[1200];
float f, t;
int f2, n, flag;

int main(){
    for (int i = 0; i < sample; i++){
        ADCdata[i] = Ain;
        wait(1./1000);
    }
    for (int i = 0; i < sample; i++){
        if (ADCdata[i] <= 0.01 && flag == 0){
            t = i;
            flag = 1;
        }else if (ADCdata[i] <= 0.01){
            t = i - t;
            break;
        }
    }
    for (int i = 0; i < sample; i++){
        pc.printf("%1.3f\r\n", ADCdata[i]);
        wait(0.1);
    }
    f = 1000/t;
    f2 = round(f);
    while(1){
        for(float i=0; i<2; i+=0.025){
            Aout = 0.5 + 0.5*sin(i*3.14159);
            wait(1./(80*f));
        }

        if (Switch == 1){
            redLED = 1;
            greenLED = 0;
            display = 0;
        }else {
            redLED = 0;
            greenLED = 1;
            n = f2 / 100;
            display = table[n];
            wait(0.5);
            n = (f2 % 100) / 10;
            display = table[n];
            wait(0.5);
            n = f2 % 10;
            display = table[n] + 0x80;
            wait(0.5);  
        }
    }
}