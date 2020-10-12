/*****************************************************************************
 * Module       : DCM                                                        *
 * File Name    : Dcm_Cfg.h                                                  *
 * Description  : Header file of call back function                          *
 * Created ON   : 20/4/2020                                                  *
 *****************************************************************************/

#ifndef INCLUDE_DCM_CBK_H_
#define INCLUDE_DCM_CBK_H_



extern void Dcm_TpRxIndication (PduIdType id, Std_ReturnType result);

extern void Dcm_TpTxConfirmation(PduIdType id,Std_ReturnType result);

extern BufReq_ReturnType Dcm_StartOfReception(PduIdType id,
                                              const PduInfoType* info,
                                              PduLengthType TpSduLength,
                                              PduLengthType* bufferSizePtr);

extern BufReq_ReturnType Dcm_CopyRxData(PduIdType id,
                                        const PduInfoType* info,
                                        PduLengthType* bufferSizePtr);

extern BufReq_ReturnType Dcm_CopyTxData(PduIdType id,
                                        const PduInfoType* info,
                                        const RetryInfoType* retry,
                                        PduLengthType* availableDataPtr);

#endif /* INCLUDE_DCM_CBK_H_ */
