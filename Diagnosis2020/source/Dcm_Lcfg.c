/******************************************************************************
 * Module      : DCM                                                          *
 * SubModule   : Used in DSL,DSD and DSP                                      *
 * File Name   : Lcfg.c                                                       *
 * Description : Source file of Link Time Configuration Parameters            *
 * Created on  : Mar 29, 2020                                                 *
 ******************************************************************************/


#include "include/Dcm_Lcfg.h"
#include "include/Dcm_Dsp.h"


/*****************************************************************************
 *                       DSP Used Parameters Configuration                   *
 *****************************************************************************/


                        /* DSP--UDS--PROTOCOL--CONFIGURATION */
/**************************************************
 * DSP--UDS--SECURITY-ROWS-CONFIGURATION          *
 **************************************************/


Dcm_DspSecurityRowType DspSecurityRow_Lev1 =
{
  NULL,
  FALSE,
  DCM_DSP_SECURITY_LEV_1_DELAY_TIME,
  DCM_DSP_SECURITY_LEV_1_COMPARE_KEY_FNC,
  DCM_DSP_SECURITY_LEV_1_DELAY_TIME_ON_BOOT,
  Dcm_DSP_SECURITY_LEV_1_GET_ATTEMPT_COUNTER_FNC,
  DCM_DSP_SECURITY_LEV_1_GET_ADR_SIZE_SEED_FNC,
  DCM_DSP_SECURITY_LEV_1_GET_SEED_FNC,
  DCM_DSP_SECURITY_LEV_1_KEY_SIZE,
  DCM_SEC_LEV_1,
  DCM_DSP_SECURITY_LEV_1_NUM_ATT_DELAY,
  DCM_DSP_SECURITY_LEV_1_SEED_SIZE,
  DCM_DSP_SECURITY_LEV_1_SET_ATTEMPT_COUNTER_FNC,
  DCM_DSP_SECURITY_LEV_1_USE_PORT
};

Dcm_DspSecurityRowType DspSecurityRow_Lev2 =
{
  NULL,
  FALSE,
  DCM_DSP_SECURITY_LEV_2_DELAY_TIME,
  DCM_DSP_SECURITY_LEV_2_COMPARE_KEY_FNC,
  DCM_DSP_SECURITY_LEV_2_DELAY_TIME_ON_BOOT,
  Dcm_DSP_SECURITY_LEV_2_GET_ATTEMPT_COUNTER_FNC,
  DCM_DSP_SECURITY_LEV_2_GET_ADR_SIZE_SEED_FNC,
  DCM_DSP_SECURITY_LEV_2_GET_SEED_FNC,
  DCM_DSP_SECURITY_LEV_2_KEY_SIZE,
  DCM_SEC_LEV_2,
  DCM_DSP_SECURITY_LEV_2_NUM_ATT_DELAY,
  DCM_DSP_SECURITY_LEV_2_SEED_SIZE,
  DCM_DSP_SECURITY_LEV_2_SET_ATTEMPT_COUNTER_FNC,
  DCM_DSP_SECURITY_LEV_2_USE_PORT
};

const Dcm_DspSecurityRowType *DspSecurityRows[] =
{
 &DspSecurityRow_Lev1,
 &DspSecurityRow_Lev2
};

Dcm_DspSecurityType UDSDspSecurity =
{
   DspSecurityRows
};



/*
 * DSP--UDS--SERVICES-SECURITY-ROWS-CONFIGURATION
 */


    /* Security Row Ref for UDS_Read_Data_By_ID Service */
const Dcm_DspSecurityRowType *UDS_0x22_SecurityRowRef[] =
{
 &DspSecurityRow_Lev1,
 &DspSecurityRow_Lev2
};

    /* Security Row Ref for UDS_Write_Data_By_ID Service */
const Dcm_DspSecurityRowType *UDS_0x2E_SecurityRowRef[] =
{
 &DspSecurityRow_Lev1,
 &DspSecurityRow_Lev2
};


/*
 * DSP--UDS--READ-Data-Identifiers-SECURITY-ROWS-CONFIGURATION
 */

const Dcm_DspSecurityRowType *UDS_VINNum_DidReadSecurityRowRef[] =
{
 &DspSecurityRow_Lev2
};

const Dcm_DspSecurityRowType *UDS_VINNum_DidWriteSecurityRowRef[] =
{
 &DspSecurityRow_Lev2
};

const Dcm_DspSecurityRowType *UDS_0x010A_DidReadSecurityRowRef[] =
{
 &DspSecurityRow_Lev2
};

const Dcm_DspSecurityRowType *UDS_0x010A_DidWriteSecurityRowRef[] =
{
 &DspSecurityRow_Lev2
};


/**********************
 * UDS_SUPPORTED RIDS *
 **********************/

/*****************
 * CHECK-MEMORY  *
 *****************/

const Dcm_DspSecurityRowType *UDS_CheckMemory_RDidAuthorizationSecurityRowRef[] =
{
 &DspSecurityRow_Lev2
};



/****************************************
 * DSP--UDS--SESSION-ROWS-CONFIGURATION *
 ****************************************/


Dcm_DspSessionRowType UDS_Dsp_DefaultSessionRow =
{
 DCM_DEFAULT_SESSION,
 DCM_DSP_UDS_DEFAULT_SESSION_P2_SERVER_MAX,
 DCM_DSP_UDS_DEFAULT_SESSION_P2_STAR_SERVER_MAX
};

Dcm_DspSessionRowType UDS_Dsp_ExtendedSessionRow =
{
 DCM_EXTENDED_DIAGNOSTIC_SESSION,
 DCM_DSP_UDS_EXTENDED_SESSION_P2_SERVER_MAX,
 DCM_DSP_UDS_EXTENDED_SESSION_P2_STAR_SERVER_MAX
};


const Dcm_DspSessionRowType *UDS_DspSessionRows[] =
{
 &UDS_Dsp_DefaultSessionRow,
 &UDS_Dsp_ExtendedSessionRow
};

Dcm_DspSessionType UDS_DspSession =
{
  UDS_DspSessionRows
};


