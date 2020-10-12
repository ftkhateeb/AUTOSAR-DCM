/******************************************************************************
 * Module      : DCM                                                          *
 * SubModule   : DSL                                                          *
 * File Name   : Dsl.c                                                        *
 * Description : Source file for Dsl Sub-Module                               *
 * Created on  : April 1, 2020                                                *
 ******************************************************************************/

#include "include/Dcm_Dsl.h"
#include "include/Dcm_Dsd.h"
#include "include/Dcm_Dsp.h"
#include "include/Dcm.h"
#include <string.h>
#include <stdio.h>

/**********************************
 * DSL-STATIC-FUNTIONS-PROTOTYPES *
 **********************************/

             /* Service that locks the buffer */
static inline LockBuffer(Dcm_DslBufferStatusType *Lock);

       /* For handling concurrent Tester Present Requests  */
static boolean IsTesterPresentRequest(const PduInfoType* request);

                    /* Establish a connection */
static boolean Connect(PduIdType rxPduId,const Dcm_DslProtocolRxType **connectionRx,
                                       const Dcm_DslMainConnectionType **mainConnection,
                                       const Dcm_DslConnectionType **connection,
                                       const Dcm_DslProtocolRowType **protocolRow,
                                             Dcm_DslRunTimeDataType **ProtocolRunTime);

static boolean GetRxChannel(PduIdType rxPduId,const Dcm_DslProtocolRxType **connectionRx);


static inline void SetS3ServerSessionTimeoutTimer(void);

static void DslSendNRC(Dcm_NegativeResponseCodeType NRC);

static inline void ReleaseRxTxBuffers(void);



/******************************************
 *           GLOBAL-VARIABLES             *
 ******************************************/

/* Global_var to be initialized by Dsl_Init and used by other services */

const  Dcm_DslProtocolRowType *Glb_ActiveProtocolRowList;

uint8 responsePendingCounter = 0;
uint8 Dsl_DiagRespMaxNumRespPend = 0;

Dcm_DslRunTimeDataType   Glb_DslRunTimeInit;
Dcm_DslLocalBufferType   Glb_LocalBuffer;

const Dcm_DspSessionType    *DcmSession;
const Dcm_DslProtocolRxType  **glb_ProtocolrxList;

const Dcm_DslProtocolRowType    *Glb_ProtocolConnectionRow;
const Dcm_DslConnectionType      *Glb_ProtocolConnection;
const Dcm_DslMainConnectionType  *Glb_ProtocolMainConnection;
const Dcm_DslProtocolRxType      *Glb_ProtocolRxConnection;
      Dcm_DslRunTimeDataType     *Glb_DslProtocolRunTimePtr;

uint32 totalSduLength = DCM_INITIALIZED_TO_ZERO;


/*****************************************************************************
 *                                                                           *
 *                                FUNCTIONS                                  *
 *                                                                           *
 *****************************************************************************/


/******************
 * BUFFER-LOCKING *
 ******************/
static inline LockBuffer(Dcm_DslBufferStatusType *Lock)
{
    *Lock = DCM_BUFFER_LOCKED;
}



/****************************
 * GET-ACTIVE-SESSION-LEVEL *
 ****************************/
Std_ReturnType Dcm_GetSesCtrlType(Dcm_SesCtrlType* SesCtrlType)
{
    *SesCtrlType = Glb_DslProtocolRunTimePtr->DslSessionHandling->sessionRow.DspSessionLevel;

    return E_OK;
}



/***********************
 * GET-ACTIVE-PROTOCOL *
 ***********************/
void Dsl_GetActiveProtoclRow(const Dcm_DslProtocolRowType **ActiveProtocolRow)
{
    *ActiveProtocolRow = Glb_ProtocolConnectionRow;
}



/*******************************
 * CHANGE-ACTIVE-SESSION-LEVEL *
 *******************************/
boolean ChangeSessionLevel (Dcm_SesCtrlType Newsession)
{
    uint8 index;
    boolean found = FALSE;
    const Dcm_DspSessionRowType **SessionRow = NULL;
    SessionRow = DcmSession->DspSessionRow;

    switch(Newsession)
    {
       case DCM_DEFAULT_SESSION:
       case DCM_PROGRAMMING_SESSION:
       case DCM_EXTENDED_DIAGNOSTIC_SESSION:
       case DCM_SAFETY_SYSTEM_DIAGNOSTIC_SESSION:

           /* Compare received Session Level to be set with Configured Session Levels */
           for(index = 0; (index < DCM_DSP_UDS_NUM_OF_SUPPORTED_SESSIONS) && (found == FALSE); index++)
           {
               /* if the session requested is founded, it changes to be the active Session Level */
               if(Newsession == SessionRow[index]->DspSessionLevel)
               {
                   found = TRUE;
                   Glb_DslProtocolRunTimePtr->DslSessionHandling->sessionRow.DspSessionLevel = Newsession;

                   Glb_DslProtocolRunTimePtr->DslSessionHandling->sessionRow.DspSessionP2ServerMax = SessionRow[index]->DspSessionP2ServerMax;
                   Glb_DslProtocolRunTimePtr->DslSessionHandling->sessionRow.DspSessionP2StarServerMax = SessionRow[index]->DspSessionP2StarServerMax;
               }
               else
               {
                   /*MISRA RULE */
               }
           }

       break;

       default:
           /* if it enter default case this is mean that its not in configured diagnostic sessions */
           break;
    }

 return found;
}



