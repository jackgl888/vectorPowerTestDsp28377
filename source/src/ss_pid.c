#include "ss_pid.h"




//#pragma CODE_SECTION( pi_calc, "ramfuncs");








/**
 **********************************************************
 * @函数：void PI_DeInit( PI_TypeDef *p)
 * @描述：PI数据复位
 * @输入：*p：定义的PI数据
 * @返回：无 
 * @作者：
 * @版本：
 **********************************************************
 */
void pi_reset( PI_STRUCT *p)
{
    p->a0           = 0;
    p->a1           = 0;

    p->Ek_0         = 0;     //  Error[k-0]
    p->Ek_1         = 0;     //  Error[k-1]

    p->T            = 0;
    p->Kp           = 0;
    p->Ti           = 0;

    p->Inc          = 0;
    p->Output       = 0;

    p->OutMax       = 0;
    p->OutMin       = 0;
}
/**
 **********************************************************
 * @函数：void PI_init ( PI_TypeDef *p)
 * @描述：PI参数计算
 * @输入：*p：定义的PI数据
 * @返回：无 
 * @作者：
 * @版本：
 **********************************************************
 */
void pi_init ( PI_STRUCT *p)
{   

    p->Ti       = p->Ti <= 0 ? 10e6 : p->Ti;
    p->a0       = p->Kp * (1.0f + (float32)p->T / p->Ti);
    p->a1       = p->Kp;
}

/**
 **********************************************************
 * @函数：PI_float32 PI_Calc( PI_TypeDef *p)
 * @描述：PI增量式结算
 * @输入：*p：PI结构体
 * @返回：无
 * @作者：
 * @版本：
 **********************************************************
 */
void pi_calc( PI_STRUCT *p)
{
    p->Ek_0     = p->Ref - p->Fdb;  //计算误差

    p->Inc      = _IQmpy(p->a0, p->Ek_0) - _IQmpy(p->a1, p->Ek_1);//PI计算

    p->Output  += p->Inc;

    if(p->Output > p->OutMax)p->Output  = p->OutMax;
    if(p->Output < p->OutMin)p->Output  = p->OutMin;

    p->Ek_1 = p->Ek_0;
}

/**
 **********************************************************
 * @函数：void PI_Clean( PI_TypeDef *p)
 * @描述：清楚PI计算中的缓存
 * @输入：*p：定义的PI数据
 * @返回：无 
 * @作者：
 * @版本：
 **********************************************************
 */
void pi_clc( PI_STRUCT *p)
{
    p->Ek_0         = 0;     //  Error[k]
    p->Ek_1         = 0;     //  Error[k-1]
    p->Inc          = 0;
    p->Output       = 0;
}









