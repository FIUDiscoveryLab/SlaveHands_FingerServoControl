#include <vector>
#include <string>
#include <sstream>

#include "mbed.h"
#include "MODSERIAL.h"
#include "Servo.h"

/**
 * main.cpp
 * SlaveHands_FingerServoControl - main
 * This program reads input serial data and writes to Telebot's servoed appendages
 * @author Irvin Steve Cardena
 * @github  kPatch
 * @twitter ElectroFunks
 * @libraries:
 *      https://developer.mbed.org/users/jdenkers/code/Servo/docs/352133517ccc/classServo.html#a930298007c1f6dde60b2ca8abe951af7
 **/


Serial device (USBTX, USBRX);

/////////////////////////////////////////////
// Utility Functions
void flushSerialBuffer(void);
/////////////////////////////////////////////

/////////////////////////////////////////////
// MODSERIAL
/////////////////////////////////////////////
// Connect the TX of the GPS module to p10 RX input
// MODSERIAL pc(USBTX, USBRX);
 
volatile bool newline_detected = false;

// Called everytime a new character goes into
// the RX buffer. Test that character for \n
// Note, rxGetLastChar() gets the last char that
// we received but it does NOT remove it from
// the RX buffer.
void rxCallback(MODSERIAL_IRQ_INFO *q) {
    MODSERIAL *serial = q->serial;
    if ( serial->rxGetLastChar() == '\n') {
        newline_detected = true;
    }
}
/////////////////////////////////////////////

/////////////////////////////////////////////
// Serial Interrupts
/////////////////////////////////////////////
void Tx_interrupt();
void Rx_interrupt();
void send_line();
void read_line();

// Circular buffers for serial TX and RX data - used by interrupt routines
const int buffer_size = 255;
// might need to increase buffer size for high baud rates
char tx_buffer[buffer_size+1];
char rx_buffer[buffer_size+1];
// Circular buffer pointers
// volatile makes read-modify-write atomic 
volatile int tx_in=0;
volatile int tx_out=0;
volatile int rx_in=0;
volatile int rx_out=0;
// Line buffers for sprintf and sscanf
char tx_line[80];
char rx_line[80];

/////////////////////////////////////////////

// Left Hand-Finger Servos
Servo Servo21(p21);
Servo Servo22(p22);
Servo Servo23(p23);
Servo Servo24(p24);
Servo Servo25(p25);
Servo Servo26(p26);

// Right Hand-Finger Servos
Servo Servo5 (p5);          // Pinky  Finger
Servo Servo6 (p6);          // Ring   Finger
Servo Servo7 (p7);          // Middle Finger
Servo Servo8 (p8);          // Index  Finger
Servo Servo9 (p9);          // Thumb  Finger Flexion
Servo Servo10(p10);         // Thumb  Finger Opposition

//void testFinger(Servo& servo);

