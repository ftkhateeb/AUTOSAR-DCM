/******************************************************************************
 * Module      : DCM                                                          *
 * SubModule   : Used in DSL,DSD and DSP                                      *
 * File Name   : Lcfg.h                                                       *
 * Description : Header file of Link Time Configuration Parameters            *
 * Created on  : Mar 29, 2020                                                 *
 ******************************************************************************/

#ifndef INCLUDE_DCM_LCFG_H_
#define INCLUDE_DCM_LCFG_H_


#include "include/Std_Types.h"
#include "include/ComStack_Types.h"
#include "include/Dcm_Types.h"
#include "include/Dcm_Cfg.h"



/******************************************************************
 * Type Definition for Pointers to Internal and External Services *
 ******************************************************************/
typedef Std_ReturnType (*DcmDsdSidTabFncType)(Dcm_ExtendedOpStatusType OpStatus,
                         Dcm_MsgContextType* pMsgContext, Dcm_NegativeResponseCodeType* ErrorCode);
typedef Std_ReturnType (*DcmDsdSidIntrnalFncType)(Dcm_MsgContextType*  pMsgContext,
                         Dcm_NegativeResponseCodeType* ErrorCode);


/**************************************************************
 * Type Definition for Pointers to Callout/Callback Functions *
 **************************************************************/

/* Read Data by Identifier 0x22 */
typedef Std_ReturnType (*Dcm_DataConditionCheckReadFncType)(Dcm_NegativeResponseCodeType* ErrorCode);
typedef Std_ReturnType (*Dcm_DataConditionCheckReadErrorFncType)(Dcm_OpStatusType OpStatus,Dcm_NegativeResponseCodeType* ErrorCode);
typedef Std_ReturnType (*Dcm_DataReadDataLengthSynFncType)(uint16* DataLength);
typedef Std_ReturnType (*Dcm_DataReadDataLengthAsynFncType)(Dcm_OpStatusType OpStatus, uint16* DataLength);
typedef Std_ReturnType (*Dcm_DataReadSynFncType)(uint8* Data);
typedef Std_ReturnType (*Dcm_DataReadAsynFncType)(Dcm_OpStatusType OpStatus,uint8* Data);
typedef Std_ReturnType (*Dcm_DataReadAsynErrorFncType)(Dcm_OpStatusType OpStatus,uint8* Data,Dcm_NegativeResponseCodeType* ErrorCode);

/* Write Data By ID 0x2E */
typedef Std_ReturnType (*Dcm_DataWriteSynFncType)(const uint8* Data, Dcm_NegativeResponseCodeType* ErrorCode);
typedef Std_ReturnType (*Dcm_DataWriteSynDynFncType)(const uint8* Data,uint16 DataLength, Dcm_NegativeResponseCodeType* ErrorCode);
typedef Std_ReturnType (*Dcm_DataWriteAsynFncType)(const uint8* Data,Dcm_OpStatusType OpStatus,Dcm_NegativeResponseCodeType* ErrorCode);
typedef Std_ReturnType (*Dcm_DataWriteAsynDynFncType)(const uint8* Data, uint16 DataLength, Dcm_OpStatusType OpStatus,Dcm_NegativeResponseCodeType* ErrorCode);

/* Security Access 0x27 */
typedef Std_ReturnType (*Dcm_CompareKeyFncType)(const uint8* Key, Dcm_OpStatusType OpStatus, Dcm_NegativeResponseCodeType* ErrorCode);
typedef Std_ReturnType (*Dcm_GetAttemptCounterFncType)(Dcm_OpStatusType OpStatus, uint8* AttemptCounter);
typedef Std_ReturnType (*Dcm_GetSeed_ADRSizeFncType)(const uint8* SecurityAccessDataRecord, Dcm_OpStatusType OpStatus, uint8* Seed, Dcm_NegativeResponseCodeType* ErrorCode);
typedef Std_ReturnType (*Dcm_GetSeedFncType)( Dcm_OpStatusType OpStatus, uint8* Seed, Dcm_NegativeResponseCodeType* ErrorCode );
typedef Std_ReturnType (*Dcm_SetAttemptCounterFncType)(Dcm_OpStatusType OpStatus, uint8 AttemptCounter);

