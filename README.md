# ez_dsp

A  Lib for Dsp28335

Use simple api to operate dsp

Api Document:https://taoduhui.github.io/ez_dsp/document/ez-dsp

EN|[中文](README-ZH.md)
## import

## ez_pwm

###  intro

ez_pwm is a part of ez_dsp lib, you can use it to init and generate pwm,spwm and svpwm.

### usage

For using this lib,

You need to modify the PwmGen_Config Area definitions in the ez_pwm.c to adapter your project

```cpp
#define SystemFrequency 40000000 
#define SpwmRefresh_us 100  //every 100us,spwm will update the ePwm's compare larger then 100 and  less then  400
#define SpwmUseTimSeq 0     //number of Timer you want to use for spwm generate
#define SinAccuracy 1024    //Sin Table Length
volatile struct PwmGen_GlobalSpwmExtensionParameter PwmGen_GlobalSpwmParameter={0,0,{0,0,0,0,0,0}};
```

**Use the ePwm**

first, Call `PwmGen_Setup_Use_Pwm( [The_ePwm_Number] )` above all in global area,if you want to Active the ePwm Module

just like following:

```cpp
PwmGen_Setup_Use_Pwm(1);
PwmGen_Setup_Use_Pwm(2);
```

in the above code, we want to use the ePwm1 and ePwm2 to generate pwm

**Define the interrupts**

following a series of PwmGen_Setup_Use_Pwm

you need to define the interrupt function "PwmGen_Event_Pwm( [The_ePwm_Number] )"

and this function will be called, when the Counter==0 (default)

or you can call PwmGen_Setup_Use_Interrup_on_XXXX to change the interrupt action

the body is the interrupt function body

```cpp
PwmGen_Event_Pwm(2){

}

PwmGen_Event_SPwm(1)
{
    volatile int a = 0;
}
```

**Run as Spwm Mode**

After the Interrupt functions, if you want to let some ePwm work at Spwm mode

you should call the macro function `PwmGen_Setup_EnableSpwmUpdateTask( [The_Timer_Bumber] )`

and in its function body, write down `PwmGen_Setup_Use_as_Spwm( [The ePwm Number you want to set as Spwm] );`

Once you set a ePwm Module work at Spwm Mode, a Timer Resource will be occupied

but it's only one Timer wiil be occupied, whatever how much ePwm work at Spwm mode

Example:

```cpp
PwmGen_Setup_EnableSpwmUpdateTask(0){
    PwmGen_Setup_Use_as_Spwm(1);
}
```

**Set the Parameters**

After all the definitions, the struct ePwm_Config is avaiable
they will be named by `Pwm[The_ePwm_Number]_Config` ( let X is `[The_ePwm_Number]` in the following )

if you has called the corresponding PwmGen_Setup_Use_Pwm macro function in the definition area

 You can just input PwmX_Config. and press Alt+/ to get hint which is about what the PwmGen can do
 or you can download the Stm32CubeAuto to Automatic Press the Alt+/  https://github.com/Taoduhui/Stm32CubeAuto

Finally you should call the macro function `PwmGen_Setup_By_Config([The_ePwm_Number])` or
  `PwmGen_Setup_By_Config_As_SpwmGen([The_ePwm_Number],SpwmUseTimSeq,SpwmRefresh_us)` to Setup the ePwm

 the parameter `SpwmUseTimSeq` and `SpwmRefresh_us` should not passing directly
  
 if you want to modify the TimerNumber or the SpwmRefreshTime, you should modify the macro definitions at PwmGen Config Area

 ```cpp
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
 ```
You can open ez_pwm.c to get complete example

## ez_tim

