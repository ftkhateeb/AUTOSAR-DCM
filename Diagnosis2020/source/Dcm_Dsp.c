/******************************************************************************
 * Module      : DCM                                                          *
 * SubModule   : DSP                                                          *
 * File Name   : Dsp.c                                                        *
 * Description : Source file for Dsp Sub-Module                               *
 * Created on  : April 6, 2020                                                *
 ******************************************************************************/


#include "include/Dcm_Dsp.h"
#include "include/Dcm_Dsl.h"
#include "include/Dcm_Dsd.h"
#include "include/Dcm.h"
#include <string.h>

/**********************************
 * DSP-STATIC-FUNTIONS-PROTOTYPES *
 **********************************/

/*
 * Static functions prototypes for DSP Sub-Module
 */
  static void Dsp_UDS_main(void);
  static void DspUdsPendingProcessing(const Dcm_DsdServiceType *currentServicePtr,
                        Dcm_MsgContextType* pMsgContext,Dcm_NegativeResponseCodeType* ErrorCode);

  static void Dsp_GetCurrentDspRunTIme(void);

/*
 * Static functions prototypes for security service
 */
 static void Dsp_UDS_SecAccessDelayHandler(void);

 static boolean Dsp_LookUpSecLev(Dcm_SecLevelType secLev ,const Dcm_DspSecurityRowType **securityLevRow);

 static inline boolean Dsp_CheckSecuritySubFuncValidity(uint8 subFunc);

/*
 * Static functions prototypes for Read Data By ID service
 */
 static Std_ReturnType Dsp_ReadDidData( uint16 DID, uint8* Data,
                                 Dcm_OpStatusType OpStatus,
                                 uint16 *DataLength,
                                 Dcm_NegativeResponseCodeType *ErrorCode);
 static inline void ExtractDid (uint8 index,uint16 *DID,uint8* Request);

 static boolean Dsp_IsDidAvailable (uint16 DID);

 static void LookUpDid(uint16 DID,const Dcm_DspDidType **requestedDidPtr);

/******************************************
 * WRITE-DATA-STATIC-INTERACES-PROTOTYPES *
 ******************************************/

static Std_ReturnType Dsp_WriteDidData(uint16 DID, const uint8* Data,
                                  Dcm_OpStatusType OpStatus, uint16 DataLength,
                                  Dcm_NegativeResponseCodeType *ErrorCode);


/************************************************
 * ROUTINE-CONTROL-STATIC-INTERACES-PROTOTYPES *
 ************************************************/

static  boolean lookUpRoutine(uint16 RoutineIdentifier,const Dcm_DspRoutineType **requestedRoutine);
static  Std_ReturnType Dsp_RequestRoutineResults(uint16 RID,Dcm_ExtendedOpStatusType OpStatus,
                                              const Dcm_DspRoutineType *RequestedRoutinePtr,
                                              Dcm_MsgContextType* pMsgContext,
                                              Dcm_NegativeResponseCodeType* ErrorCode);
static  Std_ReturnType Dsp_StartRoutine(uint16 RID,Dcm_ExtendedOpStatusType OpStatus,
                                              const Dcm_DspRoutineType *RequestedRoutinePtr,
                                              Dcm_MsgContextType* pMsgContext,
                                              Dcm_NegativeResponseCodeType* ErrorCode);
static  Std_ReturnType Dsp_StopRoutine(uint16 RID,Dcm_ExtendedOpStatusType OpStatus,
                                              const Dcm_DspRoutineType *RequestedRoutinePtr,
                                              Dcm_MsgContextType* pMsgContext,
                                              Dcm_NegativeResponseCodeType* ErrorCode);

/******************************************
 *           GLOBAL-VARIABLES             *
 ******************************************/

/*
 * Global Var to handle services during run time
 */
DspRunTimeDataType *Glb_DspRunTime;

/*
 * constant Global Var points to RIDs configurations
 */
const Dcm_DspRoutineType **Dsp_UDSRoutineControl;

/*
 * constant Global Var points to DIDs configurations
 */
const Dcm_DspDidType **Dsp_UDSDid;

/*
 * constant Global Var points to security Rows configurations
 */

const Dcm_DspSecurityRowType **DspSecurityRow;

/*
 *  Global Var contains Security timing para configurations
 */
uint16 Dsp_UdsSecurityDelayOnBootCounter;

/*
 * Global flag to indicate the availability of security service on power up
 */
boolean IsSecurityDelayTimeOnBootExpired;

/*****************************************************************************
 *                                                                           *
 *                          FUNCTIONS AND SERVICES                           *
 *                                                                           *
 *****************************************************************************/

/***********************************
 * INTIALIZATION-OF-DSP-SUB-MODULE *
 ***********************************/
void Dsp_Init(const Dcm_ConfigType* ConfigPtr)
{
    /*
     * Initialize global var with the configured DIDs
     */
    Dsp_UDSDid = ConfigPtr->DcmDsp->DspDid;

    /*
     * Initialize global var with the configured RIDs
     */
    Dsp_UDSRoutineControl = ConfigPtr->DcmDsp->DspRoutines;

   /*
    * Initialize security flag with false at the beginning
    * to prevent security access during power up
    */
    IsSecurityDelayTimeOnBootExpired = FALSE;

    /*
     * Initialize security delay on boot to the configured timer
     */
    Dsp_UdsSecurityDelayOnBootCounter =DCM_CONVERT_MS_TO_MAIN_CYCLES(DCM_CONVERT_SEC_TO_MS(DCM_DSP_SECURITY_LEV_LOCKED_DELAY_TIME_ON_BOOT));

    /*
     * Initialize global var with the configured security Rows
     */
    DspSecurityRow = ConfigPtr->DcmDsp->DcmDspSecurity->DcmDspSecurityRow;
}


/*********************
 * DSP-MAIN-FUNCTION *
 *********************/
void DspMain(void)
{
    /*
     * MAIN func for UDS protocol
     */
    Dsp_UDS_main();

    /*
     * periodic service to handle security delays
     */
    Dsp_UDS_SecAccessDelayHandler();
}


/**************************************
 * MAIN-FUNC-FOR-UDS-PRTOCOL-SERVECES *
 **************************************/
static void Dsp_UDS_main(void)
{
  /*
   * handling pending response result
   */
  if (DCM_PENDING == Glb_DspRunTime->glb_DspOpStatus)
  {
      /*
       * Calling the service which returned pending again
       */
    DspUdsPendingProcessing(Glb_DspRunTime->DspPending->currentServicePtr,Glb_DspRunTime->DspPending->msgcontext, Glb_DspRunTime->DspPending->ErrorCode);
  }
  /*
   * Handling Cancel response
   */
  else if(DCM_CANCEL == Glb_DspRunTime->glb_DspOpStatus)
  {
      DspUdsPendingProcessing(Glb_DspRunTime->DspPending->currentServicePtr,Glb_DspRunTime->DspPending->msgcontext, Glb_DspRunTime->DspPending->ErrorCode);
      Glb_DspRunTime->glb_DspOpStatus = DCM_INITIAL;
  }
  else
  {
      /* MISRA RULE */
  }
}

/***********************************
 *GET-CURRENT-PROTOCOL-DSP-HANDLER *
 ***********************************/
static void Dsp_GetCurrentDspRunTIme(void)
{
  const Dcm_DslProtocolRowType *ActiveProtocol;

  /*
   * Get Active protocol
   */
  Dsl_GetActiveProtoclRow(&ActiveProtocol);
  /*
   * link Global RunTime DSP pointer with the one configured for active protocol
   */
  Glb_DspRunTime = ActiveProtocol->DsprunTime;
}


/************************************
 * PENDING-RESPONSE-SERVICE-HANDLER *
 ************************************/
static void DspUdsPendingProcessing(const Dcm_DsdServiceType *currentServicePtr,
                        Dcm_MsgContextType* pMsgContext,Dcm_NegativeResponseCodeType* ErrorCode)
{
  Std_ReturnType result = E_OK;

  /*
   * Calling the service which returned pending
   */
  result = currentServicePtr->DcmDsdSidTabFnc(Glb_DspRunTime->glb_DspOpStatus,pMsgContext,ErrorCode);

  if (E_OK == result || E_NOT_OK == result )
  {
      /*
       * Initialize the operation status to DCM_INITIAL
       */
      Glb_DspRunTime->glb_DspOpStatus = DCM_INITIAL;

      /*
       * Forward the response to DSD Sub-Module
       */
      DspToDsdForwardingResponse(result);
  }
  else if(DCM_E_PENDING == result)
  {
      Glb_DspRunTime->glb_DspOpStatus = DCM_PENDING;
  }
  else
  {
      /* MISRA RULE */
  }

}

/*****************************************
 * CANCELLING-CURRENT-SERVICE-PROCESSING *
 *****************************************/
void DspInternal_CancelPendingRequest(void)
{
    /*
     * Initialize the operation status to DCM_INITIAL
     */
    Glb_DspRunTime->glb_DspOpStatus = DCM_CANCEL;
}

/**********************************************************
 * TRIGGERED-SIGNAL-FROM-DSD-TO-ENABLE-SERVICE-PROCESSING *
 **********************************************************/
void DsdDspIndication(const Dcm_DsdServiceType *currentServicePtr,
                                Dcm_MsgContextType* pMsgContext,Dcm_NegativeResponseCodeType* ErrorCode)
{
   Std_ReturnType result = E_OK;
  /*
   * make global pointer points to the active protocol DSP run time
   */
   Dsp_GetCurrentDspRunTIme();

  /*
   * Checking if the coming requested service is handled externally
   */
  if(NULL != currentServicePtr->DcmDsdSidTabFnc)
  {
      /*
       * Calling the service handler
       */
      result = currentServicePtr->DcmDsdSidTabFnc(Glb_DspRunTime->glb_DspOpStatus,pMsgContext,ErrorCode);
      if (E_OK == result || E_NOT_OK == result)
      {
          Glb_DspRunTime->glb_DspOpStatus = DCM_INITIAL;

          /*
           * Forward response to DSD
           */
          DspToDsdForwardingResponse(result);
      }
      /*
       * In case the requested service returned pending
       * switch the service control to DSP man function
       * passing service parameter to DSP run time handler
       */
      else if(DCM_E_PENDING == result)
      {
          Glb_DspRunTime->glb_DspOpStatus = DCM_PENDING;
          Glb_DspRunTime->DspPending->currentServicePtr = currentServicePtr;
          Glb_DspRunTime->DspPending->msgcontext = pMsgContext;
          Glb_DspRunTime->DspPending->ErrorCode = ErrorCode;
      }
      else
      {
          /* MISRA RULE */
      }
  }
  else
  {
      /*
       * Checking if the coming requested service is handled internally
       */
      if(NULL != currentServicePtr->DcmDsdSidIntrnalFnc)
      {
        result = currentServicePtr->DcmDsdSidIntrnalFnc(pMsgContext,ErrorCode);
        /*
         * Forward response to DSD
         */
        DspToDsdForwardingResponse(result);

      }
      else
      {
          result = E_NOT_OK;
          *ErrorCode = DCM_E_GENERALREJECT;
      }
  }

}

/********************************
 * SESSION-VERIFICATION-HANDLER *
 ********************************/
boolean DspSessionLevelPermission(const uint8 numOfLevels, Dcm_DspSessionRowType const* const* sessionLevels)
{

    uint8 levelIndex ;

    /* Flag to indicate that session level has been found */
    uint8 found = FALSE;

    /* Buffer to store current session */
    Dcm_SesCtrlType activeSession = DCM_DEFAULT_SESSION;

    /*
     * Is the number of configured levels not equal to zero?
     * if yes -> Get the current active session.
     */
    if (DCM_INITIALIZED_TO_ZERO != numOfLevels)
    {
            /* Getting current session */
         Dcm_GetSesCtrlType(&activeSession);

          for (levelIndex = DCM_INITIALIZED_TO_ZERO; (levelIndex < numOfLevels) && (found == FALSE); ++levelIndex)
          {
              /*
               * does the active level match with the configured one
               * if yes -> found = TRUE
               */
            if (activeSession == sessionLevels[levelIndex]->DspSessionLevel)
            {
                found = TRUE;
            }
            else
            {
                /* MISRA RULE */
            }
          }
    }
    else
    {
        found = TRUE;
    }

    /* Return the permission status */
    return found;
}


/*********************************
 * SECURITY-VERIFICATION-HANDLER *
 *********************************/
boolean DspSecurityLevelPermission(const uint8 numOfLevels, Dcm_DspSecurityRowType const* const* securityLevels)
{
             /* */
    uint8 levelIndex ;

    /* Flag to indicate that security level has been found */
    uint8 found = FALSE;

    /* Buffer to store current security level */
    Dcm_SesCtrlType currentSecLev = 0x00;

    /*
     * Is the number of configured levels not equal to zero?
     * if yes -> Get the current active security Level.
     */
    if (DCM_INITIALIZED_TO_ZERO != numOfLevels)
    {
            /* Getting current session */
         Dsl_GetSecurityLevel(&currentSecLev);

          for (levelIndex = DCM_INITIALIZED_TO_ZERO; (levelIndex < numOfLevels) && (found == FALSE); ++levelIndex)
          {
              /*
               * does the active level match with the configured one
               * if yes -> found = TRUE
               */
            if (currentSecLev == securityLevels[levelIndex]->Dcm_DspSecurityLevel)
            {
                found = TRUE;
            }
            else
            {
                /* MISRA RULE */
            }
          }
    }
    else
    {
        found = TRUE;
    }

    /* Return the permission status */
    return found;
}




