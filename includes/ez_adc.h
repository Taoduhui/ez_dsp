/*
 * ez_adc.h
 *
 *  Created on: 2022Äê4ÔÂ13ÈÕ
 *      Author: Taodu
 */

#ifndef CUSTOM_INCLUDES_EZ_ADC_H_
#define CUSTOM_INCLUDES_EZ_ADC_H_

#include <ez_dsp.h>

void Ez_Adc_Sample(void);

#define Adc_Port_A0 00
#define Adc_Port_A1 01
#define Adc_Port_A2 02
#define Adc_Port_A3 03
#define Adc_Port_A4 04
#define Adc_Port_A5 05
#define Adc_Port_A6 06
#define Adc_Port_A7 07
#define Adc_Port_B0 08
#define Adc_Port_B1 09
#define Adc_Port_B2 10
#define Adc_Port_B3 11
#define Adc_Port_B4 12
#define Adc_Port_B5 13
#define Adc_Port_B6 14
#define Adc_Port_B7 15

struct Adc_Port_Result_Name{
    Uint16 A0;
    Uint16 A1;
    Uint16 A2;
    Uint16 A3;
    Uint16 A4;
    Uint16 A5;
    Uint16 A6;
    Uint16 A7;
    Uint16 B0;
    Uint16 B1;
    Uint16 B2;
    Uint16 B3;
    Uint16 B4;
    Uint16 B5;
    Uint16 B6;
    Uint16 B7;
};

union Adc_Port_Result{
    Uint16 All[16];
    struct Adc_Port_Result_Name Port;
};

#define  Adc_Init_Clock() \
    EALLOW;\
    SysCtrlRegs.HISPCP.all = 0x3;\
    EDIS;

#define Adc_Init_Interrupt() \
    EALLOW;\
    PieVectTable.ADCINT = &Adc_Event_Body;\
    EDIS;\

#define Adc_Init_Peripherals() \
    InitAdc();

#define Adc_Init_AdcConfig() \
    InitAdc();

#define Adc_Init_EnableInterrupt() \
    PieCtrlRegs.PIEIER1.bit.INTx6 = 1;\
    AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;\
    IER |= M_INT1;\
    EINT;\
    ERTM;

#define Adc_Init_UsePort(Adc_Port) \
    if(AdcRegs.ADCMAXCONV.all<4){ \
        AdcRegs.ADCCHSELSEQ1.all =  AdcRegs.ADCCHSELSEQ1.all | (Adc_Port << (AdcRegs.ADCMAXCONV.all - 0)*4 ) ;\
    }else if(AdcRegs.ADCMAXCONV.all<8){\
        AdcRegs.ADCCHSELSEQ2.all =  AdcRegs.ADCCHSELSEQ2.all | (Adc_Port << (AdcRegs.ADCMAXCONV.all - 4)*4 ) ;\
    }else if(AdcRegs.ADCMAXCONV.all<12){\
        AdcRegs.ADCCHSELSEQ3.all =  AdcRegs.ADCCHSELSEQ3.all | (Adc_Port << (AdcRegs.ADCMAXCONV.all - 8)*4 ) ;\
    }else if(AdcRegs.ADCMAXCONV.all<16){\
        AdcRegs.ADCCHSELSEQ4.all =  AdcRegs.ADCCHSELSEQ4.all | (Adc_Port << (AdcRegs.ADCMAXCONV.all - 12)*4 ) ;\
    }\
    Adc_Port_Result_Offset.All[Adc_Port] = AdcRegs.ADCMAXCONV.all;\
    AdcRegs.ADCMAXCONV.all ++;

#define Adc_Get_Result(Adc_Port)  (*((&AdcRegs.ADCRESULT0) + Adc_Port_Result_Offset.All[Adc_Port]) >> 4)

#define Adc_Init_Run() \
    AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 1;

#define Adc_Event()                \
    void Adc_Event_inner(void);             \
    interrupt void Adc_Event_Body(void)    \
    {                                           \
        Adc_Event_inner();                 \
        AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;  \
        AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;    \
        PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;\
        Adc_Init_Run();\
        return;     \
    }                                           \
    void Adc_Event_inner()

#define Adc_Setup_UsePort(Adc_Port) Adc_Init_UsePort(Adc_Port);

#define Adc_Setup_UseAdcPorts() \
    interrupt void Adc_Event_Body(void); \
    union Adc_Port_Result Adc_Port_Result_Offset; \
    void Adc_Private_UseAdcPort_inner(void); \
    void Adc_Private_UseAdcPort_Contructor(void)    \
    {                                           \
        Adc_Init_Clock();\
        Adc_Init_Interrupt();\
        Adc_Init_AdcConfig();\
        Adc_Private_UseAdcPort_inner();                 \
        return;     \
    }\
    void Adc_Private_UseAdcPort_inner()

#define Adc_Setup_ActiveAdc() \
    Adc_Private_UseAdcPort_Contructor();\
    Adc_Init_EnableInterrupt();\
    Adc_Init_Run();

#endif /* CUSTOM_INCLUDES_EZ_ADC_H_ */
