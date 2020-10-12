/******************************************************************************
 * Module      : DCM                                                          *
 * SubModule   : DSL                                                          *
 * File Name   : Dsl.h                                                        *
 * Description : Header file of Dsl Sub-Module                                *
 * Created on  : April 6, 2020                                                *
 ******************************************************************************/

#ifndef INCLUDE_DCM_DSL_H_
#define INCLUDE_DCM_DSL_H_

#include "Dcm_Lcfg.h"

/*************************************************
 *                                               *
 *                 DSL-PROTOTYPES                *
 *                                               *
 *************************************************/

extern void Dsl_Init(const Dcm_ConfigType* ConfigPtr);

extern void DslMain(void);

extern void DslSetSecurityLevel(Dcm_SecLevelType secLevel);

extern void Dcm_PduRToDslIndication(PduIdType id, Std_ReturnType result);

extern void Reset_S3ServerTimer(void);

extern void Dsl_GetActiveProtoclRow(const Dcm_DslProtocolRowType **ActiveProtocolRow);

extern void Dsl_TpTxConfirmation(PduIdType id,Std_ReturnType result);

extern void DsdToDslForwardingResponse(DsdProcessingDoneResponseType rsponseStatus,uint32 resLength);

extern boolean ChangeSessionLevel(Dcm_SesCtrlType Newsession);

extern BufReq_ReturnType AcceptNewConnection(PduIdType id,
                                             const PduInfoType* info,
                                             PduLengthType TpSduLength,
                                             PduLengthType* bufferSizePtr);

extern BufReq_ReturnType CopyRequestData(PduIdType id,
                                         const PduInfoType* info,
                                         PduLengthType* bufferSizePtr);

extern BufReq_ReturnType Dcm_CopyTransmitData(PduIdType id,
                                              const PduInfoType* info,
                                              const RetryInfoType* retry,
                                              PduLengthType* availableDataPtr);

extern Std_ReturnType PduR_DcmTransmit(PduIdType TxPduId, PduInfoType *TxPduInfo);

extern Std_ReturnType Dcm_ResetToDefaultSession(void);

extern Std_ReturnType Dcm_GetSesCtrlType(Dcm_SesCtrlType* SesCtrlType);

extern Std_ReturnType DslInternal_SetSesCtrlType(Dcm_SesCtrlType SesCtrlType);

extern Std_ReturnType Dsl_GetSecurityLevel(Dcm_SecLevelType *SecLevel);


#endif /* INCLUDE_DCM_DSL_H_ */