/********************
 * SERVICE-HANDLERS *
 ********************/





/**********************************
 * TESTER-PRESENT-SERVICE-HANDLER *
 **********************************/
Std_ReturnType Dsp_UDS_TesterPresent0x3E(Dcm_MsgContextType*  pMsgContext,Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = E_OK;
    /*
     * Checking request format
     */
    if (1 == pMsgContext->reqDataLen)
    {
        /*
         * Checking if sub-function supported
         */
       if (pMsgContext->reqData[0] == DCM_INITIALIZED_TO_ZERO)
       {
          /*
           * Checking if positive response suppressed
           */
         if (FALSE == pMsgContext->msgAddInfo.suppressPosResponse)
         {
            /*
             * preparing the response
             */
           pMsgContext->resData[0] = 0x00;
           pMsgContext->resDataLen = 1;

         }
         else
         {
             /* MISRA RULE */
         }
         Reset_S3ServerTimer();
       }
       else
       {
          /*
           * Update the error code with NRC
           */
         *ErrorCode = DCM_E_SUBFUNCTIONNOTSUPPORTED;
        /*
         * Update return status
         */
         result = E_NOT_OK;
       }
    }
    else
    {
        /*
         * Update the error code with NRC
         */
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        /*
         * Update return status
         */
         result = E_NOT_OK;
    }

  return result;
}




/***********************************
 * SESSION-CONTROL-SERVICE-HANDLER *
 ***********************************/
Std_ReturnType  Dsp_UDS_DiagnosticSessionControl0x10(Dcm_MsgContextType*  pMsgContext,Dcm_NegativeResponseCodeType* ErrorCode )
{

    Std_ReturnType result = E_OK;
    Dcm_SesCtrlType subFun;

    /*
     * Checking sub function format
     */
    if (pMsgContext->reqDataLen >= 1 )
    {
        subFun = pMsgContext->reqData[0];

        /*
         * Checking the support of the requested sub function
         */
        switch(subFun)
        {
           case DCM_DEFAULT_SESSION:
           case DCM_EXTENDED_DIAGNOSTIC_SESSION:
           case DCM_SAFETY_SYSTEM_DIAGNOSTIC_SESSION:
               if (FALSE == pMsgContext->msgAddInfo.suppressPosResponse)
               {
                   /*
                    * prepare the response
                    */
                   pMsgContext->resData[0] = subFun;
                   pMsgContext->resDataLen = 1;

               }
               else
               {
                   /* MISRA RULE*/
               }

               /*
                * Set the active session to the requested one
                */
               DslInternal_SetSesCtrlType(subFun);
              break;
           default:
               /*
                * Update the error code with NRC
                */
               *ErrorCode = DCM_E_SUBFUNCTIONNOTSUPPORTED;
               /*
                * Update return status
                */
               result = E_NOT_OK;
              break;
        }
    }
    else
    {
        /*
         * Update the error code with NRC
         */
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        /*
         * Update return status
         */
        result = E_NOT_OK;
    }

  return result;
}


/**************************************
 * READ-DATA-BY-ID-SERVICE-INTERFACES *
 **************************************/



/****************************
 * EXTRACT-DID-FROM-REQUEST *
 ****************************/
static inline void ExtractDid (uint8 index,uint16 *DID,uint8* Request)
{
    /*
     * GET DID index
     */
    index = index * 2;

    /*
     * Pick first byte of DID
     */
    *DID = Request[index];

    /*
     * Move first byte of DID to be MSB
     */
    *DID <<= 8;

    /*
     * Pick second byte of DID
     */
    *DID |= Request[++index];
}


/********************************
 * CHECKING-AVAILABILITY-OF-DID *
 ********************************/
static boolean Dsp_IsDidAvailable (uint16 DID)
{
  boolean found = FALSE;
  uint8 index;

  /*
   * Get DIDS configuration
   */
  const Dcm_DspDidType **supportedDids = Dsp_UDSDid;

  /*
   * Searching for the requested DID
   */
  for (index = 0; (index < DCM_NUM_OF_SUPPORTED_DIDS) && (found == FALSE); ++index)
  {
    if (DID == supportedDids[index]->DspDidIdentifier)
    {
      if (TRUE == supportedDids[index]->DspDidUsed)
      {
       found = TRUE;
      }
      else
      {
        /* MISRA RULE */
      }
    }
    else
    {
      /* MISRA RULE */
    }
  }

  return found;
}


/*********************************
 * LOOK-UP-FOR-DID-CONFIGURATION *
 *********************************/
static void LookUpDid(uint16 DID,const Dcm_DspDidType **requestedDidPtr)
{
    boolean found = FALSE;
    uint16 index = DCM_INITIALIZED_TO_ZERO;

    /*
     * Get DIDS configuration
     */
    const Dcm_DspDidType **supportedDids = Dsp_UDSDid;

    /*
     * Searching for the requested DID
     */
   for (; (index < DCM_NUM_OF_SUPPORTED_DIDS) && (found == FALSE); ++index)
   {
     if (DID == supportedDids[index]->DspDidIdentifier)
     {
       found = TRUE;

       /*
        * Return Requested DID
        */
       *requestedDidPtr = *(supportedDids + index);
     }
     else
     {
       /* MISRA RULE */
     }
   }
}


/*********************
 * READ-REQUSTED-DID *
 *********************/
static Std_ReturnType Dsp_ReadDidData( uint16 DID, uint8* Data,
                                Dcm_OpStatusType OpStatus,
                                uint16 *DataLength,
                                Dcm_NegativeResponseCodeType *ErrorCode)
{

#if (DCM_DEV_ERROR_DETECT == TRUE)

    if ((DCM_PENDING != OpStatus)&&(DCM_CANCEL != OpStatus)&&(DCM_FORCE_RCRRP_OK != OpStatus)&&(DCM_INITIAL != OpStatus))
    {
      Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_READ_DID_DATA, DCM_E_PARAM);
    }
    else
    {
      /* MISRA RULE */
    }

