/*
 * sys.c
 *
 *  Created on: 2022��3��30��
 *      Author: Taodu
 */


#include <ez_sys.h>
#include "DSP28x_Project.h"

void ConstructBegin(){
    DINT;
    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();
}

void ConstructComplete(){
    EINT;
    ERTM;
}
