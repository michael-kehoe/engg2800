#include "print.h"
#include "digital.h"
#include "main.h"
#include "HSerial.h"
#include "time.h"
#include "analog.h"
#include "proto-math.h"
#include "interrupt.h"
#include <stdio.h> // for size_t
#include <stdlib.h>
#include <string.h>
#include "power.h"




#define RSSI 1 //RSSI ADC
#define POT 0 //POT ADC
#define RX2A 2 //RX2A ADC
#define LCDE 3 //LCD ENABLE
#define LED 4 //POWER
#define MLED 13 //MORSE LED
#define LCD
//#define DEBUG0
//#define DEBUG
//#define DEBUG1
//#define DEBUG3
#define DEBUG4
//#define DEBUG5
#define final
unsigned int drn_reset = 0;
unsigned long time_count = 0;
uint8_t input_state = 0;
unsigned long morsecode = 0;
uint8_t message_length = 0;
char message[14];
uint8_t message_counter = 0;
char beacons[10];
unsigned int highest_direction =0;
unsigned int highest_RSSI=0;
int8_t NO_Transmitters = -1;

void Setup()
{
	power_twi_disable();
	power_spi_disable();
	power_timer1_disable();
	power_timer2_disable();
	HSerialBegin(9600,8,0,1);
	AnalogBegin(DEFAULT);
	ClearTerminal();
	print("GROUP 5");
	println();
	drn_reset = AnalogRead(POT,2);
	PinMode(MLED, OUTPUT);
	PinMode(LED, OUTPUT);
	PinMode(16, INPUT);
	PinMode(17, INPUT);
	DigitalWrite(LED, HIGH);
	DelayMilliseconds(1000);
	InitTimer(0,interrupt, 5,155);

}

int main (void)
{
	Setup();
	while(1)
    {
		MoveCursor(0,2);
		CTEOF();
		print("Direction: ");
		print(Direction(drn_reset),DEC);
		print("  Signal Strength: ");
		print(SStrength(),DEC);
		DelayMilliseconds(500);
    }
}
int Direction(int drn_reset)
{

	int current_value = AnalogRead(POT,1);
	if(current_value > drn_reset)
	{
		return (unsigned int) (360-(((current_value-drn_reset)*10)/(42)));
	}
	else if(drn_reset > current_value)
	{
		return (unsigned int) (((drn_reset-current_value)*10)/(42));
	}
	else return 0;
}
int SStrength()
{
	return Map(AnalogRead(RSSI,1), 0, 640, 0, 100);
}
void interrupt()
{
	Find_Antenna(Direction(drn_reset), AnalogRead(RSSI,1));
	uint8_t locvar = DigitalRead(RX2A);
	DigitalWrite(13,DigitalRead(RX2A));
	if(input_state != locvar)
	{
		if(input_state == HIGH)
		{
			#ifdef DEBUG0
			print("ON ");
			print(time_count);
			println();
			#endif
			if((time_count >=7)&&(time_count<11)) //DOT
				{
					#ifdef DEBUG
					print("DOT");
					println();
					#endif
					morsecode = morsecode * 10;
					morsecode += 1;
					#ifdef DEBUG1
					print("MC: ");
					print(morsecode,DEC);
					println();
					#endif
				}
			else if((time_count >=21)&&(time_count<25)) //DASH
				{
					#ifdef DEBUG
						print("DASH");
						println();
					#endif
					morsecode = morsecode * 10;
					morsecode += 2;
					#ifdef DEBUG1
						print("MC: ");
						print(morsecode,DEC);
						println();
					#endif
				}
		}
		else
		{
			#ifdef DEBUG0
			print("OFF ");
			print(time_count);
			println();
			#endif
			if((time_count >=62)&&(time_count<66)) //1 SECOND GAP
				{
					#ifdef DEBUG
						print("1 SECOND GAP");
						println();
					#endif

					#ifdef DEBUG3
						print(c);
						println();
					#endif
					morsecode = 0;
				}
			else if((time_count >=20)&&(time_count<24)) //CHARACTER GAP
				{
					#ifdef DEBUG
						print("CHARACTER GAP");
						println();
					#endif
					if(intlen(morsecode) < 6)
						{
						char c =  DecodeCharacter(morsecode);
						strncat(message, &c, 1);
						message_length++;
						#ifdef DEBUG5
							print("Message: ");
							print(message);
							print("   messlen: ");
							print(message_length,DEC);
							println();
						#endif
						}
					#ifdef DEBUG3
						print(c);
						println();
					#endif
					morsecode = 0;
				}
			else if((time_count >=62)&&(time_count<400)) //AFTER WORD GAP
						{
							#ifdef DEBUG
								print("AFTER WORD GAP");
								println();
							#endif
							if(intlen(morsecode) < 6)
								{
								char c =  DecodeCharacter(morsecode);
								strncat(message, &c, 1);
								message_length++;
							#ifdef DEBUG5
							print("Message: ");
							print(message);
							print("   messlen: ");
							print(message_length,DEC);
							println();
							#endif
								}
							#ifdef DEBUG3
								print(c);
								println();
							#endif
							morsecode = 0;
						}
			else if((time_count>401)) //EOT
						{
							#ifdef DEBUG
								print("END OF TRANSMISSION");
								println();
							#endif
							if(intlen(morsecode) < 6)
								{
								char c =  DecodeCharacter(morsecode);
								strncat(message, &c, 1);
								message_length++;
								#ifdef DEBUG5
								print("Message: ");
								print(message);
								print("   messlen: ");
								print(message_length,DEC);
								println();
								#endif
								}
							#ifdef DEBUG3
								print(c);
								println();
							#endif
							#ifdef final
							//SPIT OUT THE DECODED MESSAGE
							morsecode = 0;
							analyse();
							MoveCursor(0,5+NO_Transmitters);
							CTEOF();
							message_counter++;
							print("ID ");
							//print(message_counter,DEC);
							print(NO_Transmitters+1,DEC);
							print(" Bearing ");
							//print(0,DEC);
							print(beacons[NO_Transmitters-1],DEC);
							print(" Message ");
							print(message);
							#endif
							#ifdef DEBUG4
								message[0] = '\0';
								message_length = 0;
							#endif
						}
		}
		time_count = 0;
		input_state = locvar;
		time_count++;
	}
	else
	{
		time_count++;
	}
}