/***********************************************
 * DSP-UDS-SERVICES-SESSION-ROWS-CONFIGURATION *
 ***********************************************/

const Dcm_DspSessionRowType *UDS_0x27_SessionRowRef[] =
{
  &UDS_Dsp_DefaultSessionRow,
  &UDS_Dsp_ExtendedSessionRow
};



const Dcm_DspSessionRowType *UDS_0x3E_SessionRowRef[] =
{
 &UDS_Dsp_ExtendedSessionRow
};


const Dcm_DspSessionRowType *UDS_0x22_SessionRowRef[] =
{
 &UDS_Dsp_ExtendedSessionRow
};

const Dcm_DspSessionRowType *UDS_0x2E_SessionRowRef[] =
{
 &UDS_Dsp_ExtendedSessionRow
};

const Dcm_DspSessionRowType *UDS_0x31_SessionRowRef[] =
{
 &UDS_Dsp_ExtendedSessionRow
};

/***************************************************************
 * DSP--UDS--READ-Data-Identifiers-SESSION-ROWS-CONFIGURATION *
 ***************************************************************/

const Dcm_DspSessionRowType *UDS_batteryPositiveVoltageDidRead_SessionRowRef[] =
{
 &UDS_Dsp_ExtendedSessionRow
};

const Dcm_DspSessionRowType *UDS_VINNum_DidRead_SessionRowRef[] =
{
 &UDS_Dsp_ExtendedSessionRow
};

const Dcm_DspSessionRowType *UDS_0x010A_DidRead_SessionRowRef[] =
{
 &UDS_Dsp_ExtendedSessionRow
};


/***************************************************************
 * DSP--UDS--Write-Data-Identifiers-SECURITY-ROWS-CONFIGURATION *
 ***************************************************************/

const Dcm_DspSessionRowType *UDS_batteryPositiveVoltageDidWrite_SessionRowRef[] =
{
 &UDS_Dsp_ExtendedSessionRow
};

const Dcm_DspSessionRowType *UDS_VINNum_DidWrite_SessionRowRef[] =
{
 &UDS_Dsp_ExtendedSessionRow
};

const Dcm_DspSessionRowType *UDS_0x010A_DidWrite_SessionRowRef[] =
{
 &UDS_Dsp_ExtendedSessionRow
};


/**************************************
 * UDS_SUPPORTED-RIDS-SESSION-LEVELS  *
 **************************************/


/*****************
 * CHECK-MEMORY  *
 *****************/

const Dcm_DspSessionRowType *UDS_CheckMemory_RDidAuthorizationSessionRowRef[] =
{
 &UDS_Dsp_ExtendedSessionRow
};





/**********************
 * UDS_SUPPORTED DIDS *
 **********************/


/****************************
 * BATTERY-POSITIVE-VOLTAGE *
 ****************************/

Dcm_DspDidReadType UDS_batteryPositiveVoltageDidRead =
{
 NULL,
 UDS_batteryPositiveVoltageDidRead_SessionRowRef,
 DCM_DSP_UDS_BATTERY_POSITIVE_VOLTAGE_DID_READ_NUM_OF_SUPPORTED_SESSIONS,
 DCM_DSP_UDS_BATTERY_POSITIVE_VOLTAGE_DID_READ_NUM_OF_SUPPORTED_SEC_LEV
};

Dcm_DspDidWriteType UDS_batteryPositiveVoltageDidWrite =
{
 NULL,
 UDS_batteryPositiveVoltageDidRead_SessionRowRef,
 DCM_DSP_UDS_BATTERY_POSITIVE_VOLTAGE_DID_WRITE_NUM_OF_SUPPORTED_SESSIONS,
 DCM_DSP_UDS_BATTERY_POSITIVE_VOLTAGE_DID_WRITE_NUM_OF_SUPPORTED_SEC_LEV
};

Dcm_DspDidInfoType UDS_batteryPositiveVoltageDidInfo =
{
  &UDS_batteryPositiveVoltageDidRead,
  &UDS_batteryPositiveVoltageDidWrite
 };

/*
 * Configuration parameters for Data signal
 */
Dcm_DspDataType UDS_batteryPositiveVoltageDidData =
{
  DCM_DSP_UDS_BATTERY_POSITIVE_VOLTAGE_SIGNAL_DID_DATA_BYTE_SIZE,
  DCM_DSP_UDS_BATTERY_POSITIVE_VOLTAGE_SIGNAL_DID_DATA_CONDITION_CHECK_READ_FNC_USED,
  DCM_DSP_UDS_BATTERY_POSITIVE_VOLTAGE_SIGANAL_DID_DATA_CONDITION_CHECK_READ_FNC,
  DCM_DSP_UDS_BATTERY_POSITIVE_VOLTAGE_SIGANAL_DID_DATA_CONDITION_CHECK_READ_ASYN_FNC,
  DCM_UDS_DSP_BATTERY_POSITIVE_VOLTAGE_SIGNAL_DID_DATA_READ_DATA_LENGTH_SYN_FNC,
  DCM_UDS_DSP_BATTERY_POSITIVE_VOLTAGE_SIGNAL_DID_DATA_READ_DATA_LENGTH_ASYN_FNC,
  DCM_UDS_DSP_BATTERY_POSITIVE_VOLTAGE_SIGNAL_DID_DATA_READ_DATA_SYN_FNC,
  DCM_UDS_DSP_BATTERY_POSITIVE_VOLTAGE_SIGNAL_DID_DATA_READ_DATA_ASYN_FNC,
  DCM_UDS_DSP_BATTERY_POSITIVE_VOLTAGE_SIGNAL_DID_DATA_READ_DATA_ASYN_ERROR_FNC,
  DCM_UDS_DSP_BATTERY_POSITIVE_VOLTAGE_SIGNAL_DID_DATA_WRITE_DATA_SYN_FNC,
  DCM_UDS_DSP_BATTERY_POSITIVE_VOLTAGE_SIGNAL_DID_DATA_WRITE_DATA_ASYN_FNC,
  DCM_UDS_DSP_BATTERY_POSITIVE_VOLTAGE_SIGNAL_DID_DATA_WRITE_DATA_SYN_DYN_FNC,
  DCM_UDS_DSP_BATTERY_POSITIVE_VOLTAGE_SIGNAL_DID_DATA_WRITE_DATA_ASYN_DYN_FNC,
  DCM_UDS_DSP_BATTERY_POSITIVE_VOLTAGE_SIGNAL_DID_DATA_USE_PORT,
  DCM_DSP_UDS_BATTERY_POSITIVE_VOLTAGE_SIGNAL_DID_DATA_TYPE
};