/* Routine Control 0x31 */
typedef Std_ReturnType (*Dcm_StartRoutineFncType)(Dcm_OpStatusType OpStatus, Dcm_NegativeResponseCodeType* ErrorCode);
typedef Std_ReturnType (*Dcm_StopRoutineFncType)(Dcm_OpStatusType OpStatus, Dcm_NegativeResponseCodeType* ErrorCode);
typedef Std_ReturnType (*Dcm_RequestRoutineResultsFncType)(Dcm_OpStatusType OpStatus,
                                                           uint8* dataOutVar,
                                                           uint16* variableDataLength,
                                                           Dcm_NegativeResponseCodeType* ErrorCode);


                                                    /******************
                                                     * DCM Containers *
                                                     ******************/

                                           /*******************************************
 ********************************************    DSP Used Configuration Parameters    ******************************
                                            *******************************************/


/**********************************************
 *            * Container Info *              *
 *                                            *
 * Container_Name        : Dcm_DspSecurityRow *
 * Parent_Container_Name : Dcm_DspSecurity    *
 * Container_Index       : 10.3.5.15.2        *
 **********************************************/
typedef struct
{
    uint32                             *Dcm_DspSecurityADRSize;
    boolean                             Dcm_DspSecurityAttemptCounterEnabled;
    float32                             Dcm_DspSecurityDelayTime;
    Dcm_CompareKeyFncType               DcmDspSecurityCompareKeyFnc;
    float32                             Dcm_DspSecurityDelayTimeOnBoot;
    Dcm_GetAttemptCounterFncType        Dcm_DspSecurityGetAttemptCounterFnc;
    Dcm_GetSeed_ADRSizeFncType          Dcm_DspSecurityGetSeed_ADRSizeFnc;
    Dcm_GetSeedFncType                  Dcm_DspSecurityGetSeedFnc;
    uint32                              Dcm_DspSecurityKeySize;
    Dcm_SecLevelType                    Dcm_DspSecurityLevel;
    uint8                               Dcm_DspSecurityNumAttDelay;
    uint32                              Dcm_DspSecuritySeedSize;
    Dcm_SetAttemptCounterFncType        Dcm_DspSecuritySetAttemptCounterFnc;
    Dcm_DspDataUsePortType              Dcm_DspSecurityUsePort;
}Dcm_DspSecurityRowType;



/*******************************************
 *         * Container Info *              *
 *                                         *
 * Container_Name        : Dcm_DspSecurity *
 * Parent_Container_Name : Dcm_Dsp         *
 * Container_Index       : 10.3.5.15.1     *
 *******************************************/
typedef struct
{
 const Dcm_DspSecurityRowType    **DcmDspSecurityRow;
}Dcm_DspSecurityType;



/*********************************************
 *         * Container Info *                *
 *                                           *
 * Container_Name        : Dcm_DspSessionRow *
 * Parent_Container_Name : Dcm_DspSession    *
 * Container_Index       : 10.3.5.15.4       *
 *********************************************/
typedef struct
{
   Dcm_SesCtrlType          DspSessionLevel;
   float32                  DspSessionP2ServerMax;
   float32                  DspSessionP2StarServerMax;
}Dcm_DspSessionRowType;



/******************************************
 *          * Container Info *            *
 *                                        *
 * Container_Name        : Dcm_DspSession *
 * Parent_Container_Name : Dcm_Dsp        *
 * Container_Index       : 10.3.5.15.3    *
 ******************************************/
typedef struct
{
  const Dcm_DspSessionRowType   **DspSessionRow;
}Dcm_DspSessionType;



/******************************************************
 *            * Container Info *                      *
 *                                                    *
 * Container_Name        : Dcm_DspCommonAuthorization *
 * Parent_Container_Name : Dcm_Dsp                    *
 * Container_Index       : 10.3.5.2                   *
 ******************************************************/
typedef struct
{
   const Dcm_DspSessionRowType       **DspCommonAuthorizationSessionRef;
   const Dcm_DspSecurityRowType      **DspCommonAuthorizationSecurityLevelRef;
   const uint8                         numSessLevels;
   const uint8                         numSecLevels;
}Dcm_DspCommonAuthorizationType;



