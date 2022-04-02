/*
 * pwm.h
 *
 *  Created on: 2022-3-29
 *      Author: Taodu
 */

#ifndef CUSTOM_PWM_PWM_H_
#define CUSTOM_PWM_PWM_H_

#include <ez_dsp.h>

#define Pwm_Port_1 EPwm1Regs
#define Pwm_Port_2 EPwm2Regs
#define Pwm_Port_3 EPwm3Regs
#define Pwm_Port_4 EPwm4Regs
#define Pwm_Port_5 EPwm5Regs
#define Pwm_Port_6 EPwm6Regs

#define Pwm_Pair_Type_Same 1
#define Pwm_Pair_Type_Opposite 0


#define PI 3.14159265

struct PwmGen_SpwmConfig
{
    float deg_percent;
    float TimeRatio;
    float Amplify;
    float Period_us;
};

struct PwmGen_PwmConfig
{
    float Period_s;
    float DutyCycle;
    Uint16 Pair_Type;
    float RiseDeadBand_s;
    float FallDeadBand_s;
    Uint16 IsSpwm;
    struct PwmGen_SpwmConfig SpwmConfig;
};

struct PwmGen_SpwmEnableState
{
    Uint16 Spwm1Enable : 1;
    Uint16 Spwm2Enable : 1;
    Uint16 Spwm3Enable : 1;
    Uint16 Spwm4Enable : 1;
    Uint16 Spwm5Enable : 1;
    Uint16 Spwm6Enable : 1;
};

struct PwmGen_GlobalSpwmExtensionParameter
{
    int IsInitedTim;
    int SpwmCount;
    struct PwmGen_SpwmEnableState SpwmEnableState;
};


#define PwmGen_InitPwmTimeBase(Pwm_Port) \
    (Pwm_Port).TBCTL.bit.SYNCOSEL = 3;   \
    (Pwm_Port).TBCTL.bit.PHSEN = 0;      \
    (Pwm_Port).TBPHS.half.TBPHS = 0;     \
    (Pwm_Port).TBCTL.bit.CTRMODE = 2;    \
    (Pwm_Port).TBCTL.bit.HSPCLKDIV = 1;  \
    (Pwm_Port).TBCTL.bit.CLKDIV = 0;     \
    (Pwm_Port).TBCTR = 0x0000;

// Period:s Duty_Cycle:0-1
#define PwmGen_InitCompare(Pwm_Port)     \
    (Pwm_Port).CMPCTL.bit.SHDWAMODE = 1; \
    (Pwm_Port).CMPCTL.bit.SHDWBMODE = 1;

#define PwmGen_UpdateCompare(Pwm_Port, Period, Duty_Cycle) \
    (Pwm_Port).TBPRD = Period * SystemFrequency;           \
    (Pwm_Port).CMPA.half.CMPA = Period * SystemFrequency * Duty_Cycle;

#define PwmGen_InitPwmInterrup(SeqNum)                      \
    EPwm##SeqNum##Regs.ETSEL.bit.INTEN = 1;                 \
    EPwm##SeqNum##Regs.ETSEL.bit.INTSEL = 1;                \
    EPwm##SeqNum##Regs.ETPS.bit.INTPRD = 1;                 \
    EALLOW;                                                 \
    PieVectTable.EPWM##SeqNum##_INT = &Pwm##SeqNum##_Event; \
    EDIS;

#define PwmGen_Event_Pwm(SeqNum)                \
    void Pwm##SeqNum##_inner(void);             \
    interrupt void Pwm##SeqNum##_Event(void)    \
    {                                           \
        Pwm##SeqNum##_inner();                  \
        EPwm##SeqNum##Regs.ETCLR.bit.INT = 1;   \
        PieCtrlRegs.PIEACK.all = PIEACK_GROUP3; \
    }                                           \
    void Pwm##SeqNum##_inner()

#define PwmGen_EnableInterrupt(SeqNum) \
    IER |= M_INT3;                     \
    PieCtrlRegs.PIEIER3.bit.INTx##SeqNum = 1;

#define PwmGen_InitActionQaul(Pwm_Port, Pwm_Pair_Type) \
    if (Pwm_Pair_Type == 0)                            \
    {                                                  \
        (Pwm_Port).AQCTLA.bit.CAU = 2;                 \
        (Pwm_Port).AQCTLA.bit.CAD = 1;                 \
        (Pwm_Port).AQCTLB.bit.CAU = 2;                 \
        (Pwm_Port).AQCTLB.bit.CAD = 1;                 \
    }                                                  \
    else                                               \
    {                                                  \
        (Pwm_Port).AQCTLA.bit.CAU = 2;                 \
        (Pwm_Port).AQCTLA.bit.CAD = 1;                 \
        (Pwm_Port).AQCTLB.bit.CAU = 1;                 \
        (Pwm_Port).AQCTLB.bit.CAD = 2;                 \
    }

