/*
 * ez_adc.c
 *
 *  Created on: 2022Äê4ÔÂ13ÈÕ
 *      Author: Taodu
 */


#include <ez_adc.h>



Adc_Setup_UseAdcPorts(){
    Adc_Setup_UsePort(Adc_Port_A0);
    Adc_Setup_UsePort(Adc_Port_A1);
    Adc_Setup_UsePort(Adc_Port_A2);
}



Adc_Event(){
    volatile Uint16 a0 = Adc_Get_Result(Adc_Port_A0);
    volatile Uint16 a1 = Adc_Get_Result(Adc_Port_A1);
    volatile Uint16 a2 = Adc_Get_Result(Adc_Port_A2);
}


void Ez_Adc_Sample(){
    Adc_Setup_ActiveAdc();
}


