

#include "includes.h"






/*
*********************************************************************************************************
*                                        INITIALIZE A TASK'S STACK
*
* Description: This function is called by either OSTaskCreate() or OSTaskCreateExt() to initialize the
*              stack frame of the task being created.  This function is highly processor specific.
*
* Arguments  : task          is a pointer to the task code，任务函数入口
*
*              p_arg         is a pointer to a user supplied data area that will be passed to the task
*                            when the task first executes.，任务的参数
*
*              ptos          is a pointer to the top of stack.  It is assumed that 'ptos' points to
*                            a 'free' entry on the task stack.  If OS_STK_GROWTH is set to 1 then
*                            'ptos' will contain the HIGHEST valid address of the stack.  Similarly, if
*                            OS_STK_GROWTH is set to 0, the 'ptos' will contains the LOWEST valid address
*                            of the stack. 任务堆栈地址
*
*              opt           specifies options that can be used to alter the behavior of OSTaskStkInit().
*                            (see uCOS_II.H for OS_TASK_OPT_xxx).
*
* Returns    : Always returns the location of the new top-of-stack once the processor registers have
*              been placed on the stack in the proper order.，返回新的栈顶
*
* Note(s)    : 1) Interrupts are enabled when your task starts executing.
*              2) All tasks run in Thread mode, using process stack.
*********************************************************************************************************
*/

OS_STK *OSTaskStkInit (void (*task)(void *p_arg), void *p_arg, OS_STK *ptos, INT16U opt)
{
    OS_STK *stk;


    (void)opt;                                   /* 'opt' is not used, prevent warning                 */
    stk       = ptos;                            /* Load stack pointer                                 */

                                                 /* Registers stacked as if auto-saved on exception    */
    *(stk)    = (INT32U)0x01000000L;             /* xPSR                                               */
    *(--stk)  = (INT32U)task;                    /* Entry Point                                        */
    *(--stk)  = (INT32U)0xFFFFFFFEL;             /* R14 (LR) (init value will cause fault if ever used)*/
    *(--stk)  = (INT32U)0x12121212L;             /* R12                                                */
    *(--stk)  = (INT32U)0x03030303L;             /* R3                                                 */
    *(--stk)  = (INT32U)0x02020202L;             /* R2                                                 */
    *(--stk)  = (INT32U)0x01010101L;             /* R1                                                 */
    *(--stk)  = (INT32U)p_arg;                   /* R0 : argument                                      */

                                                 /* Remaining registers saved on process stack         */
    *(--stk)  = (INT32U)0x11111111L;             /* R11                                                */
    *(--stk)  = (INT32U)0x10101010L;             /* R10                                                */
    *(--stk)  = (INT32U)0x09090909L;             /* R9                                                 */
    *(--stk)  = (INT32U)0x08080808L;             /* R8                                                 */
    *(--stk)  = (INT32U)0x07070707L;             /* R7                                                 */
    *(--stk)  = (INT32U)0x06060606L;             /* R6                                                 */
    *(--stk)  = (INT32U)0x05050505L;             /* R5                                                 */
    *(--stk)  = (INT32U)0x04040404L;             /* R4                                                 */

    return (stk);
}






/*----------------------- (C) COPYRIGHT @ 2012 liycobl -----------------  end of file -----------------*/