Dcm_DspDidSignalType UDS_batteryPositiveVoltageDidSignal_1 =
{
  DCM_DSP_UDS_BATTERY_POSITIVE_VOLTAGE_SIGANAL_DID_BYTE_OFFSET,
 &UDS_batteryPositiveVoltageDidData
};
const Dcm_DspDidSignalType *UDS_batteryPositiveVoltageDidSignals[] =
{
 DCM_DSP_UDS_BATTERY_POSITIVE_VOLTAGE_SIGANAL_DID_BYTE_OFFSET,
 &UDS_batteryPositiveVoltageDidSignal_1
};


Dcm_DspDidType UDS_batteryPositiveVoltageDid =
{
 DCM_BATTERY_POSITIVE_VOLTAGE_DID,
 DCM_DSP_UDS_BATTERY_POSITIVE_VOLTAGE_DID_USED,
&UDS_batteryPositiveVoltageDidInfo,
 NULL,
 UDS_batteryPositiveVoltageDidSignals,
 DCM_DSP_UDS_BATTERY_POSITIVE_VOLTAGE_DID_NUM_OF_CONFIGURED_SIGNALS
};

/**************
 * VIN-NUMBER *
 **************/
Dcm_DspDidReadType UDS_VINNum_DidRead =
{
 UDS_VINNum_DidReadSecurityRowRef,
 UDS_VINNum_DidRead_SessionRowRef,
 DCM_DSP_UDS_VIN_NUM_DID_READ_NUM_OF_SUPPORTED_SESSIONS,
 DCM_DSP_UDS_VIN_NUM_DID_READ_NUM_OF_SUPPORTED_SEC_LEV
};

Dcm_DspDidWriteType UDS_VINNum_DidWrite =
{
 UDS_VINNum_DidWriteSecurityRowRef,
 UDS_VINNum_DidWrite_SessionRowRef,
 DCM_DSP_UDS_VIN_NUM_DID_WRITE_NUM_OF_SUPPORTED_SESSIONS,
 DCM_DSP_UDS_VIN_NUM_DID_WRITE_NUM_OF_SUPPORTED_SEC_LEV
};

Dcm_DspDidInfoType UDS_VINNum_DidInfo =
{
 &UDS_VINNum_DidRead,
 &UDS_VINNum_DidWrite
};

/*
 * Configuration parameters for Data signal
 */
Dcm_DspDataType UDS_VINNum_DidData =
{
  DCM_DSP_UDS_VIN_NUM_SIGNAL_DID_DATA_BYTE_SIZE,
  DCM_DSP_UDS_VIN_NUM_SIGNAL_DID_DATA_CONDITION_CHECK_READ_FNC_USED,
  DCM_DSP_UDS_VIN_NUM_SIGNAL_DID_DATA_CONDITION_CHECK_READ_FNC,
  DCM_DSP_UDS_VIN_NUM_SIGNAL_DID_DATA_CONDITION_CHECK_READ_ASYN_FNC,
  DCM_UDS_DSP_VIN_NUM_SIGNAL_DID_DATA_READ_DATA_LENGTH_SYN_FNC,
  DCM_UDS_DSP_VIN_NUM_SIGNAL_DID_DATA_READ_DATA_LENGTH_ASYN_FNC,
  DCM_UDS_DSP_VIN_NUM_SIGNAL_DID_DATA_READ_DATA_SYN_FNC,
  DCM_UDS_DSP_VIN_NUM_SIGNAL_DID_DATA_READ_DATA_ASYN_FNC,
  DCM_UDS_DSP_VIN_NUM_SIGNAL_DID_DATA_READ_DATA_ASYN_ERROR_FNC,
  DCM_UDS_DSP_VIN_NUM_SIGNAL_DID_DATA_WRITE_DATA_SYN_FNC,
  DCM_UDS_DSP_VIN_NUM_SIGNAL_DID_DATA_WRITE_DATA_ASYN_FNC,
  DCM_UDS_DSP_VIN_NUM_SIGNAL_DID_DATA_WRITE_DATA_SYN_DYN_FNC,
  DCM_UDS_DSP_VIN_NUM_SIGNAL_DID_DATA_WRITE_DATA_ASYN_DYN_FNC,
  DCM_UDS_DSP_VIN_NUM_SIGNAL_DID_DATA_USE_PORT,
  DCM_DSP_UDS_VIN_NUM_SIGNAL_DID_DATA_TYPE
};

Dcm_DspDidSignalType UDS_VINNum_DidSignal_1 =
{
 DCM_DSP_UDS_VIN_NUM_SIGANAL_DID_BYTE_OFFSET,
&UDS_VINNum_DidData
};

const Dcm_DspDidSignalType *UDS_VINNum_DidSignals[] =
{
 &UDS_VINNum_DidSignal_1
};


Dcm_DspDidType UDS_VINNum_Did =
{
 DCM_VIN_NUM_DID,
 DCM_DSP_UDS_VIN_NUM_DID_USED,
 &UDS_VINNum_DidInfo,
 NULL,
 UDS_VINNum_DidSignals,
 DCM_DSP_UDS_VIN_NUM_DID_NUM_OF_CONFIGURED_SIGNALS
};



/************
 * 010A-DID *
 ************/

