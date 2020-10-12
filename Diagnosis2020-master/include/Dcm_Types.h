/******************************************************************************
 * Module      : DCM                                                          *
 * File Name   : Dcm_Types.h                                                  *
 * Description : Header file of Dcm Type Definitions                          *
 * Created on  : April 6, 2020                                                *
 ******************************************************************************/

#ifndef INCLUDE_DCM_TYPES_H_
#define INCLUDE_DCM_TYPES_H_


#include "Rte_Dcm_Type.h"
#include "Com_Types.h"
#include "ComStack_Types.h"

/**************
 * DCM-BUFFER *
 **************/

/* Base type for diagnostic message item */
typedef uint8 Dcm_MsgItemType;

/* Base type for diagnostic message (request, positive or negative response) */
typedef Dcm_MsgItemType*  Dcm_MsgType;

/*
 * Length of diagnostic message (request, positive or negative response).
 *  The maximum length is dependent of the underlying transport protocol/media.
 */
typedef uint32 Dcm_MsgLenType;

/* Additional information on message request. */

typedef struct
{
   uint8      reqType;
   uint8      suppressPosResponse;
}Dcm_MsgAddInfoType;

/*
 * This message context identifier can be used to determine the relation
 * between request and response confirmation.
 */
typedef uint8 Dcm_IdContextType;

/*
 *  This data structure contains all information which is necessary
 *  to process a diagnostic message from request to response and response confirmation.
 */
typedef struct
{
    Dcm_MsgType         reqData;
    Dcm_MsgLenType      reqDataLen;
    Dcm_MsgType         resData;
    Dcm_MsgLenType      resDataLen;
    Dcm_MsgAddInfoType  msgAddInfo;
    Dcm_MsgLenType      resMaxDataLen;
    Dcm_IdContextType   idContext;
    PduIdType           dcmRxPduId;
}Dcm_MsgContextType;


/*****************
 * DCM-PROTOCOLS *
 *****************/
typedef uint8 Dcm_ProtocolType;
#define DCM_OBD_ON_CAN                  ((Dcm_ProtocolType)0x00)
#define DCM_OBD_ON_FLEXRAY              ((Dcm_ProtocolType)0x01)
#define DCM_OBD_ON_IP                   ((Dcm_ProtocolType)0x02)
#define DCM_UDS_ON_CAN                  ((Dcm_ProtocolType)0x03)
#define DCM_UDS_ON_FLEXRAY              ((Dcm_ProtocolType)0x04)
#define DCM_UDS_ON_IP                   ((Dcm_ProtocolType)0x05)
#define DCM_ROE_ON_CAN                  ((Dcm_ProtocolType)0x06)
#define DCM_ROE_ON_FLEXRAY              ((Dcm_ProtocolType)0x07)
#define DCM_ROE_ON_IP                   ((Dcm_ProtocolType)0x08)
#define DCM_PERIODICTRANS_ON_CAN        ((Dcm_ProtocolType)0x09)
#define DCM_PERIODICTRANS_ON_FLEXRAY    ((Dcm_ProtocolType)0x0A)
#define DCM_PERIODICTRANS_ON_IP         ((Dcm_ProtocolType)0x0B)
#define DCM_NO_ACTIVE_PROTOCOL          ((Dcm_ProtocolType)0x0C)
#define DCM_UDS_ON_LIN                  ((Dcm_ProtocolType)0x0D)



/************************************
 *INDICATE-CURRENT-OPERATION-STATUS *
 ************************************/

typedef uint8  Dcm_ExtendedOpStatusType ;
#define DCM_INITIAL                 ((Dcm_ExtendedOpStatusType)0x00)
#define DCM_PENDING                 ((Dcm_ExtendedOpStatusType)0x01)
#define DCM_CANCEL                  ((Dcm_ExtendedOpStatusType)0x02)
#define DCM_FORCE_RCRRP_OK          ((Dcm_ExtendedOpStatusType)0x03)
#define DCM_POS_RESPONSE_SENT       ((Dcm_ExtendedOpStatusType)0x04)
#define DCM_POS_RESPONSE_FAILED     ((Dcm_ExtendedOpStatusType)0x05)
#define DCM_NEG_RESPONSE_SENT       ((Dcm_ExtendedOpStatusType)0x06)
#define DCM_NEG_RESPONSE_FAILED     ((Dcm_ExtendedOpStatusType)0x07)



/*************************************
 * EXTENDED-TYPES-OF-STD-RETURN-TYPE *
 *************************************/

