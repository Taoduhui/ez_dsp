/*
 * tim.h
 *
 *  Created on: 2022��3��30��
 *      Author: Taodu
 */

#ifndef CUSTOM_TIM_TIM_H_
#define CUSTOM_TIM_TIM_H_

#include <ez_dsp.h>

#define TimerFreq 150

#define Tim_Use_Timer(TimSeq,Period_us) \
    long temp;\
\
    EALLOW;\
    SysCtrlRegs.PCLKCR3.bit.CPUTIMER##TimSeq##ENCLK = 1;\
\
    PieVectTable.TINT##TimSeq = &TIM##TimSeq##_IRQ;\
\
    CpuTimer##TimSeq.RegsAddr = &CpuTimer##TimSeq##Regs;\
\
    CpuTimer##TimSeq.CPUFreqInMHz = TimerFreq;\
    CpuTimer##TimSeq.PeriodInUSec = Period_us/2;\
    temp = (long) (TimerFreq * Period_us/2);\
    CpuTimer##TimSeq.RegsAddr->PRD.all = temp;\
\
    CpuTimer##TimSeq.RegsAddr->TPR.all  = 0;\
    CpuTimer##TimSeq.RegsAddr->TPRH.all  = 0;\
\
    CpuTimer##TimSeq.RegsAddr->TCR.bit.TSS = 1;\
    CpuTimer##TimSeq.RegsAddr->TCR.bit.TRB = 1; \
    CpuTimer##TimSeq.RegsAddr->TCR.bit.SOFT = 0;\
    CpuTimer##TimSeq.RegsAddr->TCR.bit.FREE = 0;\
    CpuTimer##TimSeq.RegsAddr->TCR.bit.TIE = 1;\
    CpuTimer##TimSeq.InterruptCount = 0;\
    CpuTimer##TimSeq.RegsAddr->TCR.bit.TSS = 0;\
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;\
    EDIS;\
    IER |= M_INT1;

#define Tim_StartTimer(TimSeq) CpuTimer##TimSeq.RegsAddr->TCR.bit.TSS = 0;

#define Tim_StopTimer(TimSeq) CpuTimer##TimSeq.RegsAddr->TCR.bit.TSS = 1;

#define Tim_ResetTimer(TimSeq) CpuTimer##TimSeq.RegsAddr->TCR.bit.TRB = 1;

#define Tim_TimerEvent(SeqNum)                \
void TIM##SeqNum##_inner(void);             \
interrupt void TIM##SeqNum##_IRQ(void)    \
{                                           \
    TIM##SeqNum##_inner();                  \
    CpuTimer##SeqNum##Regs.TCR.bit.TIF = 1;          \
    PieCtrlRegs.PIEACK.bit.ACK1 = 1;        \
}                                           \
void TIM##SeqNum##_inner()

//interrupt void TIM0_IRQ(void);
//
//void TIM0_Init(float,float);

#endif /* CUSTOM_TIM_TIM_H_ */
