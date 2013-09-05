/*
 * main.h
 *
 *  Created on: 22/12/2010
 *      Author: build
 */

#ifndef MAIN_H_
#define MAIN_H_
void interrupt();
void Setup();
int Direction(int drn_reset);
int intlen(int start);
char DecodeCharacter(int buf);
void analyse();
int SStrength();
char DecodeCharacter(int buf);
void Find_Antenna( int current_direction, int current_RSSI);
void Update_Transmitters(int Direction);
void Create_Transmitter(int Direction);


#endif /* MAIN_H_ */