/***********************************************
 *          * Container Info *                 *
 *                                             *
 * Container_Name        : Dcm_DspStartRoutine *
 * Parent_Container_Name : Dcm_DspRoutine      *
 * Container_Index       : 10.3.5.14.5         *
 ***********************************************/
typedef struct
{
  const  Dcm_StartRoutineFncType            DspStartRoutineFnc;
  const  Dcm_DspCommonAuthorizationType    *DspStartRoutineCommonAuthorizationRef;
}Dcm_DspStartRoutineType;



/**********************************************
 *         * Container Info *                 *
 *                                            *
 * Container_Name        : Dcm_DspStopRoutine *
 * Parent_Container_Name : Dcm_DspRoutine     *
 * Container_Index       : 10.3.5.14.10       *
 **********************************************/
typedef struct
{
  const  Dcm_StopRoutineFncType              DspStopRoutineFnc;
  const  Dcm_DspCommonAuthorizationType     *DspStopRoutineCommonAuthorizationRef;
}Dcm_DspStopRoutineType;



/********************************************************
 *                          * Container Info *          *
 *                                                      *
 * Container_Name        : Dcm_DspRequestRoutineResults *
 * Parent_Container_Name : Dcm_DspRoutine               *
 * Container_Index       : 10.3.5.14.2                  *
 ********************************************************/
typedef struct
{
  const Dcm_RequestRoutineResultsFncType      DspRequestRoutineResultsFnc;
  const Dcm_DspCommonAuthorizationType       *DspRequestRoutineResultsCommonAuthorizationRef;
}Dcm_DspRequestRoutineResultsType;



/******************************************
 *        * Container Info *              *
 *                                        *
 * Container_Name        : Dcm_DspRoutine *
 * Parent_Container_Name : Dcm_Dsp        *
 * Container_Index       : 10.3.5.14.1    *
 ******************************************/
typedef struct
{
       uint16                                DspRoutineIdentifier;
       boolean                               DspRoutineUsed;
 const Dcm_DspStartRoutineType              *DspStartRoutine;
 const Dcm_DspStopRoutineType               *DspStopRoutine;
 const Dcm_DspRequestRoutineResultsType     *DspRequestRoutineResults;
}Dcm_DspRoutineType;

/******************************************
 *          * Container Info *            *
 *                                        *
 * Container_Name        : Dcm_DspDidWrite*
 * Parent_Container_Name : Dcm_DspDidInfo *
 * Container_Index       : 10.3.5.3.4     *
 ******************************************/
typedef struct
{
   const Dcm_DspSecurityRowType  **DcmDspDidWriteSecurityLevelRef;
   const Dcm_DspSessionRowType   **DcmDspDidWriteSessionRef;
   const uint8                     numSessLevels;
   const uint8                     numSecLevels;
}Dcm_DspDidWriteType;


/******************************************
 *          * Container Info *            *
 *                                        *
 * Container_Name        : Dcm_DspDidRead *
 * Parent_Container_Name : Dcm_DspDidInfo *
 * Container_Index       : 10.3.5.3.3     *
 ******************************************/
typedef struct
{
   const Dcm_DspSecurityRowType  **DcmDspDidReadSecurityLevelRef;
   const Dcm_DspSessionRowType   **DcmDspDidReadSessionRef;
   const uint8                     numSessLevels;
   const uint8                     numSecLevels;
}Dcm_DspDidReadType;



/******************************************
 *           * Container Info *           *
 *                                        *
 * Container_Name        : Dcm_DspDidInfo *
 * Parent_Container_Name : Dcm_DspDid     *
 * Container_Index       : 10.3.5.3.2     *
 ******************************************/
typedef struct
{
  Dcm_DspDidReadType  *DspDidRead;
  Dcm_DspDidWriteType *DspDidWrite;
}Dcm_DspDidInfoType;



/********************************************
 *          * Container Info *              *
 *                                          *
 * Container_Name        : Dcm_DspData      *
 * Parent_Container_Name : Dcm_DspDidSignal *
 * Container_Index       : 10.3.5.5.1       *
 ********************************************/
