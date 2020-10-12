/******************************************************************************
 * Module      : DCM                                                          *
 * SubModule   : DSP                                                          *
 * File Name   : Dsp.h                                                        *
 * Description : Header file of Dsp Sub-Module                                *
 * Created on  : April 6, 2020                                                *
 ******************************************************************************/

#ifndef INCLUDE_DCM_DSP_H_
#define INCLUDE_DCM_DSP_H_


#include "Dcm_Lcfg.h"
#include "Dcm_CommonMacros.h"

/*************************************************
 *                                               *
 *                 DSP-PROTOTYPES                *
 *                                               *
 *************************************************/

extern void Dsp_Init(const Dcm_ConfigType* ConfigPtr);

extern void DsdDspIndication(const Dcm_DsdServiceType *currentServicePtr,
                             Dcm_MsgContextType* pMsgContext,
                             Dcm_NegativeResponseCodeType* ErrorCode);
					
extern void DspInternal_CancelPendingRequest(void);

extern void DspMain(void);

extern boolean DspSecurityLevelPermission(const uint8 numOfLevels, Dcm_DspSecurityRowType const* const* securityLevels);

extern boolean DspSessionLevelPermission(const uint8 numOfLevels, Dcm_DspSessionRowType const* const* sessionLevels);

extern Std_ReturnType Dsp_UDS_DiagnosticSessionControl0x10(Dcm_MsgContextType* pMsgContext,
                                                           Dcm_NegativeResponseCodeType* ErrorCode);

extern Std_ReturnType Dsp_UDS_TesterPresent0x3E(Dcm_MsgContextType* pMsgContext,
                                                Dcm_NegativeResponseCodeType* ErrorCode );

extern Std_ReturnType Dsp_UDS_ReadDataByIdentifier_0x22(Dcm_ExtendedOpStatusType OpStatus,
                                                        Dcm_MsgContextType* pMsgContext,
                                                        Dcm_NegativeResponseCodeType* ErrorCode);

extern Std_ReturnType Dsp_UDS_SecurityAccess_0x27(Dcm_ExtendedOpStatusType OpStatus,
                                                  Dcm_MsgContextType* pMsgContext,
                                                  Dcm_NegativeResponseCodeType* ErrorCode);

extern Std_ReturnType Dsp_UDS_WriteDataByIdentifier_0x2E(Dcm_ExtendedOpStatusType OpStatus,
                                                         Dcm_MsgContextType* pMsgContext,
                                                         Dcm_NegativeResponseCodeType* ErrorCode);

extern Std_ReturnType Dsp_UDS_RoutineControl_0x31(Dcm_ExtendedOpStatusType OpStatus,
                                                  Dcm_MsgContextType* pMsgContext,
                                                  Dcm_NegativeResponseCodeType* ErrorCode);

#endif /* INCLUDE_DCM_DSP_H_ */
