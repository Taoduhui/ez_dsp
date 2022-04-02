/*
 * pwm.c
 *
 *  Created on: 2022-3-29
 *      Author: Taodu
 */


#include <math.h>
#include <ez_pwm.h>


/****************************************
For using this SDK,
You need to modify the following parameters
to adapter your project
******************BEGIN******************/

/************ PwmGen  Config **************/
#define SystemFrequency 40000000
#define SpwmRefresh_us 100 //larger then 100 and  less then  400
#define SpwmUseTimSeq 0    //
#define SinAccuracy 1024
volatile struct PwmGen_GlobalSpwmExtensionParameter PwmGen_GlobalSpwmParameter={0,0,{0,0,0,0,0,0}};
volatile float SinTable[SinAccuracy];
/**************User Definition Begin****************/

//Call PwmGen_Setup_Use_Pwm( [The_ePwm_Number] ) above all,if you want to Active the ePwm Module

PwmGen_Setup_Use_Pwm(1);
PwmGen_Setup_Use_Pwm(2);

//following a series of PwmGen_Setup_Use_Pwm
//you need to define the interrupt function "PwmGen_Event_Pwm( [The_ePwm_Number] )"
//and this function will be called, when the Counter==0 (default)
//or you can call PwmGen_Setup_Use_Interrup_on_XXXX to change the interrupt action
PwmGen_Event_Pwm(2){

}

PwmGen_Event_SPwm(1)
{
    volatile int a = 0;
}

//After the Interrupt functions, if you want to let some ePwm work at Spwm mode
//you should call the macro function PwmGen_Setup_EnableSpwmUpdateTask( [The_Timer_Bumber] )
//and in its function body, write down PwmGen_Setup_Use_as_Spwm( [The_ePwm_Number you want to set as Spwm] );
//Once you set a ePwm Module work at Spwm Mode, a Timer Resource will be occupied
//but it's only one Timer wiil be occupied, whatever how much ePwm work at Spwm mode


PwmGen_Setup_EnableSpwmUpdateTask(0){
    PwmGen_Setup_Use_as_Spwm(1);
}
/**************User Definition End****************/
/****************User Code  Begin**************/
/* After the definitions, the struct ePwm_Config is avaiable
 * they will be named by "Pwm[The_ePwm_Number]_Config" (let X is [The_ePwm_Number] in the following)
 *      if you has called the corresponding PwmGen_Setup_Use_Pwm macro function in the definition area
 *
 *
 * You can just input PwmX_Config. and press Alt+/ to get hint which is about what the PwmGen can do
 * or you can download the Stm32CubeAuto to Automatic Press the Alt+/ (wwww) https://github.com/Taoduhui/Stm32CubeAuto
 *
 * Finally you should call the macro function PwmGen_Setup_By_Config([The_ePwm_Number]) or
 *      PwmGen_Setup_By_Config_As_SpwmGen([The_ePwm_Number],SpwmUseTimSeq,SpwmRefresh_us) to Setup the ePwm
 *
 * the parameter SpwmUseTimSeq and SpwmRefresh_us should not passed into directly
 * if you want to modify the TimerNumber or the SpwmRefreshTime, you should modify the macro definitions at PwmGen Config Area
 * *********************************************/
// Example:
void PwmGen_Init_Example()
{
    Pwm1_Config.Period_s = 0.0001;
    Pwm1_Config.DutyCycle=0.5;
    Pwm1_Config.Pair_Type = Pwm_Pair_Type_Opposite;
    Pwm1_Config.RiseDeadBand_s = 0.000002;
    Pwm1_Config.FallDeadBand_s =  0.000002;
    Pwm1_Config.IsSpwm = 1;
    Pwm1_Config.SpwmConfig.Amplify = 1;
    Pwm1_Config.SpwmConfig.TimeRatio = 1;
    Pwm1_Config.SpwmConfig.Period_us = 10000;
    PwmGen_Setup_By_Config_As_SpwmGen(1,SpwmUseTimSeq,SpwmRefresh_us);

    Pwm2_Config.Period_s = 0.0001;
    Pwm2_Config.DutyCycle=0.5;
    Pwm2_Config.Pair_Type = Pwm_Pair_Type_Opposite;
    Pwm2_Config.RiseDeadBand_s = 0.000002;
    Pwm2_Config.FallDeadBand_s =  0.000002;
    PwmGen_Setup_By_Config(2)
}

/****************User Code End**************/
/******************END******************/
/* SDK Source
 * ***************BEGIN*******************/
float SinMapping(float deg_percent, float TimeRatio)
{
    return (int)(deg_percent * SinAccuracy * TimeRatio) % SinAccuracy;
}

