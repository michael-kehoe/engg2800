#include <util/delay.h>

void setup() {                
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(13, OUTPUT);     
}

void Dot()
{
  digitalWrite(13, HIGH);   // set the LED on
  _delay_ms(140);              // wait for a second
  digitalWrite(13, LOW);    // set the LED off
  _delay_ms(100);     
}
void Dash()
{
   digitalWrite(13, HIGH);   // set the LED on
  _delay_ms(380);              // wait for a second
  digitalWrite(13, LOW);    // set the LED off
  _delay_ms(100);     

}

void Intro()
{
  Dot();
  Dash();
  
  Dot();
  Dash();
  
  Dot();
  Dash();
  
  Dot();
  Dash();
  
  Dot();
  Dash();
  
}

void Message(char test)
{
  switch(test)
  {
    case 'A':
    Dot();
    Dash();
    break;
    case 'B':
    Dash();
    Dot();
    Dot();
    Dot();
    break;
    case 'C':
    Dash();
    Dot();
    Dash();
    Dot();
    break;
    case 'D':
    Dash();
    Dot();
    Dot();
    break;
    case 'E':
    Dot();
    break;
    case 'F':
    Dot();
    Dot();
    Dash();
    Dot();
    break;
    case 'G':
    Dash();
    Dash();
    Dot();
    break;
    case 'H':
    Dot();
    Dot();
    Dot();
    Dot();
    break;
    case 'I':
    Dot();
    Dot();
    break;
    case 'J':
    Dot();
    Dash();
    Dash();
    Dash();
    break;
    case 'K':
    Dash();
    Dot();
    Dash();
    break;
    case 'L':
    Dot();
    Dash();
    Dot();
    Dot();
    break;
    case 'M':
    Dash();
    Dash();
    break;
    case 'N':
    Dash();
    Dot();
    break;
    case 'O':
    Dash();
    Dash();
    Dash();
    break;
    case 'P':
     Dot();
     Dash();
     Dash();
     Dot();
    break;
    case 'Q':
    Dash();
    Dash();
    Dot();
    Dash();
    break;
    case 'R':
    Dot();
    Dash();
    Dot();
    break;
    case 'S':
    Dot();
    Dot();
    Dot();
    break;
    case 'T':
    Dash();
    break;
    case 'U':
    Dot();
    Dot();
    Dash();
    break;
    case 'V':
    Dot();
    Dot();
    Dot();
    Dash();
    break;
    case 'W':
    Dot();
    Dash();
    Dash();
    break;
    case 'X':
    Dash();
    Dot();
    Dot();
    Dash();
    break;
    case 'Y':
    Dash();
    Dot();
    Dash();
    Dash();
    break;
    case 'Z':
    Dash();
    Dash();
    Dot();
    Dot();
    break;
  }
      
}
void SendMessage(const char str[])
{
   Intro();
   _delay_ms(940);
   for(int i=0; i < strlen(str);i++)
   {
     Message(str[i]);
     _delay_ms(250);
   }
   _delay_ms(5000);
   Dot();
   _delay_ms(250);
   Dash();
   Dot();
   _delay_ms(250);
    Dash();
   Dot();
   Dot();
    _delay_ms(10000);
}

void loop()
{
 SendMessage("THE");
 SendMessage("LABS");
 SendMessage("CLEAN");
}