#endif


    Std_ReturnType result;
   /*
    * Static var to handle pending response by request DID data Length
    */
    static boolean lengthPending = FALSE;
    static uint16  TotalDataLen = DCM_INITIALIZED_TO_ZERO;
    static uint8   DidLen =  DCM_INITIALIZED_TO_ZERO;
    /*
     * Static var to handle pending response by request DID data
     */
    static boolean dataPending = FALSE;

    /*
     * Flag to indicate if pending status returned
     */
    boolean pendingFlagRaised = FALSE;
    /*
     * Index to access specific signal data
     */
    static uint32 signalIndex     = DCM_INITIALIZED_TO_ZERO;

    static uint32 numberOfSignals =  DCM_INITIALIZED_TO_ZERO;

    /*
     * pointer to get DID
     */
    static const Dcm_DspDidType *requestedDidPtr = NULL;

    /*
     * In case the first call of DID
     */
    if (DCM_INITIAL == OpStatus)
    {
      /*
       * Get requested DID
       */
       LookUpDid(DID,&requestedDidPtr);
       numberOfSignals = requestedDidPtr->DspDidNumOfSignals;
    }

       /*
         * IS the requested DID supported in the active session level
         */
      if(TRUE == DspSessionLevelPermission(requestedDidPtr->DspDidInfoRef->DspDidRead->numSessLevels//
                                                ,requestedDidPtr->DspDidInfoRef->DspDidRead->DcmDspDidReadSessionRef))
       {
          /*
           * IS the requested DID supported in the active security level
           */
          if(TRUE == DspSecurityLevelPermission(requestedDidPtr->DspDidInfoRef->DspDidRead->numSecLevels//
                                                      ,requestedDidPtr->DspDidInfoRef->DspDidRead->DcmDspDidReadSecurityLevelRef))
            {

              for ( ;signalIndex < numberOfSignals; signalIndex++)
              {
                /*
                 * Handling DID request according to its configured port interface
                 */
                switch (requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataUsePort)
                {
                  case DCM_USE_DATA_SYNCH_CLIENT_SERVER:
                  case DCM_USE_DATA_SYNCH_FNC:

                         /*
                          * Is condition check enable? if Yes then check
                          */
                      if (TRUE == requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataConditionCheckReadFncUsed)
                      {
                        result = requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataConditionCheckReadFnc(ErrorCode);

                                /* Interface return value DET */
                        #if (DCM_DEV_ERROR_DETECT == TRUE)

                            if (E_OK != result)
                            {
                                Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_CONDITION_CHECK_READ_1_SID, DCM_E_INTERFACE_RETURN_VALUE);
                            }
                            else
                            {
                                /* MISRA RULE */
                            }

                        #endif

                      }
                      else
                      {
                        result = E_OK;
                      }

                      /*
                       * Depending on condition check status
                       */
                      if (E_OK == result)
                      {

                          if (DCM_UINT8_DYN == requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataType)
                          {
                              /*
                               * Get data length by calling configured function
                               */
                            result = requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataReadDataLengthSynFnc(DataLength);
                                    /* Interface return value DET */
                            #if (DCM_DEV_ERROR_DETECT == TRUE)

                                if (E_OK != result)
                                {
                                    Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_READ_DATA_LENGTH_1_SID, DCM_E_INTERFACE_RETURN_VALUE);
                                }
                                else
                                {
                                    /* MISRA RULE */
                                }

                            #endif

                              TotalDataLen += *DataLength;
                          }
                          else
                          {
                              *DataLength = requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataByteSize;
                              TotalDataLen += *DataLength;
                          }
                          /*
                           * keep tracking of the remaining bytes of response buffer
                           */

                          if (Glb_DspRunTime->DspDidRead->remainingBytes  >= (*DataLength + (DCM_DID_LENGTH - DidLen)))
                          {
                              if (DCM_INITIALIZED_TO_ZERO == signalIndex)
                              {
                                  DidLen = DCM_DID_LENGTH;
                                  /*
                                   * prepare the response
                                   * pick first byte of the DID
                                   */
                                 Data[Glb_DspRunTime->DspDidRead->dataIndex]  = ((DID >> DCM_BYTE) & 0xFF);

                                 /*
                                  * Pick Second byte of the DID
                                  */
                                 Data[++Glb_DspRunTime->DspDidRead->dataIndex] = (DID & 0xFF);

                                 /*
                                  * Decrement response buffer
                                  */
                                 Glb_DspRunTime->DspDidRead->remainingBytes -= (*DataLength + DCM_DID_LENGTH);
                              }
                              else
                              {
                                  /*
                                   * Decrement response buffer
                                   */
                                  Glb_DspRunTime->DspDidRead->remainingBytes -= (*DataLength);

                              }


                              Glb_DspRunTime->DspDidRead->dataIndex = (requestedDidPtr->DspDidSignal[signalIndex]->DspDidByteOffset + 2);
                             /*
                              * Get the requested DID Data
                              */
                             result = requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataReadSynFnc(&Data[Glb_DspRunTime->DspDidRead->dataIndex]);
                                     /* Interface return value DET */
                             #if (DCM_DEV_ERROR_DETECT == TRUE)

                                 if (E_OK != result)
                                 {
                                     Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_READ_DATA_1_SID, DCM_E_INTERFACE_RETURN_VALUE);
                                 }
                                 else
                                 {
                                     /* MISRA RULE */
                                 }
                             #endif
                          }
                          else
                          {
                              #if (DCM_DEV_ERROR_DETECT == TRUE)
                              Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_READ_DID_DATA, DCM_E_INTERFACE_BUFFER_OVERFLOW);
                              #endif

                              /*
                               * Update return status
                               */
                              result = E_NOT_OK;
                              /*
                               * Update NRC
                               */
                              *ErrorCode = DCM_E_RESPONSETOOLONG;
                              /* To terminate the loop */
                              pendingFlagRaised = TRUE;
                          }
                      }
                      break;
                  case DCM_USE_DATA_ASYNCH_CLIENT_SERVER:
                  case DCM_USE_DATA_ASYNCH_FNC:

                      /*
                       * Is condition check enable? if Yes then check
                       */
                      if ((TRUE == requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataConditionCheckReadFncUsed) &&(DCM_USE_DATA_ASYNCH_CLIENT_SERVER == requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataUsePort))
                      {
                        result = requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataConditionCheckReadAsynFnc(OpStatus,ErrorCode);
                                        /* Interface return value DET */
                            #if (DCM_DEV_ERROR_DETECT == TRUE)

                                if ((E_OK != result) && (E_NOT_OK != result) && (DCM_E_PENDING != result) )
                                {
                                    Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_CONDITION_CHECK_READ_2_SID, DCM_E_INTERFACE_RETURN_VALUE);
                                }
                                else
                                {
                                    /* MISRA RULE */
                                }
                                if (E_NOT_OK == result)
                                {
                                    if (!((*ErrorCode >= 0x01) && (*ErrorCode <= 0xFF)))
                                    {
                                        Det_ReportRuntimeError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_CONDITION_CHECK_READ_2_SID, DCM_E_INVALID_VALUE);
                                    }
                                    else
                                    {
                                        /* MISRA RULE*/
                                    }

                                }
                                else
                                {
                                   /* MISRA RULE */
                                }

                            #endif
                      }
                      else
                      {
                        result = E_OK;
                      }

                      /*
                       * Depending on condition check status
                       */
                      if(E_OK == result)
                      {
                          /*
                           * Checking if it is the first call
                           */
                          if (DCM_INITIAL == OpStatus)
                          {

                              if (DCM_UINT8_DYN == requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataType)
                              {
                                  /*
                                   * Get data length by calling configured function
                                   */
                                result = requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataReadDataLengthAsynFnc(OpStatus,DataLength);
                                                /* Interface return value DET */
                                    #if (DCM_DEV_ERROR_DETECT == TRUE)

                                        if ((E_OK != result) && (DCM_E_PENDING != result) )
                                        {
                                            Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_READ_DATA_LENGTH_2_SID, DCM_E_INTERFACE_RETURN_VALUE);
                                        }
                                        else
                                        {
                                            /* MISRA RULE */
                                        }

                                    #endif

                                TotalDataLen += *DataLength;
                              }
                              else
                              {
                                  *DataLength = requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataByteSize;
                                  TotalDataLen += *DataLength;
                                  result = E_OK;
                              }
                            /*
                             * Checking result status
                             */
                            if(DCM_E_PENDING != result)
                            {
                                /*
                                 * keep tracking of the remaining bytes of response buffer
                                 */
                                if (Glb_DspRunTime->DspDidRead->remainingBytes >= (*DataLength + (DCM_DID_LENGTH - DidLen)))
                                 {

                                    if(DCM_INITIALIZED_TO_ZERO == signalIndex)
                                    {
                                        DidLen = DCM_DID_LENGTH;
                                        /*
                                         * prepare the response
                                         * pick first byte of the DID
                                         */
                                         Data[Glb_DspRunTime->DspDidRead->dataIndex]  = ((DID >> DCM_BYTE) & 0xFF);

                                         Data[++Glb_DspRunTime->DspDidRead->dataIndex] = (DID & 0xFF);

                                         Glb_DspRunTime->DspDidRead->remainingBytes -= (*DataLength + DCM_DID_LENGTH);
                                    }
                                    else
                                    {
                                        Glb_DspRunTime->DspDidRead->remainingBytes -= (*DataLength);
                                    }

                                     Glb_DspRunTime->DspDidRead->dataIndex = (requestedDidPtr->DspDidSignal[signalIndex]->DspDidByteOffset + DCM_DID_LENGTH);

                                     result = requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataReadAsynFnc(OpStatus,&Data[Glb_DspRunTime->DspDidRead->dataIndex]);
                                                 /* Interface return value DET */
                                     #if (DCM_DEV_ERROR_DETECT == TRUE)

                                         if ((E_OK != result) && (DCM_E_PENDING != result) )
                                         {
                                             Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_READ_DATA_2_SID, DCM_E_INTERFACE_RETURN_VALUE);
                                         }
                                         else
                                         {
                                             /* MISRA RULE */
                                         }

                                     #endif


                                     if (DCM_E_PENDING == result)
                                     {
                                         /*
                                          * response is pending due to DID data request
                                          */
                                         dataPending = TRUE;
                                         pendingFlagRaised = TRUE;
                                     }
                                     else
                                     {
                                        /* MISRA RULE */
                                     }
                                 }
                                else
                                {
                                    #if (DCM_DEV_ERROR_DETECT == TRUE)
                                    Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_READ_DID_DATA, DCM_E_INTERFACE_BUFFER_OVERFLOW);
                                    #endif

                                    /*
                                     * Update return status
                                     */
                                    result = E_NOT_OK;
                                    /*
                                     * Update NRC
                                     */
                                    *ErrorCode = DCM_E_RESPONSETOOLONG;
                                    /* To terminate the loop */
                                    pendingFlagRaised = TRUE;
                                }

                            }
                            else
                            {
                                /*
                                 * response is pending due to DID length request
                                 */
                              lengthPending = TRUE;
                              pendingFlagRaised = TRUE;
                            }
                         }
                             /*
                              * in case the previous result was pending due to data
                              */
                         else if ((DCM_PENDING == OpStatus) && (TRUE == dataPending))
                         {
                             Glb_DspRunTime->glb_DspOpStatus = DCM_INITIAL;
                             OpStatus = DCM_INITIAL;
                             /*
                              * Trying to fetch data
                              */
                             result = requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataReadAsynFnc(OpStatus,&Data[Glb_DspRunTime->DspDidRead->dataIndex]);
                                     /* Interface return value DET */
                             #if (DCM_DEV_ERROR_DETECT == TRUE)

                                 if ((E_OK != result) && (DCM_E_PENDING != result) )
                                 {
                                     Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_READ_DATA_2_SID, DCM_E_INTERFACE_RETURN_VALUE);
                                 }
                                 else
                                 {
                                     /* MISRA RULE */
                                 }

                             #endif


                            if (DCM_E_PENDING == result)
                            {
                               dataPending = TRUE;
                               pendingFlagRaised = TRUE;
                            }
                            else
                            {
                              dataPending = FALSE;
                            }
                         }

                          /*
                           * in case the previous result was pending due to Length
                           */
                         else if ((DCM_PENDING == OpStatus) && (TRUE == lengthPending))
                         {
                             OpStatus = DCM_INITIAL;
                             Glb_DspRunTime->glb_DspOpStatus = DCM_INITIAL;
                             result = requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataReadDataLengthAsynFnc(OpStatus,DataLength);
                                  /* Interface return value DET */
                             #if (DCM_DEV_ERROR_DETECT == TRUE)

                                 if ((E_OK != result) && (DCM_E_PENDING != result) )
                                 {
                                     Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_READ_DATA_LENGTH_2_SID, DCM_E_INTERFACE_RETURN_VALUE);
                                 }
                                 else
                                 {
                                     /* MISRA RULE */
                                 }

                             #endif



                             TotalDataLen += *DataLength;

                           if (DCM_E_PENDING != result)
                           {
                                lengthPending = FALSE;
                                if (Glb_DspRunTime->DspDidRead->remainingBytes  >= (*DataLength + (DCM_DID_LENGTH - DidLen)))
                                {
                                    if (DCM_INITIALIZED_TO_ZERO == signalIndex)
                                    {
                                        DidLen = DCM_DID_LENGTH;

                                        Data[Glb_DspRunTime->DspDidRead->dataIndex]  = ((DID >> DCM_BYTE) & 0xFF);

                                        Data[++Glb_DspRunTime->DspDidRead->dataIndex] = (DID & 0xFF);

                                        Glb_DspRunTime->DspDidRead->remainingBytes -= (*DataLength + DCM_DID_LENGTH);
                                    }
                                    else
                                    {
                                        Glb_DspRunTime->DspDidRead->remainingBytes -= (*DataLength);
                                    }
                                   Glb_DspRunTime->DspDidRead->dataIndex = (requestedDidPtr->DspDidSignal[signalIndex]->DspDidByteOffset + DCM_DID_LENGTH);

                                   result = requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataReadAsynFnc(OpStatus,&Data[Glb_DspRunTime->DspDidRead->dataIndex]);
                                          /* Interface return value DET */
                                       #if (DCM_DEV_ERROR_DETECT == TRUE)

                                           if ((E_OK != result) && (DCM_E_PENDING != result) )
                                           {
                                               Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_READ_DATA_2_SID, DCM_E_INTERFACE_RETURN_VALUE);
                                           }
                                           else
                                           {
                                               /* MISRA RULE */
                                           }

                                       #endif

                                   if (DCM_E_PENDING == result)
                                   {
                                      dataPending = TRUE;
                                      pendingFlagRaised = TRUE;
                                   }
                                   else
                                   {
                                      dataPending = FALSE;
                                   }
                                }
                                else
                                {
                                    #if (DCM_DEV_ERROR_DETECT == TRUE)
                                    Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_READ_DID_DATA, DCM_E_INTERFACE_BUFFER_OVERFLOW);
                                    #endif

                                    /*
                                     * Update return status
                                     */
                                    result = E_NOT_OK;
                                    /*
                                     * Update NRC
                                     */
                                    *ErrorCode = DCM_E_RESPONSETOOLONG;
                                    /* To terminate the loop */
                                    pendingFlagRaised = TRUE;
                                }

                           }
                           else
                           {
                             lengthPending = TRUE;
                             pendingFlagRaised = TRUE;
                           }
                         }
                         else if (DCM_CANCEL == OpStatus)
                         {
                             if (TRUE == lengthPending)
                             {
                                 result = requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataReadDataLengthAsynFnc(OpStatus,DataLength);
                             }
                             else
                             {
                                 result = requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataReadAsynFnc(OpStatus,&Data[Glb_DspRunTime->DspDidRead->dataIndex]);
                             }
                         /*
                          * Update return result
                          */
                           result = E_NOT_OK;
                         }
                         else
                         {
                             /* MISRA RULE */
                         }
                     }
                     else if(DCM_E_PENDING == result)
                     {
                         /* To terminate the loop */
                         pendingFlagRaised = TRUE;
                     }
                     else if(E_NOT_OK == result)
                     {
                         /* To terminate the loop */
                         pendingFlagRaised = TRUE;
                     }
                     else
                     {
                         /* MISRA RULE */
                     }
                       break;
                  case DCM_USE_DATA_ASYNCH_FNC_ERROR:
                  case DCM_USE_DATA_ASYNCH_CLIENT_SERVER_ERROR:

                      /*
                       * Is condition check enable? if Yes then check
                       */
                      if (TRUE == requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataConditionCheckReadFncUsed)
                      {
                        result = requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataConditionCheckReadAsynFnc(OpStatus, ErrorCode);
                                        /* Interface return value DET */
                            #if (DCM_DEV_ERROR_DETECT == TRUE)

                                if ((E_OK != result) && (E_NOT_OK != result) && (DCM_E_PENDING != result) )
                                {
                                    Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_CONDITION_CHECK_READ_2_SID, DCM_E_INTERFACE_RETURN_VALUE);
                                }
                                else
                                {
                                    /* MISRA RULE */
                                }
                                if (E_NOT_OK == result)
                                {
                                    if (!((*ErrorCode >= 0x01) && (*ErrorCode <= 0xFF)))
                                    {
                                        Det_ReportRuntimeError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_CONDITION_CHECK_READ_2_SID, DCM_E_INVALID_VALUE);
                                    }
                                    else
                                    {
                                        /* MISRA RULE*/
                                    }

                                }
                                else
                                {
                                   /* MISRA RULE */
                                }

                            #endif


                      }
                      else
                      {
                        result = E_OK;
                      }

                      /*
                       * Depending on condition check status
                       */
                      if(E_OK == result)
                      {
                          /*
                           * Checking if it is the first call
                           */
                          if (DCM_INITIAL == OpStatus)
                          {

                              if (DCM_UINT8_DYN == requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataType)
                              {
                                  /*
                                   * Get data length by calling configured function
                                   */
                                result = requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataReadDataLengthAsynFnc(OpStatus,DataLength);
                                            /* Interface return value DET */
                                #if (DCM_DEV_ERROR_DETECT == TRUE)

                                    if ((E_OK != result) && (DCM_E_PENDING != result) )
                                    {
                                        Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_READ_DATA_LENGTH_2_SID, DCM_E_INTERFACE_RETURN_VALUE);
                                    }
                                    else
                                    {
                                        /* MISRA RULE */
                                    }

                                #endif


                                TotalDataLen += *DataLength;
                              }
                              else
                              {
                                  *DataLength = requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataByteSize;
                                  TotalDataLen += *DataLength;
                                  result = E_OK;
                              }
                            /*
                             * Checking result status
                             */
                            if(DCM_E_PENDING != result)
                            {
                                /*
                                 * keep tracking of the remaining bytes of response buffer
                                 */
                                if (Glb_DspRunTime->DspDidRead->remainingBytes >= (*DataLength + (DCM_DID_LENGTH - DidLen)))
                                 {
                                    if(DCM_INITIALIZED_TO_ZERO == signalIndex)
                                    {
                                        DidLen = DCM_DID_LENGTH;
                                        /*
                                         * prepare the response
                                         * pick first byte of the DID
                                         */
                                         Data[Glb_DspRunTime->DspDidRead->dataIndex]  = ((DID >> DCM_BYTE) & 0xFF);

                                         Data[++Glb_DspRunTime->DspDidRead->dataIndex] = (DID & 0xFF);

                                         Glb_DspRunTime->DspDidRead->remainingBytes -= (*DataLength + DCM_DID_LENGTH);
                                    }
                                    else
                                    {
                                        Glb_DspRunTime->DspDidRead->remainingBytes -= (*DataLength);
                                    }
                                     Glb_DspRunTime->DspDidRead->dataIndex = (requestedDidPtr->DspDidSignal[signalIndex]->DspDidByteOffset + DCM_DID_LENGTH);

                                     result = requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataReadAsynErrorFnc(OpStatus,&Data[Glb_DspRunTime->DspDidRead->dataIndex],ErrorCode);
                                                 /* Interface return value DET */
                                     #if (DCM_DEV_ERROR_DETECT == TRUE)

                                         if ((E_OK != result) && (DCM_E_PENDING != result) )
                                         {
                                             Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_READ_DATA_3_SID, DCM_E_INTERFACE_RETURN_VALUE);
                                         }
                                         else
                                         {
                                             /* MISRA RULE */
                                         }
                                         if (E_NOT_OK == result)
                                         {
                                             if (!((*ErrorCode >= 0x01) && (*ErrorCode <= 0xFF)))
                                             {
                                                 Det_ReportRuntimeError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_READ_DATA_3_SID, DCM_E_INVALID_VALUE);
                                             }
                                             else
                                             {
                                                 /* MISRA RULE*/
                                             }

                                         }
                                         else
                                         {
                                            /* MISRA RULE */
                                         }

                                     #endif

                                     if (DCM_E_PENDING == result)
                                     {
                                         /*
                                          * response is pending due to DID data request
                                          */
                                         dataPending = TRUE;
                                         pendingFlagRaised = TRUE;
                                     }
                                     else
                                     {
                                        /* MISRA RULE */
                                     }
                                 }
                                 else
                                 {
                                    #if (DCM_DEV_ERROR_DETECT == TRUE)
                                    Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_READ_DID_DATA, DCM_E_INTERFACE_BUFFER_OVERFLOW);
                                    #endif

                                     /*
                                     * Update return status
                                     */
                                    result = E_NOT_OK;
                                    /*
                                     * Update NRC
                                     */
                                    *ErrorCode = DCM_E_RESPONSETOOLONG;
                                    /* To terminate the loop */
                                    pendingFlagRaised = TRUE;
                                 }

                            }
                            else
                            {
                                /*
                                 * response is pending due to DID length request
                                 */
                              lengthPending = TRUE;
                              pendingFlagRaised = TRUE;
                            }
                         }
                             /*
                              * in case the previous result was pending due to data
                              */
                         else if ((DCM_PENDING == OpStatus) && (TRUE == dataPending))
                         {
                             Glb_DspRunTime->glb_DspOpStatus = DCM_INITIAL;

                             /*
                              * Trying to fetch data
                              */
                             result = requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataReadAsynErrorFnc(OpStatus,&Data[Glb_DspRunTime->DspDidRead->dataIndex],ErrorCode);
                                         /* Interface return value DET */
                             #if (DCM_DEV_ERROR_DETECT == TRUE)

                                 if ((E_OK != result) && (DCM_E_PENDING != result) )
                                 {
                                     Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_READ_DATA_3_SID, DCM_E_INTERFACE_RETURN_VALUE);
                                 }
                                 else
                                 {
                                     /* MISRA RULE */
                                 }
                                 if (E_NOT_OK == result)
                                 {
                                     if (!((*ErrorCode >= 0x01) && (*ErrorCode <= 0xFF)))
                                     {
                                         Det_ReportRuntimeError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_READ_DATA_3_SID, DCM_E_INVALID_VALUE);
                                     }
                                     else
                                     {
                                         /* MISRA RULE*/
                                     }

                                 }
                                 else
                                 {
                                    /* MISRA RULE */
                                 }

                             #endif


                            if (DCM_E_PENDING == result)
                            {
                               dataPending = TRUE;
                               pendingFlagRaised = TRUE;
                            }
                            else
                            {
                              dataPending = FALSE;
                            }
                         }

                          /*
                           * in case the previous result was pending due to Length
                           */
                         else if ((DCM_PENDING == OpStatus) && (TRUE == lengthPending))
                         {
                             Glb_DspRunTime->glb_DspOpStatus = DCM_INITIAL;
                           result = requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataReadDataLengthAsynFnc(OpStatus,DataLength);
                                       /* Interface return value DET */
                           #if (DCM_DEV_ERROR_DETECT == TRUE)

                               if ((E_OK != result) && (DCM_E_PENDING != result) )
                               {
                                   Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_READ_DATA_LENGTH_2_SID, DCM_E_INTERFACE_RETURN_VALUE);
                               }
                               else
                               {
                                   /* MISRA RULE */
                               }

                           #endif

                           TotalDataLen += *DataLength;
                           if (DCM_E_PENDING != result)
                           {
                                lengthPending = FALSE;
                                if (Glb_DspRunTime->DspDidRead->remainingBytes  >= (*DataLength + (DCM_DID_LENGTH - DidLen)))
                                {
                                    if (DCM_INITIALIZED_TO_ZERO == signalIndex)
                                    {
                                        DidLen = DCM_DID_LENGTH;
                                        Data[Glb_DspRunTime->DspDidRead->dataIndex]  = ((DID >> DCM_BYTE) & 0xFF);

                                        Data[++Glb_DspRunTime->DspDidRead->dataIndex] = (DID & 0xFF);
                                        Glb_DspRunTime->DspDidRead->dataIndex++;

                                        Glb_DspRunTime->DspDidRead->remainingBytes -= (*DataLength + DCM_DID_LENGTH);
                                    }
                                    else
                                    {
                                        Glb_DspRunTime->DspDidRead->remainingBytes -= (*DataLength);
                                    }
                                   Glb_DspRunTime->DspDidRead->dataIndex += requestedDidPtr->DspDidSignal[signalIndex]->DspDidByteOffset;

                                   result = requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataReadAsynErrorFnc(OpStatus,&Data[Glb_DspRunTime->DspDidRead->dataIndex],ErrorCode);
                                                   /* Interface return value DET */
                                       #if (DCM_DEV_ERROR_DETECT == TRUE)

                                           if ((E_OK != result) && (DCM_E_PENDING != result) )
                                           {
                                               Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_READ_DATA_3_SID, DCM_E_INTERFACE_RETURN_VALUE);
                                           }
                                           else
                                           {
                                               /* MISRA RULE */
                                           }
                                           if (E_NOT_OK == result)
                                           {
                                               if (!((*ErrorCode >= 0x01) && (*ErrorCode <= 0xFF)))
                                               {
                                                   Det_ReportRuntimeError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_READ_DATA_3_SID, DCM_E_INVALID_VALUE);
                                               }
                                               else
                                               {
                                                   /* MISRA RULE*/
                                               }

                                           }
                                           else
                                           {
                                              /* MISRA RULE */
                                           }

                                       #endif

                                   if (DCM_E_PENDING == result)
                                   {
                                      dataPending = TRUE;
                                      pendingFlagRaised = TRUE;
                                   }
                                   else
                                   {
                                      dataPending = FALSE;
                                   }
                                }
                                else
                                {
                                    #if (DCM_DEV_ERROR_DETECT == TRUE)
                                    Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_READ_DID_DATA, DCM_E_INTERFACE_BUFFER_OVERFLOW);
                                    #endif

                                    /*
                                     * Update return status
                                     */
                                    result = E_NOT_OK;
                                    /*
                                     * Update NRC
                                     */
                                    *ErrorCode = DCM_E_RESPONSETOOLONG;
                                    /* To terminate the loop */
                                    pendingFlagRaised = TRUE;
                                }

                           }
                           else
                           {
                             lengthPending = TRUE;
                           }
                         }
                         else if (DCM_CANCEL == OpStatus)
                         {
                             if (TRUE == lengthPending)
                             {
                                 result = requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataReadDataLengthAsynFnc(OpStatus,DataLength);

                             }
                             else
                             {
                                 result = requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataReadAsynFnc(OpStatus,&Data[Glb_DspRunTime->DspDidRead->dataIndex]);
                             }
                         /*
                          * Update return result
                          */
                           result = E_NOT_OK;
                         }
                         else
                         {
                             /* MISRA RULE */
                         }
                     }
                     else if (DCM_E_PENDING == result)
                     {
                         pendingFlagRaised = TRUE;
                     }
                     else if (E_NOT_OK == result)
                     {
                         pendingFlagRaised = TRUE;
                     }
                     else
                     {
                       /* MISRA RULE */
                     }

                      break;
                  default:

                      /*
                       * Update return result
                       */
                      result = E_NOT_OK;

                      /*
                       * Update NRC
                       */
                      *ErrorCode = DCM_E_GENERALREJECT;
                      break;
                }
                if (TRUE == pendingFlagRaised)
                {
                    break;
                }
                else
                {
                    /* MISRA RULE */
                }
              }
          }
          else
          {

              /*
               * Update return result
               */
            result = E_NOT_OK;

            /*
             * Update NRC
             */
            *ErrorCode = DCM_E_SECURITYACCESSDENIED;
          }
      }
      else
      {
          /*
           * Update return result
           */

          result = E_NOT_OK;

          /*
           * Update NRC
           */
           *ErrorCode = DCM_E_SUBFUNCTIONNOTSUPPORTEDINACTIVESESSION;

      }
      if (E_OK == result)
      {
         *DataLength = TotalDataLen;
         signalIndex = DCM_INITIALIZED_TO_ZERO;
         TotalDataLen = DCM_INITIALIZED_TO_ZERO;
         DidLen = DCM_INITIALIZED_TO_ZERO;
      }
      else if (E_NOT_OK == result)
      {
          signalIndex = DCM_INITIALIZED_TO_ZERO;
          TotalDataLen = DCM_INITIALIZED_TO_ZERO;
          DidLen = DCM_INITIALIZED_TO_ZERO;
      }
      else
      {
          /* MISRA RULE */
      }

      return result;
}