void InitSinTable()
{
    float i = 0;
    for (; i < SinAccuracy; i += 1)
    {
        SinTable[(int)i] = (sin((i / SinAccuracy) * 2 * PI) + 1) / 2;
    }
}

float GetSine(float deg_percent, float TimeRatio, float Amplify)
{
    int mappingRes = (int)SinMapping(deg_percent, TimeRatio);
    return SinTable[mappingRes] * Amplify;
}

/*****************************************
 * The following code is the prototype of macros
 * You can uncomment it for debug
 *****************************************/
//
// void PwmGen_Setup_By_Config()
// {
//     PwmGen_Setup_Init_Pwm(1);
//     PwmGen_Setup_Init_Pwm_Period_And_DutyCycle(1, Pwm1_Config.Period_s, Pwm1_Config.DutyCycle);
//     PwmGen_Setup_Init_Two_Channel_Output_Action(1, Pwm1_Config.Pair_Type);
//     PwmGen_Setup_Use_DeadBand(1, Pwm1_Config.RiseDeadBand_s, Pwm1_Config.FallDeadBand_s);
//     if (Pwm1_Config.IsSpwm == 1)
//     {
//         if (PwmGen_GlobalSpwmParameter.IsInitedTim == 0)
//         {
//             InitSinTable();
//         }
//         PwmGen_Setup_Use_SPWM_Generator(1);
//     }
//     PwmGen_Setup_Init_Complete(1);
// }
// volatile float64 Spwm1_Counter = 0;
// Tim_TimerEvent(0)
// {
//     if (PwmGen_GlobalSpwmParameter.SpwmEnableState.Spwm1Enable == 1)
//     {
//         Spwm1_Counter++;
//         float64 Period_us = Pwm1_Config.Period * 1000000;
//         float64 Counter_us = Spwm1_Counter * SpwmRefresh_us;
//         if (Counter_us >= Period_us)
//         {
//             Spwm1_Counter = 0;
//         }
//         // Pwm1_Config.SpwmConfig.deg_percent = Spwm1_Counter*20/Pwm1_Config.Period*1000000;
//         Pwm1_Config.SpwmConfig.deg_percent = Counter_us / Period_us;
//         volatile float t = GetSine(Pwm1_Config.SpwmConfig.deg_percent,
//                                    Pwm1_Config.SpwmConfig.TimeRatio,
//                                    Pwm1_Config.SpwmConfig.Amplify);
//         EPwm1Regs.CMPA.half.CMPA = EPwm1Regs.TBPRD * t;
//     }
// }

// interrupt void SPwm1_Event()
// {
//     EPwm1Regs.ETCLR.bit.INT = 1;
//     PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
// }

// void Try_TimUpdateSpwmCompare()
// {
//     if (PwmGen_GlobalSpwmParameter.SpwmEnableState.Spwm1Enable == 1)
//     {
//         PwmGen_Tim_UpdateSpwmCompare(1)
//     }
// }

// void Use_SPWM_Generator()
// {
//     PwmGen_GlobalSpwmParameter.SpwmEnableState.Spwm1Enable = 1;
//     if (PwmGen_GlobalSpwmParameter.IsInitedTim == 0)
//     {
//         Tim_Use_Timer(0, SpwmRefresh_us);
//         TIM_StartTimer(0);
//         PwmGen_GlobalSpwmParameter.IsInitedTim = 1;
//     }
//     EALLOW;
//     PieVectTable.EPWM1_INT = &SPwm1_Event;
//     EDIS;
// }

// void UpdateCompare(float Period, float Duty_Cycle)
// {
//     EPwm1Regs.TBPRD = Period * SystemFrequency;
//     EPwm1Regs.CMPA.half.CMPA = Period * SystemFrequency * Duty_Cycle;
// }