/**********************
 * SET-SECURITY-LEVEL *
 **********************/
void DslSetSecurityLevel(Dcm_SecLevelType secLevel)
{
   Glb_DslProtocolRunTimePtr->DslSecurityHandling->securityLevel = secLevel;
}



/*****************************
 * GET-ACTIVE-SECURITY-LEVEL *
 *****************************/
Std_ReturnType Dsl_GetSecurityLevel(Dcm_SecLevelType *SecLevel)
{
  *SecLevel = Glb_DslProtocolRunTimePtr->DslSecurityHandling->securityLevel;

  return E_OK;
}



/*********************************
 * REQUEST-FINISHED-CONFIRMATION *
 *********************************/
void Dsl_TpTxConfirmation(PduIdType id,Std_ReturnType result)
{
    ReleaseRxTxBuffers();
}



/****************************
 * RESET-SESSION-TO-DEFAULT *
 ****************************/
Std_ReturnType Dcm_ResetToDefaultSession(void)
{
    Glb_DslProtocolRunTimePtr->DslSessionHandling->sessionRow.DspSessionLevel = DCM_DEFAULT_SESSION;
    Glb_DslProtocolRunTimePtr->DslSessionHandling->sessionRow.DspSessionP2ServerMax = DcmSession->DspSessionRow[0]->DspSessionP2ServerMax;
    Glb_DslProtocolRunTimePtr->DslSessionHandling->sessionRow.DspSessionP2ServerMax = DcmSession->DspSessionRow[0]->DspSessionP2StarServerMax;
    Glb_DslProtocolRunTimePtr->DslSessionHandling->S3ServerStarted = FALSE;

    return E_OK;
}



/*********************
 * SET-SESSION-LEVEL *
 *********************/