typedef struct
{
    uint16                                       DspDataByteSize;
    boolean                                      DspDataConditionCheckReadFncUsed;
    Dcm_DataConditionCheckReadFncType            DspDataConditionCheckReadFnc;
    Dcm_DataConditionCheckReadErrorFncType       DspDataConditionCheckReadAsynFnc;
    Dcm_DataReadDataLengthSynFncType             DspDataReadDataLengthSynFnc;
    Dcm_DataReadDataLengthAsynFncType            DspDataReadDataLengthAsynFnc;
    Dcm_DataReadSynFncType                       DspDataReadSynFnc;
    Dcm_DataReadAsynFncType                      DspDataReadAsynFnc;
    Dcm_DataReadAsynErrorFncType                 DspDataReadAsynErrorFnc;
    Dcm_DataWriteSynFncType                      DspDataWriteSynFnc;
    Dcm_DataWriteAsynFncType                     DspDataWriteAsynFnc;
    Dcm_DataWriteSynDynFncType                   DspDataWriteSynDynFnc;
    Dcm_DataWriteAsynDynFncType                  DspDataWriteAsynDynFnc;
    Dcm_DspDataUsePortType                       DspDataUsePort;
    Dcm_DspDataTypeType                          DspDataType;
}Dcm_DspDataType;



/********************************************
 *         * Container Info *               *
 *                                          *
 * Container_Name        : Dcm_DspDidSignal *
 * Parent_Container_Name : Dcm_DspDid       *
 * Container_Index       : 10.3.5.3.4       *
 ********************************************/
typedef struct
{
  uint16               DspDidByteOffset;
  Dcm_DspDataType     *DspDidDataRef;
}Dcm_DspDidSignalType;



/**************************************
 *       * Container Info *           *
 *                                    *
 * Container_Name        : Dcm_DspDid *
 * Parent_Container_Name : Dcm_Dsp    *
 * Container_Index       : 10.3.5.3.1 *
 **************************************/
typedef struct Dcm_DspDidType
{
  uint16                          DspDidIdentifier;
  boolean                         DspDidUsed;
  const  Dcm_DspDidInfoType      *DspDidInfoRef;
  struct Dcm_DspDidType         **DspDidRef;
  const Dcm_DspDidSignalType    **DspDidSignal;
  const uint32                    DspDidNumOfSignals;
}Dcm_DspDidType;



/**********************************************
 *        * Container Info *                  *
 *                                            *
 * Container_Name        : Dcm_DataIdentifier *
 * Parent_Container_Name : Dcm_Dsp            *
 * Container_Index       : 10.3.5.3.1         *
 **********************************************/
typedef struct
{
   uint16   DspDidIdentifier;
   boolean  DspDidUsed;
}Dcm_DataIdentifierType;



/******************************************
 *      * Container Info *                *
 *                                        *
 * Container_Name        : Dcm_Dsp        *
 * Parent_Container_Name : Dcm_ConfigSet  *
 * Container_Index       : 10.3.5         *
 ******************************************/
typedef struct
{
  const  Dcm_DspSessionType      *DcmDspSession;
  const  Dcm_DspSecurityType     *DcmDspSecurity;
  const  Dcm_DspDidType         **DspDid;
  const  Dcm_DspRoutineType     **DspRoutines;
  const  Dcm_DataIdentifierType *supportedDid;
}Dcm_DspType;


                                             /******************************************
 **********************************************    DSD Used Configuration Parameters   *************************************************************
                                              ******************************************/

typedef uint8  DsdProcessingDoneResponseType;
#define  DCM_DSD_TX_RESPONSE_READY          ((DsdProcessingDoneResponseType)0x00)
#define  DCM_DSD_TX_RESPONSE_SUPPRESSED     ((DsdProcessingDoneResponseType)0x01)

typedef uint8   Dcm_DsdServiceStatusType;
#define DCM_SERVICE_IS_NOT_AVAILABLE        (Dcm_DsdServiceStatusType)(0)
#define DCM_SERVICE_IS_AVAILABLE            (Dcm_DsdServiceStatusType)(1)

typedef uint8   Dcm_DsdSubFuncStatusType;
#define  DCM_SERVICE_HAS_NO_SUBFUNCTIONS    (Dcm_DsdSubFuncStatusType)(0)
#define  DCM_SERVICE_HAS_SUBFUNCTIONS       (Dcm_DsdSubFuncStatusType)(1)