Dcm_DspDidReadType UDS_0x010A_DidRead =
{
 UDS_0x010A_DidReadSecurityRowRef,
 UDS_0x010A_DidRead_SessionRowRef,
 DCM_DSP_UDS_0X010A_DID_READ_NUM_OF_SUPPORTED_SESSIONS,
 DCM_DSP_UDS_0X010A_DID_READ_NUM_OF_SUPPORTED_SEC_LEV
};

Dcm_DspDidWriteType UDS_0x010A_DidWrite =
{
 UDS_0x010A_DidWriteSecurityRowRef,
 UDS_0x010A_DidWrite_SessionRowRef,
 DCM_DSP_UDS_0X010A_DID_WRITE_NUM_OF_SUPPORTED_SESSIONS,
 DCM_DSP_UDS_0X010A_DID_WRITE_NUM_OF_SUPPORTED_SEC_LEV
};

Dcm_DspDidInfoType UDS_0x010A_DidInfo =
{
 &UDS_0x010A_DidRead,
 &UDS_0x010A_DidWrite
};

/*
 * Configuration parameters for Data signal
 */

Dcm_DspDataType UDS_0x010A_Did_EngineCoolantTempData =
{
  DCM_DSP_UDS_0X010A_ENGINE_COOLANT_TEMP_SIGNAL_DID_DATA_BYTE_SIZE,
  DCM_DSP_UDS_0X010A_ENGINE_COOLANT_TEMP_SIGNAL_DID_DATA_CONDITION_CHECK_READ_FNC_USED,
  DCM_DSP_UDS_0X010A_ENGINE_COOLANT_TEMP_SIGNAL_DID_DATA_CONDITION_CHECK_READ_FNC,
  DCM_DSP_UDS_0X010A_ENGINE_COOLANT_TEMP_SIGNAL_DID_DATA_CONDITION_CHECK_READ_ASYN_FNC,
  DCM_UDS_DSP_0X010A_ENGINE_COOLANT_TEMP_SIGNAL_DID_DATA_READ_DATA_LENGTH_SYN_FNC,
  DCM_UDS_DSP_0X010A_ENGINE_COOLANT_TEMP_SIGNAL_DID_DATA_READ_DATA_LENGTH_ASYN_FNC,
  DCM_UDS_DSP_0X010A_ENGINE_COOLANT_TEMP_SIGNAL_DID_DATA_READ_DATA_SYN_FNC,
  DCM_UDS_DSP_0X010A_ENGINE_COOLANT_TEMP_SIGNAL_DID_DATA_READ_DATA_ASYN_FNC,
  DCM_UDS_DSP_0X010A_ENGINE_COOLANT_TEMP_SIGNAL_DID_DATA_READ_DATA_ASYN_ERROR_FNC,
  DCM_UDS_DSP_0X010A_ENGINE_COOLANT_TEMP_SIGNAL_DID_DATA_WRITE_DATA_SYN_FNC,
  DCM_UDS_DSP_0X010A_ENGINE_COOLANT_TEMP_SIGNAL_DID_DATA_WRITE_DATA_ASYN_FNC,
  DCM_UDS_DSP_0X010A_ENGINE_COOLANT_TEMP_SIGNAL_DID_DATA_WRITE_DATA_SYN_DYN_FNC,
  DCM_UDS_DSP_0X010A_ENGINE_COOLANT_TEMP_SIGNAL_DID_DATA_WRITE_DATA_ASYN_DYN_FNC,
  DCM_UDS_DSP_0X010A_ENGINE_COOLANT_TEMP_SIGNAL_DID_DATA_USE_PORT,
  DCM_DSP_UDS_0X010A_ENGINE_COOLANT_TEMP_SIGNAL_DID_DATA_TYPE
};


/*
 * Configuration parameters for Data signal
 */
Dcm_DspDataType UDS_0x010A_Did_ThrottlePositionData =
{
 DCM_DSP_UDS_0X010A_THROTTLE_POSITION_SIGNAL_DID_DATA_BYTE_SIZE,
 DCM_DSP_UDS_0X010A_THROTTLE_POSITION_SIGNAL_DID_DATA_CONDITION_CHECK_READ_FNC_USED,
 DCM_DSP_UDS_0X010A_THROTTLE_POSITION_SIGNAL_DID_DATA_CONDITION_CHECK_READ_FNC,
 DCM_DSP_UDS_0X010A_THROTTLE_POSITION_SIGNAL_DID_DATA_CONDITION_CHECK_READ_ASYN_FNC,
 DCM_UDS_DSP_0X010A_THROTTLE_POSITION_SIGNAL_DID_DATA_READ_DATA_LENGTH_SYN_FNC,
 DCM_UDS_DSP_0X010A_THROTTLE_POSITION_SIGNAL_DID_DATA_READ_DATA_LENGTH_ASYN_FNC,
 DCM_UDS_DSP_0X010A_THROTTLE_POSITION_SIGNAL_DID_DATA_READ_DATA_SYN_FNC,
 DCM_UDS_DSP_0X010A_THROTTLE_POSITION_SIGNAL_DID_DATA_READ_DATA_ASYN_FNC,
 DCM_UDS_DSP_0X010A_THROTTLE_POSITION_SIGNAL_DID_DATA_READ_DATA_ASYN_ERROR_FNC,
 DCM_UDS_DSP_0X010A_THROTTLE_POSITION_SIGNAL_DID_DATA_WRITE_DATA_SYN_FNC,
 DCM_UDS_DSP_0X010A_THROTTLE_POSITION_SIGNAL_DID_DATA_WRITE_DATA_ASYN_FNC,
 DCM_UDS_DSP_0X010A_THROTTLE_POSITION_SIGNAL_DID_DATA_WRITE_DATA_SYN_DYN_FNC,
 DCM_UDS_DSP_0X010A_THROTTLE_POSITION_SIGNAL_DID_DATA_WRITE_DATA_ASYN_DYN_FNC,
 DCM_UDS_DSP_0X010A_THROTTLE_POSITION_SIGNAL_DID_DATA_USE_PORT,
 DCM_DSP_UDS_0X010A_THROTTLE_POSITION_SIGNAL_DID_DATA_TYPE
};


/*
 * Configuration parameters for Data signal
 */
