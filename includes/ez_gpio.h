/*
 * ez_gpio.h
 *
 *  Created on: 2022Äê4ÔÂ14ÈÕ
 *      Author: Taodu
 */

#ifndef CUSTOM_INCLUDES_EZ_GPIO_H_
#define CUSTOM_INCLUDES_EZ_GPIO_H_

#include <ez_dsp.h>

void Ez_Gpio_Sample();

#define Gpio_DefaultValue_High 1
#define Gpio_DefaultValue_Low 0

#define Gpio_Event() \
    void Gpio_Event_Inner(void);\
    interrupt void Gpio_Event_Body(){\
        Gpio_Event_Inner();\
        PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;\
    }\
    void Gpio_Event_Inner()

#define  GPIO_Read(Seq,IoNum) GpioDataRegs.GP##Seq##DAT.bit.GPIO##IoNum

#define  GPIO_Write(Seq,IoNum,Value) GpioDataRegs.GP##Seq##DAT.bit.GPIO##IoNum = Value

#define GPIO_Init_UseGpio_As_Output(Seq,IoNum,Gpio_DefaultValue) \
    EALLOW;\
    GpioCtrlRegs.GP##Seq##MUX1.bit.GPIO##IoNum = 0; \
    GpioCtrlRegs.GP##Seq##DIR.bit.GPIO##IoNum = 1;\
    GpioCtrlRegs.GP##Seq##PUD.bit.GPIO##IoNum=Gpio_DefaultValue;\
    EDIS;

#define GPIO_Init_UseGpio_As_Input(Seq,IoNum,Gpio_DefaultValue) \
    EALLOW;\
    GpioCtrlRegs.GP##Seq##MUX1.bit.GPIO##IoNum = 0;\
    GpioCtrlRegs.GP##Seq##DIR.bit.GPIO##IoNum = 0;\
    GpioCtrlRegs.GP##Seq##PUD.bit.GPIO##IoNum=Gpio_DefaultValue;\
    EDIS;


#define GPIO_Init_UseGpio_As_InterruptTrigger(Seq,IoNum) \
    EALLOW;\
    GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = IoNum;\
    XIntruptRegs.XINT1CR.bit.POLARITY = 0;\
    EDIS;


#define GPIO_Init_Interrupt() \
    EALLOW;\
    PieVectTable.XINT1 = &Gpio_Event_Body;\
    EDIS;

#define GPIO_Init_EnableInterrupt() \
    EALLOW;\
    XIntruptRegs.XINT1CR.bit.ENABLE = 1;\
    PieCtrlRegs.PIEIER1.bit.INTx4 = 1;\
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;

#endif /* CUSTOM_INCLUDES_EZ_GPIO_H_ */