typedef uint8   Dcm_DsdSubServiceStatusType;
#define DCM_SUB_SERVICE_IS_NOT_AVAILABLE    (Dcm_DsdSubServiceStatusType)(0)
#define DCM_SUB_SERVICE_IS_AVAILABLE        (Dcm_DsdSubServiceStatusType)(1)



/*********************************************
 *           * Container Info *              *
 *                                           *
 * Container_Name        : Dcm_DsdSubService *
 * Parent_Container_Name : Dcm_DsdService    *
 * Container_Index       : 10.3.3.6          *
 *********************************************/
typedef struct
{
   uint8                           DsdSubServiceId;
   Dcm_DsdSubServiceStatusType     DsdSubServiceUsed;
   const Dcm_DspSessionRowType   **DsdSubServiceSessionLevelRef;
   const uint8                     numOfSessLevel;
}Dcm_DsdSubServiceType;



/************************************************
 *         * Container Info *                   *
 *                                              *
 * Container_Name        : Dcm_DsdService       *
 * Parent_Container_Name : Dcm_DsdServiceTable  *
 * Container_Index       : 10.3.3.2             *
 ************************************************/
typedef struct
{
    Dcm_DsdServiceStatusType         DsdServiceUsed;
    uint8                            DsdSidTabServiceId;
    Dcm_DsdSubFuncStatusType         DsdSidTabSubfuncAvail;
    const Dcm_DspSessionRowType    **DcmDsdSidTabSessionLevelRef;
    const Dcm_DspSecurityRowType   **DcmDsdSidTabSecurityLevelRef;
    const Dcm_DsdSubServiceType     *DsdSubService;
    DcmDsdSidTabFncType              DcmDsdSidTabFnc;
    DcmDsdSidIntrnalFncType          DcmDsdSidIntrnalFnc;
    const uint8                      numOfSessLevel;
    const uint8                      numOfSecLevel;
}Dcm_DsdServiceType;



/***********************************************
 *          * Container Info *                 *
 *                                             *
 * Container_Name        : Dcm_DsdServiceTable *
 * Parent_Container_Name : Dcm_Dsd             *
 * Container_Index       : 10.3.3.5            *
 ***********************************************/
typedef struct
{
    uint8                      DsdSidTabId;
   const Dcm_DsdServiceType   **DsdService;
}Dcm_DsdServiceTableType;



/*****************************************
 *          * Container Info *           *
 *                                       *
 * Container_Name        : Dcm_DsdType   *
 * Parent_Container_Name : Dcm_ConfigSet *
 * Container_Index       : 10.3.3.1      *
 *****************************************/
typedef struct
{
   const Dcm_DsdServiceTableType   *DsdServiceTable;
}Dcm_DsdType;



                                             /******************************************
 **********************************************    DSL Used Configuration Parameters   **************************************************************
                                              ******************************************/


#define DCM_NUM_OF_RX_CHANNELS                                      (5)
#define DCM_NUM_OF_TX_CHANNELS                                      (10)

#define DCM_DSL_LOCAL_BUFFER_LENGTH                                 (8)

typedef uint8 Dcm_DslProtocolRowStatusType;
#define  DCM_PROTOCOL_IS_NOT_AVAILABLE                  (Dcm_DslProtocolRowStatusType)(0)
#define  DCM_PROTOCOL_IS_AVAILABLE                      (Dcm_DslProtocolRowStatusType)(1)

typedef uint8   Dcm_DslProtocolRxAddrTypeType;
#define  DCM_FUNCTIONAL_TYPE                            (Dcm_DslProtocolRxAddrTypeType)(1)
#define  DCM_PHYSICAL_TYPE                              (Dcm_DslProtocolRxAddrTypeType)(0)

typedef uint8   Dcm_DslBufferStatusType;
#define DCM_BUFFER_LOCKED                               (Dcm_DslBufferStatusType)(0)
#define DCM_BUFFER_UNLOCKED                             (Dcm_DslBufferStatusType)(1)

