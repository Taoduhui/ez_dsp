# ez_dsp

一个Dsp28335的库

使用简洁的api操作dsp

Api文档:https://taoduhui.github.io/ez_dsp/document/ez-dsp

[EN]((README.md))|中文

## 导入

## ez_pwm

###  简介

ez_pwm是ez-dsp库的一部分，你可以使用ez-pwm来生成pwm、spwm和svpwm波

### usage

若要使用ez_pwm，你可能需要修改在ez_pwm.c中PwmGen_Config区域的宏定义的值来适配你的项目

```cpp
#define SystemFrequency 40000000 
#define SpwmRefresh_us 100  //every 100us,spwm will update the ePwm's compare larger then 100 and  less then  400
#define SpwmUseTimSeq 0     //number of Timer you want to use for spwm generate
#define SinAccuracy 1024    //Sin Table Length
volatile struct PwmGen_GlobalSpwmExtensionParameter PwmGen_GlobalSpwmParameter={0,0,{0,0,0,0,0,0}};
```

**使用ePwm**

首先，若要启用ePwm模块，需要在全局区调用 `PwmGen_Setup_Use_Pwm( [The_ePwm_Number] )` 

如下

```cpp
PwmGen_Setup_Use_Pwm(1);
PwmGen_Setup_Use_Pwm(2);
```

在以上代码，我们希望使用ePwm1和ePwm2

**定义中断**

在一系列`PwmGen_Setup_Use_Pwm`之后

你需要定义中断函数 `PwmGen_Event_Pwm( [The_ePwm_Number] )`

默认情况下，当计时器==0时会触发中断

或者你可以调用`PwmGen_Setup_Use_Interrup_on_XXXX`来改变中断触发的方式

函数体为中断函数体

```cpp
PwmGen_Event_Pwm(2){

}

PwmGen_Event_SPwm(1)
{
    volatile int a = 0;
}
```

**Spwm模式**


在中断功能之后，如果要让某些 ePwm 在 Spwm 模式下工作

您应该调用宏函数`PwmGen_Setup_EnableSpwmUpdateTask( [The_Timer_Bumber] )`

并在其函数体中写下`PwmGen_Setup_Use_as_Spwm( [The ePwm Number you want to set as Spwm] );`

将 ePwm 模块设置为 Spwm 模式后，将会占用一个计时器资源

但只有一个计时器被占用，无论有多少ePwm在Spwm模式下工作

Example:

```cpp
PwmGen_Setup_EnableSpwmUpdateTask(0){
    PwmGen_Setup_Use_as_Spwm(1);
}
```

**Set the Parameters**

在完成全部定义后

如果 你曾调用过相应的`PwmGen_Setup_Use_Pwm`,那么 结构体 ePwm_Config 将可用

他们会被命名为  `Pwm[The_ePwm_Number]_Config` (以下 X 皆为 `[The_ePwm_Number]` 的缩写)

你可以输入 `PwmX_Config` 然后按下 `Alt+/` 来呼出关于PwmGen功能的自动提示

或者你可以下载 Stm32CubeAuto 来自动唤起  https://github.com/Taoduhui/Stm32CubeAuto

参数 `SpwmUseTimSeq` and `SpwmRefresh_us` 不应直接传入

如果你想要修改Spwm使用的Timer或者Spwm刷新的时间，你应该到PwmGen Config出处修改宏定义

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

你可以打开`ez_pwm.c`查看完整的样例

## ez_tim