/***********************************
 * READ-DATA-BY-ID-SERVICE-HANDLER *
 ***********************************/
Std_ReturnType Dsp_UDS_ReadDataByIdentifier_0x22(Dcm_ExtendedOpStatusType OpStatus,
                                        Dcm_MsgContextType* pMsgContext, Dcm_NegativeResponseCodeType* ErrorCode)
{

Std_ReturnType result = E_OK;
uint8 NumDids = DCM_INITIALIZED_TO_ZERO;
uint16 DID = DCM_INITIALIZED_TO_ZERO;
static boolean posRes = FALSE;
static uint16 dataLen = DCM_INITIALIZED_TO_ZERO;
static uint8 DidIndex = DCM_INITIALIZED_TO_ZERO;


    /*
     * Check format
     */
     if (0 == pMsgContext->reqDataLen % 2)
     {
         /*
          * Get number of requested DID
          */
         NumDids = (pMsgContext->reqDataLen)/2;

         /* [SWS_Dcm_01335]
          * Does the number of requested Dids exceed the configured max number?
          * if No then continue
          */
         if (NumDids <= DCM_DSP_UDS_MAX_DID_TO_READ)
         {
             for (; DidIndex < NumDids; DidIndex++)
             {

                 /*
                  * Fetch the requested DID from the request Message
                  */
                 ExtractDid(DidIndex,&DID,pMsgContext->reqData);

                 /*
                  * Check if the DID supported
                  */
                if (Dsp_IsDidAvailable(DID))
                {

                    /*
                     * request DID from the Application
                     */
                    result = Dsp_ReadDidData(DID,pMsgContext->resData,OpStatus,&dataLen,ErrorCode);

                    if (E_OK == result)
                    {
                      /*
                       * In case at least one DID return E_OK
                       */
                      posRes = TRUE;

                      /*
                       * Update response length
                       */
                      pMsgContext->resDataLen += ((uint32)dataLen + DCM_DID_LENGTH);
                      Glb_DspRunTime->DspDidRead->dataIndex++;
                    }

                    /*
                     * In case the Application needs extra time
                     */
                    else if (DCM_E_PENDING == result)
                    {
                    /*
                     * Set the flag to break the loop
                     */
                      break;

                    }
                    else
                    {
                      /* MISRA RULE*/
                    }
                }
                else
                {
                    /*
                     * Update NRC
                     */
                   *ErrorCode = DCM_E_REQUESTOUTOFRANGE;

                   /*
                    * Update NRC
                    */
                    result = E_NOT_OK;
                }

              }
                 /*
                  * in case all DIDs are serviced and not pending status
                  */
              if ((DCM_E_PENDING != result) && (DidIndex == NumDids))
              {

                  if (TRUE == posRes)
                  {
                      result = E_OK;
                  }
                  else
                  {

                  }
                  /*
                   * Reinitialize DID run time data
                   */
                  DidIndex = DCM_INITIALIZED_TO_ZERO;
                  Glb_DspRunTime->DspDidRead->remainingBytes = DCM_DSL_UDS_MAIN_BUFFER_SIZE - 1;  /* Update response len*/
                  posRes = FALSE;
                  Glb_DspRunTime->DspDidRead->dataIndex = DCM_INITIALIZED_TO_ZERO;

              }

         }
         else
         {
             /*
              * Update NRC
              */
             *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
              result = E_NOT_OK;
         }
     }
     else
     {
         /*
          * Update NRC
          */
         *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
         result = E_NOT_OK;
     }
  return result;
}