typedef uint8   Dcm_DslBufferUserType;
#define DCM_NOT_IN_USE                                  (Dcm_DslBufferUserType)(0)
#define DCM_IN_USE                                      (Dcm_DslBufferUserType)(1)
#define DCM_PROVIDED_TO_PDUR                            (Dcm_DslBufferUserType)(2)
#define DCM_DSD_PENDING_RESPONSE_SIGNALED               (Dcm_DslBufferUserType)(3)
#define DCM_TRANSMIT_SIGNALED                           (Dcm_DslBufferUserType)(4)
#define DCM_PROVIDED_TO_DSD                             (Dcm_DslBufferUserType)(5)

typedef uint8 Dcm_DslProtocolStateType;
#define DCM_IDLE                                        (Dcm_DslProtocolStateType)(0)
#define DCM_WAITING_DIAGNOSTIC_RESPONSE                 (Dcm_DslProtocolStateType)(1)
#define DCM_DIAGNOSTIC_RESPONSE_PENDING                 (Dcm_DslProtocolStateType)(2)
#define DCM_TRANSMITTING_GLOBAL_BUFFER_DATA_TO_PDUR     (Dcm_DslProtocolStateType)(3)
#define DCM_TRANSMITTING_LOCAL_BUFFER_DATA_TO_PDUR      (Dcm_DslProtocolStateType)(4)



/* Dcm Buffer Lock Status Container */
typedef struct
{
    Dcm_DslBufferStatusType       Lock;
}Dcm_DslBufferStateType;



/* Read Data by Identifier 0x22 Service Runtime Container */
typedef struct
{
  uint32                    dataIndex;
  uint32                    remainingBytes;
}DspDidReadRunTimeType;



/* Security Access Service 0x27 Runtime Container */
typedef struct
{
  boolean  IsAttmptsCounterExceeded;
  uint32   DspSecurityDelayCounter;
  Std_ReturnType  glb_PrvSeedResult;
}DspSecurityRunTimeType;



/* Services Pending Result Handling Runtime Container */
typedef struct
{
 Dcm_MsgContextType* msgcontext;
 Dcm_NegativeResponseCodeType* ErrorCode;
 const Dcm_DsdServiceType *currentServicePtr;
}DspPendingHanlingType;



/* Dsp Sub-Module Runtime Container */
typedef struct
{
  DspPendingHanlingType   *DspPending;
  DspSecurityRunTimeType  *DspSecurity;
  DspDidReadRunTimeType   *DspDidRead;
  Dcm_ExtendedOpStatusType glb_DspOpStatus;
}DspRunTimeDataType;



/* Dsd Sub-Module Runtime Container */
typedef struct
{
      PduIdType                           PduTxID;
      PduIdType                           PduRxID;
      Dcm_MsgType                         diaReqFromDsl;
      uint32                              reqDataLen;
      Dcm_MsgType                         diaResToDsl;
      uint32                              resDataLen;
      Dcm_NegativeResponseCodeType        Error_Code;
      Dcm_DslProtocolRxAddrTypeType       addType;
const Dcm_DsdServiceTableType            *serviceTable;
}DsdRunTimeDataType;



/* Negative Response Buffer Container */
typedef struct
{
    Dcm_DslBufferUserType       BufferCurrentUse;
    PduInfoType                *NRInfo;
} Dcm_DslNRBufferType;



/* Concurrent Tester Present Buffer Container */
typedef struct
{
    Dcm_DslBufferStateType  bufferState;
    Dcm_DslBufferUserType   localRxBufferCurrentUse;
    PduInfoType            *testerPresentReq;
}Dcm_DslLocalBufferType;



/* Dcm Rx Buffer Container */
typedef struct
{
    PduInfoType             dcmRxBuffer;
    uint32                  GlbRXBuffIndex;
    uint32                  remainingBytes;
    uint32                  totalSduLength;
    boolean                 posResSuppresed;
    Dcm_DslBufferStateType  glbRxBufferState;
    PduIdType               diagReqestRxPduId;
    Dcm_DslBufferUserType   glbRxBufferCurrentUse;
}DCM_RxBufferType;



/* Dcm Tx Buffer Container */
typedef struct
{
    PduInfoType             dcmTxBuffer;
    uint32                  GlbTXBuffIndex;
    Dcm_DslBufferStateType  glbTxBufferState;
    PduIdType               diagResponseTxPduId;
    Dcm_DslBufferUserType   glbTxBufferCurrentUse;
}DCM_TxBufferType;



