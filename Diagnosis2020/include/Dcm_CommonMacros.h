/******************************************************************************
 * File Name   : Dcm_CommonMacros.h                                           *
 * Description : Header file of Common Used Dcm Macros                        *
 * Created on  : April 6, 2020                                                *
 ******************************************************************************/


#ifndef INCLUDE_DCM_COMMONMACROS_H_
#define INCLUDE_DCM_COMMONMACROS_H_

#include "Dcm_Types.h"
#include "Dcm_Cfg.h"

#define DCM_CONVERT_MS_TO_MAIN_CYCLES(TimeoutTimerInMs)  ((TimeoutTimerInMs)/DCM_MAIN_FUNCTION_PERIOD_TIME_MS)

#define DCM_CONVERT_SEC_TO_MS(TimeoutTimerInSec)         ((TimeoutTimerInSec)*1000)

#define DECREMENT_TIMER_COUNTS(CurrentTimerCounts)       {if(CurrentTimerCounts > 0) {CurrentTimerCounts--;}}


#endif /* INCLUDE_DCM_COMMONMACROS_H_ */