/***********************************
 * WRITE-DATA-BY-ID-SERVICE-HANDLER *
 ***********************************/
Std_ReturnType Dsp_UDS_WriteDataByIdentifier_0x2E(Dcm_ExtendedOpStatusType OpStatus,
                                        Dcm_MsgContextType* pMsgContext, Dcm_NegativeResponseCodeType* ErrorCode)
{

    Std_ReturnType result = E_NOT_OK;

    uint16 DID;

    static uint16 dataLen = 0;

    /*
     * Get First DID Byte
     */
    DID  = ((uint16)pMsgContext->reqData[0] << 8);

    /*
     * Get Second DID Byte
     */
    DID += ((uint16)pMsgContext->reqData[1]);


    /*
     * In case the first call of the function
     */

   if (DCM_INITIAL == OpStatus)
   {
        /*
         * Check format
         */
         if (2 < pMsgContext->reqDataLen )
         {
             /*
              * Check if the DID supported
              */
            if (Dsp_IsDidAvailable(DID))
            {

                    dataLen = (uint16)(pMsgContext->reqDataLen - DCM_DID_LENGTH);
                    /*
                     * request DID from the Application
                     */
                    result = Dsp_WriteDidData(DID, &pMsgContext->reqData[2], OpStatus,dataLen, ErrorCode);

                    if (E_OK == result)
                    {
                      /*
                       * Update response length
                       */
                       pMsgContext->resDataLen =  DCM_DID_LENGTH;
                       pMsgContext->resData[1]  = ((DID >> DCM_BYTE) & 0xFF);
                       pMsgContext->resData[2]  = (DID & 0xFF);

                    }
                    else
                    {
                      /* MISRA RULE*/
                    }
            }
            else
            {
                /*
                 * Update NRC
                 */
               *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            }
         }
         else
         {
             /*
              * Update NRC
              */
             *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
         }
   }
   else if (DCM_PENDING == OpStatus)
   {
       /*
        * request DID from the Application
        */
       result = Dsp_WriteDidData(DID, &pMsgContext->reqData[2] ,OpStatus, dataLen, ErrorCode);

       if (E_OK == result)
       {
         /*
          * Update response length
          */
          pMsgContext->resDataLen =  DCM_DID_LENGTH;
          pMsgContext->resData[0]  = ((DID >> DCM_BYTE) & 0xFF);
          pMsgContext->resData[1]  = (DID & 0xFF);

       }
       else
       {
         /* MISRA RULE*/
       }

   }
   else
   {
       /* MISRA RULE*/
   }

 return result;
}

/*********************
 * WRITE-REQUSTED-DID *
 *********************/
static Std_ReturnType Dsp_WriteDidData(uint16 DID, const uint8* Data,
                                 Dcm_OpStatusType OpStatus, uint16 DataLength,
                                 Dcm_NegativeResponseCodeType *ErrorCode)
{

#if (DCM_DEV_ERROR_DETECT == TRUE)

    if ((DCM_PENDING != OpStatus)&&(DCM_CANCEL != OpStatus)&&(DCM_FORCE_RCRRP_OK != OpStatus)&&(DCM_INITIAL != OpStatus))
    {
      Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_WRITE_DID_DATA, DCM_E_PARAM);
    }
    else
    {
      /* MISRA RULE */
    }

#endif

 Std_ReturnType result;


/*
 * Flag to indicate if pending status returned
 */
boolean pendingFlagRaised = FALSE;
/*
 * Index to access specific signal data
 */
static uint32 signalIndex  =  DCM_INITIALIZED_TO_ZERO;

static uint32 numberOfSignals = DCM_INITIALIZED_TO_ZERO;

static uint8 signalData[DCM_MAX_SIGNAL_DATA_LENGTH] = {DCM_INITIALIZED_TO_ZERO};

static uint16 signalOffset = DCM_INITIALIZED_TO_ZERO;

static uint16 signalLen = DCM_INITIALIZED_TO_ZERO;

static uint16 TotalSignalLen = DCM_INITIALIZED_TO_ZERO;

static boolean isDynLen = FALSE;

static uint8 *signalDataPtr = signalData;
/*
 * pointer to get DID
 */
static const Dcm_DspDidType *requestedDidPtr = NULL;

    /*
     * In case the first call of DID
     */
    if (DCM_INITIAL == OpStatus)
    {
      /*
       * Get requested DID
       */
       LookUpDid(DID,&requestedDidPtr);
       numberOfSignals = requestedDidPtr->DspDidNumOfSignals;
    }

   /*
     * IS the requested DID supported in the active session level
     */
  if(TRUE == DspSessionLevelPermission(requestedDidPtr->DspDidInfoRef->DspDidRead->numSessLevels//
                                            ,requestedDidPtr->DspDidInfoRef->DspDidRead->DcmDspDidReadSessionRef))
   {
      /*
       * IS the requested DID supported in the active security level
       */
      if(TRUE == DspSecurityLevelPermission(requestedDidPtr->DspDidInfoRef->DspDidRead->numSecLevels//
                                                  ,requestedDidPtr->DspDidInfoRef->DspDidRead->DcmDspDidReadSecurityLevelRef))
        {

          for ( ;signalIndex < numberOfSignals; signalIndex++)
          {
            /*
             * Handling DID request according to its configured port interface
             */
            switch (requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataUsePort)
            {
              case DCM_USE_DATA_SYNCH_CLIENT_SERVER:
              case DCM_USE_DATA_SYNCH_FNC:




                      if (DCM_UINT8_DYN == requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataType)
                      {
                          if (signalIndex == (numberOfSignals - 1))
                          {
                              signalLen = DataLength - TotalSignalLen;

                              signalOffset = requestedDidPtr->DspDidSignal[signalIndex]->DspDidByteOffset;
                              memmove(signalDataPtr, Data + signalOffset, signalLen);
                                  /*
                                   * Get data length by calling configured function
                                   */
                               result = requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataWriteSynDynFnc(signalDataPtr ,signalLen,ErrorCode);
                                       /* Interface return value DET */
                                #if (DCM_DEV_ERROR_DETECT == TRUE)

                                    if ((E_OK != result) && (E_NOT_OK != result) )
                                    {
                                       Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_WRITE_DATA_2_SID, DCM_E_INTERFACE_RETURN_VALUE);
                                    }
                                    else
                                    {
                                       /* MISRA RULE */
                                    }
                                    if (E_NOT_OK == result)
                                    {
                                        if (!((*ErrorCode >= 0x01) && (*ErrorCode <= 0xFF)))
                                        {
                                            Det_ReportRuntimeError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_WRITE_DATA_2_SID, DCM_E_INVALID_VALUE);
                                        }
                                        else
                                        {
                                            /* MISRA RULE*/
                                        }

                                    }
                                    else
                                    {
                                       /* MISRA RULE */
                                    }

                                #endif

                          }
                          else
                          {
                              /* MISRA RULE */
                          }
                      }
                      else
                      {
                          signalLen = requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataByteSize;

                          TotalSignalLen += signalLen;

                          signalOffset = requestedDidPtr->DspDidSignal[signalIndex]->DspDidByteOffset;

                          memmove(signalDataPtr, Data + signalOffset, signalLen);

                          result = requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataWriteSynFnc(signalDataPtr, ErrorCode);

                                       /* Interface return value DET */
                           #if (DCM_DEV_ERROR_DETECT == TRUE)

                               if ((E_OK != result) && (E_NOT_OK != result) )
                               {
                                   Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_WRITE_DATA_1_SID, DCM_E_INTERFACE_RETURN_VALUE);
                               }
                               else
                               {
                                   /* MISRA RULE */
                               }
                               if (E_NOT_OK == result)
                               {
                                   if (!((*ErrorCode >= 0x01) && (*ErrorCode <= 0xFF)))
                                   {
                                       Det_ReportRuntimeError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_WRITE_DATA_1_SID, DCM_E_INVALID_VALUE);
                                   }
                                   else
                                   {
                                       /* MISRA RULE*/
                                   }

                               }
                               else
                               {
                                  /* MISRA RULE */
                               }

                           #endif

                      }

                  break;
              case DCM_USE_DATA_ASYNCH_CLIENT_SERVER:
              case DCM_USE_DATA_ASYNCH_FNC_ERROR:
              case DCM_USE_DATA_ASYNCH_CLIENT_SERVER_ERROR:


              if (DCM_INITIAL == OpStatus)
              {

                  if (DCM_UINT8_DYN == requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataType)
                  {
                      if (signalIndex == (numberOfSignals - 1))
                      {
                          signalLen = DataLength - TotalSignalLen;

                          signalOffset = requestedDidPtr->DspDidSignal[signalIndex]->DspDidByteOffset;
                          memmove(signalDataPtr, Data + signalOffset, signalLen);
                              /*
                               * Get data length by calling configured function
                               */
                           result = requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataWriteAsynDynFnc(signalDataPtr,signalLen,OpStatus,ErrorCode);
                           /* Interface return value DET */
                           #if (DCM_DEV_ERROR_DETECT == TRUE)

                               if ((E_OK != result) && (E_NOT_OK != result) &&(DCM_E_PENDING != result))
                               {
                                 Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_WRITE_DATA_4_SID, DCM_E_INTERFACE_RETURN_VALUE);
                               }
                               else
                               {
                                 /* MISRA RULE */
                               }
                               if (E_NOT_OK == result)
                               {
                                   if (!((*ErrorCode >= 0x01) && (*ErrorCode <= 0xFF)))
                                   {
                                       Det_ReportRuntimeError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_WRITE_DATA_4_SID, DCM_E_INVALID_VALUE);
                                   }
                                   else
                                   {
                                       /* MISRA RULE*/
                                   }

                               }
                               else
                               {
                                  /* MISRA RULE */
                               }

                           #endif



                           if (DCM_E_PENDING == result)
                           {
                               isDynLen = TRUE;
                               pendingFlagRaised = TRUE;
                           }
                           else
                           {
                               /* MISRA RULE*/
                           }
                      }
                      else
                      {
                          /* MISRA RULE */
                      }
                  }
                  else
                  {
                      signalLen = requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataByteSize;
                      TotalSignalLen += signalLen;
                      signalOffset = requestedDidPtr->DspDidSignal[signalIndex]->DspDidByteOffset;
                      memmove(signalData, Data + signalOffset, signalLen);

                      result = requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataWriteAsynFnc(signalDataPtr,OpStatus,ErrorCode);
                      /* Interface return value DET */
                      #if (DCM_DEV_ERROR_DETECT == TRUE)

                          if ((E_OK != result) && (E_NOT_OK != result) &&(DCM_E_PENDING != result) )
                          {
                            Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_WRITE_DATA_3_SID, DCM_E_INTERFACE_RETURN_VALUE);
                          }
                          else
                          {
                            /* MISRA RULE */
                          }
                          if (E_NOT_OK == result)
                          {
                              if (!((*ErrorCode >= 0x01) && (*ErrorCode <= 0xFF)))
                              {
                                  Det_ReportRuntimeError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_WRITE_DATA_3_SID, DCM_E_INVALID_VALUE);
                              }
                              else
                              {
                                  /* MISRA RULE*/
                              }

                          }
                          else
                          {
                             /* MISRA RULE */
                          }

                      #endif

                      if (DCM_E_PENDING == result)
                     {
                         pendingFlagRaised = TRUE;
                     }
                     else
                     {
                         /* MISRA RULE */
                     }
                  }
              }
              else if (DCM_PENDING == OpStatus)
              {
                  if (TRUE == isDynLen)
                  {
                     isDynLen = FALSE;
                     result = requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataWriteAsynDynFnc(signalDataPtr,signalLen, OpStatus,ErrorCode);
                     /* Interface return value DET */
                     #if (DCM_DEV_ERROR_DETECT == TRUE)

                         if ((E_OK != result) && (E_NOT_OK != result) &&(DCM_E_PENDING != result))
                         {
                           Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_WRITE_DATA_4_SID, DCM_E_INTERFACE_RETURN_VALUE);
                         }
                         else
                         {
                           /* MISRA RULE */
                         }
                         if (E_NOT_OK == result)
                         {
                             if (!((*ErrorCode >= 0x01) && (*ErrorCode <= 0xFF)))
                             {
                                 Det_ReportRuntimeError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_WRITE_DATA_4_SID, DCM_E_INVALID_VALUE);
                             }
                             else
                             {
                                 /* MISRA RULE*/
                             }

                         }
                         else
                         {
                            /* MISRA RULE */
                         }

                     #endif



                     if (DCM_E_PENDING == result)
                     {
                         isDynLen = TRUE;
                         pendingFlagRaised = TRUE;
                     }
                     else if (E_OK == result)
                     {
                         OpStatus = DCM_INITIAL;
                     }
                     else
                     {
                         /* MISRA RULE */
                     }

                  }
                  else
                  {
                     result = requestedDidPtr->DspDidSignal[signalIndex]->DspDidDataRef->DspDataWriteAsynFnc(signalDataPtr, OpStatus, ErrorCode);
                     /* Interface return value DET */
                     #if (DCM_DEV_ERROR_DETECT == TRUE)

                         if ((E_OK != result) && (E_NOT_OK != result) &&(DCM_E_PENDING != result))
                         {
                           Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_WRITE_DATA_3_SID, DCM_E_INTERFACE_RETURN_VALUE);
                         }
                         else
                         {
                           /* MISRA RULE */
                         }
                         if (E_NOT_OK == result)
                         {
                             if (!((*ErrorCode >= 0x01) && (*ErrorCode <= 0xFF)))
                             {
                                 Det_ReportRuntimeError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_WRITE_DATA_3_SID, DCM_E_INVALID_VALUE);
                             }
                             else
                             {
                                 /* MISRA RULE*/
                             }

                         }
                         else
                         {
                            /* MISRA RULE */
                         }

                     #endif

                     if (DCM_E_PENDING == result)
                     {
                         pendingFlagRaised = TRUE;
                     }
                     else if (E_OK == result)
                     {
                         OpStatus = DCM_INITIAL;
                     }
                     else
                     {
                         /* MISRA RULE */
                     }

                  }
              }
                break;
              default:

                  /*
                   * Update return result
                   */
                  result = E_NOT_OK;
                  /*
                   * Update NRC
                   */
                  *ErrorCode = DCM_E_GENERALREJECT;
                  pendingFlagRaised = TRUE;
                  break;
            }
            if (TRUE == pendingFlagRaised)
            {
                break;
            }
            else
            {
                /* MISRA RULE */
            }
          }
      }
      else
      {

          /*
           * Update return result
           */
        result = E_NOT_OK;

        /*
         * Update NRC
         */
        *ErrorCode = DCM_E_SECURITYACCESSDENIED;
      }
  }
  else
  {
      /*
       * Update return result
       */

      result = E_NOT_OK;

      /*
       * Update NRC
       */
       *ErrorCode = DCM_E_SUBFUNCTIONNOTSUPPORTEDINACTIVESESSION;

  }
  if (E_OK == result)
  {
     signalIndex = DCM_INITIALIZED_TO_ZERO;
     TotalSignalLen = DCM_INITIALIZED_TO_ZERO;
  }
  else if (E_NOT_OK == result)
  {
         signalIndex = DCM_INITIALIZED_TO_ZERO;
         TotalSignalLen = DCM_INITIALIZED_TO_ZERO;
  }
  else
  {
      /* MISRA RULE */
  }

  return result;
}


