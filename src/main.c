/**************************************************************
Authors: 
Søren Møller Christensen 
Nicklas Stubkjær Holm

This code can detect metals by analyzing the signal amplitude output of a
simple LC amplifier circuit. This is due to the magnectic field of the coil
inducing the nearby metal, which will change the permeability of the core of the coil, 
and thus the frequency and amplitude of the LC's circuit signal will change aswell, as we have

f = 1/(2*pi*sqrt(L*C))

where L is dependent on the permeability.

Project was created in the Digital Intrumentation class at the Technical University of Denmark


Microcontroller used: PIC16F877P
See schematic in picture folder 
**************************************************************/

#include <main.h>

//Declaring variables
//Calibrated values
unsigned int16 cal_min = 0;
unsigned int16 cal_max = 0;
unsigned int16 cal_amp = 0;

//Temporary amplitude calculated when program is executing
unsigned int16 temp_amp = 0;

//Variable to tell if metal is detected (0: No metal, 1 : Metal)
int metal = 0;

//Declaring functions
int button_press();
void setup();
void calibrate();
void search_metal();

void main(){
	//Variable for text that will be printed
	char buf[17];
	
	//Calling setup function to be run once
	setup();
   
	//Clearing LCD display and displaying "Program started"
	lcd_clear();
	lcd_gotoxy(1,1);
	sprintf(buf, "Program started");
	lcd_print(buf);
	
	//Waiting 8s for the circuit to stabilize
	delay_ms(8000);
	
	//Performing first calibration
	calibrate();
   
	//Program code to run forever
	while(1){
		//Searching for metal
        search_metal();
        
		//If the button is pressed, then perform calibration
        if(button_press()){
			calibrate();
        }
        
		//If metal is detected, then display that metal is detected
        if(metal == 1){					//Checking if metal is detected
            lcd_clear();
            lcd_gotoxy(1,1);
            sprintf(buf, "Metal detected!");
            lcd_print(buf);
            lcd_gotoxy(1,2);
            sprintf(buf, "%ld - %ld   ", cal_amp, temp_amp);
            lcd_print(buf);
            pwm_set_frequency(50);			//Setting frequency to 50 Hz to make buzzer go off
			output_high(PIN_D1);			//Turning on the LED diode

        } else{		//If metal is not detected, then display that metal is not detected
            lcd_clear();
            lcd_gotoxy(1,1);
            sprintf(buf, "Searching");
            lcd_print(buf);
            lcd_gotoxy(1,2);
            sprintf(buf, "%ld - %ld   ", cal_amp, temp_amp);
            lcd_print(buf);
            pwm_set_frequency(30000);		//Setting frequency to 30 kHz
            output_low(PIN_D1);				//Turning off the LED diode
        }
	}
}

//Function to check if a button is pressed (0 : No button pressed, 1 : Button pressed)
int button_press(){
	if (input(PIN_D0)){
		return 1;
	}
	return 0;
}

//Function to search for metal
void search_metal(){
	//Declaring variables
	unsigned int16 temp_min = 1023;
	unsigned int16 temp_max = 0;
	unsigned int16 temp = 0;
	unsigned int16 j = 0;
   
	//Performing 2000 analog measurements and finds the min and max values
	for(j = 0; j < 2000; j++){
		temp = read_adc();
		if(temp > temp_max){
			temp_max = temp;
		}
		if(temp < temp_min){
			temp_min = temp;
		}	
	}
	
	//Calculating the amplitude
	temp_amp = temp_max - temp_min;
   
	//If the calculated amplitude is lower than the calibrated amplitude (Plus a safety margin), then metal is detected
	if((temp_amp + 5) < cal_amp){
		metal = 1;
	} else{
		metal = 0;
	}
}

//Function to calibrate the amplitude
void calibrate(){
	//Declaring variables
	unsigned int16 j = 0; 
	float temp = 0;
	char buf[17];
   
	//Printing "Calibrating" to LCD display
	lcd_gotoxy(1,1);
	sprintf(buf, "Calibrating");
	lcd_print(buf);
	
	//Setting min value to 10000 and max to 0
	cal_min = 10000;
	cal_max = 0;
	
	//Performing 5000 analog measurements to find the min and max values
	for(j = 0; j < 5000; j++){
		temp = read_adc();
		if(temp > cal_max){
			cal_max = temp;
		}	
		if(temp < cal_min){
			cal_min = temp;
		}
	}
	
	//Calculating the calibrated amplitude
	cal_amp = cal_max - cal_min;
	
	//Printing "Calibration done"
	lcd_gotoxy(1,1);
	sprintf(buf, "Calibration done");
	lcd_print(buf);
}

//Function to set up the PIC processor and periharials
void setup(){
	//Setup
	setup_oscillator(OSC_8MHz, 12);
	setup_ccp1(CCP_PWM);
	setup_timer_1 ( T1_INTERNAL | T1_DIV_BY_2 ); // setting timer to 1 MHz. The timer will therefore count up with a time interval of 1 �s
	setup_timer_0 ( T0_INTERNAL | T1_DIV_BY_1 ); //Setting the timer to 2 MHz, the timer will count up every 0.5 �s
	setup_timer_2(T2_DIV_BY_1,199,1);
	enable_interrupts(int_timer1);
	enable_interrupts(global); // turns on all enabled interrupts
	lcd_init();
	lcd_clear();
   
	//Setting SDA og SCL as open collector
	output_float(PIN_C4);
	output_float(PIN_C3);
   
	//Setup ADC as extern reference (For at kunne kommunikere med LM35)
	setup_vref(0xE2);
	setup_adc(ADC_CLOCK_DIV_32);
	setup_adc_ports(sAN6 | VSS_VDD);
	set_adc_channel(6);
	
	//Set PWM frequency out of the hearable spectrum
	pwm_set_frequency(30000);
	
	//Set pin low
	output_low(PIN_D1);
}