char DecodeCharacter(int buf)
{
	if(buf == 12)
		{
			return 'A';
		}
	else if(buf == 2111)
		{
			return 'B';
		}
	else if(buf == 2121)
		{
			return 'C';
		}
	else if(buf == 211)
		{
			return 'D';
		}
	else if(buf == 1)
		{
			return 'E';
		}
	else if(buf == 1121)
		{
			return 'F';
		}
	else if(buf == 221)
		{
			return 'G';
		}
	else if(buf == 1111)
		{
			return 'H';
		}
	else if(buf == 11)
		{
			return 'I';
		}
	else if(buf == 1222)
		{
			return 'J';
		}
	else if(buf == 212)
		{
			return 'K';
		}
	else if(buf == 1211)
		{
			return 'L';
		}
	else if(buf == 22)
		{
			return 'M';
		}
	else if(buf == 21)
		{
			return 'N';
		}
	else if(buf == 222)
		{
			return 'O';
		}
	else if(buf == 1221)
		{
			return 'P';
		}
	else if(buf == 2212)
		{
			return 'Q';
		}
	else if(buf == 121)
		{
			return 'R';
		}
	else if(buf == 111)
		{
			return 'S';
		}
	else if(buf == 2)
		{
			return 'T';
		}
	else if(buf == 112)
		{
			return 'U';
		}
	else if(buf == 1112)
		{
			return 'V';
		}
	else if(buf == 122)
		{
			return 'W';
		}
	else if(buf == 2112)
		{
			return 'X';
		}
	else if(buf == 2122)
		{
			return 'Y';
		}
	else if(buf == 2211)
		{
			return 'Z';
		}
	/*else if(buf == 22222)
		{
			return 0;
		}
	else if(buf == 12222)
		{
			return 1;
		}
	else if(buf == 11222)
		{
			return 2;
		}
	else if(buf == 11122)
		{
			return 3;
		}
	else if(buf == 1112)
		{
			return 4;
		}
	else if(buf == 11111)
		{
			return 5;
		}
	else if(buf == 211111)
		{
			return 6;
		}
	else if(buf == 22111)

			return 7;
		}
	else if(buf == 22211)
		{
			return 8;
		}
	else if(buf == 22221)
		{
			return 9;
		}*/
	else return '?';
}
void analyse()
{
	if(((message[message_length-1])=='D') && ((message[message_length-2])=='N') && ((message[message_length-3])=='E'))
	{
		message[message_length-3] = '\0';
	}
}

int intlen(int start) {
    int end = 0;
    while(start > 0)
    {
        start = start/10;
        end++;
    }
    return end;
}

void Find_Antenna(int current_direction, int current_RSSI)
{
	if ((((current_direction-highest_direction) < 10)||  ((highest_direction -current_direction) < 10)) && (current_RSSI > highest_RSSI))
	  			{
					highest_direction = current_direction;
					highest_RSSI = current_RSSI;
	  				Update_Transmitters(current_direction);
					#ifdef DEBUG
					print("Update: ");
					print(current_direction,DEC);
					println();
					#endif
	  			}
			else if ((((current_direction-highest_direction) > 10)||  ((highest_direction -current_direction) > 10)) && (current_RSSI > highest_RSSI))
	  			{
	  				highest_direction = current_direction;
					highest_RSSI = current_RSSI;
					Create_Transmitter(current_direction);
					#ifdef DEBUG
					print("Create: ");
					print(current_direction,DEC);
					println();
					#endif


	  			}
}

void Create_Transmitter(int Direction)
{
	NO_Transmitters++;
	beacons[NO_Transmitters - 1] = Direction;
}
void Update_Transmitters(int Direction)
{
	for(uint8_t k=0; k < 10; k++)
	{
		if(((beacons[k] - Direction) < 10) || (((Direction - beacons[k]) < 10)))
		{
			beacons[k] = Direction;
		}
	}
}