int main() {
    
    int i=0;
    int rx_i=0;
    device.baud(57600);
 
// Setup a serial interrupt function to receive data
    device.attach(&Rx_interrupt, Serial::RxIrq);
// Setup a serial interrupt function to transmit data
    device.attach(&Tx_interrupt, Serial::TxIrq);    
    
    device.printf("SlaveHands_FingerServoControl\n\r");
    device.printf("Created:\t06/12/2015\n\r");
    device.printf("Updated:\t06/12/2015\n\r");
    device.printf("Version:\t0.1\n\r");
    device.printf("Authors:\tIrvin Steve Cardenas (kPatch)\n\r");
    device.printf("\t\t[other_authors]\n\r");
    

    Servo5.Enable(1500,20000);
    Servo6.Enable(1500,20000);
    Servo7.Enable(1500,20000);
    Servo8.Enable(1500,20000);
    Servo9.Enable(1500,20000);
    Servo10.Enable(1500,20000);
    
    Servo21.Enable(1500,20000);
    Servo22.Enable(1500,20000);
    Servo23.Enable(1500,20000);
    Servo24.Enable(1500,20000);
    Servo25.Enable(1500,20000);
    Servo26.Enable(1500,20000);
    
    flushSerialBuffer();

    while(1) {
// Read a line from the large rx buffer from rx interrupt routine
        read_line();
// Read ASCII number from rx line buffer
        //sscanf(rx_line,"%x",&rx_i);
// Parse Command
        device.printf("%s\n\r", rx_line);
        
        char * pch; //reference to each character in the arrray param
        //int order[6]={0,0,0,0,0,0}; //Clear the array,
        int order[12]={0,0,0,0,0,0,0,0,0,0,0,0}; //Clear the array,
        
        int i = 0; //counter for looping
        pch = strtok (rx_line," ");
        //while ((pch != NULL) && ( i <= 6 )){
        while ((pch != NULL) && ( i <= 12 )){
            order[i] = atoi (pch); //refere to element in the array.(atoi - convert string to integer)
            pch = strtok (NULL, " "); //strok string tokenzier.
            i++; //
        }
        //printf ("servo5=%d servo6=%d servo7=%d servo8=%d servo9=%d servo10=%d\n\r",order[0],order[1],order[2],order[3],order[4],order[5]);

        
//        string buf; // Have a buffer string
//        stringstream ss(rx_line); // Insert the string into a stream
//        vector<string> tokens; // Create vector to hold our words
//        while (ss >> buf) {
//            tokens.push_back(buf);
//        }

        Servo5.SetPosition(order[0]);
        Servo6.SetPosition(order[1]);
        Servo7.SetPosition(order[2]);
        Servo8.SetPosition(order[3]);
        Servo9.SetPosition(order[4]);
        Servo10.SetPosition(order[5]);
        
//        Servo21.SetPosition(order[0]);
//        Servo22.SetPosition(order[1]);
//        Servo23.SetPosition(order[2]);
//        Servo24.SetPosition(order[3]);
//        Servo25.SetPosition(order[4]);
//        Servo26.SetPosition(order[5]);

        Servo21.SetPosition(order[6]);
        Servo22.SetPosition(order[7]);
        Servo23.SetPosition(order[8]);
        Servo24.SetPosition(order[9]);
        Servo25.SetPosition(order[10]);
        Servo26.SetPosition(order[11]);        
        
//        Print the pwm servo information
//        printf ("servo5=%d servo6=%d servo7=%d servo8=%d servo9=%d servo10=%d\n\r",order[0],order[1],order[2],order[3],order[4],order[5]);


//        for (int pos = 1000; pos < 2000; pos += 25) {
//            Servo5.SetPosition(pos);
//            wait_ms(20);
//            Servo6.SetPosition(pos);
//            wait_ms(20);
//            Servo7.SetPosition(pos);
//            wait_ms(20);
//            Servo8.SetPosition(pos);
//            wait_ms(20);
//            Servo9.SetPosition(pos);
//            wait_ms(20);
//            Servo10.SetPosition(pos);
//            wait_ms(20);
//        }
//        
//        for (int pos = 2000; pos > 1000; pos -= 25) {
//            Servo5.SetPosition(pos);
//            //wait_ms(20);
//            wait_ms(20);
//            Servo6.SetPosition(pos);
//            wait_ms(20);
//            Servo7.SetPosition(pos);
//            wait_ms(20);
//            Servo8.SetPosition(pos);
//            wait_ms(20);
//            Servo9.SetPosition(pos);
//            wait_ms(20);
//            Servo10.SetPosition(pos);
//            wait_ms(20);
//        }
    }
}

//void testFinger(Servo& servo)
//{
//    for (int pos = 1000; pos < 2000; pos += 25) {
//        &servo->SetPosition(pos);
//        wait_ms(20);
//    }
//    
//    for (int pos = 2000; pos > 1000; pos -= 25) {
//        &servo->SetPosition(pos);
//        wait_ms(20);
//    }
//}


// Read a line from the large rx buffer from rx interrupt routine
void read_line() {
    int i;
    i = 0;
// Start Critical Section - don't interrupt while changing global buffer variables
    NVIC_DisableIRQ(UART1_IRQn);
// Loop reading rx buffer characters until end of line character
    while ((i==0) || (rx_line[i-1] != '\r')) {
// Wait if buffer empty
        if (rx_in == rx_out) {
// End Critical Section - need to allow rx interrupt to get new characters for buffer
            NVIC_EnableIRQ(UART1_IRQn);
            while (rx_in == rx_out) {
            }
// Start Critical Section - don't interrupt while changing global buffer variables
            NVIC_DisableIRQ(UART1_IRQn);
        }
        rx_line[i] = rx_buffer[rx_out];
        i++;
        rx_out = (rx_out + 1) % buffer_size;
    }
// End Critical Section
    NVIC_EnableIRQ(UART1_IRQn);
    rx_line[i-1] = 0;
    return;
}

// Interupt Routine to read in data from serial port
void Rx_interrupt() {
//    led1=1;
// Loop just in case more than one character is in UART's receive FIFO buffer
// Stop if buffer full
    while ((device.readable()) && (((rx_in + 1) % buffer_size) != rx_out)) {
        rx_buffer[rx_in] = device.getc();
// Uncomment to Echo to USB serial to watch data flow
//        monitor_device.putc(rx_buffer[rx_in]);
        rx_in = (rx_in + 1) % buffer_size;
    }
//    led1=0;
    return;
}
 
 
// Interupt Routine to write out data to serial port
void Tx_interrupt() {
//    led2=1;
// Loop to fill more than one character in UART's transmit FIFO buffer
// Stop if buffer empty
    while ((device.writeable()) && (tx_in != tx_out)) {
        device.putc(tx_buffer[tx_out]);
        tx_out = (tx_out + 1) % buffer_size;
    }
//    led2=0;
    return;
}

// Utility function to flush serial buffer
void flushSerialBuffer(void) { 
    char char1; 
    while (device.readable()) 
    { 
        char1 = device.getc(); 
    } 
    return; 
} 