/**************************************
 * SECURITY-ACCESS-SERVICE-INTERFACES *
 **************************************/





/*********************************
 * SECURITY-ACCESS-DELAY-HANDLER *
 *********************************/
static void Dsp_UDS_SecAccessDelayHandler(void)
{
    /*
     * Check if Max number of Attempts exceeded
     */
    if (Glb_DspRunTime->DspSecurity->IsAttmptsCounterExceeded == TRUE)
    {
        DECREMENT_TIMER_COUNTS(Glb_DspRunTime->DspSecurity->DspSecurityDelayCounter);
        if(0 == Glb_DspRunTime->DspSecurity->DspSecurityDelayCounter)
        {
            Glb_DspRunTime->DspSecurity->IsAttmptsCounterExceeded = FALSE;
        }
        else
        {
            /* MISRA RULE*/
        }
    }

    /*
     * Check if required delay on power up expired
     */
    else if (IsSecurityDelayTimeOnBootExpired == FALSE)
    {
        DECREMENT_TIMER_COUNTS(Dsp_UdsSecurityDelayOnBootCounter);
        if (DCM_INITIALIZED_TO_ZERO == Dsp_UdsSecurityDelayOnBootCounter)
        {
            IsSecurityDelayTimeOnBootExpired = TRUE;
        }
        else
        {
            /* MISRA RULE*/
        }
    }
    else
    {
        /* MISRA RULE */
    }

}


/******************************************
 * CHECK-SECURITY-ACCESS-REQUEST-SUB-FUNC *
 ******************************************/
static inline boolean Dsp_CheckSecuritySubFuncValidity(uint8 subFunc)
{
    boolean result = FALSE;

    /*
     * Check the range of the sub function
     */
    if((subFunc > 0) && (subFunc <= 0x7E))
    {
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }
    return result;
}

/****************************
 * LOOK-UP-FOR-SECURITY-ROW *
 ****************************/
static boolean Dsp_LookUpSecLev(Dcm_SecLevelType secLev ,const Dcm_DspSecurityRowType **securityLevRow)
{
   boolean found = FALSE;
   uint16 index = DCM_INITIALIZED_TO_ZERO;

   /*
    * Searching for requested level row
    */
   for (; (index < DCM_DSP_NUM_OF_CONFIGURED_SECURITY_LEV) && (found == FALSE); ++index)
   {
     if (secLev == DspSecurityRow[index]->Dcm_DspSecurityLevel)
     {
       found = TRUE;

       /*
        * return the configured level row
        */
       *securityLevRow = *(DspSecurityRow + index);
     }
     else
     {
       /* MISRA RULE */
     }
   }
   return found;
}

/***********************************
 * SECURITY-ACCESS-SERVICE-HANDLER *
 ***********************************/
