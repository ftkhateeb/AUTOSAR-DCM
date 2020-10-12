/******************************************************************************
 * Module      : DCM                                                          *
 * SubModule   : DSD                                                          *
 * File Name   : Dsd.h                                                        *
 * Description : Header file of Dsd Sub-Module                                *
 * Created on  : April 6, 2020                                                *
 ******************************************************************************/

#ifndef INCLUDE_DCM_DSD_H_
#define INCLUDE_DCM_DSD_H_


#include "Dcm_Lcfg.h"
#include "Dcm_CommonMacros.h"

/*************************************************
 *                                               *
 *                 DSD-PROTOTYPES                *
 *                                               *
 *************************************************/

extern void DsdMain(void);

extern void DspToDsdForwardingResponse(Std_ReturnType ResponseType);

extern void DslToDsdDataIndication(const Dcm_DslProtocolRowType *Active_protocol);

extern void DsdSetDiagMsgContext(Dcm_MsgContextType **DiagMsg,
                                 DsdRunTimeDataType **DsdRunTime,
                                 const Dcm_DslProtocolRowType *Active_Protocol);

#endif /* INCLUDE_DCM_DSD_H_ */