Dcm_DspDataType UDS_0x010A_Did_EngineSpeedData =
{
 DCM_DSP_UDS_0X010A_ENGINE_SPEED_SIGNAL_DID_DATA_BYTE_SIZE,
 DCM_DSP_UDS_0X010A_ENGINE_SPEED_SIGNAL_DID_DATA_CONDITION_CHECK_READ_FNC_USED,
 DCM_DSP_UDS_0X010A_ENGINE_SPEED_SIGNAL_DID_DATA_CONDITION_CHECK_READ_FNC,
 DCM_DSP_UDS_0X010A_ENGINE_SPEED_SIGNAL_DID_DATA_CONDITION_CHECK_READ_ASYN_FNC,
 DCM_UDS_DSP_0X010A_ENGINE_SPEED_SIGNAL_DID_DATA_READ_DATA_LENGTH_SYN_FNC,
 DCM_UDS_DSP_0X010A_ENGINE_SPEED_SIGNAL_DID_DATA_READ_DATA_LENGTH_ASYN_FNC,
 DCM_UDS_DSP_0X010A_ENGINE_SPEED_SIGNAL_DID_DATA_READ_DATA_SYN_FNC,
 DCM_UDS_DSP_0X010A_ENGINE_SPEED_SIGNAL_DID_DATA_READ_DATA_ASYN_FNC,
 DCM_UDS_DSP_0X010A_ENGINE_SPEED_SIGNAL_DID_DATA_READ_DATA_ASYN_ERROR_FNC,
 DCM_UDS_DSP_0X010A_ENGINE_SPEED_SIGNAL_DID_DATA_WRITE_DATA_SYN_FNC,
 DCM_UDS_DSP_0X010A_ENGINE_SPEED_SIGNAL_DID_DATA_WRITE_DATA_ASYN_FNC,
 DCM_UDS_DSP_0X010A_ENGINE_SPEED_SIGNAL_DID_DATA_WRITE_DATA_SYN_DYN_FNC,
 DCM_UDS_DSP_0X010A_ENGINE_SPEED_SIGNAL_DID_DATA_WRITE_DATA_ASYN_DYN_FNC,
 DCM_UDS_DSP_0X010A_ENGINE_SPEED_SIGNAL_DID_DATA_USE_PORT,
 DCM_DSP_UDS_0X010A_ENGINE_SPEED_SIGNAL_DID_DATA_TYPE
};


Dcm_DspDidSignalType UDS_0x010A_Did_EngineCoolantTempSignal =
{
 DCM_DSP_UDS_0X010A_ENGINE_COOLANT_TEMP_SIGANAL_DID_BYTE_OFFSET,
&UDS_0x010A_Did_EngineCoolantTempData
};

Dcm_DspDidSignalType UDS_0x010A_Did_ThrottlePositionSignal =
{
 DCM_DSP_UDS_0X010A_THROTTLE_POSITION_SIGANAL_DID_BYTE_OFFSET,
&UDS_0x010A_Did_ThrottlePositionData
};

Dcm_DspDidSignalType UDS_0x010A_Did_EngineSpeedSignal =
{
 DCM_DSP_UDS_0X010A_ENGINE_SPEED_SIGANAL_DID_BYTE_OFFSET,
&UDS_0x010A_Did_EngineSpeedData
};

const Dcm_DspDidSignalType *UDS_0x010A_DidSignals[] =
{
 &UDS_0x010A_Did_EngineCoolantTempSignal,
 &UDS_0x010A_Did_ThrottlePositionSignal,
 &UDS_0x010A_Did_EngineSpeedSignal
};


Dcm_DspDidType UDS_0x010A_Did =
{
 0x010A,
 DCM_DSP_UDS_0X010A_DID_USED,
 &UDS_0x010A_DidInfo,
 NULL,
 UDS_0x010A_DidSignals,
 DCM_DSP_UDS_0X010A_DID_NUM_OF_CONFIGURED_SIGNALS
};


/******************************
 * LIST-OF-SUPPORTED-UDS-DIDS *
 ******************************/

const Dcm_DspDidType *supportedDids[] =
{
 &UDS_batteryPositiveVoltageDid,
 &UDS_VINNum_Did,
 &UDS_0x010A_Did
};

/**********************
 * UDS_SUPPORTED RIDS *
 **********************/

/*****************
 * CHECK-MEMORY  *
 *****************/

const Dcm_DspCommonAuthorizationType  Dcm_DspCheckMemoryRoutineCommonAuthorization =
{
 NULL,
 NULL,
 DCM_DSP_UDS_CHECK_MEMORY_NUM_RID_NUM_OF_SUPPORTED_SESSIONS,
 DCM_DSP_UDS_CHECK_MEMORY_RID_NUM_OF_SUPPORTED_SEC_LEV
};

const Dcm_DspStartRoutineType   Dcm_DspCheckMemoryStartRoutine =
{
 DCM_DSP_UDS_START_CHECK_MEMORY_ROUTINE_FNC,
 &Dcm_DspCheckMemoryRoutineCommonAuthorization
};


const Dcm_DspStopRoutineType   Dcm_DspCheckMemoryStopRoutine =
{
 DCM_DSP_UDS_STOP_CHECK_MEMORY_ROUTINE_FNC,
 &Dcm_DspCheckMemoryRoutineCommonAuthorization
};

const Dcm_DspRequestRoutineResultsType  Dcm_DspCheckMemoryRequestRoutineResult =
{
 DCM_DSP_UDS_REQUEST_CHECK_MEMORY_RESULTS_ROUTINE_FNC,
 &Dcm_DspCheckMemoryRoutineCommonAuthorization
};

const Dcm_DspRoutineType  Dcm_DspCheckMemoryRoutine =
{
 DCM_UDS_CHECK_MEMORY_RID,
 DCM_DSP_UDS_CHECK_MEMORY_ROUTINE_ROUTINE_USED,
 &Dcm_DspCheckMemoryStartRoutine,
 &Dcm_DspCheckMemoryStopRoutine,
 &Dcm_DspCheckMemoryRequestRoutineResult
};