Std_ReturnType Dsp_UDS_SecurityAccess_0x27(Dcm_ExtendedOpStatusType OpStatus,
                                        Dcm_MsgContextType* pMsgContext, Dcm_NegativeResponseCodeType* ErrorCode)
{

  Std_ReturnType result = E_OK;

  /* Constant variable to pick security level row */
  const Dcm_DspSecurityRowType  *secLevelRow;

  /* variable to pick security sub function */
  boolean subFunc_Validity = FALSE;
  boolean isReqSeed = FALSE;
  uint8 subFunc = 0;
  static Dcm_SecLevelType equivLev = 0;
  static uint8 securityKeyAttcounter = 0;  /* This Var should be in NO-INIT-RAM Section*/
  Dcm_SecLevelType currentSecLevel = 0;

  uint8 attmpCounter = 0;

  static boolean IsGetSeedPending    = FALSE;
  static boolean IsCompareKeyPending = FALSE;

  /*
   * In case the first call of the service
   */
  if (DCM_INITIAL == OpStatus)
  {
      /*
       * Checking if the time on boot expired
       */
     if (TRUE == IsSecurityDelayTimeOnBootExpired)
     {

         /*
          * In case the user exceeded number of attempts
          */
         if (FALSE == Glb_DspRunTime->DspSecurity->IsAttmptsCounterExceeded)
         {
             /*
              * Check request format
              */
           if (1 <= pMsgContext->reqDataLen)
           {
               /* Get sub function*/
              subFunc = pMsgContext->reqData[0];

              /*
               * Check sub function validity
               */
             subFunc_Validity = Dsp_CheckSecuritySubFuncValidity(subFunc);

             /*
              * In case sub function is valid
              */
             if (TRUE == subFunc_Validity)
             {
                 /*
                  * In case sub Function is ODD then it is Get seed sub function
                  */
                 isReqSeed = subFunc & 0x01;

                 if (isReqSeed)
                 {
                     /*
                      * Get Equivalent security level
                      */
                    equivLev = (subFunc + 1) / 2;

                    /*
                     * Get Active security level
                     */
                    Dsl_GetSecurityLevel(&currentSecLevel);

                    /*
                     * In case requested level is already active then return response with zero
                     */
                    if (equivLev == currentSecLevel)
                    {
                        pMsgContext->resData[0] = 0x00;
                        pMsgContext->resDataLen = 1;
                    }
                    else
                    {
                        /*
                         * Get Equivalent level Row
                         */
                        if (Dsp_LookUpSecLev(equivLev, &secLevelRow))
                        {
                            /*
                             * check use port interface
                             */
                            switch (secLevelRow->Dcm_DspSecurityUsePort)
                            {
                              case  DCM_USE_DATA_ASYNCH_CLIENT_SERVER:
                              case  DCM_USE_DATA_ASYNCH_CLIENT_SERVER_ERROR:

                                  /*
                                   * Request seed from the Application
                                   */
                               result = secLevelRow->Dcm_DspSecurityGetSeedFnc(OpStatus, &pMsgContext->resData[1], ErrorCode);
                                #if (DCM_DEV_ERROR_DETECT == TRUE)

                                    if ((E_OK != result) && (E_NOT_OK != result) &&(DCM_E_PENDING != result))
                                    {
                                      Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_GET_SEED_SID, DCM_E_INTERFACE_RETURN_VALUE);
                                    }
                                    else
                                    {
                                      /* MISRA RULE */
                                    }

                                #endif

                               if (E_OK == result)
                               {
                                   /*
                                    * Prepare the response
                                    */
                                  pMsgContext->resData[0] = subFunc;
                                  Glb_DspRunTime->DspSecurity->glb_PrvSeedResult = E_OK;
                                  pMsgContext->resDataLen = (secLevelRow->Dcm_DspSecuritySeedSize + 1);
                               }
                               else if (DCM_E_PENDING == result)
                               {
                                   /*
                                    * In case pending due to GetSeed request
                                    */
                                 IsGetSeedPending = TRUE;
                               }
                            #if (DCM_DEV_ERROR_DETECT == TRUE)
                                else if (E_NOT_OK == result)
                                {

                                  if (!((*ErrorCode >= 0x01) && (*ErrorCode <= 0xFF)))
                                  {
                                      Det_ReportRuntimeError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_GET_SEED_SID, DCM_E_INVALID_VALUE);
                                  }
                                  else
                                  {
                                      /* MISRA RULE*/
                                  }
                                }
                            #endif
                               else
                               {
                                    /* MISRA RULE */
                               }

                               break;
                             default:
                                    result = E_NOT_OK;
                                    *ErrorCode = DCM_E_GENERALREJECT;
                              break;
                            }
                        }
                        else
                        {
                           result = E_NOT_OK;
                           *ErrorCode = DCM_E_REQUESTSEQUENCEERROR;
                        }

                    }

                 }
                 else
                 {

                     /*
                      * In case of compare Key request
                      * Firstly the last requested key should be E_OK
                      */
                     if (E_OK == Glb_DspRunTime->DspSecurity->glb_PrvSeedResult)
                     {
                         /*
                          * Get requested level Row
                          */
                         if (Dsp_LookUpSecLev(equivLev, &secLevelRow))
                         {
                              /*
                               * check use port interface
                               */
                              switch (secLevelRow->Dcm_DspSecurityUsePort)
                              {
                              case  DCM_USE_DATA_ASYNCH_CLIENT_SERVER:
                              case  DCM_USE_DATA_ASYNCH_CLIENT_SERVER_ERROR:

                                   /*
                                    * Request from the Application to compare the received key
                                    */
                                 result  = secLevelRow->DcmDspSecurityCompareKeyFnc(&pMsgContext->reqData[1],OpStatus, ErrorCode);
                                #if (DCM_DEV_ERROR_DETECT == TRUE)

                                    if ((E_OK != result) && (E_NOT_OK != result) && (DCM_E_PENDING != result) && (DCM_E_COMPARE_KEY_FAILED != result) )
                                    {
                                      Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_COMPARE_KEY_SID, DCM_E_INTERFACE_RETURN_VALUE);
                                    }
                                    else
                                    {
                                      /* MISRA RULE */
                                    }

                                #endif

                                 /*
                                  * In case invalid Key
                                  */
                                 if (DCM_E_COMPARE_KEY_FAILED == result)
                                 {
                                     /*
                                      * Increase Attempts counter by one
                                      */
                                     securityKeyAttcounter += 1;

                                     /*
                                      * Update return result
                                      */
                                     result = E_NOT_OK;

                                      /*
                                       * Update NRC
                                       */
                                     *ErrorCode = DCM_E_INVALIDKEY;

                                         /*
                                          * Checking if the user wants to get or set number of attempts from the application
                                          */
                                     if (secLevelRow->Dcm_DspSecurityAttemptCounterEnabled)
                                     {
                                         /*
                                          * Setting number of error attempts
                                          */
                                      result =  secLevelRow->Dcm_DspSecuritySetAttemptCounterFnc(securityKeyAttcounter,OpStatus);
                                        #if (DCM_DEV_ERROR_DETECT == TRUE)

                                            if ((E_OK != result) && (E_NOT_OK != result) && (DCM_E_PENDING != result) )
                                            {
                                              Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_SET_SECURITY_ATTEMPT_COUNTER_SID, DCM_E_INTERFACE_RETURN_VALUE);
                                            }
                                            else
                                            {
                                              /* MISRA RULE */
                                            }

                                        #endif

                                        /*
                                         * Getting number of error attempts
                                         */
                                       result = secLevelRow->Dcm_DspSecurityGetAttemptCounterFnc(OpStatus,&attmpCounter);
                                        #if (DCM_DEV_ERROR_DETECT == TRUE)

                                            if ((E_OK != result) && (E_NOT_OK != result) && (DCM_E_PENDING != result) )
                                            {
                                              Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_GET_SECURITY_ATTEMPT_COUNTER_SID, DCM_E_INTERFACE_RETURN_VALUE);
                                            }
                                            else
                                            {
                                              /* MISRA RULE */
                                            }

                                        #endif

                                          /*
                                           * Checking if the user exceeded Max. number of attempts
                                           */
                                        if (attmpCounter >= secLevelRow->Dcm_DspSecurityNumAttDelay)
                                        {
                                            Glb_DspRunTime->DspSecurity->IsAttmptsCounterExceeded = TRUE;
                                            securityKeyAttcounter = DCM_INITIALIZED_TO_ZERO;

                                            /*
                                             * Update return result
                                             */
                                            result = E_NOT_OK;
                                            *ErrorCode = DCM_E_EXCEEDNUMBEROFATTEMPTS;

                                            if (secLevelRow->Dcm_DspSecurityDelayTimeOnBoot <= secLevelRow->Dcm_DspSecurityDelayTime)
                                            {
                                                   /* Get configured delay due to exceeding max number of attempts */
                                                Glb_DspRunTime->DspSecurity->DspSecurityDelayCounter = DCM_CONVERT_MS_TO_MAIN_CYCLES(DCM_CONVERT_SEC_TO_MS(secLevelRow->Dcm_DspSecurityDelayTime));
                                            }
                                            else
                                            {
                                                /*
                                                 * In case delay on boot delay is longer then use it since it is mentioned in SWS
                                                 */
                                                Glb_DspRunTime->DspSecurity->DspSecurityDelayCounter = DCM_CONVERT_MS_TO_MAIN_CYCLES(DCM_CONVERT_SEC_TO_MS(secLevelRow->Dcm_DspSecurityDelayTimeOnBoot));
                                            }
                                        }
                                        else
                                        {
                                           /* MISRA RULE */
                                        }
                                     }
                                     else
                                     {
                                         /*
                                          * If not configured then handle it internally
                                          */
                                         if (securityKeyAttcounter >= secLevelRow->Dcm_DspSecurityNumAttDelay)
                                         {
                                             result = E_NOT_OK;
                                             *ErrorCode = DCM_E_EXCEEDNUMBEROFATTEMPTS;
                                             Glb_DspRunTime->DspSecurity->IsAttmptsCounterExceeded = TRUE;
                                             securityKeyAttcounter = DCM_INITIALIZED_TO_ZERO;
                                             if (secLevelRow->Dcm_DspSecurityDelayTimeOnBoot <= secLevelRow->Dcm_DspSecurityDelayTime)
                                             {
                                                 Glb_DspRunTime->DspSecurity->DspSecurityDelayCounter = DCM_CONVERT_MS_TO_MAIN_CYCLES(DCM_CONVERT_SEC_TO_MS(secLevelRow->Dcm_DspSecurityDelayTime));
                                             }
                                             else
                                             {
                                                 Glb_DspRunTime->DspSecurity->DspSecurityDelayCounter = DCM_CONVERT_MS_TO_MAIN_CYCLES(DCM_CONVERT_SEC_TO_MS(secLevelRow->Dcm_DspSecurityDelayTimeOnBoot));
                                             }
                                         }
                                     }
                                 }
                                 else if (E_OK == result)
                                 {
                                     /*
                                      * Asking DSL to Change security level to the requested level
                                      */
                                     DslSetSecurityLevel(secLevelRow->Dcm_DspSecurityLevel);
                                     securityKeyAttcounter = DCM_INITIALIZED_TO_ZERO;

                                     /*
                                      * Prepare response
                                      */
                                     pMsgContext->resData[0] = subFunc;
                                     pMsgContext->resDataLen = 1;
                                 }
                                 else if (DCM_E_PENDING == result)
                                 {
                                     /*
                                      * Pending response due to compare key request
                                      */
                                     IsCompareKeyPending = TRUE;
                                 }
                                #if (DCM_DEV_ERROR_DETECT == TRUE)
                                else if (E_NOT_OK == result)
                                {

                                  if (!((*ErrorCode >= 0x01) && (*ErrorCode <= 0xFF)))
                                  {
                                      Det_ReportRuntimeError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_COMPARE_KEY_SID, DCM_E_INVALID_VALUE);
                                  }
                                  else
                                  {
                                      /* MISRA RULE*/
                                  }
                                }
                                #endif
                                 else
                                 {
                                      /* MISRA RULE */
                                 }
                                 break;
                                default:
                                    result = E_NOT_OK;
                                    *ErrorCode = DCM_E_GENERALREJECT;
                              break;

                             }
                         }
                         else
                         {
                             result = E_NOT_OK;
                             *ErrorCode = DCM_E_REQUESTSEQUENCEERROR;
                         }
                     }
                     else
                     {
                         result = E_NOT_OK;
                         *ErrorCode = DCM_E_GENERALREJECT;
                     }
                 }
             }
             else
             {
                 result = E_NOT_OK;
                 *ErrorCode = DCM_E_SUBFUNCTIONNOTSUPPORTED;
             }
          }
          else
          {
             result = E_NOT_OK;
             *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
          }
        }
        else
        {
         result = E_NOT_OK;
         *ErrorCode = DCM_E_REQUIREDTIMEDELAYNOTEXPIRED;
        }
     }
     else
     {
       result = E_NOT_OK;
       *ErrorCode = DCM_E_GENERALREJECT;
     }
  }
  else if((DCM_PENDING == OpStatus) || (DCM_CANCEL == OpStatus))
  {
      if (TRUE == IsGetSeedPending)
      {
          /*
           * Request seed from the Application
           */
       result = secLevelRow->Dcm_DspSecurityGetSeedFnc(OpStatus, &pMsgContext->resData[1], ErrorCode);
        #if (DCM_DEV_ERROR_DETECT == TRUE)

            if ((E_OK != result) && (E_NOT_OK != result) &&(DCM_E_PENDING != result))
            {
              Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_GET_SEED_SID, DCM_E_INTERFACE_RETURN_VALUE);
            }
            else
            {
              /* MISRA RULE */
            }

        #endif

       if (E_OK == result)
       {
           /*
            * Prepare the response
            */
          pMsgContext->resData[0] = subFunc;
          Glb_DspRunTime->DspSecurity->glb_PrvSeedResult = E_OK;
          pMsgContext->resDataLen = (secLevelRow->Dcm_DspSecuritySeedSize + 1);
       }
       else if (DCM_E_PENDING == result)
       {
           /*
            * In case pending due to GetSeed request
            */
         IsGetSeedPending = TRUE;
       }
        #if (DCM_DEV_ERROR_DETECT == TRUE)
        else if (E_NOT_OK == result)
        {

          if (!((*ErrorCode >= 0x01) && (*ErrorCode <= 0xFF)))
          {
              Det_ReportRuntimeError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_GET_SEED_SID, DCM_E_INVALID_VALUE);
          }
          else
          {
              /* MISRA RULE*/
          }
        }
        #endif
       else
       {
            /* MISRA RULE */
       }
     }
     else if (TRUE == IsCompareKeyPending)
     {
          /*
           * Request from the Application to compare the received key
           */
        result  = secLevelRow->DcmDspSecurityCompareKeyFnc(&pMsgContext->reqData[1],OpStatus, ErrorCode);
        #if (DCM_DEV_ERROR_DETECT == TRUE)

            if ((E_OK != result) && (E_NOT_OK != result) && (DCM_E_PENDING != result) && (DCM_E_COMPARE_KEY_FAILED != result) )
            {
              Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_COMPARE_KEY_SID, DCM_E_INTERFACE_RETURN_VALUE);
            }
            else
            {
              /* MISRA RULE */
            }

        #endif

        /*
         * In case invalid Key
         */
        if (DCM_E_COMPARE_KEY_FAILED == result)
        {
            /*
             * Increase Attempts counter by one
             */
            securityKeyAttcounter += 1;

            /*
             * Update return result
             */
            result = E_NOT_OK;

             /*
              * Update NRC
              */
            *ErrorCode = DCM_E_INVALIDKEY;

                /*
                 * Checking if the user wants to get or set number of attempts from the application
                 */
            if (secLevelRow->Dcm_DspSecurityAttemptCounterEnabled)
            {
                /*
                 * Setting number of error attempts
                 */
              result = secLevelRow->Dcm_DspSecuritySetAttemptCounterFnc(securityKeyAttcounter,OpStatus);
                #if (DCM_DEV_ERROR_DETECT == TRUE)

                    if ((E_OK != result) && (E_NOT_OK != result) && (DCM_E_PENDING != result) )
                    {
                      Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_SET_SECURITY_ATTEMPT_COUNTER_SID, DCM_E_INTERFACE_RETURN_VALUE);
                    }
                    else
                    {
                      /* MISRA RULE */
                    }

                #endif

               /*
                * Getting number of error attempts
                */
              result = secLevelRow->Dcm_DspSecurityGetAttemptCounterFnc(OpStatus,&attmpCounter);
                #if (DCM_DEV_ERROR_DETECT == TRUE)

                    if ((E_OK != result) && (E_NOT_OK != result) && (DCM_E_PENDING != result) )
                    {
                      Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_GET_SECURITY_ATTEMPT_COUNTER_SID, DCM_E_INTERFACE_RETURN_VALUE);
                    }
                    else
                    {
                      /* MISRA RULE */
                    }

                #endif

                 /*
                  * Checking if the user exceeded Max. number of attempts
                  */
               if (attmpCounter >= secLevelRow->Dcm_DspSecurityNumAttDelay)
               {
                   Glb_DspRunTime->DspSecurity->IsAttmptsCounterExceeded = TRUE;
                   securityKeyAttcounter = DCM_INITIALIZED_TO_ZERO;

                   /*
                    * Update return result
                    */
                   result = E_NOT_OK;
                   *ErrorCode = DCM_E_EXCEEDNUMBEROFATTEMPTS;

                   if (secLevelRow->Dcm_DspSecurityDelayTimeOnBoot <= secLevelRow->Dcm_DspSecurityDelayTime)
                   {
                          /* Get configured delay due to exceeding max number of attempts */
                       Glb_DspRunTime->DspSecurity->DspSecurityDelayCounter = DCM_CONVERT_MS_TO_MAIN_CYCLES(DCM_CONVERT_SEC_TO_MS(secLevelRow->Dcm_DspSecurityDelayTime));
                   }
                   else
                   {
                       /*
                        * In case delay on boot delay is longer then use it since it is mentioned in SWS
                        */
                       Glb_DspRunTime->DspSecurity->DspSecurityDelayCounter = DCM_CONVERT_MS_TO_MAIN_CYCLES(DCM_CONVERT_SEC_TO_MS(secLevelRow->Dcm_DspSecurityDelayTimeOnBoot));
                   }
               }
               else
               {
                  /* MISRA RULE */
               }
            }
            else
            {
                /*
                 * If not configured then handle it internally
                 */
                if (securityKeyAttcounter >= secLevelRow->Dcm_DspSecurityNumAttDelay)
                {
                    result = E_NOT_OK;
                    *ErrorCode = DCM_E_EXCEEDNUMBEROFATTEMPTS;
                    Glb_DspRunTime->DspSecurity->IsAttmptsCounterExceeded = TRUE;
                    securityKeyAttcounter = DCM_INITIALIZED_TO_ZERO;
                    if (secLevelRow->Dcm_DspSecurityDelayTimeOnBoot <= secLevelRow->Dcm_DspSecurityDelayTime)
                    {
                        Glb_DspRunTime->DspSecurity->DspSecurityDelayCounter = DCM_CONVERT_MS_TO_MAIN_CYCLES(DCM_CONVERT_SEC_TO_MS(secLevelRow->Dcm_DspSecurityDelayTime));
                    }
                    else
                    {
                        Glb_DspRunTime->DspSecurity->DspSecurityDelayCounter = DCM_CONVERT_MS_TO_MAIN_CYCLES(DCM_CONVERT_SEC_TO_MS(secLevelRow->Dcm_DspSecurityDelayTimeOnBoot));
                    }
                }
            }
        }
        else if (E_OK == result)
        {
            /*
             * Asking DSL to Change security level to the requested level
             */
            DslSetSecurityLevel(secLevelRow->Dcm_DspSecurityLevel);
            securityKeyAttcounter = DCM_INITIALIZED_TO_ZERO;

            /*
             * Prepare response
             */
            pMsgContext->resData[0] = subFunc;
            pMsgContext->resDataLen = 1;
        }
        else if (DCM_E_PENDING == result)
        {
            /*
             * Pending response due to compare key request
             */
            IsCompareKeyPending = TRUE;
        }
        #if (DCM_DEV_ERROR_DETECT == TRUE)
        else if (E_NOT_OK == result)
        {

          if (!((*ErrorCode >= 0x01) && (*ErrorCode <= 0xFF)))
          {
              Det_ReportRuntimeError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_COMPARE_KEY_SID, DCM_E_INVALID_VALUE);
          }
          else
          {
              /* MISRA RULE*/
          }
        }
        #endif
        else
        {
             /* MISRA RULE */
        }

      }
  }
  else
  {
      /* MISRA RULE */
  }
     return result;
}





/******************************
 * ROUTINE-CONTROL-INTERFACES *
 ******************************/




/***********************
 * LOOK-UP-FOR-RID-ROW *
 ***********************/
