/******************************************************************************
 * Module      : DCM                                                          *
 * File Name   : Dcm.c                                                        *
 * Description : Source file of Dcm Module                                    *
 * Created on  : April 6, 2020                                                *
 ******************************************************************************/


#include "include/Dcm.h"
#include "include/Dcm_Cbk.h"


uint8 Dcm_Status = DCM_NOT_INITIALIZED;

/*************************************************************************
 * Service Name      : Dcm_Init                                          *
 * Service ID[hex]   : 0x01                                              *
 * Sync/Async        : Synchronous                                       *
 * Reentrancy        : Non Reentrant                                     *
 * Parameters(in)    : ConfigPtr                                         *
 * Parameters(inOut) : None                                              *
 * Parameters(out)   : None                                              *
 * Return value      : None                                              *
 * Description       : Service for basic initialization of DCM module.   *
 *************************************************************************/
void Dcm_Init(const Dcm_ConfigType* ConfigPtr)
{
#if(DCM_DEV_ERROR_DETECT == TRUE)
    if (NULL == ConfigPtr)
    {
        Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID,DCM_INIT_SID,DCM_E_PARAM_CONFIG);
    }
    else
    {
        /* MISRA RULE */
    }

#endif
  if (DCM_NOT_INITIALIZED ==  Dcm_Status)
  {
    Dcm_Status =  DCM_INITIALIZED;
    Dsl_Init(ConfigPtr);
    Dsp_Init(ConfigPtr);
  }
  else
  {
      /* MISRA RULE */
  }
}


/*************************************************************************
 * Service Name      : Dcm_StartOfReception                              *
 * Service ID[hex]   : 0x46                                              *
 * Sync/Async        : Synchronous                                       *
 * Reentrancy        : Reentrant                                         *
 * Parameters(in)    : 1- id                                             *
 *                     2- info                                           *
 *                     3- TpSduLength                                    *
 * Parameters(inOut) : None                                              *
 * Parameters(out)   : bufferSizePtr                                     *
 * Return value      : BufReq_ReturnType                                 *
 * Description       : This Interface is called at the start of          *
 *                     receiving an N-SDU. The N-SDU might be fragmented *
 *                     intomultipleN-PDUs(FFwithoneormorefollowing CFs)  *
 *                     or might consist of a single N-PDU (SF). The      *
 *                     service shall provide the currently available     *
 *                     maximum buffer size when invoked with TpSduLength *
 *                     equal to 0.                                       *
 *************************************************************************/
BufReq_ReturnType Dcm_StartOfReception(PduIdType id, const PduInfoType* info,
                                       PduLengthType TpSduLength, PduLengthType* bufferSizePtr)
{
    BufReq_ReturnType result;
#if (DCM_DEV_ERROR_DETECT == TRUE)

    if (Dcm_Status == DCM_NOT_INITIALIZED)
    {
        Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_START_OF_RECEPTION_SID, DCM_E_UNINIT);
    }
    else
    {
        /* MISRA RULE */
    }


    if (NULL == info)
    {
        Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_START_OF_RECEPTION_SID, DCM_E_PARAM_POINTER);
    }
    else
    {
        /* MISRA RULE */
    }
    if (DCM_DSL_UDS_MAIN_BUFFER_SIZE == TpSduLength)
    {
        Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_START_OF_RECEPTION_SID, DCM_E_INTERFACE_BUFFER_OVERFLOW);
    }
    else
    {
        /* MISRA RULE */
    }


#endif


    result = AcceptNewConnection(id, info, TpSduLength, bufferSizePtr);

    return result;
}



/*************************************************************************
 * Service Name      : Dcm_CopyRxData                                    *
 * Service ID[hex]   : 0x44                                              *
 * Sync/Async        : Synchronous                                       *
 * Reentrancy        : Reentrant                                         *
 * Parameters(in)    : 1- id                                             *
 *                     2- info                                           *
 * Parameters(inOut) : None                                              *
 * Parameters(out)   : bufferSizePtr                                     *
 * Return value      : BufReq_ReturnType                                 *
 * Description       : This Interface is called to provide the received  *
 *                     data of an I-PDU segment (N-PDU) to the upper     *
 *                     layer. Each call to this function provides the    *
 *                     next part of the I-PDU data. The size of the      *
 *                     remaining buffer is written to the position       *
 *                     indicated by bufferSizePtr.                       *
 *************************************************************************/
 BufReq_ReturnType Dcm_CopyRxData(PduIdType id, const PduInfoType* info,
                                  PduLengthType* bufferSizePtr )
{
     BufReq_ReturnType result;
#if (DCM_DEV_ERROR_DETECT == TRUE)

    if (Dcm_Status == DCM_NOT_INITIALIZED)
    {
        Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_COPY_RX_DATA_SID, DCM_E_UNINIT);
    }
    else
    {
        /* MISRA RULE */
    }


    if (NULL == info)
    {
        Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_COPY_RX_DATA_SID, DCM_E_PARAM_POINTER);
    }
    else
    {
        /* MISRA RULE */
    }

