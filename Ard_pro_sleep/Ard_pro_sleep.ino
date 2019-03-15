// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       Ard_pro_sleep.ino
    Created:	3/14/2019 10:10:49 PM
    Author:     AzureAD\pnmagalhaes
*/

// Define User Types below here or use a .h file
//


// Define Function Prototypes that use User Types below here or use a .h file
//


// Define Functions below here or use other .ino or cpp files
//
#include <avr/sleep.h>
#define INTERRUPT_PIN 2
// The setup() function runs once each time the micro-controller starts
void setup()
{
	Serial.begin(115200);
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(INTERRUPT_PIN, INPUT_PULLUP);
	digitalWrite(LED_BUILTIN, HIGH);
	delay(500);
	Serial.println("IoTetris: I am wake!! Yeah");
	delay(500);	//delay needed

}

// Add the main program code into the continuous loop() function
void loop()
{
	delay(2000);
	go_to_sleep();

}
void go_to_sleep()
{
	sleep_enable();
	attachInterrupt(0, wakeUp_function, LOW);
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	digitalWrite(LED_BUILTIN, LOW);
	delay(500);
	Serial.println("Gooing to sleep");
	delay(100);
	sleep_cpu();
	Serial.println("Continua aqui depois de atender a interrupcao");
	digitalWrite(LED_BUILTIN, HIGH);
}
void wakeUp_function()
{
	Serial.println("IoTetris: Now I am working in my tasks!");
	sleep_disable();
	detachInterrupt(0);
}