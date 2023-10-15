/*
 * Safe_Lock.c
 *
 * Created: 8/31/2023 2:22:12 AM
 * Author : khater
 */ 

#include "LCD.h"
#include "keypad.h"
#include "timer.h"
volatile uint8_t key, i, mood= 1, flag=0; // mood -> case closed or opened , flag -> correct digit entered
volatile uint8_t code[4]={0}; // code to be entered to open the safe
volatile uint8_t password[4]={0}; // password of the safe
volatile uint8_t reset_pass[10]={1,1,1,1,1,1,1,1,1,1}; // the number enter to reset password 
volatile uint8_t reset_condition= 1; // case locked cant change password

int main(void)
{
	LCD_init();
	keypad_init();
	/*
	C.C = 1024/16 usec => 1024/16 usec * OCR = 100000 usec => OCR =	1562
	*/
	timer0_ctc_init(1562); // 100msec
	
	 
	while(1)
	{
	}
}

ISR(TIMER0_COMP_vect){
	////////////////////////////////
	/////////////ENTER CODE///////////
	for(i =0; i<=3; i++){
		
		while(keypad_read() == 'O'); // code numbers
		key = keypad_read();
		if(key >='0' && key <='9'){
			code[i]= key -'0';
			LCD_write_char('*');
		}
		else {
			flag = 1;
		}
			
		while(keypad_read() != 'O');
			
	}
	////////////////////////////////
	/////////PASSWORD CHECK////////
	while(keypad_read() == 'O'); // Un-Lock case
	key = keypad_read();
	if(key == '='){
		if(mood == 0){
			mood = 1;
		}
		for(i=0; i<=3 ; i++){
			if(code[i] == password [i] && flag == 0){
				mood= 0; // Case opened
			}
			
		}
		if(mood == 0){
			LCD_write_command(1);
			LCD_write_string("Case opened");
			reset_condition = 0;
		}
		else{
			LCD_write_command(1);
			LCD_write_string("wrong pass");
			mood =1;
			reset_condition =1;
		}
	}
	while(keypad_read() != 'O');
	
	////////////////////////////////
	//////////RESET MODE///////////
	while(keypad_read() == 'O'); // num
	key = keypad_read();
	if(key == '-' && mood == 0){
		LCD_write_command(1);
		LCD_write_string("reset mood");
		_delay_ms(500);
		LCD_write_command(1);
		
		for(i=0; i<=9 ; i++){
			while(keypad_read() == 'O'); // code numbers
			key = keypad_read();
			if(key == '0'){
				reset_pass[i]= key -'0';
				LCD_write_char('*');
			}
			else{
				flag = 1;
			}
			while(keypad_read() != 'O');
		}

		if(flag == 1){
			LCD_write_string("wrong pass");
			mood = 1;
			_delay_ms(500);
			LCD_write_command(1);
		}
		else{
			for(i=0; i<=3; i++){
				password[i] = 0;
			}
			LCD_write_command(1);
			LCD_write_string("password reset comp");
			_delay_ms(500);
			LCD_write_command(1);
			mood =1;
			reset_condition=1;
		}
	}
	while(keypad_read() != 'O');
	////////////////////////////////
	////////Change Password////////
	
	while(keypad_read() == 'O'); // num 
	key = keypad_read();
	if(reset_condition == 0){
		if(key =='/'){
			LCD_write_command(1);
			LCD_write_string("enter new pass");
			_delay_ms(300);
			LCD_write_command(1);
			for(i=0; i<=3; i++){
				while(keypad_read() == 'O'); // num
				key = keypad_read();
				if(key >='0' && key <='9'){
					password[i]= key -'0';
					
					LCD_write_char('*');
				}
				while(keypad_read() != 'O'); //button still pressed
			}
			LCD_write_command(1);
			LCD_write_string("pass changed");
			_delay_ms(200);
		}
	}
	else{
		LCD_write_command(1);
		LCD_write_string("unlock first");
		_delay_ms(200);	
	}
	while(keypad_read() != 'O');
	LCD_write_command(1);
	
	////////////////////////////////
   /////////////CASE LOCK///////////
	while(keypad_read() == 'O'); // Lock case
	key = keypad_read();
	if(key == 'x' && mood ==0){
		LCD_write_command(1);
		LCD_write_string("safe locked");
		mood =1;
		reset_condition =1;
		_delay_ms(300);
	}
	
	while(keypad_read() != 'O'); //button still pressed
	LCD_write_command(1);
	
	
	
	//while(keypad_read() != 'O');
}
