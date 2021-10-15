#include <avr/io.h>
#include "servo.h"
#include <util/delay.h>
#include <stdbool.h>
#include "h_bridge.h" // DC_motor

#define Eindschakelaar_DC_MOTOR1 (1 << PK2) // DC_MOTOR NO eindschakelaar BENEDEN
#define Eindschakelaar_DC_MOTOR2 (1 << PK3)// DC_MOTOR NO eindschakelaar BOVEN

#define button_1 (1 << PF1)
#define button_2 (1 << PF2)
#define button_3 (1 << PF3)
/*
#define led_test1 (1 << PB7)
#define led_test2 (1 << PB6)
#define led_test3 (1 << PB5)
#define led_test4 (1 << PB4)
*/
#define led_1 (1 << PJ1)       //GEVAREN LICHT
#define led_geel1 (1 << PC4)
#define led_geel2 (1 << PH0)
#define led_geel3 (1 << PD3)
#define led_geel4 (1 << PD0)
#define led_rood12 (1 << PJ0)
#define led_rood34 (1 << PH1)
#define led_groen12 (1 << PD2)
#define led_groen34 (1 << PD1)



#define Lichtscherm_aankomend_vaarverkeer_1 (1 << PK0)
#define Lichtscherm_aankomend_vaarverkeer_2 (1 << PK1)

//#define Anemometer (1 << PK1) //avr_analog_pin_9_anemometer

void init(void)

{
    init_h_bridge();
}

void motor_brug_omhoog ()
{
        h_bridge_set_percentage(60);
        _delay_ms(100);
}

void motor_brug_omlaag ()
{
        h_bridge_set_percentage(-60);
        _delay_ms(100);

}

void motor_brug_stilstand ()
        {
            h_bridge_set_percentage(0);
            _delay_ms(100);
        }


/////////////////////////////////Slagbomen gaan open/////////////////////////////////////////////////////////////////////////
void ServoOpen(int ServoPercentage)
{
init_servo();
servo1_set_percentage(ServoPercentage);
servo2_set_percentage(ServoPercentage);
}


//////////////////////////////////Slagbomen gaan dicht////////////////////////////////////////////////////////////////////////
void ServoDicht(int ServoPercentage)
{
init_servo();
servo1_set_percentage(ServoPercentage);
servo2_set_percentage(ServoPercentage);
}

///////////////////////////////////functie KnipperLED voor slagboom///////////////////////////////////////////////////////////////////////
void knipperLed_4()
{
    for(int tellerLED = 0; tellerLED != 4; tellerLED++)
    {
        PORTJ &= ~(led_1);
        _delay_ms (500);
        PORTJ |= (led_1);
        _delay_ms (500);
    }
        PORTJ &= ~(led_1);
    }

    void turnLED_OFF() {
    PORTJ &= ~(led_1);
    } // Function: Zet LED uit.

    void knipperLed_INFINITY(){
    PORTJ &= ~(led_1);
    _delay_ms (50);
    PORTJ |= (led_1);
    _delay_ms (50);
    }





int main(void)
{
    PORTF |= (button_1);
    PORTF |= (button_2);
    PORTF |= (button_3);

    DDRF |= (button_1);
    DDRF |= (button_2);
    DDRF |= (button_3);

    DDRJ  &= ~(led_1);
    PORTJ &= ~(led_1);

    int ServoPercentageOmhoog = -100;
    int ServoPercentageOmlaag = 100;

//    bool activateKnipperLed = false;

    bool activateKnipperLed = false;

    init();

   while(1)
   {
//Led's wanneer de brug dicht is
       if(!(PINK & (Eindschakelaar_DC_MOTOR1)))
       {
            PORTC |= (led_geel1);
            PORTD |= (led_geel3);
            PORTJ |= (led_rood12);
            PORTH |= (led_rood34);
       }

//Led's wanneer de brug open is
        if(!(PINK & (Eindschakelaar_DC_MOTOR2)))
        {
            PORTD |= (led_groen12);
            PORTD |= (led_groen34);
        }


 //boot komt aan
        if((!(PINF & (button_1))) || (!(PINK & (Lichtscherm_aankomend_vaarverkeer_1))))//brug omhoog
        {
            knipperLed_4();
            ServoDicht(ServoPercentageOmlaag);
            activateKnipperLed = true;
            motor_brug_omhoog();
        }
                else if((PINK & (Eindschakelaar_DC_MOTOR2)))
                {
                    motor_brug_stilstand();
                }


//boot gaat weg
        if((!(PINF & (button_2))) || (!(PINK & (Lichtscherm_aankomend_vaarverkeer_2)))) //brug naar beneden
        {
            activateKnipperLed = false;
            motor_brug_omlaag();
        }
                else if((PINK & (Eindschakelaar_DC_MOTOR1)))
                {
                    motor_brug_stilstand();
                    ServoOpen(ServoPercentageOmhoog);
                }

//led's voor het weg verkeer
        if (activateKnipperLed == true)
        {
        knipperLed_INFINITY();
        }
        else {
        turnLED_OFF();
        }

//handmatige stop
        if(!(PINF & (button_3))) //motor stoppen
        {
            motor_brug_stilstand();
        }
   }
return 0;
}
