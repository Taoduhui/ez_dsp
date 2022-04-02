/*
 * tim.c
 *
 *  Created on: 2022��3��30��
 *      Author: Taodu
 */

#include <ez_tim.h>
//
//interrupt void TIM0_IRQ(void)
//{
//    EALLOW;
//    CpuTimer0Regs.TCR.bit.TIF = 1;
//    PieCtrlRegs.PIEACK.bit.ACK1 = 1;
//    EDIS;
//}
//
//void TIM0_Init(float Freq,float Period)
//{
//    long temp;
//
//    EALLOW;
//    SysCtrlRegs.PCLKCR3.bit.CPUTIMER0ENCLK = 1;
//    EDIS;
//
//    EALLOW;
//    PieVectTable.TINT0 = &TIM0_IRQ;
//    EDIS;
//
//    EALLOW;
//    // Initialize address pointers to respective timer registers:
//    CpuTimer0.RegsAddr = &CpuTimer0Regs;
//    // Initialize timer period:
//    CpuTimer0.CPUFreqInMHz = Freq;
//    CpuTimer0.PeriodInUSec = Period;
//    temp = (long) (TimerFreq * Period);
//    CpuTimer0.RegsAddr->PRD.all = temp;
//    // Set pre-scale counter to divide by 1 (SYSCLKOUT):
//    CpuTimer0.RegsAddr->TPR.all  = 0;
//    CpuTimer0.RegsAddr->TPRH.all  = 0;
//    // Initialize timer control register:
//    CpuTimer0.RegsAddr->TCR.bit.TSS = 1;   // 1 = Stop timer, 0 = Start/Restart Timer
//    CpuTimer0.RegsAddr->TCR.bit.TRB = 1;
//    CpuTimer0.RegsAddr->TCR.bit.SOFT = 0;
//    CpuTimer0.RegsAddr->TCR.bit.FREE = 0;  // Timer Free Run Disabled
//    CpuTimer0.RegsAddr->TCR.bit.TIE = 1;   // 0 = Disable/  1 = Enable Timer Interrupt
//    // Reset interrupt counter:
//    CpuTimer0.InterruptCount = 0;
//    CpuTimer0.RegsAddr->TCR.bit.TSS = 0;   // 1 = Stop timer, 0 = Start/Restart Timer
//    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
//    EDIS;
//    IER |= M_INT1;
//}