/******************************
 * LIST-OF-SUPPORTED-UDS-RIDS *
 ******************************/

const Dcm_DspRoutineType *supportedRids[] =
{
 &Dcm_DspCheckMemoryRoutine
};






/*************************
 * DSP-UDS-CONFIGURATION *
 *************************/

const Dcm_DspType Dsp =
{
  &UDS_DspSession,
  &UDSDspSecurity,
  supportedDids,
  supportedRids,
  NULL
};


/*************************************
 * DSD Used Parameters Configuration *
 *************************************/


/********************************************
 * DSD--UDS_ON_CAN--SERVICES--CONFIGURATION *
 ********************************************/


/*
 * Configuration parameter for tester present
 */

Dcm_DsdServiceType UDS_TesterPresent_0x3E =
{
  DCM_DSD_UDS_TESTER_PRESENT_0X3E_0X01_SERVICE_USED,
  DCM_TESTER_PRESENT_SID,
  DCM_DSD_UDS_TESTER_PRESENT_0X3E_0X01_Sub_func_Avail,
  UDS_0x3E_SessionRowRef,
  NULL,
  DCM_DSD_0X3E_0X01_SUB_SERVICE,
  DCM_DSD_0X3E_0X01_EXTERNAL_FNC,
  DCM_DSD_0X3E_0X01_INTERNAL_FNC,
  DCM_DSD_UDS_TESTER_PRESENT_0X3E_0X01_NUM_OF_SUPPORTED_SESSIONS,
  DCM_DSD_UDS_TESTER_PRESENT_0X3E_0X01_NUM_OF_SUPPORTED_SEC_LEV
};

/*
 * Configuration parameter for security access
 */

Dcm_DsdServiceType UDS_SecurityAccess_0x27 =
{
  DCM_DSD_UDS_SECURITY_ACCESS_0X27_0X01_SERVICE_USED,
  DCM_SECURITY_ACCESS_SID,
  DCM_DSD_UDS_SECURITY_ACCESS_0X27_0X01_Sub_func_Avail,
  UDS_0x27_SessionRowRef,
  NULL,
  DCM_DSD_0X27_0X01_SUB_SERVICE,
  DCM_DSD_0X27_0X01_EXTERNAL_FNC,
  DCM_DSD_0X27_0X01_INTERNAL_FNC,
  DCM_DSD_UDS_SECURITY_ACCESS_0X27_0X01_NUM_OF_SUPPORTED_SESSIONS,
  DCM_DSD_UDS_SECURITY_ACCESS_0X27_0X01_NUM_OF_SUPPORTED_SEC_LEV
};

/*
 * Configuration parameter for session control
 */
Dcm_DsdServiceType UDS_SessionControl_0x10 =
{
  DCM_DSD_UDS_SESSION_CONTROL_0X10_0X01_SERVICE_USED,
  DCM_SESSION_CONTROL_SID,
  DCM_DSD_UDS_SESSION_CONTROL_0X10_0X01_Sub_func_Avail,
  NULL,
  NULL,
  DCM_DSD_0X10_0X01_SUB_SERVICE,
  DCM_DSD_0X10_0X01_EXTERNAL_FNC,
  DCM_DSD_0X10_0X01_INTERNAL_FNC,
  DCM_DSD_UDS_SESSION_CONTROL_0X10_0X01_NUM_OF_SUPPORTED_SESSIONS,
  DCM_DSD_UDS_SESSION_CONTROL_0X10_0X01_NUM_OF_SUPPORTED_SEC_LEV
 };

/*
 * Configuration parameter for read data by identifier
 */
Dcm_DsdServiceType UDS_ReadDataById_0x22 =
{
  DCM_DSD_UDS_READ_DATA_BY_ID_0X22_0X01_SERVICE_USED,
  DCM_READ_DATAT_BY_ID_SID,
  DCM_DSD_UDS_READ_DATA_BY_ID_0X22_0X01_Sub_func_Avail,
  UDS_0x22_SessionRowRef,
  UDS_0x22_SecurityRowRef,
  DCM_DSD_0X22_0X01_SUB_SERVICE,
  DCM_DSD_0X22_0X01_EXTERNAL_FNC,
  DCM_DSD_0X22_0X01_INTERNAL_FNC,
  DCM_DSD_UDS_READ_DATA_BY_ID_0X22_0X01_NUM_OF_SUPPORTED_SESSIONS,
  DCM_DSD_UDS_READ_DATA_BY_ID_0X22_0X01_NUM_OF_SUPPORTED_SEC_LEV
};

/*
 * Configuration parameter for write data by identifier
 */
Dcm_DsdServiceType UDS_WriteDataById_0x2E =
{
  DCM_DSD_UDS_WRITE_DATA_BY_ID_0X2E_0X01_SERVICE_USED,
  DCM_WRITE_DATAT_BY_ID_SID,
  DCM_DSD_UDS_WRITE_DATA_BY_ID_0X2E_0X01_Sub_func_Avail,
  UDS_0x31_SessionRowRef,
  UDS_0x2E_SecurityRowRef,
  DCM_DSD_0X2E_0X01_SUB_SERVICE,
  DCM_DSD_0X2E_0X01_EXTERNAL_FNC,
  DCM_DSD_0X2E_0X01_INTERNAL_FNC,
  DCM_DSD_UDS_WRITE_DATA_BY_ID_0X2E_0X01_NUM_OF_SUPPORTED_SESSIONS,
  DCM_DSD_UDS_WRITE_DATA_BY_ID_0X2E_0X01_NUM_OF_SUPPORTED_SEC_LEV
};

/*
 * Configuration parameter for routine control
 */