static boolean lookUpRoutine(uint16 RoutineIdentifier,const Dcm_DspRoutineType **requestedRoutine)
{
    boolean found = FALSE;
    uint16 index = 0;

    /*
     * Get RID configurations
     */
    const Dcm_DspRoutineType **supportedRoutine = Dsp_UDSRoutineControl;

    /*
     * Search for requested RID configuration
     */
   for (; (index < DCM_NUM_OF_SUPPORTED_ROUTINES) && (found == FALSE); ++index)
   {
     if (RoutineIdentifier == supportedRoutine[index]->DspRoutineIdentifier)
     {
       found = TRUE;

       /*
        * Return requested RID configuration
        */
       *requestedRoutine = *(supportedRoutine + index);
     }
     else
     {
       /* MISRA RULE */
     }
   }
  return found;
}


/***********************************
 * ROUTINE-CONTROL-SERVICE-HANDLER *
 ***********************************/
Std_ReturnType Dsp_UDS_RoutineControl_0x31(Dcm_ExtendedOpStatusType OpStatus,
                           Dcm_MsgContextType* pMsgContext,
                           Dcm_NegativeResponseCodeType* ErrorCode)
{

    Std_ReturnType result = E_NOT_OK;

    uint8 subFunc = DCM_INITIALIZED_TO_ZERO;
    uint16 Rid    = DCM_INITIALIZED_TO_ZERO;

    boolean IsRoutineFounded = FALSE;

    /*
     * Make it static to handle pending cases
     */
    static const Dcm_DspRoutineType *requestedRoutine = NULL;

    /*
     * Extract sub function from Request Message
     */
    subFunc = pMsgContext->reqData[0];

    /*
     * Get First RID Byte
     */
    Rid  = ((uint16)pMsgContext->reqData[1] << 8);

    /*
     * Get Second RID Byte
     */
    Rid += ((uint16)pMsgContext->reqData[2]);


    /*
     * In case the first call of the service
     */
    if (DCM_INITIAL == OpStatus)
    {
        /*
         * Check request format
         */
        if (3 == pMsgContext->reqDataLen)
        {
            /*
             * Get requested RID configuration
             */
            IsRoutineFounded = lookUpRoutine(Rid,&requestedRoutine);


                  /* [SWS_Dcm_00568] - [SWS_Dcm_00569] */
            if (TRUE == IsRoutineFounded && TRUE == requestedRoutine->DspRoutineUsed)
            {

                /*
                 * check which operation is requested
                 */
                switch (subFunc)
                {
                case DCM_UDS_START_ROUTINE:
                    result = Dsp_StartRoutine(Rid,OpStatus, requestedRoutine, pMsgContext, ErrorCode);
                    break;

                case DCM_UDS_STOP_ROUTINE:
                    result = Dsp_StopRoutine(Rid,OpStatus, requestedRoutine,pMsgContext,ErrorCode);
                    break;

                case DCM_UDS_REQUEST_ROUTINE_RESULTS:
                    result = Dsp_RequestRoutineResults(Rid,OpStatus, requestedRoutine,pMsgContext,ErrorCode);
                    break;

                default:
                    /* [SWS_Dcm_00869] */
                    *ErrorCode = DCM_E_SUBFUNCTIONNOTSUPPORTED;
                    break;
                }
            }
            else
            {
                *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            }
        }
        else
        {
            *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        }

   }
    /*
     * In case the last operation returned pending
     * Call it again
     */
   else if(DCM_PENDING == OpStatus)
   {
    switch (subFunc)
    {
    case DCM_UDS_START_ROUTINE:
        result = Dsp_StartRoutine(Rid,OpStatus, requestedRoutine, pMsgContext, ErrorCode);
    break;

    case DCM_UDS_STOP_ROUTINE:
        result = Dsp_StopRoutine(Rid,OpStatus, requestedRoutine,pMsgContext,ErrorCode);
    break;

    case DCM_UDS_REQUEST_ROUTINE_RESULTS:
        result = Dsp_RequestRoutineResults(Rid,OpStatus, requestedRoutine,pMsgContext,ErrorCode);
    break;

    default:
        /* [SWS_Dcm_00869] */
        *ErrorCode = DCM_E_SUBFUNCTIONNOTSUPPORTED;
    break;
    }
  }
  else
  {
      /* MISRA RULE */
  }

return result;
}

/************************************
 * REQUEST-ROUTINE-RESULT-OPERATION *
 ************************************/
static  Std_ReturnType Dsp_RequestRoutineResults(uint16 RID,Dcm_ExtendedOpStatusType OpStatus,
                                              const Dcm_DspRoutineType *RequestedRoutinePtr,
                                              Dcm_MsgContextType* pMsgContext,
                                              Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = E_NOT_OK;
    uint16 varDataLen = 0;

    /*
     * IS the requested RID supported in the active session level
     */
    if (TRUE == DspSessionLevelPermission(RequestedRoutinePtr->DspRequestRoutineResults->DspRequestRoutineResultsCommonAuthorizationRef->numSessLevels,
                                          RequestedRoutinePtr->DspRequestRoutineResults->DspRequestRoutineResultsCommonAuthorizationRef->DspCommonAuthorizationSessionRef))
    {
        /*
         * IS the requested RID supported in the active security level
         */

        if(TRUE == DspSecurityLevelPermission(RequestedRoutinePtr->DspRequestRoutineResults->DspRequestRoutineResultsCommonAuthorizationRef->numSecLevels,
                                              RequestedRoutinePtr->DspRequestRoutineResults->DspRequestRoutineResultsCommonAuthorizationRef->DspCommonAuthorizationSecurityLevelRef))
        {
                                /* [SWS_Dcm_01141] */
            result = RequestedRoutinePtr->DspRequestRoutineResults->DspRequestRoutineResultsFnc(OpStatus,&pMsgContext->resData[3], &varDataLen,ErrorCode);
            #if (DCM_DEV_ERROR_DETECT == TRUE)

                if ((E_OK != result) && (E_NOT_OK != result) && (DCM_E_PENDING != result) && (DCM_E_FORCE_RCRRP != result))
                {
                  Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_REQUEST_ROUTINE_RESULTS_SID, DCM_E_INTERFACE_RETURN_VALUE);
                }
                else
                {
                  /* MISRA RULE */
                }
                if (varDataLen > pMsgContext->resMaxDataLen)
                {
                    Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_REQUEST_ROUTINE_RESULTS_SID, DCM_E_INTERFACE_BUFFER_OVERFLOW);
                }
                else
                {
                    /* MISRA RULE */
                }

            #endif

            if (E_OK == result)
            {
                /*
                 * prepare response
                 */
               pMsgContext->resDataLen  = (uint32)varDataLen;
               pMsgContext->resData[0]  = DCM_UDS_REQUEST_ROUTINE_RESULTS;
               pMsgContext->resData[1]  = ((RID >> DCM_BYTE) & 0xFF);
               pMsgContext->resData[2]  = (RID & 0xFF);
               pMsgContext->resDataLen += 3;

            }
            #if (DCM_DEV_ERROR_DETECT == TRUE)
            else if (E_NOT_OK == result)
            {

              if (!((*ErrorCode >= 0x01) && (*ErrorCode <= 0xFF)))
              {
                  Det_ReportRuntimeError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_REQUEST_ROUTINE_RESULTS_SID, DCM_E_INVALID_VALUE);
              }
              else
              {
                  /* MISRA RULE*/
              }

            }
            #endif
            else
            {
                /* MISRA RULE */
            }
        }
        else
        {
            *ErrorCode = DCM_E_SECURITYACCESSDENIED;
        }

    }
    else
    {
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
    }
 return result;
}


/***************************
 * START-ROUTINE-OPERATION *
 ***************************/
static  Std_ReturnType Dsp_StartRoutine(uint16 RID,Dcm_ExtendedOpStatusType OpStatus,
                                              const Dcm_DspRoutineType *RequestedRoutinePtr,
                                              Dcm_MsgContextType* pMsgContext,
                                              Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = E_NOT_OK;

    /*
     * IS the requested RID supported in the active session level
     */
    if (TRUE == DspSessionLevelPermission(RequestedRoutinePtr->DspStartRoutine->DspStartRoutineCommonAuthorizationRef->numSessLevels,
                                          RequestedRoutinePtr->DspStartRoutine->DspStartRoutineCommonAuthorizationRef->DspCommonAuthorizationSessionRef))
    {
        /*
         * IS the requested RID supported in the active security level
         */
        if(TRUE == DspSecurityLevelPermission(RequestedRoutinePtr->DspStartRoutine->DspStartRoutineCommonAuthorizationRef->numSecLevels,
                                              RequestedRoutinePtr->DspStartRoutine->DspStartRoutineCommonAuthorizationRef->DspCommonAuthorizationSecurityLevelRef))
        {
                                /* [SWS_Dcm_01141] */
            result = RequestedRoutinePtr->DspStartRoutine->DspStartRoutineFnc(OpStatus, ErrorCode);
                #if (DCM_DEV_ERROR_DETECT == TRUE)

                    if ((E_OK != result) && (E_NOT_OK != result) && (DCM_E_PENDING != result) && (DCM_E_FORCE_RCRRP != result))
                    {
                      Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_START_ROUTINE_SID, DCM_E_INTERFACE_RETURN_VALUE);
                    }
                    else
                    {
                      /* MISRA RULE */
                    }

                #endif

            if (E_OK == result)
            {
                /*
                 * Prepare response
                 */
                pMsgContext->resData[0]  = DCM_UDS_START_ROUTINE;
                pMsgContext->resData[1]  = ((RID >> DCM_BYTE) & 0xFF);
                pMsgContext->resData[2]  = (RID & 0xFF);
                pMsgContext->resDataLen  = 3;
            }

            #if (DCM_DEV_ERROR_DETECT == TRUE)
            else if (E_NOT_OK == result)
            {
              if (!((*ErrorCode >= 0x01) && (*ErrorCode <= 0xFF)))
              {
                  Det_ReportRuntimeError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_START_ROUTINE_SID, DCM_E_INVALID_VALUE);
              }
              else
              {
                  /* MISRA RULE*/
              }
            }
            #endif
            else
            {
                /* MISRA RULE */
            }

        }
        else
        {
            *ErrorCode = DCM_E_SECURITYACCESSDENIED;
        }

    }
    else
    {
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
    }
 return result;
}

/***************************
 * STOP-ROUTINE-OPERATION *
 ***************************/
static  Std_ReturnType Dsp_StopRoutine(uint16 RID,Dcm_ExtendedOpStatusType OpStatus,
                                              const Dcm_DspRoutineType *RequestedRoutinePtr,
                                              Dcm_MsgContextType* pMsgContext,
                                              Dcm_NegativeResponseCodeType* ErrorCode)
{

    Std_ReturnType result = E_NOT_OK;

    /*
     * IS the requested RID supported in the active session level
     */
    if (TRUE == DspSessionLevelPermission(RequestedRoutinePtr->DspStopRoutine->DspStopRoutineCommonAuthorizationRef->numSessLevels,
                                          RequestedRoutinePtr->DspStopRoutine->DspStopRoutineCommonAuthorizationRef->DspCommonAuthorizationSessionRef))
    {
        /*
         * IS the requested RID supported in the active security level
         */
        if(TRUE == DspSecurityLevelPermission(RequestedRoutinePtr->DspStopRoutine->DspStopRoutineCommonAuthorizationRef->numSecLevels,
                                              RequestedRoutinePtr->DspStopRoutine->DspStopRoutineCommonAuthorizationRef->DspCommonAuthorizationSecurityLevelRef))
        {
                                /* [SWS_Dcm_01141] */
            result = RequestedRoutinePtr->DspStopRoutine->DspStopRoutineFnc(OpStatus, ErrorCode);
            #if (DCM_DEV_ERROR_DETECT == TRUE)

                if ((E_OK != result) && (E_NOT_OK != result) && (DCM_E_PENDING != result) && (DCM_E_FORCE_RCRRP != result))
                {
                  Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_STOP_ROUTINE_SID, DCM_E_INTERFACE_RETURN_VALUE);
                }
                else
                {
                  /* MISRA RULE */
                }

            #endif

            if (E_OK == result)
            {
                /*
                 * prepare response
                 */
                pMsgContext->resData[0]  = DCM_UDS_STOP_ROUTINE;
                pMsgContext->resData[1]  = ((RID >> DCM_BYTE) & 0xFF);
                pMsgContext->resData[2]  = (RID & 0xFF);
                pMsgContext->resDataLen  = 3;
            }

            #if (DCM_DEV_ERROR_DETECT == TRUE)

            else if (E_NOT_OK == result)
            {

              if (!((*ErrorCode >= 0x01) && (*ErrorCode <= 0xFF)))
              {
                  Det_ReportRuntimeError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_START_ROUTINE_SID, DCM_E_INVALID_VALUE);
              }
              else
              {
                  /* MISRA RULE*/
              }
            }
            #endif
            else
            {
                /* MISRA RULE */
            }
        }
        else
        {
            *ErrorCode = DCM_E_SECURITYACCESSDENIED;
        }

    }
    else
    {
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
    }
 return result;
}