#endif

  result = CopyRequestData(id, info, bufferSizePtr);

  return result;
}



/*************************************************************************
 * Service Name      : Dcm_TpRxIndication                                *
 * Service ID[hex]   : 0x45                                              *
 * Sync/Async        : Synchronous                                       *
 * Reentrancy        : Reentrant                                         *
 * Parameters(in)    : 1- id                                             *
 *                     2- result                                         *
 * Parameters(inOut) : None                                              *
 * Parameters(out)   : None                                              *
 * Return value      : None                                              *
 * Description       : Called after an I-PDU has been received via the   *
 *                     TP API, the result indicates whether the          *
 *                     transmission was successful or not.               *
 *************************************************************************/
void Dcm_TpRxIndication (PduIdType id, Std_ReturnType result)
{
#if (DCM_DEV_ERROR_DETECT == TRUE)

    if (Dcm_Status == DCM_NOT_INITIALIZED)
    {
        Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_TP_RX_INDICATION_SID, DCM_E_UNINIT);
    }
    else
    {
        /* MISRA RULE */
    }


#endif


    Dcm_PduRToDslIndication(id, result);
}




/*************************************************************************
 * Service Name      : Dcm_CopyRxData                                    *
 * Service ID[hex]   : 0x43                                              *
 * Sync/Async        : Synchronous                                       *
 * Reentrancy        : Reentrant                                         *
 * Parameters(in)    : 1- id                                             *
 *                     2- info                                           *
 *                     3- retry                                          *
 * Parameters(inOut) : None                                              *
 * Parameters(out)   : availableDataPtr                                  *
 * Return value      : BufReq_ReturnType                                 *
 * Description       : This Interface is called to acquire the transmit  *
 *                     data of an I-PDU segment (N-PDU). Each call to    *
 *                     this function provides the next part of the I-PDU *
 *                     data unless retry->TpDataState is TP_DATARETRY.   *
 *                     In this case the function restarts to copy the    *
 *                     data beginning at the offset from the current     *
 *                     position indicated by retry->TxTpDataCnt. The     *
 *                     size of the remaining data is written to the      *
 *                     position indicated by availableDataPtr.           *
 *************************************************************************/

BufReq_ReturnType Dcm_CopyTxData(PduIdType id, const PduInfoType* info,
                                 const RetryInfoType* retry, PduLengthType* availableDataPtr)
{
    BufReq_ReturnType result;
#if (DCM_DEV_ERROR_DETECT == TRUE)

    if (Dcm_Status == DCM_NOT_INITIALIZED)
    {
        Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_COPY_TX_DATA_SID, DCM_E_UNINIT);
    }
    else
    {
        /* MISRA RULE */
    }


    if (NULL == info)
    {
        Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_COPY_TX_DATA_SID, DCM_E_PARAM_POINTER);
    }
    else
    {
        /* MISRA RULE */
    }

#endif



    result = Dcm_CopyTransmitData(id, info, retry, availableDataPtr);

    return result;

}


/*************************************************************************
 * Service Name      : Dcm_TxConfirmation                                *
 * Service ID[hex]   : 0x40                                              *
 * Sync/Async        : Synchronous                                       *
 * Reentrancy        : Reentrant                                         *
 * Parameters(in)    : 1- TxPduId                                        *
 *                     2- result                                         *
 * Parameters(inOut) : None                                              *
 * Parameters(out)   : None                                              *
 * Return value      : None                                              *
 * Description       : This Interface is called after the I-PDU has been *
 *                     transmitted on its network, the result indicates  *
 *                     whether the transmission was successful or not.   *
 *************************************************************************/
void Dcm_TpTxConfirmation(PduIdType TxPduId,Std_ReturnType result)
{

#if (DCM_DEV_ERROR_DETECT == TRUE)

    if (Dcm_Status == DCM_NOT_INITIALIZED)
    {
       Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_TP_TX_CONFIRMATION_SID, DCM_E_UNINIT);
    }
    else
    {
        /* MISRA RULE */
    }


#endif

    Dsl_TpTxConfirmation(TxPduId, result);
}



/*************************************************************************
 * Service Name      : Dcm_MainFunction                                  *
 * Service ID[hex]   : 0x25                                              *
 * Type              : Scheduled                                         *
 * Description       : This service is used for processing the tasks of  *
 *                     the main loop.                                    *
 *************************************************************************/
void Dcm_MainFunction(void)
{
#if (DCM_DEV_ERROR_DETECT == TRUE)

    if (Dcm_Status == DCM_NOT_INITIALIZED)
    {
       Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_MAIN_FUNCTION_SID, DCM_E_UNINIT);
    }
    else
    {
        /* MISRA RULE */
    }


#endif

    DsdMain();
    DspMain();
    DslMain();
}



/**********************
 * GET-SECURITY-LEVEL *
 **********************/
Std_ReturnType Dcm_GetSecurityLevel( Dcm_SecLevelType* SecLevel )
{
    Std_ReturnType result;

    result = Dsl_GetSecurityLevel(SecLevel);

    return result;
}