Std_ReturnType DslInternal_SetSesCtrlType(Dcm_SesCtrlType SesCtrlType)
{

          /* checking whether the new session is the same with the
           * running session now if not then we change it and reset timers
           */
   if (Glb_DslProtocolRunTimePtr->DslSessionHandling->sessionRow.DspSessionLevel != SesCtrlType)
   {
                    /* Change the current session level */
       if (TRUE == ChangeSessionLevel(SesCtrlType))
       {
                   /* Reset session Timer */
           SetS3ServerSessionTimeoutTimer();
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
  return E_OK;
}



/******************************
 * SET-S3SERVER-TIMEOUT-TIMER *
 *****************************/
static inline void SetS3ServerSessionTimeoutTimer(void)
{
    /* Convert Configured S3Server Session Time in (ms) to a number of DcmMain Cycles */
  Glb_DslProtocolRunTimePtr->DslSessionHandling->S3ServerTimeoutCount = DCM_CONVERT_MS_TO_MAIN_CYCLES(Glb_ProtocolConnectionRow->DslProtocolTimeLimit->TimStrS3Server);
    /* A flag being raised to indicate DslMain that S3Server Timeout Timer is ready to Dcrement */
  Glb_DslProtocolRunTimePtr->DslSessionHandling->S3ServerStarted = TRUE;
}



/********************************
 * RESET-S3SERVER-TIMEOUT-TIMER *
 ********************************/
void Reset_S3ServerTimer(void)
{
    SetS3ServerSessionTimeoutTimer();
}



/*********************************
 * NEGATIVE-RESPONSE-PREPARATION *
 *********************************/
static void DslSendNRC(Dcm_NegativeResponseCodeType NRC)
{

    Std_ReturnType PduR_DcmTransmitResult;

  if( DCM_NOT_IN_USE == Glb_DslProtocolRunTimePtr->NegativeResponseBuffer->BufferCurrentUse)
  {

      /* Dcm shall send Negative Response from separate buffer as refereed in [SWS_Dcm_00119] */
      Glb_DslProtocolRunTimePtr->NegativeResponseBuffer->NRInfo->SduDataPtr[0] = DCM_NEGATIVE_RESPONSE_SID;
      Glb_DslProtocolRunTimePtr->NegativeResponseBuffer->NRInfo->SduDataPtr[1] = Glb_DslProtocolRunTimePtr->DCM_RxBufferInfo->dcmRxBuffer.SduDataPtr[DCM_SID];
      Glb_DslProtocolRunTimePtr->NegativeResponseBuffer->NRInfo->SduDataPtr[2] = NRC;

            /* Payload of negative response */
      Glb_DslProtocolRunTimePtr->NegativeResponseBuffer->NRInfo->SduLength = 3;
      /* Update Use Status of NR Buffer as is already Transmitting Response */
      Glb_DslProtocolRunTimePtr->NegativeResponseBuffer->BufferCurrentUse = DCM_TRANSMIT_SIGNALED;

      /* When the diagnostic response of a DcmDslMainConnection is ready, the DSL
          submodule shall trigger PduR to copy it by calling CopyTx[SWS_Dcm_00115] */
      PduR_DcmTransmitResult = PduR_DcmTransmit(Glb_DslProtocolRunTimePtr->DCM_TxBufferInfo->diagResponseTxPduId, Glb_DslProtocolRunTimePtr->NegativeResponseBuffer->NRInfo);
      if (E_OK != PduR_DcmTransmitResult)
      {
          /* Release Dcm Both Tx and Rx Buffers for next Tester Request */
          Glb_DslProtocolRunTimePtr->NegativeResponseBuffer->BufferCurrentUse = DCM_NOT_IN_USE;

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



/***********************
 * RELEASE-DCM-BUFFERS *
 ***********************/
static inline void ReleaseRxTxBuffers(void)
{
    Glb_DslProtocolRunTimePtr->DCM_RxBufferInfo->glbRxBufferState.Lock = DCM_BUFFER_UNLOCKED;
    Glb_DslProtocolRunTimePtr->DCM_RxBufferInfo->glbRxBufferCurrentUse = DCM_NOT_IN_USE;
    Glb_DslProtocolRunTimePtr->DCM_TxBufferInfo->glbTxBufferState.Lock = DCM_BUFFER_UNLOCKED;
    Glb_DslProtocolRunTimePtr->DCM_TxBufferInfo->glbTxBufferCurrentUse = DCM_NOT_IN_USE;

    Glb_DslProtocolRunTimePtr->DCM_TxBufferInfo->GlbTXBuffIndex = DCM_INITIALIZED_TO_ZERO;
}



/*******************************
 * FORWARD-RESPONSE-INDICATION *
 *******************************/
void DsdToDslForwardingResponse(DsdProcessingDoneResponseType rsponseStatus,uint32 resLength)
{
    switch(rsponseStatus)
    {
        case DCM_DSD_TX_RESPONSE_READY:
            Glb_DslProtocolRunTimePtr->DCM_TxBufferInfo->glbTxBufferCurrentUse = DCM_DSD_PENDING_RESPONSE_SIGNALED;
            Glb_DslProtocolRunTimePtr->DCM_TxBufferInfo->dcmTxBuffer.SduLength = resLength;
            break;
        case DCM_DSD_TX_RESPONSE_SUPPRESSED:
            ReleaseRxTxBuffers();
            break;
        default:
            break;
    }
}



/**********************
 * DSL-INITIALIZATION *
 **********************/
void Dsl_Init(const Dcm_ConfigType* ConfigPtr)
{

  glb_ProtocolrxList = ConfigPtr->DcmDsl->DslProtocol->DslProtocolRxGlbConnectionIDList;
  Dsl_DiagRespMaxNumRespPend = ConfigPtr->DcmDsl->DslDiagResp->Dcm_DslDiagRespMaxNumRespPend;
  DcmSession = ConfigPtr->DcmDsp->DcmDspSession;

  /* Initialization of global variable */
    Glb_DslProtocolRunTimePtr = &Glb_DslRunTimeInit;

    Glb_DslProtocolRunTimePtr->DslSessionHandling->S3ServerStarted       = FALSE;
    Glb_DslProtocolRunTimePtr->DslSessionHandling->S3ServerTimeoutCount  = DCM_INITIALIZED_TO_ZERO;
    Glb_DslProtocolRunTimePtr->DCM_RxBufferInfo->glbRxBufferCurrentUse   = DCM_NOT_IN_USE;
    Glb_DslProtocolRunTimePtr->DCM_RxBufferInfo->glbRxBufferState.Lock   = DCM_BUFFER_UNLOCKED;
    Glb_DslProtocolRunTimePtr->DCM_TxBufferInfo->glbTxBufferCurrentUse   = DCM_NOT_IN_USE;
    Glb_DslProtocolRunTimePtr->DslProtocolHandling->protocolStarted      = FALSE;
    Glb_DslProtocolRunTimePtr->responsePendingCount  = DCM_INITIALIZED_TO_ZERO;
    Glb_DslProtocolRunTimePtr->localRxBuffer->bufferState.Lock = DCM_BUFFER_UNLOCKED;
    Glb_DslProtocolRunTimePtr->localRxBuffer->localRxBufferCurrentUse = DCM_NOT_IN_USE;
    Glb_DslProtocolRunTimePtr->DslSessionHandling->sessionRow.DspSessionLevel= DCM_DEFAULT_SESSION;
}



/*****************************************
 * GET-CORRESPONDING-RX-CHANNEL-TO-PDUID *
 *****************************************/
static boolean GetRxChannel(PduIdType rxPduId,const Dcm_DslProtocolRxType **connectionRx)

{
    uint8 index;
    boolean found = FALSE;

    for (index = 0; (index < DCM_NUM_OF_RX_CHANNELS) && (found == FALSE); index++)
    {
        /* check if corresponding channel to PduId is supported */
        if (rxPduId == glb_ProtocolrxList[index]->DcmDslProtocolRxPduId)
        {
            found = TRUE;
            *connectionRx = *(glb_ProtocolrxList + index);
        }
        else
        {
            /* MISRA RULE */
        }

    }
    return found;
}


/*****************************************************
 * CONNECT-RECEIVED-PDU-TO-APPROPRIATE-PROTOCOL-PATH *
 *****************************************************/
static boolean Connect(PduIdType rxPduId,const Dcm_DslProtocolRxType **connectionRx,
                                         const Dcm_DslMainConnectionType **mainConnection,
                                         const Dcm_DslConnectionType **connection,
                                         const Dcm_DslProtocolRowType **protocolRow,
                                         Dcm_DslRunTimeDataType **ProtocolRunTime)

{

   boolean found = FALSE;
   int index;

     for (index = 0; (index < DCM_NUM_OF_RX_CHANNELS) && (found == FALSE); index++)
     {
             /* check if rxPduId is configured if yes it Establishes the connection */
         if (rxPduId == glb_ProtocolrxList[index]->DcmDslProtocolRxPduId)
         {
             *connectionRx = *(glb_ProtocolrxList+index);
                 /* Getting Main Connection that configured for this channel */
             *mainConnection = (*connectionRx)->DslMainConnectionParent;

                 /* Getting Connection that configured for this channel */
             *connection = (*mainConnection)->DslConnectionParent;

                 /* Getting Protocol Row that configured for this channel */
             *protocolRow = (*connection)->DslProtocolRowParent;

             *ProtocolRunTime = (*protocolRow)->DslrunTime;

                 /* the connection is set successfully */
                         found = TRUE;
         }
         else
         {
            /* MISRA RULE */
         }
     }
  return found;
}




/*************************************************
 * CHECK-IF-REQUEST-IS-FUNCTIONAL-TESTER-PRESENT *
 *************************************************/
static boolean IsTesterPresentRequest(const PduInfoType* request)
{

  boolean ret = FALSE;

       /* check if the received request is functional tester present and suppress positive response message bit is set to true */
  if ((DCM_TESTER_PRESENT_SID == request->SduDataPtr[DCM_SID_INDEX]) && (request->SduDataPtr[DCM_SUB_FUN_INDEX] & SUPPRESS_POS_RSP_MSG_INDICATION_BIT))
  {
      /* Tester Present Command with suppress positive response is indicated */
      ret = TRUE;
  }
  return ret;
}



/***********************************
 * ACCEPT-NEW-REQUEST-VERIFICATION *
 ***********************************/
BufReq_ReturnType AcceptNewConnection(PduIdType id,
                                        const PduInfoType* info,
                                        PduLengthType TpSduLength,
                                        PduLengthType* bufferSizePtr)
{
	const Dcm_DslProtocolRxType     *protocolRx = NULL;
	const Dcm_DslMainConnectionType *mainConnection = NULL;

        /* The status of the Buffer Request */
    BufReq_ReturnType  result = BUFREQ_OK;
    boolean found = FALSE;

    found = GetRxChannel(id,&protocolRx);
    mainConnection = protocolRx->DslMainConnectionParent;
    if (TRUE == found)
	{
           /*
			*  Is the service invoked with TpSduLength = 0
			*  if yes -> Provide max. buffer
			*/
        if ((DCM_INITIALIZED_TO_ZERO != TpSduLength))
        {
			/* [SWS_Dcm_00444]
			* Is the request msg length overflows the DCM buffer?
			* if yes -> result = BUFREQ_E_OVFL
			*/
	     	if (DCM_DSL_UDS_MAIN_BUFFER_SIZE >= TpSduLength)
			{
				/* [SWS_Dcm_00557]
				* Is the request belonging to the same connection that is already
				* using the buffer?
				* if yes -> result = BUFREQ_E_NOT_OK
				*/
				if (DCM_BUFFER_LOCKED == Glb_DslProtocolRunTimePtr->DCM_RxBufferInfo->glbRxBufferState.Lock)
				{
					/* [SWS_Dcm_00557],[SWS_Dcm_01168]
					* Is the  request belonging to the same connection ?
					* Is it Tester Present request ?
					* Is it Functional addressing?
					* if yes -> result = BUFREQ_OK
					*/

					if ((Glb_ProtocolMainConnection->DcmDslProtocolRxConnectionId == mainConnection->DcmDslProtocolRxConnectionId)&&//
						(IsTesterPresentRequest(info)) &&//
						(DCM_FUNCTIONAL_TYPE == protocolRx->DcmDslProtocolRxAddr))
					{
						/*
						* Is local buffer unlock and not in use?
						* if yes -> result = BUFREQ_OK
						* and PROVIDE_TO_PDUR
						*/
						if((DCM_BUFFER_UNLOCKED == Glb_DslProtocolRunTimePtr->localRxBuffer->bufferState.Lock)&&//
							(DCM_NOT_IN_USE == Glb_DslProtocolRunTimePtr->localRxBuffer->localRxBufferCurrentUse))
						{
							result = BUFREQ_OK;

								/* Return available buffer size */
							*bufferSizePtr = DCM_DSL_LOCAL_BUFFER_LENGTH;

								/* Provide buffer to pduR to store tester present request data */
							Glb_DslProtocolRunTimePtr->localRxBuffer->localRxBufferCurrentUse = DCM_PROVIDED_TO_PDUR;

								/* [SWS_Dcm_00241]
								* Lock the buffer during usage
								*/
								LockBuffer(&Glb_DslProtocolRunTimePtr->localRxBuffer->bufferState.Lock);

						}
						else
						{
							result = BUFREQ_E_NOT_OK;
						}
					}
					else
					{
				    result = BUFREQ_E_NOT_OK;
					}
				}
				else
				{
					/*
					* Is the id valid ?
					* if yes -> ACCEPT the connection
					*/
					if(!Connect(id,&Glb_ProtocolRxConnection,&Glb_ProtocolMainConnection,
						&Glb_ProtocolConnection,&Glb_ProtocolConnectionRow,&Glb_DslProtocolRunTimePtr))
					{
						/* Connection Failed */
						result = BUFREQ_E_NOT_OK;
					}
					else
					{
						/* Connection Accepted */
						result = BUFREQ_OK;

						/* Return available buffer size */
						*bufferSizePtr = DCM_DSL_UDS_MAIN_BUFFER_SIZE;

						/*[SWS_Dcm_00241]
						* Lock the global buffer during the usage
						*/
						LockBuffer(&Glb_DslProtocolRunTimePtr->DCM_RxBufferInfo->glbRxBufferState.Lock);

						/* Provide the global DCM buffer to pduR to store Req. data.*/
						Glb_DslProtocolRunTimePtr->DCM_RxBufferInfo->glbRxBufferCurrentUse = DCM_PROVIDED_TO_PDUR;
						Glb_DslProtocolRunTimePtr->DCM_RxBufferInfo->totalSduLength = TpSduLength;
						Glb_DslProtocolRunTimePtr->DslProtocolHandling->protocolStarted = TRUE;
						Glb_DslProtocolRunTimePtr->DCM_RxBufferInfo->dcmRxBuffer.SduLength = 0;
						Glb_DslProtocolRunTimePtr->DCM_TxBufferInfo->diagResponseTxPduId = Glb_ProtocolMainConnection->DslProtocolTx->DslTxConfirmationPduId;
					}

			    } 
			
			}
			else
	        {    /* the request msg length overflows the DCM buffer */
			result = BUFREQ_E_OVFL;

             /* return buffer size with zero length */
			*bufferSizePtr = DCM_INITIALIZED_TO_ZERO;
		    }  

	    }
        else
        {
              /* MISRA RULE */
        }

	}
    else
    {
        result = BUFREQ_E_NOT_OK;
    }
	return result;
}



/***************************
 * COPY-DATA-TO-DCM-BUFFER *
 ***************************/
BufReq_ReturnType CopyRequestData(PduIdType id,
                                const PduInfoType* info,
                                PduLengthType* bufferSizePtr)
{

  /* The status of the Buffer Request */
    BufReq_ReturnType  result = BUFREQ_OK;

   /*
    * Is the global DCM buffer Locked and provided to pduR?
    * if yes -> use it to copy the data
    */
   if ((DCM_BUFFER_LOCKED == Glb_DslProtocolRunTimePtr->DCM_RxBufferInfo->glbRxBufferState.Lock) &&//
      (DCM_PROVIDED_TO_PDUR == Glb_DslProtocolRunTimePtr->DCM_RxBufferInfo->glbRxBufferCurrentUse))
  {

       /* Remaining bytes of the DCM buffer */
       Glb_DslProtocolRunTimePtr->DCM_RxBufferInfo->remainingBytes = DCM_DSL_UDS_MAIN_BUFFER_SIZE - Glb_DslProtocolRunTimePtr->DCM_RxBufferInfo->GlbRXBuffIndex;

          /*[SWS_Dcm_00996]
           * Is SDU length equal to zero?
           * if yes -> return bufferSizePtr with the remaining size
           */
       if (DCM_INITIALIZED_TO_ZERO == info->SduLength)
       {
           /* return bufferSizePtr with the remaining size */
           *bufferSizePtr = Glb_DslProtocolRunTimePtr->DCM_RxBufferInfo->remainingBytes;
       }
       else
       {
              /* [SWS_Dcm_00443]
               * Copying data from info->SduDataPtr
               * to the buffer
               * memmove() is used to copy a block of
               * memory from a location to another.
               * It is declared in string.h
               */
              memmove(Glb_DslProtocolRunTimePtr->DCM_RxBufferInfo->dcmRxBuffer.SduDataPtr + Glb_DslProtocolRunTimePtr->DCM_RxBufferInfo->GlbRXBuffIndex,
                      info->SduDataPtr, info->SduLength);

              /* Increasing the current position of index by the SDU length */
              Glb_DslProtocolRunTimePtr->DCM_RxBufferInfo->GlbRXBuffIndex += info->SduLength;
              Glb_DslProtocolRunTimePtr->DCM_RxBufferInfo->dcmRxBuffer.SduLength += info->SduLength;


              /* return the remaining bytes */
             *bufferSizePtr = DCM_DSL_UDS_MAIN_BUFFER_SIZE - Glb_DslProtocolRunTimePtr->DCM_RxBufferInfo->GlbRXBuffIndex;
             /*
              * Is buffer index reaches the end of SDU?
              * if yes -> reset bufferIndex
              */
             if(Glb_DslProtocolRunTimePtr->DCM_RxBufferInfo->GlbRXBuffIndex == Glb_DslProtocolRunTimePtr->DCM_RxBufferInfo->totalSduLength)
             {
                 /* Reset buffer index */
                 Glb_DslProtocolRunTimePtr->DCM_RxBufferInfo->GlbRXBuffIndex = DCM_INITIALIZED_TO_ZERO;
                 Glb_DslProtocolRunTimePtr->DCM_RxBufferInfo->remainingBytes = DCM_INITIALIZED_TO_ZERO;
             }
             else
             {
                 /* MISRA rule */
             }

         }

  }
  else
  {
         /*
          * Is the local DCM buffer Locked and provided to pduR?
          * if yes -> use it to copy the data
          */
      if ((DCM_BUFFER_LOCKED == Glb_DslProtocolRunTimePtr->localRxBuffer->bufferState.Lock)&&//
          (DCM_PROVIDED_TO_PDUR == Glb_DslProtocolRunTimePtr->localRxBuffer->localRxBufferCurrentUse))
      {
          /*[SWS_Dcm_00996]
           * Is SDU length equal to zero?
           * if yes -> return bufferSizePtr with the remaining size
           */
          if(DCM_INITIALIZED_TO_ZERO == info->SduLength)
          {
              /* [SWS_Dcm_00443]
               * return bufferSizePtr with the remaining size
               */
            *bufferSizePtr = DCM_DSL_UDS_MAIN_BUFFER_SIZE;

            /*Return BUFREQ_OK */
             return result;
          }
          else
          {

             /*[SWS_Dcm_00443]
              * Copying data from info->SduDataPtr
              * to the buffer
              * memmove() is used to copy a block of
              * memory from a location to another.
              * It is declared in string.h
              */
              memmove(Glb_DslProtocolRunTimePtr->localRxBuffer->testerPresentReq->SduDataPtr,info->SduDataPtr, info->SduLength);

          }

      }
      else
      {
          result = BUFREQ_E_NOT_OK;
      }

  }
    return result;
}



/*****************************
 * COPY-DATA-FROM-DCM-BUFFER *
 *****************************/
BufReq_ReturnType Dcm_CopyTransmitData(PduIdType id,const PduInfoType* info,const RetryInfoType* retry,
                                       PduLengthType* availableDataPtr)
{

    BufReq_ReturnType  result            = BUFREQ_OK ;
    static uint32 AvailableBytesInDcm    = DCM_INITIALIZED_TO_ZERO ;

    /*
     * Only abnormal cases change the state.
     * result has 3 states (BUFREQ_OK, BUFREQ_E_NOT_OK,BUFREQ_E_BUSY).
     * DCM Buffer Must be Provided to DSD.
     */

    AvailableBytesInDcm  = DCM_DSL_UDS_MAIN_BUFFER_SIZE - Glb_DslProtocolRunTimePtr->DCM_TxBufferInfo->GlbTXBuffIndex;

    if (DCM_TRANSMIT_SIGNALED == Glb_DslProtocolRunTimePtr->DCM_TxBufferInfo->glbTxBufferCurrentUse)
    {
        if(AvailableBytesInDcm >= info->SduLength)
        {
            if (DCM_INITIALIZED_TO_ZERO == info->SduLength)
            {
                /* Query the available data in the Upper layer module if SduLength =0 */

                *availableDataPtr = AvailableBytesInDcm;

            }
             else
             {

                if (NULL == retry)
                {
                    memmove(info->SduDataPtr,
                            Glb_DslProtocolRunTimePtr->DCM_TxBufferInfo->dcmTxBuffer.SduDataPtr + Glb_DslProtocolRunTimePtr->DCM_TxBufferInfo->GlbTXBuffIndex,
                            info->SduLength);

                    Glb_DslProtocolRunTimePtr->DCM_TxBufferInfo->GlbTXBuffIndex  += info->SduLength;
                    Glb_DslProtocolRunTimePtr->PrevSduLen       = info->SduLength;

                    /* Data copied, then removed */
                }
                else if (TP_CONFPENDING ==retry->TpDataState)
                {
                   memmove(info->SduDataPtr, Glb_DslProtocolRunTimePtr->DCM_TxBufferInfo->dcmTxBuffer.SduDataPtr + Glb_DslProtocolRunTimePtr->DCM_TxBufferInfo->GlbTXBuffIndex, info->SduLength);

                    Glb_DslProtocolRunTimePtr->PrevSduLen = info->SduLength;
                    AvailableBytesInDcm                  += DCM_INITIALIZED_TO_ZERO;

                }
                else if (TP_DATACONF   == retry->TpDataState)
                {
                    memmove(info->SduDataPtr,
                            Glb_DslProtocolRunTimePtr->DCM_TxBufferInfo->dcmTxBuffer.SduDataPtr + Glb_DslProtocolRunTimePtr->PrevSduLen +Glb_DslProtocolRunTimePtr->DCM_TxBufferInfo->GlbTXBuffIndex,
                            info->SduLength);

                    Glb_DslProtocolRunTimePtr->DCM_TxBufferInfo->GlbTXBuffIndex +=Glb_DslProtocolRunTimePtr->PrevSduLen;
                    AvailableBytesInDcm                       +=Glb_DslProtocolRunTimePtr->PrevSduLen ;
                    Glb_DslProtocolRunTimePtr->PrevSduLen      = info->SduLength;

                    /*data that has been copied before this call is confirmed and can be removed*/
                }

                else if (TP_DATARETRY == retry->TpDataState)
                {
                    memmove(info->SduDataPtr, Glb_DslProtocolRunTimePtr->DCM_TxBufferInfo->dcmTxBuffer.SduDataPtr + Glb_DslProtocolRunTimePtr->DCM_TxBufferInfo->GlbTXBuffIndex - retry->TxTpDataCnt, info->SduLength); /* ques?*/

                    Glb_DslProtocolRunTimePtr->DCM_TxBufferInfo->GlbTXBuffIndex -= DCM_INITIALIZED_TO_ZERO;
                    AvailableBytesInDcm                       -= DCM_INITIALIZED_TO_ZERO;
                }

                else
                {
                    result = BUFREQ_E_NOT_OK;

                }

            }
        }

        else
        {
            /*
             * If not enough transmit data is available, no data is copied
             * by the upper layer module and BUFREQ_E_BUSY is returned
             */
            result =BUFREQ_E_BUSY;
        }

    }
    else
    {
        if (DCM_TRANSMIT_SIGNALED == Glb_DslProtocolRunTimePtr->NegativeResponseBuffer->BufferCurrentUse)
        {
            memmove(info->SduDataPtr, Glb_DslProtocolRunTimePtr->NegativeResponseBuffer->NRInfo->SduDataPtr, info->SduLength);

            /* Release Negative response buffer Buffers for next NRC response */
            Glb_DslProtocolRunTimePtr->NegativeResponseBuffer->BufferCurrentUse = DCM_NOT_IN_USE;

        }
        else
        {
            result = BUFREQ_E_NOT_OK;
        }
    }

 return result;
}



/************************************************
 * INDICATION-THAT-DATA-IS-COPIED-TO-DCM-BUFFER *
 ************************************************/
void Dcm_PduRToDslIndication( PduIdType id, Std_ReturnType result )
{
    /* This Result sent by CanTp to describe the result of StartOfReception
       and CopyRxData Processes and then A course of Actions has to be taken */
    if ( E_OK == result )
    {
        /* Check if the Request was Functional TesterPresent with "suppressPosRspMsgIndicationBit"
            set to TRUE, Received on "TesterPresent" Buffer */
        if ( DCM_BUFFER_LOCKED == Glb_DslProtocolRunTimePtr->localRxBuffer->bufferState.Lock )
        {
            /* Execute TesterPresent Request by Resetting S3Server Timer
                          as referred in [SWS_Dcm_00112] [SWS_Dcm_00113]*/
            SetS3ServerSessionTimeoutTimer();

            /* Release TesterPresent Buffer for Next Request */
            Glb_DslProtocolRunTimePtr->localRxBuffer->bufferState.Lock = DCM_BUFFER_UNLOCKED;
            Glb_DslProtocolRunTimePtr->localRxBuffer->localRxBufferCurrentUse = DCM_NOT_IN_USE;
        }

            /* If a Request Received in the Main Dcm Buffer from PduR */
        else if ((DCM_BUFFER_LOCKED == Glb_DslProtocolRunTimePtr->DCM_RxBufferInfo->glbRxBufferState.Lock)&&//
                  (DCM_PROVIDED_TO_PDUR == Glb_DslProtocolRunTimePtr->DCM_RxBufferInfo->glbRxBufferCurrentUse) )
        {

            /* Check if the Request was Functional TesterPresent with "suppressPosRspMsgIndicationBit"
                    set to TRUE, Received on Main Dcm Buffer*/
            if ((DCM_FUNCTIONAL_TYPE == Glb_ProtocolRxConnection->DcmDslProtocolRxAddr)&&//
                 TRUE == IsTesterPresentRequest(&Glb_DslProtocolRunTimePtr->DCM_RxBufferInfo->dcmRxBuffer))
            {
                /* Execute TesterPresent Request by Resetting S3Server Timer
                    as referred in [SWS_Dcm_00112] [SWS_Dcm_00113]*/
                SetS3ServerSessionTimeoutTimer();

                /* Release TesterPresent Buffer and set its Status as
                    NOT_IN_USE to be able to accept Next Request */
                Glb_DslProtocolRunTimePtr->DCM_RxBufferInfo->glbRxBufferState.Lock = DCM_BUFFER_UNLOCKED;
                Glb_DslProtocolRunTimePtr->DCM_RxBufferInfo->glbRxBufferCurrentUse = DCM_NOT_IN_USE;
            }
            else
            {
                /* Check if the Protocol has been Started After Request Reception */
                if (TRUE == Glb_DslProtocolRunTimePtr->DslProtocolHandling->protocolStarted)
                {
                  /* Provide Both Rx and Tx Buffers to Dsd */
                  Glb_DslProtocolRunTimePtr->DCM_RxBufferInfo->glbRxBufferCurrentUse = DCM_PROVIDED_TO_DSD;
                  Glb_DslProtocolRunTimePtr->DCM_TxBufferInfo->glbTxBufferCurrentUse = DCM_PROVIDED_TO_DSD;

                  /* Initiate the Maximum Number of Pending Respond can be sent */
                  responsePendingCounter = Dsl_DiagRespMaxNumRespPend;
                  Glb_DslProtocolRunTimePtr->DslProtocolHandling->resTimeoutCount = DCM_CONVERT_MS_TO_MAIN_CYCLES(DCM_CONVERT_SEC_TO_MS(Glb_DslProtocolRunTimePtr->DslSessionHandling->sessionRow.DspSessionP2ServerMax));
                  Glb_DslProtocolRunTimePtr->DCM_RxBufferInfo->diagReqestRxPduId = id;

                  /* Forwarding Data to Dsd as referred in [SWS_Dcm_00111], [SWS_Dcm_00035] */
                    DslToDsdDataIndication(Glb_ProtocolConnectionRow);
                }
                else
                {
                  /*MISRA RULE*/
                }

            }

        }
        else
        {
            /*MISRA RULE*/
        }

    }

        /* In case of Dcm_TpRxIndication result is different from "E_OK" the timer has to be reseted
            and the Dcm module shall not evaluate the buffer as referred in [SWS_Dcm_00344] */
    else
    {
          /* Reset S3Server Timeout Timer if there is an error
             Occurred during reception as referred in [SWS_Dcm_00141] */
        if( DCM_DEFAULT_SESSION != Glb_DslProtocolRunTimePtr->DslSessionHandling->sessionRow.DspSessionLevel)
        {
              SetS3ServerSessionTimeoutTimer();
        }

        /* Release Rx Buffer to be able to Receive next requests */
        Glb_DslProtocolRunTimePtr->DCM_RxBufferInfo->glbRxBufferState.Lock = DCM_BUFFER_UNLOCKED;
        Glb_DslProtocolRunTimePtr->DCM_RxBufferInfo->glbRxBufferCurrentUse = DCM_NOT_IN_USE;
    }
}


/***************************************************************
 * GUARANTEE-TIMING-AND-HANDLE-RESPONSE-TRANSMITTING-PROCEDURE *
 ***************************************************************/
void DslMain(void)
{
      Std_ReturnType PduR_DcmTransmitResult;

              /* A Course of Actions have to take place in Case of active Session Level is non-default */
    if ((DCM_DEFAULT_SESSION != Glb_DslProtocolRunTimePtr->DslSessionHandling->sessionRow.DspSessionLevel)&&//
            (TRUE == Glb_DslProtocolRunTimePtr->DslSessionHandling->S3ServerStarted))
    {
            /* Decrement Tester Present Timer if It is Requested */
            DECREMENT_TIMER_COUNTS(Glb_DslProtocolRunTimePtr->DslSessionHandling->S3ServerTimeoutCount);

            if (0 == Glb_DslProtocolRunTimePtr->DslSessionHandling->S3ServerTimeoutCount)
            {
                /* If a non-default session is active and S3Server Reaches Timeout, the Diagnostic
                    Session Level Shall reset to Default Session According to [SWS_Dcm_00140] */

                Dcm_ResetToDefaultSession();
                Glb_DslProtocolRunTimePtr->DslProtocolHandling->protocolStarted = FALSE;
            }
            else
            {
                /*MISRA RULE*/
            }
    }
    else
    {
            /*MISRA RULE*/
    }

      /* A Course of Actions shall be taken Based on TxBuffer Status */
      switch (Glb_DslProtocolRunTimePtr->DCM_TxBufferInfo->glbTxBufferCurrentUse)
        {
        case DCM_PROVIDED_TO_DSD:

        /* Decrement Response Timeouts Timer */
        DECREMENT_TIMER_COUNTS(Glb_DslProtocolRunTimePtr->DslProtocolHandling->resTimeoutCount);

         if (0 == Glb_DslProtocolRunTimePtr->DslProtocolHandling->resTimeoutCount)
         {
                /* if the timer of the response timeout expired, it is Re-initialized
                    by extra time till response be ready as referred in 9.2.2 */
            Glb_DslProtocolRunTimePtr->DslProtocolHandling->resTimeoutCount = DCM_CONVERT_MS_TO_MAIN_CYCLES(DCM_CONVERT_SEC_TO_MS(Glb_DslProtocolRunTimePtr->DslSessionHandling->sessionRow.DspSessionP2StarServerMax));

                /* As long as Request Processing Time DOES NOT reach Maximum Configured
                    Number of Pending Responses Allowed According to [SWS_Dcm_00024] */
            if (0 != responsePendingCounter)
            {
                  /* Send  NRC 0x78 "Response Pending" */
                       DslSendNRC(DCM_RESPONSE_PENDING);

                   /* Decrement one count of allowed number of response pending */
                 DECREMENT_TIMER_COUNTS(responsePendingCounter);
            }
            else
            {
                #if (DCM_DEV_ERROR_DETECT == TRUE)

                 Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_INIT_SID, DCM_E_INTERFACE_TIMEOUT);

                #endif

                 /* if the Request Processing time reaches DcmDslDiagRespMaxNumRespPend, the Dcm module shall stop processing
                the active diagnostic request and send NRC 0x10 General reject According to [SWS_Dcm_00120] */
                      DslSendNRC(DCM_GENERAL_REJECT);

                 /* Cancel Pending status */
                     DspInternal_CancelPendingRequest();

                 /* Release Dcm Both Tx and Rx Buffers for next Tester Request */
                      ReleaseRxTxBuffers();
            }

         }
         else
         {
             /*MISRA RULE*/
         }
      break;

            /* This Status Indicates that the Response is Evaluated and Ready to be sent to PduR */
        case DCM_DSD_PENDING_RESPONSE_SIGNALED:
            /* The Tx Buffer enters (DCM_TRANSMIT_SIGNALED) Status as it Transmitting */
            Glb_DslProtocolRunTimePtr->DCM_TxBufferInfo->glbTxBufferCurrentUse = DCM_TRANSMIT_SIGNALED;

            /* The function to Trigger PduR to call Dcm_CopyTxData to receive Response data According to [SWS_Dcm_00237] */
            PduR_DcmTransmitResult = PduR_DcmTransmit(Glb_DslProtocolRunTimePtr->DCM_TxBufferInfo->diagResponseTxPduId, &(Glb_DslProtocolRunTimePtr->DCM_TxBufferInfo->dcmTxBuffer));


          if (E_OK != PduR_DcmTransmitResult)
          {
              /* Release Dcm Both Tx and Rx Buffers for next Tester Request */
              ReleaseRxTxBuffers();
          }
          else
          {
              /* MISRA RULE */
          }

      break;

      default:

          /* DslMain Function Doesn't Take any Course of Action in case of the "glbTxBufferCurrentUse" has
                any status of (DCM_NOT_IN_USE), (DCM_IN_USE), (DCM_PROVIDED_TO_PDUR) or (DCM_TRANSMIT_SIGNALED) */
      break;
        }
    }