#define DCM_E_PENDING               ((Std_ReturnType)0x10)
#define DCM_E_COMPARE_KEY_FAILED    ((Std_ReturnType)0x11)
#define DCM_E_FORCE_RCRRP           ((Std_ReturnType)0x12)

/**********************
 * DCM-SESSION-LEVELS *
 **********************/
typedef uint8  Dcm_SesCtrlType;

#define DCM_DEFAULT_SESSION                    ((Dcm_SesCtrlType)0x00)
#define DCM_PROGRAMMING_SESSION                (Dcm_SesCtrlType)(0x01)
#define DCM_EXTENDED_DIAGNOSTIC_SESSION        (Dcm_SesCtrlType)(0x02)
#define DCM_SAFETY_SYSTEM_DIAGNOSTIC_SESSION   (Dcm_SesCtrlType)(0x03)


/*****************************
 * SUPPORTED-SECURITY-LEVELS *
 *****************************/
typedef uint8 Dcm_SecLevelType;

#define DCM_SEC_LEV_LOCKED         ((Dcm_SecLevelType)0x00)
#define DCM_SEC_LEV_1              ((Dcm_SecLevelType)0x01)
#define DCM_SEC_LEV_2              ((Dcm_SecLevelType)0x02)



/*********************
 * UDS-SUPPORTED-SID *
 *********************/

#define DCM_SID        ((uint8)0)
#define DCM_SUB_FUN    ((uint8)1)

#define DCM_NEGATIVE_RESPONSE_SID   ((uint8)0x7F)
#define DCM_POS_RESPONSE_SID        ((uint8)0x40)

#define DCM_SESSION_CONTROL_SID     ((uint8)0x10)
#define DCM_TESTER_PRESENT_SID      ((uint8)0x3E)
#define DCM_READ_DATAT_BY_ID_SID    ((uint8)0x22)
#define DCM_WRITE_DATAT_BY_ID_SID   ((uint8)0x2E)
#define DCM_SECURITY_ACCESS_SID     ((uint8)0x27)
#define DCM_ROUTINE_CONTROL_SID     ((uint8)0x31)

/**********************
 * UDS-SUPPORTED-DIDS *
 **********************/

#define DCM_DID_LENGTH                             ((uint16)2)

#define DCM_BATTERY_POSITIVE_VOLTAGE_DID           ((uint16)0x0110)
#define DCM_VIN_NUM_DID                            ((uint16)0xF190)


/*********************************
 * UDS-SUPPORTED-ROUTINE-SUBFUNC *
 *********************************/

#define DCM_UDS_CHECK_MEMORY_RID                          ((uint16)0x0202)

#define DCM_UDS_START_ROUTINE                              0x01
#define DCM_UDS_STOP_ROUTINE                               0x02
#define DCM_UDS_REQUEST_ROUTINE_RESULTS                    0x03


/*****************
 * USE-PORT-TYPE *
 *****************/

typedef uint8 Dcm_DspDataUsePortType;

#define DCM_USE_BLOCK_ID                        ((Dcm_DspDataUsePortType)0x00)
#define DCM_USE_DATA_ASYNCH_CLIENT_SERVER       ((Dcm_DspDataUsePortType)0x01)
#define DCM_USE_DATA_ASYNCH_CLIENT_SERVER_ERROR ((Dcm_DspDataUsePortType)0x02)
#define DCM_USE_DATA_ASYNCH_FNC                 ((Dcm_DspDataUsePortType)0x03)
#define DCM_USE_DATA_ASYNCH_FNC_ERROR           ((Dcm_DspDataUsePortType)0x04)
#define DCM_USE_DATA_SENDER_RECEIVE             ((Dcm_DspDataUsePortType)0x05)
#define USE_DATA_SENDER_RECEIVER_AS_SERVICE     ((Dcm_DspDataUsePortType)0x06)
#define DCM_USE_DATA_SYNCH_CLIENT_SERVER        ((Dcm_DspDataUsePortType)0x07)
#define DCM_USE_DATA_SYNCH_FNC                  ((Dcm_DspDataUsePortType)0x08)
#define DCM_USE_ECU_SIGNAL                      ((Dcm_DspDataUsePortType)0x09)



/*************
 * DATA-TYPE *
 *************/

typedef uint8  Dcm_DspDataTypeType;