/* Dcm Protocols Handling Container */
typedef struct
{
    boolean     protocolStarted;
    uint32      resTimeoutCount;
}DCM_DslProtocolHandlingType;



/* Dsl Session level Handling Container */
typedef struct
{
    boolean                 S3ServerStarted;
    uint16                  S3ServerTimeoutCount;
    Dcm_DspSessionRowType   sessionRow;
}Dcm_DslSessionHandlingType;



/* Dsl Security level Handling Container */
typedef struct
{
    Dcm_SecLevelType  securityLevel;
}Dcm_DslSecurityHandlingType;



/* Dsl Sub-Module Runtime Container */
typedef struct
{

    DCM_RxBufferType            *DCM_RxBufferInfo;
    DCM_TxBufferType            *DCM_TxBufferInfo;
    Dcm_DslLocalBufferType      *localRxBuffer;
    Dcm_DslNRBufferType         *NegativeResponseBuffer;
    Dcm_DslSessionHandlingType  *DslSessionHandling;
    Dcm_DslSecurityHandlingType *DslSecurityHandling;
    DCM_DslProtocolHandlingType *DslProtocolHandling;
    PduLengthType                PrevSduLen;
    uint8                        responsePendingCount;
}Dcm_DslRunTimeDataType;



/*****************************************
 *           * Container Info *          *
 *                                       *
 * Container_Name        : Dcm_DslBuffer *
 * Parent_Container_Name : Dcm_Dsl       *
 * Container_Index       : 10.3.4.2      *
 *****************************************/
typedef struct
{
  const uint8                  DslBufferID;
  const uint32                 DslBufferSize;
  const PduInfoType           *DslBufferData;
}Dcm_DslBufferType;



/**************************************************************************
 *                          * Container Info *                            *
 *                                                                        *
 * Container_Name        : Dcm_DslProtocolTimingRow                       *
 * Parent_Container_Name : None                                           *
 * Container_Index       : None                                           *
 * SUPPORTED_IN_SWS      : NO                                             *
 * PARA_NOT_SUPPORTED    : None                                           *
 * JUSTIFICATION         : To access timing para during run time          *
 * DESCRIPTION           : This container contains the configuration of a *
 *                         Timing parameters                              *
.**************************************************************************/
typedef struct
{
  uint16    TimStrS3Server;

}Dcm_DslProtocolTimingRowType;


     
/* To validate the Using of Cross-Reference Approach in Structures */
typedef struct Dcm_DslProtocolTxType_t      Dcm_DslProtocolTxType;

typedef struct Dcm_DslProtocolRxType_t      Dcm_DslProtocolRxType;

typedef struct Dcm_DslMainConnectionType_t  Dcm_DslMainConnectionType;

typedef struct Dcm_DslConnectionType_t      Dcm_DslConnectionType;

typedef struct Dcm_DslProtocolRowType_t     Dcm_DslProtocolRowType;


/*************************************************
 *           * Container Info *                  *
 *                                               *
 * Container_Name        : Dcm_DslProtocolTx     *
 * Parent_Container_Name : Dcm_DslMainConnection *
 * Container_Index       : 10.3.4.10             *
 *************************************************/
struct Dcm_DslProtocolTxType_t          /* Cross-Reference */
{
   const Dcm_DslMainConnectionType    *DslMainConnectionParent;
   const PduIdType                     DslTxConfirmationPduId;
};


     
/*****************************************************************************
 *                          * Container Info *                               *
 *                                                                           *
 * Container_Name        : Dcm_DslProtocolRx                                 *
 * Parent_Container_Name : Dcm_DslMainConnection                             *
 * Container_Index       : 10.3.4.9                                          *
 *****************************************************************************/
struct Dcm_DslProtocolRxType_t            /* Cross-Reference */
{
   const Dcm_DslMainConnectionType        *DslMainConnectionParent;
   const Dcm_DslProtocolRxAddrTypeType     DcmDslProtocolRxAddr;
   const PduIdType                         DcmDslProtocolRxPduId;
};


    
/*************************************************
 *              * Container Info *               *
 *                                               *
 * Container_Name        : Dcm_DslMainConnection *
 * Parent_Container_Name : Dcm_DslConnection     *
 * Container_Index       : 10.3.4.8              *
 *************************************************/