Dcm_DsdServiceType UDS_RoutineControl_0x31 =
{
  DCM_DSD_UDS_ROUTINE_CONTROL_0X31_0X01_SERVICE_USED,
  DCM_ROUTINE_CONTROL_SID,
  DCM_DSD_UDS_ROUTINE_CONTROL_0X31_0X01_Sub_func_Avail,
  UDS_0x31_SessionRowRef,
  NULL,
  DCM_DSD_0X31_0X01_SUB_SERVICE,
  DCM_DSD_0X31_0X01_EXTERNAL_FNC,
  DCM_DSD_0X31_0X01_INTERNAL_FNC,
  DCM_DSD_UDS_ROUTINE_CONTROL_0X31_0X01_NUM_OF_SUPPORTED_SESSIONS,
  DCM_DSD_UDS_ROUTINE_CONTROL_0X31_0X01_NUM_OF_SUPPORTED_SEC_LEV
};


/**********************************
 * LIST-OF-SUPPORTED-UDS-SERVICES *
 **********************************/

const Dcm_DsdServiceType  *UDS_Services[] =
{
 &UDS_SessionControl_0x10,
 &UDS_TesterPresent_0x3E,
 &UDS_ReadDataById_0x22,
 &UDS_WriteDataById_0x2E,
 &UDS_SecurityAccess_0x27,
 &UDS_RoutineControl_0x31
};

/****************************
 * UDS-ON-CAN-SERVICE-TABLE *
 ****************************/
Dcm_DsdServiceTableType UDS_ServicTab =
{
 DCM_DSD_UDS_ON_CAN_SERVICE_TABLE_ID,
 UDS_Services
};

const Dcm_DsdType Dsd =
{
 &UDS_ServicTab
};


/*************************************
 * DSL-Used-Parameters-Configuration *
 *************************************/


                            /* DSL--UDS_ON_CAN--PROTCOL--CONFIGURATION */

const Dcm_DslMainConnectionType UDS_ON_CAN_MainConnectionId0x01 ;
const Dcm_DslConnectionType UDS_ON_CAN_ConnectionId0x01;
const Dcm_DslProtocolRowType UDS_ON_CAN_ProtocolRow;


/*  UDS Buffers
 *  There are mainly 4 buffers
 */

        /*1-NRC buffer*/
uint8 UDS_NrcRes[DCM_DSL_UDS_NEGATIVE_RES_CODE_BUFFER_SIZE] = {DCM_INITIALIZED_TO_ZERO};
PduInfoType UDS_NrcResBufferInfo =
{
 UDS_NrcRes,
 NULL,
 DCM_INITIALIZED_TO_ZERO
};
      /*NRC buffer info during run time */
Dcm_DslNRBufferType UDS_NrcBufferInfo =
{
 DCM_NOT_IN_USE,
 &UDS_NrcResBufferInfo
};

   /*2- Local buffer for functional tester present request */
Dcm_MsgItemType  UDS_MsgLocal[DCM_DSL_UDS_LOCAL_BUFFER_SIZE];
PduInfoType UDS_LocalBufferInfo =
{
 UDS_MsgLocal,
 NULL,
 DCM_INITIALIZED_TO_ZERO
};

        /*Local buffer info during run time */
Dcm_DslLocalBufferType UDS_LocalBuffer =
{
 DCM_BUFFER_UNLOCKED,
 DCM_NOT_IN_USE,
 &UDS_LocalBufferInfo
};

Dcm_MsgItemType UDS_MsgReq[DCM_DSL_UDS_MAIN_BUFFER_SIZE]={DCM_INITIALIZED_TO_ZERO};

DCM_RxBufferType UDS_RxBufferInfo =
{
 UDS_MsgReq,
 NULL,
 DCM_INITIALIZED_TO_ZERO,
 DCM_INITIALIZED_TO_ZERO,
 DCM_INITIALIZED_TO_ZERO,
 DCM_INITIALIZED_TO_ZERO,
 DCM_BUFFER_UNLOCKED,
 DCM_INITIALIZED_TO_ZERO,
 DCM_NOT_IN_USE
};

Dcm_MsgItemType UDS_MsgRes[DCM_DSL_UDS_MAIN_BUFFER_SIZE];

DCM_TxBufferType UDS_TxBufferInfo =
{
 NULL,
 NULL,
 DCM_INITIALIZED_TO_ZERO,
 DCM_INITIALIZED_TO_ZERO,
 DCM_BUFFER_UNLOCKED,
 DCM_INITIALIZED_TO_ZERO,
 DCM_NOT_IN_USE
};

       /* Diagnostic Protocol Handling during run time */
DCM_DslProtocolHandlingType UDS_ON_CAN_ProtocolHanling =
{
 FALSE,
 DCM_INITIALIZED_TO_ZERO
};

      /* Session State Handling */
Dcm_DslSessionHandlingType UDS_ON_CAN_SessionHanling =
{
 FALSE,
 DCM_INITIALIZED_TO_ZERO,
 DCM_DEFAULT_SESSION,
 DCM_DSP_UDS_DEFAULT_SESSION_P2_SERVER_MAX,
 DCM_DSP_UDS_DEFAULT_SESSION_P2_STAR_SERVER_MAX
};

Dcm_DslSecurityHandlingType UDS_SecurityHanling =
{
 DCM_SEC_LEV_LOCKED
};

Dcm_MsgContextType  UDS_ON_CAN_DiagMsgContext =
{
 &UDS_MsgReq[1],
 DCM_INITIALIZED_TO_ZERO,
 &UDS_MsgRes[1],
 DCM_INITIALIZED_TO_ZERO,
 DCM_INITIALIZED_TO_ZERO,
 DCM_DSL_UDS_MAIN_BUFFER_SIZE,
 DCM_INITIALIZED_TO_ZERO,
 DCM_INITIALIZED_TO_ZERO
};

Dcm_DslRunTimeDataType UDS_ON_CAN_DslRunTime =
{
 &UDS_RxBufferInfo,
 &UDS_TxBufferInfo,
 &UDS_LocalBuffer,
 &UDS_NrcBufferInfo,
 &UDS_ON_CAN_SessionHanling,
 &UDS_SecurityHanling,
 &UDS_ON_CAN_ProtocolHanling,
  DCM_INITIALIZED_TO_ZERO,
  DCM_INITIALIZED_TO_ZERO
};

Dcm_DslProtocolTimingRowType UDS_ON_CAN_PROTOCOL_ServerTiming =
{
 DCM_DSL_UDS_S_3_SERVER_MAX
};


