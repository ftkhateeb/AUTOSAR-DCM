/******************************************************************************
 * Module      : DCM                                                          *
 * File Name   : Dcm.h                                                        *
 * Description : Header file of Dcm Module                                    *
 * Created on  : April 6, 2020                                                *
 ******************************************************************************/

#ifndef INCLUDE_DCM_H_
#define INCLUDE_DCM_H_

#include "Dcm_Dsl.h"
#include "Dcm_Dsd.h"
#include "Dcm_Dsp.h"
#include "Det.h"




/*********************
 * DCM-STATUS-MACROS *
 *********************/

#define DCM_NOT_INITIALIZED    ((uint8)0x00)
#define DCM_INITIALIZED        ((uint8)0x01)



/*
 * Dcm Module ID
 */
#define DCM_MODULE_ID                       ((uint16)53)

/*
 * Dcm instance ID
 */
#define DCM_INSTANCE_ID                     (0U)


/*******************
 * DET-ERROR-CODES *
 *******************/

#define DCM_E_INTERFACE_TIMEOUT             ((uint8)0x01)

#define DCM_E_INTERFACE_RETURN_VALUE        ((uint8)0x02)

#define DCM_E_INTERFACE_BUFFER_OVERFLOW     ((uint8)0x03)

#define DCM_E_UNINIT                        ((uint8)0x05)

#define DCM_E_PARAM                         ((uint8)0x06)

#define DCM_E_PARAM_POINTER                 ((uint8)0x07)

#define DCM_E_INIT_FAILED                   ((uint8)0x08)

#define DCM_E_PARAM_CONFIG                  ((uint8)0x09)

#define DCM_E_INVALID_VALUE                 ((uint8)0x01)

/************************
 * DCM-API-SERVICES-SID *
 ************************/

#define DCM_INIT_SID                            ((uint8)0x01)

#define DCM_START_OF_RECEPTION_SID              ((uint8)0x46)

#define DCM_COPY_RX_DATA_SID                    ((uint8)0x44)

#define DCM_TP_RX_INDICATION_SID                ((uint8)0x45)

#define DCM_COPY_TX_DATA_SID                    ((uint8)0x43)

#define DCM_TP_TX_CONFIRMATION_SID              ((uint8)0x40)

#define DCM_MAIN_FUNCTION_SID                   ((uint8)0x25)

#define DCM_CONDITION_CHECK_READ_1_SID          ((uint8)0x49)

#define DCM_CONDITION_CHECK_READ_2_SID          ((uint8)0x37)

#define DCM_READ_DATA_LENGTH_1_SID              ((uint8)0x36)

#define DCM_READ_DATA_LENGTH_2_SID              ((uint8)0x4C)

#define DCM_READ_DATA_1_SID                     ((uint8)0x34)

#define DCM_READ_DATA_2_SID                     ((uint8)0x3B)

#define DCM_READ_DATA_3_SID                     ((uint8)0x58)

#define DCM_WRITE_DATA_1_SID                    ((uint8)0x51)

#define DCM_WRITE_DATA_2_SID                    ((uint8)0x52)

#define DCM_WRITE_DATA_3_SID                    ((uint8)0x35)

#define DCM_WRITE_DATA_4_SID                    ((uint8)0x3E)

#define DCM_READ_DID_DATA                       ((uint8)0x40)

#define DCM_WRITE_DID_DATA                      ((uint8)0x41)

#define DCM_GET_SEED_SID                        ((uint8)0x44)

#define DCM_COMPARE_KEY_SID                     ((uint8)0x47)

#define DCM_GET_SECURITY_ATTEMPT_COUNTER_SID    ((uint8)0x59)

#define DCM_SET_SECURITY_ATTEMPT_COUNTER_SID    ((uint8)0x5A)

#define DCM_START_ROUTINE_SID                   ((uint8)0x5B)

#define DCM_STOP_ROUTINE_SID                    ((uint8)0x5C)

#define DCM_REQUEST_ROUTINE_RESULTS_SID         ((uint8)0x5D)


/***************************
 * DCM-FUNCTIONS-PROTOTYPE *
 ***************************/
extern void Dcm_Init(const Dcm_ConfigType* ConfigPtr);


extern void Dcm_MainFunction (void);


extern Std_ReturnType Dcm_GetSecurityLevel(Dcm_SecLevelType* SecLevel);


#endif /* INCLUDE_DCM_H_ */