struct Dcm_DslMainConnectionType_t      /* Cross-Reference */
{
    const Dcm_DslConnectionType       *DslConnectionParent;
    const uint16                       DcmDslProtocolRxConnectionId;
    const Dcm_DslProtocolRxType      **DslProtocolRx;
    const Dcm_DslProtocolTxType       *DslProtocolTx;
};


  
/**********************************************
 *          * Container Info *                *
 *                                            *
 * Container_Name        : Dcm_DslConnection  *
 * Parent_Container_Name : Dcm_DslProtocolRow *
 * Container_Index       : 10.3.4.7           *
 **********************************************/
struct Dcm_DslConnectionType_t          /* Cross-Reference */
{                                        
    const Dcm_DslProtocolRowType       *DslProtocolRowParent;
    const Dcm_DslMainConnectionType    *DslMainConnection;
};



/**********************************************
 *          * Container Info *                *
 *                                            *
 * Container_Name        : Dcm_DslProtocolRow *
 * Parent_Container_Name : Dcm_DslProtcol     *
 * Container_Index       : 10.3.4.6           *
 **********************************************/
struct Dcm_DslProtocolRowType_t         /*Cross-Reference*/
{
    Dcm_ProtocolType                         DslProtocolID;
    Dcm_DslProtocolRowStatusType             DslProtocolRowUsed;
    const Dcm_DslBufferType                **DslProtocolRxBufferRef;
    const Dcm_DslBufferType                **DslProtocolTxBufferRef;
    const Dcm_DsdServiceTableType           *DslProtocolSIDTable;
    const Dcm_DslProtocolTimingRowType      *DslProtocolTimeLimit;
    const Dcm_DslConnectionType             *DslConnection;
          Dcm_DslRunTimeDataType            *DslrunTime;
          DsdRunTimeDataType                *DsdrunTime;
          DspRunTimeDataType                *DsprunTime;
          Dcm_MsgContextType                *DiagMsgContext;
};



/******************************************
 *           * Container Info *           *
 *                                        *
 * Container_Name        : Dcm_DslProtcol *
 * Parent_Container_Name : Dcm_Dsl        *
 * Container_Index       : 10.3.4.5       *
 ******************************************/
typedef struct
{
  const Dcm_DslProtocolRowType            *DslProtocolRowList;
  const Dcm_DslProtocolRxType            **DslProtocolRxGlbConnectionIDList;
  const Dcm_DslProtocolTxType            **DslProtocolTxGlbConnectionIDList;
}Dcm_DslProtcolType;



/*******************************************
 *          * Container Info *             *
 *                                         *
 * Container_Name        : Dcm_DslDiagResp *
 * Parent_Container_Name : Dcm_Dsl         *
 * Container_Index       : 10.3.4.4        *
 *******************************************/
typedef struct
{
   uint8                    Dcm_DslDiagRespMaxNumRespPend;
}Dcm_DslDiagRespType;



/*****************************************
 *          * Container Info *           *
 *                                       *
 * Container_Name        : Dcm_Dsl       *
 * Parent_Container_Name : Dcm_ConfigSet *
 * Container_Index       : 10.3.4.1      *
 *****************************************/
typedef struct
{
   const Dcm_DslBufferType    *DslBuffer;
   const Dcm_DslDiagRespType  *DslDiagResp;
   const Dcm_DslProtcolType   *DslProtocol;
}Dcm_DslType;



/*****************************************
 *          * Container Info *           *
 *                                       *
 * Container_Name        : Dcm_ConfigSet *
 * Parent_Container_Name : DCM           *
 * Container_Index       : 10.3.2        *
 *****************************************/
typedef struct
{
   const Dcm_DsdType   *Dcm_Dsd;
   const Dcm_DslType   *DcmDsl;
   const Dcm_DspType   *DcmDsp;
}Dcm_ConfigType;



   /*Main Global Container to be used by all sub-Modules*/
extern const Dcm_ConfigType  Dcm_ConfigSet;

#endif /* INCLUDE_DCM_LCFG_H_ */