//
// void Use_Interrup_on_CounterChange() {
//    EPwm1Regs.ETSEL.bit.INTSEL= 2;
//    EPwm1Regs.ETSEL.bit.INTEN = 1;
//    EPwm1Regs.ETPS.bit.INTPRD = ET_1ST;
//}
//
//
// void Use_Interrup_on_CounterIsZero() {
//    EPwm1Regs.ETSEL.bit.INTSEL= 1;
//    EPwm1Regs.ETSEL.bit.INTEN = 1;
//    EPwm1Regs.ETPS.bit.INTPRD = ET_1ST;
//}
//
// interrupt void Pwm_Event(){
//    EPwm1Regs.ETCLR.bit.INT = 1;
//    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
//}
//
// void UseTimeBaseClockSynchronous(){
//    EALLOW;
//    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
//    SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 1;
//    EDIS;
//}
//
// void InitPwm1Gpio(){
//    InitEPwm1Gpio();
//}
//
// void InitPwm1TimeBase(){
//    EPwm1Regs.TBCTL.bit.SYNCOSEL = 3;
//    EPwm1Regs.TBCTL.bit.PHSEN = 0;
//    EPwm1Regs.TBPHS.half.TBPHS = 0;
//    EPwm1Regs.TBCTL.bit.CTRMODE = 2;
//    EPwm1Regs.TBCTL.bit.HSPCLKDIV = 1;
//    EPwm1Regs.TBCTL.bit.CLKDIV = 0;
//    EPwm1Regs.TBCTR = 0x0000;
//    EPwm1Regs.TBPRD = 37500;
//}
//
// void InitCompare(){
//    EPwm1Regs.CMPCTL.bit.SHDWAMODE = 1;
//    EPwm1Regs.CMPCTL.bit.SHDWBMODE = 1;
//    EPwm1Regs.CMPA.half.CMPA = 18750;
//}
//
// void InitActionQaul(){
//    EPwm1Regs.AQCTLA.bit.CAU = 2;
//    EPwm1Regs.AQCTLA.bit.CAD = 1;
//    EPwm1Regs.AQCTLB.bit.CAU = 2;
//    EPwm1Regs.AQCTLB.bit.CAD = 1;
//}
//
// void InitDeadBand(){
//    EPwm1Regs.DBCTL.bit.IN_MODE = 2;
//    EPwm1Regs.DBCTL.bit.POLSEL = 2;
//    EPwm1Regs.DBCTL.bit.OUT_MODE = 3;
//    EPwm1Regs.DBRED = 500;
//    EPwm1Regs.DBFED = 500;
//}
//
// void InitPwm1Interrup(){
//    EPwm1Regs.ETSEL.bit.INTEN = 1;
//    EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;
//    EPwm1Regs.ETPS.bit.INTPRD = 1;
//    EALLOW;
//    PieVectTable.EPWM1_INT = &Pwm_Event;
//    EDIS;
//}
//
// void Use_InterrupOnCounterChange(){
//    EPwm1Regs.ETSEL.bit.INTSEL= ET_CTR_PRD;      // Select INT on PRD event
//    EPwm1Regs.ETSEL.bit.INTEN = 1;                   // Enable INT
//    EPwm1Regs.ETPS.bit.INTPRD = ET_1ST;            // Generate INT on 1ST event
//}
//
// void EnableTimeClock(){
//    EALLOW;
//    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
//    EDIS;
//}
//
// void EnableInterrupt(){
//    IER |= M_INT3;
//    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;
//}
//
// void ConstructBegin(){
//    DINT;
//    InitPieCtrl();
//    IER = 0x0000;
//    IFR = 0x0000;
//    InitPieVectTable();
//}
//
// void ConstructComplete(){
//    EINT;
//    ERTM;
//}
//
// void InitPwm1(){
//    PwmGen_Setup_Pwm(1);
//    PwmGen_Setup_Pwm_Period_And_DutyCycle(1,0.001,0.5);
//    PwmGen_Setup_Two_Channel_Output_Action(1,Pwm_Pair_Type_Opposite);
//    PwmGen_Setup_Use_DeadBand(1,0.00002,0.00002);
//    PwmGen_Setup_Complete(1);
////    ConstructBegin();
////    PwmGen_InitPwmInterrup(1);
////    PwmGen_InitEPwm1Gpio(1);
////    PwmGen_InitPwmTimeBase(Pwm_Port_1);
////    PwmGen_InitCompare(Pwm_Port_1);
//    //PwmGen_UpdateCompare(Pwm_Port_1,0.001,0.5);
////    PwmGen_InitActionQaul(Pwm_Port_1,Pwm_Pair_Type_Opposite);
////    PwmGen_InitDeadBand(Pwm_Port_1,0.00002,0.00002);
////    PwmGen_EnableInterrupt(1);
////    ConstructComplete();
//    //ConstructBegin();
//    //PwmGen_InitPwm1Interrup(Pwm_Port_1);
//    //InitPwm1Interrup();
//    //InitEPwm1Gpio();
//    //UseTimeBaseClockSynchronous();
//    //InitPwm1TimeBase();
//    //InitActionQaul();
//    //InitDeadBand();
//    //EnableTimeClock();
//    //EnableInterrupt();
//    //ConstructComplete();
//}
/*****************************************************
 *SDK Source
 ************************END**************************/