#define DCM_BOOLEAN             ((Dcm_DspDataTypeType)0x01)
#define DCM_SINT16              ((Dcm_DspDataTypeType)0x02)
#define DCM_SINT16_N            ((Dcm_DspDataTypeType)0x03)
#define DCM_SINT32              ((Dcm_DspDataTypeType)0x04)
#define DCM_SINT32_N            ((Dcm_DspDataTypeType)0x05)
#define DCM_SINT8               ((Dcm_DspDataTypeType)0x06)
#define DCM_SINT8_N             ((Dcm_DspDataTypeType)0x07)
#define DCM_UINT16              ((Dcm_DspDataTypeType)0x08)
#define DCM_UINT16_N            ((Dcm_DspDataTypeType)0x09)
#define DCM_UINT32              ((Dcm_DspDataTypeType)0x0A)
#define DCM_UINT32_N            ((Dcm_DspDataTypeType)0x0B)
#define DCM_UINT8               ((Dcm_DspDataTypeType)0x0C)
#define DCM_UINT8_DYN           ((Dcm_DspDataTypeType)0x0D)
#define DCM_UINT8_N             ((Dcm_DspDataTypeType)0x0E)


/********************
 * IN-OUT-OPERATION *
 ********************/
typedef uint8  Dcm_IOOperationResponseType;
#define DCM_POSITIVE_RESPONSE           (Dcm_IOOperationResponseType)(0x00)
#define DCM_GENERAL_REJECT              (Dcm_IOOperationResponseType)(0x10)
#define DCM_BUSY_REPEAT_REQUEST         (Dcm_IOOperationResponseType)(0x21)
#define DCM_CONDITIONS_NOT_CORRECT      (Dcm_IOOperationResponseType)(0x22)
#define DCM_REQUEST_OUT_OF_RANGE       (Dcm_IOOperationResponseType)(0x31)
#define DCM_RESPONSE_PENDING            (Dcm_IOOperationResponseType)(0x78)


/***********************************
 * DCM_NEGATIVE-RESPONSE-CODE-TYPE *
 ***********************************/

#define DCM_NRC_RESPONSE_LENGTH     ((uint8)3)

typedef uint8 Dcm_NegativeResponseCodeType;

