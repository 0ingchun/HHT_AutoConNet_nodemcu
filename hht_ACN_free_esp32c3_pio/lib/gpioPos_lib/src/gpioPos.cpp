#include "gpioPos.h"

void init_gpio_Pos_Digital(uint8_t* GPIO_PINS, bool POS_STATE)
{
    if(POS_STATE)
    {
        for (uint8_t i = 0; i < ( sizeof(GPIO_PINS) / sizeof(GPIO_PINS[0]) ); i++){
            pinMode(GPIO_PINS[i], INPUT);
        }
    }
    else{
        for (uint8_t i = 0; i < ( sizeof(GPIO_PINS) / sizeof(GPIO_PINS[0]) ); i++){
            pinMode(GPIO_PINS[i], OUTPUT);
        }
    }
}

void gpio_Blink_H(uint8_t GPIO_PIN, uint32_t POS_TIMES)
{
    digitalWrite(GPIO_PIN, HIGH);
    delay(POS_TIMES);
    digitalWrite(GPIO_PIN, LOW);
    delay(POS_TIMES);
}

void gpio_Blink_L(uint8_t GPIO_PIN, uint32_t POS_TIMES)
{ 
    digitalWrite(GPIO_PIN, LOW);
    delay(POS_TIMES);
    digitalWrite(GPIO_PIN, HIGH);
    delay(POS_TIMES);
}

void gpio_Show(uint8_t GPIO_PIN, bool POS_STATE, uint32_t POS_TIMES)
{
    if (POS_STATE){
        digitalWrite(GPIO_PIN, HIGH );
    }
    else digitalWrite(GPIO_PIN, LOW);
    delay(POS_TIMES);
}

void gpio_Flip(uint8_t GPIO_PIN, uint32_t POS_TIMES)
{
    digitalWrite(GPIO_PIN, !digitalRead(GPIO_PIN));
    delay(POS_TIMES);
}

bool gpio_Press_Keep(uint8_t GPIO_PIN, bool POS_STATE, uint32_t POS_TIMES)
{
    if(POS_STATE){
        if(digitalRead(GPIO_PIN)){
            delay(POS_TIMES);
            if(!digitalRead(GPIO_PIN)){ //1Kde 下来电阻，10K的拉不动  
            return 1;
            }      
        }
    }
    else {
        if(!digitalRead(GPIO_PIN)){
            delay(POS_TIMES);
            if(!digitalRead(GPIO_PIN)){ //1Kde 下来电阻，10K的拉不动  
            return 1;
            }      
        }
    }
    return 0;
}