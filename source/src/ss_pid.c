#include "ss_minclude.h"




//#pragma CODE_SECTION( pi_calc, "ramfuncs");








/**
 **********************************************************
 * @鍑芥暟锛歷oid PI_DeInit( PI_TypeDef *p)
 * @鎻忚堪锛歅I鏁版嵁澶嶄綅
 * @杈撳叆锛�?p锛氬畾涔夌殑PI鏁版�?
 * @杩斿洖锛氭棤
 * @浣滆�咃�?
 * @鐗堟湰锛�?
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
 * @������void PI_init ( PI_TypeDef *p)
 * @������PI��������
 * @���룺*p�������PI����
 * @���أ���

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
 * @鍑芥暟锛歅I_float32 PI_Calc( PI_TypeDef *p)
 * @鎻忚堪锛歅I澧為噺寮忕粨绠�
 * @杈撳叆锛�?p锛歅I缁撴瀯浣�?
 * @杩斿洖锛氭棤
 * @浣滆�咃�?
 * @鐗堟湰锛�?
 **********************************************************
 */
void pi_calc( PI_STRUCT *p)
{
   

    p->Ek_0     = p->Ref -  p->Fdb ;// p->Fdb;  //

    p->Inc = (p->a0*p->Ek_0) -  (p->a1*p->Ek_1) ;//

    p->Output  += p->Inc;

    if(p->Output > p->OutMax)p->Output  = p->OutMax;
    if(p->Output < p->OutMin)p->Output  = p->OutMin;

    p->Ek_1 = p->Ek_0;
	
}

/**
 **********************************************************
 * @鍑芥暟锛歷oid PI_Clean( PI_TypeDef *p)
 * @鎻忚堪锛氭竻妤歅I璁＄畻涓殑缂撳�?
 * @杈撳叆锛�?p锛氬畾涔夌殑PI鏁版�?
 * @杩斿洖锛氭棤
 * @浣滆�咃�?
 * @鐗堟湰锛�?
 **********************************************************
 */
void pi_clc( PI_STRUCT *p)
{
    p->Ek_0         = 0;     //  Error[k]
    p->Ek_1         = 0;     //  Error[k-1]
    p->Inc          = 0;
    p->Output       = 0;
}