#define DCM_POS_RESP                                    ((Dcm_NegativeResponseCodeType)0x00)
#define DCM_E_GENERALREJECT                             ((Dcm_NegativeResponseCodeType)0x10)
#define DCM_E_SERVICENOTSUPPORTED                       ((Dcm_NegativeResponseCodeType)0x11)
#define DCM_E_SUBFUNCTIONNOTSUPPORTED                   ((Dcm_NegativeResponseCodeType)0x12)
#define DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT     ((Dcm_NegativeResponseCodeType)0x13)
#define DCM_E_RESPONSETOOLONG                           ((Dcm_NegativeResponseCodeType)0x14)
#define DCM_E_BUSYREPEATREQUEST                         ((Dcm_NegativeResponseCodeType)0x21)
#define DCM_E_CONDITIONSNOTCORRECT                      ((Dcm_NegativeResponseCodeType)0x22)
#define DCM_E_REQUESTSEQUENCEERROR                      ((Dcm_NegativeResponseCodeType)0x24)
#define DCM_E_NORESPONSEFROMSUBNETCOMPONENT             ((Dcm_NegativeResponseCodeType)0x25)
#define DCM_E_FAILUREPREVENTSEXECUTIONOFREQUESTEDACTION ((Dcm_NegativeResponseCodeType)0x26)
#define DCM_E_REQUESTOUTOFRANGE                         ((Dcm_NegativeResponseCodeType)0x31)
#define DCM_E_SECURITYACCESSDENIED                      ((Dcm_NegativeResponseCodeType)0x33)
#define DCM_E_INVALIDKEY                                ((Dcm_NegativeResponseCodeType)0x35)
#define DCM_E_EXCEEDNUMBEROFATTEMPTS                    ((Dcm_NegativeResponseCodeType)0x36)
#define DCM_E_REQUIREDTIMEDELAYNOTEXPIRED               ((Dcm_NegativeResponseCodeType)0x37)
#define DCM_E_UPLOADDOWNLOADNOTACCEPTED                 ((Dcm_NegativeResponseCodeType)0x70)
#define DCM_E_TRANSFERDATASUSPENDED                     ((Dcm_NegativeResponseCodeType)0x71)
#define DCM_E_GENERALPROGRAMMINGFAILURE                 ((Dcm_NegativeResponseCodeType)0x72)
#define DCM_E_WRONGBLOCKSEQUENCECOUNTER                 ((Dcm_NegativeResponseCodeType)0x73)
#define DCM_E_SUBFUNCTIONNOTSUPPORTEDINACTIVESESSION    ((Dcm_NegativeResponseCodeType)0x7E)
#define DCM_E_SERVICENOTSUPPORTEDINACTIVESESSION        ((Dcm_NegativeResponseCodeType)0x7F)
#define DCM_E_RPMTOOHIGH                                ((Dcm_NegativeResponseCodeType)0x81)
#define DCM_E_RPMTOOLOW                                 ((Dcm_NegativeResponseCodeType)0x82)
#define DCM_E_ENGINEISRUNNING                           ((Dcm_NegativeResponseCodeType)0x83)
#define DCM_E_ENGINEISNOTRUNNING                        ((Dcm_NegativeResponseCodeType)0x84)
#define DCM_E_ENGINERUNTIMETOOLOW                       ((Dcm_NegativeResponseCodeType)0x85)
#define DCM_E_TEMPERATURETOOHIGH                        ((Dcm_NegativeResponseCodeType)0x86)
#define DCM_E_TEMPERATURETOOLOW                         ((Dcm_NegativeResponseCodeType)0x87)
#define DCM_E_VEHICLESPEEDTOOHIGH                       ((Dcm_NegativeResponseCodeType)0x88)
#define DCM_E_VEHICLESPEEDTOOLOW                        ((Dcm_NegativeResponseCodeType)0x89)
#define DCM_E_THROTTLE_PEDALTOOHIGH                     ((Dcm_NegativeResponseCodeType)0x8A)
#define DCM_E_THROTTLE_PEDALTOOLOW                      ((Dcm_NegativeResponseCodeType)0x8B)
#define DCM_E_TRANSMISSIONRANGENOTINNEUTRAL             ((Dcm_NegativeResponseCodeType)0x8C)
#define DCM_E_TRANSMISSIONRANGENOTINGEAR                ((Dcm_NegativeResponseCodeType)0x8D)
#define DCM_E_BRAKESWITCH_NOTCLOSED                     ((Dcm_NegativeResponseCodeType)0x8F)
#define DCM_E_SHIFTERLEVERNOTINPARK                     ((Dcm_NegativeResponseCodeType)0x90)
#define DCM_E_TORQUECONVERTERCLUTCHLOCKED               ((Dcm_NegativeResponseCodeType)0x91)
#define DCM_E_VOLTAGETOOHIGH                            ((Dcm_NegativeResponseCodeType)0x92)
#define DCM_E_VOLTAGETOOLOW                             ((Dcm_NegativeResponseCodeType)0x93)
#define DCM_E_VMSCNC_0                                  ((Dcm_NegativeResponseCodeType)0xF0)
#define DCM_E_VMSCNC_1                                  ((Dcm_NegativeResponseCodeType)0xF1)
#define DCM_E_VMSCNC_2                                  ((Dcm_NegativeResponseCodeType)0xF2)
#define DCM_E_VMSCNC_3                                  ((Dcm_NegativeResponseCodeType)0xF3)
#define DCM_E_VMSCNC_4                                  ((Dcm_NegativeResponseCodeType)0xF4)
#define DCM_E_VMSCNC_5                                  ((Dcm_NegativeResponseCodeType)0xF5)
#define DCM_E_VMSCNC_6                                  ((Dcm_NegativeResponseCodeType)0xF6)
#define DCM_E_VMSCNC_7                                  ((Dcm_NegativeResponseCodeType)0xF7)
#define DCM_E_VMSCNC_8                                  ((Dcm_NegativeResponseCodeType)0xF8)
#define DCM_E_VMSCNC_9                                  ((Dcm_NegativeResponseCodeType)0xF9)
#define DCM_E_VMSCNC_A                                  ((Dcm_NegativeResponseCodeType)0xFA)
#define DCM_E_VMSCNC_B                                  ((Dcm_NegativeResponseCodeType)0xFB)
#define DCM_E_VMSCNC_C                                  ((Dcm_NegativeResponseCodeType)0xFC)
#define DCM_E_VMSCNC_D                                  ((Dcm_NegativeResponseCodeType)0xFD)
#define DCM_E_VMSCNC_E                                  ((Dcm_NegativeResponseCodeType)0xFE)


/***************************
 * DCM-GENERAL-USED-MACROS *
 ***************************/

#define DCM_INITIALIZED_TO_ZERO                     (0)
#define DCM_BYTE                                    (8)

#define DCM_SID_INDEX                               (0)
#define DCM_SUB_FUN_INDEX                           (1)
#define SUPPRESS_POS_RSP_MSG_INDICATION_BIT         0x80


#endif /* INCLUDE_DCM_TYPES_H_ */