DsdRunTimeDataType UDS_ON_CAN_DsdRunTime =
{
 DCM_INITIALIZED_TO_ZERO,
 DCM_INITIALIZED_TO_ZERO,
 NULL,
 DCM_INITIALIZED_TO_ZERO,
 UDS_MsgRes,
 DCM_INITIALIZED_TO_ZERO,
 DCM_INITIALIZED_TO_ZERO,
 DCM_INITIALIZED_TO_ZERO
};

DspSecurityRunTimeType UDS_ON_CAN_DspSecurityRunTime =
{
 FALSE,
 DCM_INITIALIZED_TO_ZERO,
 E_NOT_OK
};

DspPendingHanlingType UDS_ON_CAN_DspPendingHanling =
{
 NULL,
 NULL,
 NULL
};
DspDidReadRunTimeType UDS_ON_CAN_DspDidReadRunTime =
{
 DCM_INITIALIZED_TO_ZERO,
 (DCM_DSL_UDS_MAIN_BUFFER_SIZE -1)
};
DspRunTimeDataType UDS_ON_CAN_DspRunTime =
{
 &UDS_ON_CAN_DspPendingHanling,
 &UDS_ON_CAN_DspSecurityRunTime,
 &UDS_ON_CAN_DspDidReadRunTime,
  DCM_INITIAL
};

/*********************************************
 * UDS-ON-CAN-PROTCOL-TX-CONNECTION-ONE-PARA *
 *********************************************/

const Dcm_DslProtocolTxType UDS_ON_CAN_TxChConnId0x01 =
{
 &UDS_ON_CAN_MainConnectionId0x01,
 DCM_DSL_UDS_ON_CAN_CONNECTION_ONE_TX_CH_ID
};

/************************************************
 * UDS-ON-CAN-PROTCOL-RX-CH-CONNECTION-ONE-PARA *
 ************************************************/

const Dcm_DslProtocolRxType  UDS_ON_CAN_FuncRxChConnId0x01 =
{
 &UDS_ON_CAN_MainConnectionId0x01,
 DCM_FUNCTIONAL_TYPE,
 DCM_DSL_UDS_ON_CAN_CONNECTION_ONE_FUNC_RX_CH_ID
};

/************************************************
 * UDS-ON-CAN-PROTCOL-RX-CH-CONNECTION-ONE-PARA *
 ************************************************/

const Dcm_DslProtocolRxType  UDS_ON_CAN_PhysRxChConnId0x01 =
{
 &UDS_ON_CAN_MainConnectionId0x01,
 DCM_PHYSICAL_TYPE,
 DCM_DSL_UDS_ON_CAN_CONNECTION_ONE_PHYS_RX_CH_ID
};

/*********************************************
 * UDS-ON-CAN-PROTCOL-RX-CONNECTION-ONE-PARA *
 *********************************************/

const Dcm_DslProtocolRxType *UDS_ON_CAN_ConnectionId0x01ProtocolRx[] =
{
 &UDS_ON_CAN_FuncRxChConnId0x01,
 &UDS_ON_CAN_PhysRxChConnId0x01
};

/***************************************
 * UDS-ON-CAN-MAIN-CONNECTION-ONE-PARA *
 ***************************************/
const Dcm_DslMainConnectionType UDS_ON_CAN_MainConnectionId0x01 =
{
 &UDS_ON_CAN_ConnectionId0x01,
 DCM_DSL_UDS_ON_CAN_CONNECTION_ONE_ID,
 UDS_ON_CAN_ConnectionId0x01ProtocolRx,
 &UDS_ON_CAN_TxChConnId0x01
};

/**********************************
 * UDS-ON-CAN-CONNECTION-ONE-PARA *
 **********************************/
const Dcm_DslConnectionType UDS_ON_CAN_ConnectionId0x01 =
{
 &UDS_ON_CAN_ProtocolRow,
 &UDS_ON_CAN_MainConnectionId0x01
};

/********************************
 * UDS-ON-CAN-CONFIGURATION-ROW *
 ********************************/
const Dcm_DslProtocolRowType UDS_ON_CAN_ProtocolRow =
{
 DCM_UDS_ON_CAN,
 DCM_PROTOCOL_IS_AVAILABLE,
 NULL,
 NULL,
 &UDS_ServicTab,
 &UDS_ON_CAN_PROTOCOL_ServerTiming,
 &UDS_ON_CAN_ConnectionId0x01,
 &UDS_ON_CAN_DslRunTime,
 &UDS_ON_CAN_DsdRunTime,
 &UDS_ON_CAN_DspRunTime,
 &UDS_ON_CAN_DiagMsgContext
};

/**********************************
 * ALL-DCM-CONFIGURED-RX-CHANNELS *
 **********************************/
const Dcm_DslProtocolRxType  *AllRxCh[] =
{
 &UDS_ON_CAN_FuncRxChConnId0x01,
 &UDS_ON_CAN_PhysRxChConnId0x01
};

/***************************
 * DSL-PROTOCOL-PARAMETERS *
 ***************************/
const Dcm_DslProtcolType DslProtocol =
{
 &UDS_ON_CAN_ProtocolRow,
 AllRxCh,
 NULL
};

/******************************
 * CONFIGURATION-OF-AUTOMATIC *
 * REQ-CORRECTLY-REC-RES-PEND *
 * RES-MANAGEMENT-IN-DCM      *
 ******************************/
const Dcm_DslDiagRespType DslDiagResp =
{
 DCM_DSL_DIAG_RESP_MAX_NUM_RESP_PEND
};


/*********************************
 * DIAG-SESSION-LAYER-PARAMETERS *
 *********************************/
const Dcm_DslType Dsl =
{
  NULL,
 &DslDiagResp,
 &DslProtocol
};

/*****************************
 * MAIN-CONFIGURED-CONTAINER *
 *****************************/
const Dcm_ConfigType Dcm_ConfigSet =
{
 &Dsd,
 &Dsl,
 &Dsp
};