#define PwmGen_InitDeadBand(Pwm_Port, RiseEdgeDeadBand, FallEdgeDeadBand) \
    (Pwm_Port).DBCTL.bit.IN_MODE = 2;                                     \
    (Pwm_Port).DBCTL.bit.POLSEL = 2;                                      \
    (Pwm_Port).DBCTL.bit.OUT_MODE = 3;                                    \
    (Pwm_Port).DBRED = RiseEdgeDeadBand * SystemFrequency / 5;            \
    (Pwm_Port).DBFED = FallEdgeDeadBand * SystemFrequency / 5;

#define PwmGen_InitEPwm1Gpio(SeqNum) InitEPwm##SeqNum##Gpio()

#define PwmGen_Setup_Init_Pwm(SeqNum)            \
    PwmGen_InitPwmInterrup(SeqNum);          \
    PwmGen_InitEPwm1Gpio(SeqNum);            \
    PwmGen_InitPwmTimeBase(Pwm_Port_##SeqNum); \
    PwmGen_InitCompare(Pwm_Port_##SeqNum);

#define PwmGen_Setup_Init_Pwm_Period_And_DutyCycle(SeqNum, Period_Second, Duty_Cycle_LessThanOne) \
    PwmGen_UpdateCompare(Pwm_Port_##SeqNum, Period_Second, Duty_Cycle_LessThanOne);

#define PwmGen_Setup_Init_Two_Channel_Output_Action(SeqNum, Pwm_Pair_Type) \
    PwmGen_InitActionQaul(Pwm_Port_##SeqNum, Pwm_Pair_Type);

#define PwmGen_Setup_Use_DeadBand(SeqNum, RiseEdgeDeadBand_Second, FallEdgeDeadBand_Second) \
    PwmGen_InitDeadBand(Pwm_Port_##SeqNum, RiseEdgeDeadBand_Second, FallEdgeDeadBand_Second);

#define PwmGen_Setup_Init_Complete(SeqNum) \
    PwmGen_EnableInterrupt(SeqNum);

#define PwmGen_Setup_Use_Interrup_on_CounterPeriod(SeqNum) \
    EPwm##SeqNum##Regs.ETSEL.bit.INTSEL = 2;               \
    EPwm##SeqNum##Regs.ETSEL.bit.INTEN = 1;                \
    EPwm##SeqNum##Regs.ETPS.bit.INTPRD = 1;

#define PwmGen_Setup_Use_Interrup_on_CounterIsZero(SeqNum) \
    EPwm##SeqNum##Regs.ETSEL.bit.INTSEL = 1;               \
    EPwm##SeqNum##Regs.ETSEL.bit.INTEN = 1;                \
    EPwm##SeqNum##Regs.ETPS.bit.INTPRD = 1;


#define PwmGen_Tim_UpdateSpwmCompare(SeqNum)                                \
    Spwm##SeqNum##_Counter++;                                               \
    float64 Period##SeqNum##_us = Pwm##SeqNum##_Config.SpwmConfig.Period_us*2 ;\
    float64 Counter##SeqNum##_us = Spwm##SeqNum##_Counter * SpwmRefresh_us;           \
    if (Counter##SeqNum##_us >= Period##SeqNum##_us)                                            \
    {                                                                       \
        Spwm##SeqNum##_Counter = 0;                                         \
    }                                                                       \
    Pwm##SeqNum##_Config.SpwmConfig.deg_percent = Counter##SeqNum##_us / Period##SeqNum##_us;   \
    EPwm##SeqNum##Regs.CMPA.half.CMPA = EPwm##SeqNum##Regs.TBPRD *  GetSine(Pwm##SeqNum##_Config.SpwmConfig.deg_percent, \
                                                                            Pwm##SeqNum##_Config.SpwmConfig.TimeRatio,   \
                                                                            Pwm##SeqNum##_Config.SpwmConfig.Amplify);

#define PwmGen_Try_TimUpdateSpwmCompare(SeqNum)                               \
    if (PwmGen_GlobalSpwmParameter.SpwmEnableState.Spwm##SeqNum##Enable == 1) \
    {                                                                         \
        PwmGen_Tim_UpdateSpwmCompare(SeqNum)                                  \
    }

#define PwmGen_Setup_Use_as_Spwm(SeqNum)  PwmGen_Tim_UpdateSpwmCompare(SeqNum)

#define PwmGen_Setup_EnableSpwmUpdateTask(SpwmUseTimSeq) \
        void PwmGen_EnableSpwmUpdateTask_Inner();    \
        Tim_TimerEvent(SpwmUseTimSeq)                \
        {                                            \
            PwmGen_EnableSpwmUpdateTask_Inner();\
        }\
        void PwmGen_EnableSpwmUpdateTask_Inner()

#define PwmGen_Event_SPwm(SeqNum)                \
        volatile float64 Spwm##SeqNum##_Counter = 0; \
    interrupt void Pwm##SeqNum##_Event(void){}   \
    void SPwm##SeqNum##_inner(void);             \
    interrupt void SPwm##SeqNum##_Event(void)    \
    {                                            \
        SPwm##SeqNum##_inner();                  \
        EPwm##SeqNum##Regs.ETCLR.bit.INT = 1;    \
        PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;  \
    }                                            \
    void SPwm##SeqNum##_inner()

#define PwmGen_Setup_Use_SPWM_Generator(SeqNum,SpwmUseTimSeq,SpwmRefresh_us)                          \
    PwmGen_GlobalSpwmParameter.SpwmEnableState.Spwm##SeqNum##Enable = 1; \
    if (PwmGen_GlobalSpwmParameter.IsInitedTim == 0)                     \
    {                                                                    \
        Tim_Use_Timer(SpwmUseTimSeq, SpwmRefresh_us);                    \
        TIM_StartTimer(SpwmUseTimSeq);                                   \
        PwmGen_GlobalSpwmParameter.IsInitedTim = 1;                      \
    }                                                                    \
    EALLOW;                                                              \
    PieVectTable.EPWM##SeqNum##_INT = &SPwm##SeqNum##_Event;             \
    EDIS;

#define PwmGen_Setup_By_Config(SeqNum)                                             \
    PwmGen_Setup_Init_Pwm(SeqNum);                                                         \
    PwmGen_Setup_Init_Pwm_Period_And_DutyCycle(SeqNum, Pwm##SeqNum##_Config.Period_s, Pwm##SeqNum##_Config.DutyCycle); \
    PwmGen_Setup_Init_Two_Channel_Output_Action(SeqNum, Pwm##SeqNum##_Config.Pair_Type);                 \
    PwmGen_Setup_Use_DeadBand(SeqNum, Pwm##SeqNum##_Config.RiseDeadBand_s, Pwm##SeqNum##_Config.FallDeadBand_s);       \
    PwmGen_Setup_Init_Complete(SeqNum);

#define PwmGen_Setup_By_Config_As_SpwmGen(SeqNum,SpwmUseTimSeq,SpwmRefresh_us)                                             \
    PwmGen_Setup_Init_Pwm(SeqNum);                                                         \
    PwmGen_Setup_Init_Pwm_Period_And_DutyCycle(SeqNum,Pwm##SeqNum##_Config.Period_s, Pwm##SeqNum##_Config.DutyCycle); \
    PwmGen_Setup_Init_Two_Channel_Output_Action(SeqNum,Pwm##SeqNum##_Config.Pair_Type);                 \
    PwmGen_Setup_Use_DeadBand(SeqNum, Pwm##SeqNum##_Config.RiseDeadBand_s, Pwm##SeqNum##_Config.FallDeadBand_s);       \
    if (Pwm##SeqNum##_Config.IsSpwm == 1)                                                                \
    {                                                                                      \
        if (PwmGen_GlobalSpwmParameter.IsInitedTim == 0)                                   \
        {                                                                                  \
            InitSinTable();                                                                \
        }                                                                                  \
        PwmGen_Setup_Use_SPWM_Generator(SeqNum,SpwmUseTimSeq,SpwmRefresh_us);                                        \
    }                                                                                      \
    PwmGen_Setup_Init_Complete(SeqNum);

#define PwmGen_Setup_Use_Pwm(SeqNum) volatile struct PwmGen_PwmConfig Pwm##SeqNum##_Config;

float SinMapping(float, float);
void InitPwm1(void);
void InitPieCtrl();
void InitEPwm();
void InitEPwm1Gpio();
void InitPieVectTable();
void UseTimeBaseClockSynchronous();
void InitPwm1TimeBase();
void InitCompare();
void InitActionQaul();
void InitDeadBand();

void InitSinTable();
float GetSine(float, float, float);
void Use_SPWM_Generator();
interrupt void Pwm_Event(void);
void PwmGen_Init_Example();

#endif /* CUSTOM_PWM_PWM_H_ */
