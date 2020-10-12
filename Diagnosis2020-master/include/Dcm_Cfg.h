/*****************************************************************************
 * Module       : DCM                                                        *
 * File Name    : Dcm_Cfg.h                                                  *
 * Description  : Header file of all configuration parameters of DCM Module  *
 * Created ON   : 20/4/2020                                                  *
 *****************************************************************************/

#ifndef INCLUDE_DCM_CFG_H_
#define INCLUDE_DCM_CFG_H_



#include "Std_Types.h"


/******************************************
 * DSP--UDS--SECURITY-ROWS-CONFIGURATION  *
 ******************************************/

/*
 * Configuration parameter for security service delay on ECU power up
 */
#define DCM_DSP_SECURITY_LEV_LOCKED_DELAY_TIME_ON_BOOT  ((float32)0.1)

/*
 * Configuration parameter for security service delay on exceeded attempts
 */
#define DCM_DSP_SECURITY_LEV_1_DELAY_TIME               ((float32)0.1)
#define DCM_DSP_SECURITY_LEV_2_DELAY_TIME               ((float32)0.1)

/*
 * Configuration parameter for security service delay on ECU power up
 */
#define DCM_DSP_SECURITY_LEV_1_DELAY_TIME_ON_BOOT       ((float32)0.1)
#define DCM_DSP_SECURITY_LEV_2_DELAY_TIME_ON_BOOT       ((float32)0.1)

/*
 * Configuration parameter for security service MAX exceeded attempts NUM
 */
#define DCM_DSP_SECURITY_LEV_1_NUM_ATT_DELAY            ((uint8)2)
#define DCM_DSP_SECURITY_LEV_2_NUM_ATT_DELAY            ((uint8)2)

/*
 * Configuration parameter for security service Key Size
 */
#define DCM_DSP_SECURITY_LEV_1_KEY_SIZE                 ((uint32)2)
#define DCM_DSP_SECURITY_LEV_2_KEY_SIZE                 ((uint32)2)

/*
 * Configuration parameter for security service Seed Size
 */
#define DCM_DSP_SECURITY_LEV_1_SEED_SIZE                ((uint32)2)
#define DCM_DSP_SECURITY_LEV_2_SEED_SIZE                ((uint32)2)

/*
 * Configuration parameter for security service Compare_Key CallBack Func
 */
#define DCM_DSP_SECURITY_LEV_1_COMPARE_KEY_FNC          NULL
#define DCM_DSP_SECURITY_LEV_2_COMPARE_KEY_FNC          NULL

/*
 * Configuration parameter for security service Get_Seed CallBack Func
 */
#define DCM_DSP_SECURITY_LEV_1_GET_SEED_FNC             NULL
#define DCM_DSP_SECURITY_LEV_2_GET_SEED_FNC             NULL

/*
 * Configuration parameter for security service Get_Seed CallBack Func in case ADR_SIZE present
 */
#define DCM_DSP_SECURITY_LEV_1_GET_ADR_SIZE_SEED_FNC    NULL
#define DCM_DSP_SECURITY_LEV_2_GET_ADR_SIZE_SEED_FNC    NULL

/*
 * Configuration parameter for security service GetAtt_Counter CallBack Func
 */
#define Dcm_DSP_SECURITY_LEV_1_GET_ATTEMPT_COUNTER_FNC  NULL
#define Dcm_DSP_SECURITY_LEV_2_GET_ATTEMPT_COUNTER_FNC  NULL

/*
 * Configuration parameter for security service SetAtt_Counter CallBack Func
 */
#define DCM_DSP_SECURITY_LEV_1_SET_ATTEMPT_COUNTER_FNC  NULL
#define DCM_DSP_SECURITY_LEV_2_SET_ATTEMPT_COUNTER_FNC  NULL

/*
 * Configuration parameter for security service Use_Port
 */
#define DCM_DSP_SECURITY_LEV_1_USE_PORT                 DCM_USE_DATA_ASYNCH_FNC
#define DCM_DSP_SECURITY_LEV_2_USE_PORT                 DCM_USE_DATA_ASYNCH_FNC

#define DCM_DSP_NUM_OF_CONFIGURED_SECURITY_LEV          ((uint8)2)


/******************************************
 * DSP--UDS--SESSION-ROWS-CONFIGURATION   *
 ******************************************/

/*
 * Configuration parameter which indicates the num of supported sessions
 */
#define DCM_DSP_UDS_NUM_OF_SUPPORTED_SESSIONS               ((uint8)0x02)

/*
 * Configuration parameters for pending responses timer
 */
#define DCM_DSP_UDS_DEFAULT_SESSION_P2_SERVER_MAX           ((float32)0.1)
#define DCM_DSP_UDS_EXTENDED_SESSION_P2_SERVER_MAX          ((float32)0.1)

#define DCM_DSP_UDS_DEFAULT_SESSION_P2_STAR_SERVER_MAX      ((float32)0.1)
#define DCM_DSP_UDS_EXTENDED_SESSION_P2_STAR_SERVER_MAX     ((float32)0.1)



/****************************
 *   UDS_SUPPORTED DIDS     *
 ****************************/

/*
 * Configuration parameter which indicates num of supported DIDs.
 */
#define DCM_NUM_OF_SUPPORTED_DIDS              ((uint8)3)

/*
 * Configuration parameter which indicates max. num of DIDs per request.
 */
#define DCM_DSP_UDS_MAX_DID_TO_READ            ((uint8)2)


#define DCM_MAX_SIGNAL_DATA_LENGTH             ((uint16)8)

/****************************
 * BATTERY-POSITIVE-VOLTAGE *
 ****************************/

/*
 * Configuration parameter indicates the usage of DID
 */
#define DCM_DSP_UDS_BATTERY_POSITIVE_VOLTAGE_DID_USED                                   TRUE

/*
 * Configuration parameter indicates the num of Configured signals for this DID
 */
#define DCM_DSP_UDS_BATTERY_POSITIVE_VOLTAGE_DID_NUM_OF_CONFIGURED_SIGNALS              ((uint32)1)

/*
 * Configuration parameter points to the size of this DID if existed
 */
#define DCM_DSP_UDS_BATTERY_POSITIVE_VOLTAGE_SIGNAL_DID_DATA_BYTE_SIZE                  ((uint16)1)

/*
 * Configuration parameter indicates the type of the DID data
 */
#define DCM_DSP_UDS_BATTERY_POSITIVE_VOLTAGE_SIGNAL_DID_DATA_TYPE                       DCM_UINT8_N

/*
 * Configuration parameter indicates the type of the DID data
 */
#define DCM_DSP_UDS_BATTERY_POSITIVE_VOLTAGE_SIGANAL_DID_BYTE_OFFSET                    ((uint16)0)

/*
 * Configuration parameter indicates the num of supported sessions To read this DID
 */
#define DCM_DSP_UDS_BATTERY_POSITIVE_VOLTAGE_DID_READ_NUM_OF_SUPPORTED_SESSIONS         ((uint8)0)

/*
 * Configuration parameter indicates the num of supported Sec. Lev. to read this DID
 */
#define DCM_DSP_UDS_BATTERY_POSITIVE_VOLTAGE_DID_READ_NUM_OF_SUPPORTED_SEC_LEV          ((uint8)0)

/*
 * Configuration parameter indicates the num of supported sessions to write this DID
 */
#define DCM_DSP_UDS_BATTERY_POSITIVE_VOLTAGE_DID_WRITE_NUM_OF_SUPPORTED_SESSIONS        ((uint8)0)

/*
 * Configuration parameter indicates the num of supported Sec. Lev. to write this DID
 */
#define DCM_DSP_UDS_BATTERY_POSITIVE_VOLTAGE_DID_WRITE_NUM_OF_SUPPORTED_SEC_LEV                 ((uint8)0)

/*
 * Configuration parameter indicates the usage of check condition
 */
#define DCM_DSP_UDS_BATTERY_POSITIVE_VOLTAGE_SIGNAL_DID_DATA_CONDITION_CHECK_READ_FNC_USED      TRUE

/*
 * Configuration parameter for DID Check_Condition CallBack Func
 */
#define DCM_DSP_UDS_BATTERY_POSITIVE_VOLTAGE_SIGANAL_DID_DATA_CONDITION_CHECK_READ_FNC          NULL

/*
 * Configuration parameter for DID Check_Condition CallBack Func
 */
#define DCM_DSP_UDS_BATTERY_POSITIVE_VOLTAGE_SIGANAL_DID_DATA_CONDITION_CHECK_READ_ASYN_FNC     NULL

/*
 * Configuration parameter for DID Read_DataLength CallBack Func in case of SYN use port
 */
#define DCM_UDS_DSP_BATTERY_POSITIVE_VOLTAGE_SIGNAL_DID_DATA_READ_DATA_LENGTH_SYN_FNC           NULL

/*
 * Configuration parameter for DID Read_DataLength CallBack Func in case of ASYN use port
 */
#define DCM_UDS_DSP_BATTERY_POSITIVE_VOLTAGE_SIGNAL_DID_DATA_READ_DATA_LENGTH_ASYN_FNC          NULL

/*
 * Configuration parameter for DID Read_Data CallBack Func in case of SYN use port
 */
#define DCM_UDS_DSP_BATTERY_POSITIVE_VOLTAGE_SIGNAL_DID_DATA_READ_DATA_SYN_FNC                  NULL

/*
 * Configuration parameter for DID Read_Data CallBack Func in case of ASYN use port
 */
#define DCM_UDS_DSP_BATTERY_POSITIVE_VOLTAGE_SIGNAL_DID_DATA_READ_DATA_ASYN_FNC                 NULL

/*
 * Configuration parameter for DID Read_Data CallBack Func in case of ASYN error use port
 */
#define DCM_UDS_DSP_BATTERY_POSITIVE_VOLTAGE_SIGNAL_DID_DATA_READ_DATA_ASYN_ERROR_FNC           NULL

/*
 * Configuration parameter for DID Write_Data CallBack Func in case of SYN use port
 */
#define DCM_UDS_DSP_BATTERY_POSITIVE_VOLTAGE_SIGNAL_DID_DATA_WRITE_DATA_SYN_FNC                 NULL

/*
 * Configuration parameter for DID Write_Data CallBack Func in case of ASYN use port
 */
#define DCM_UDS_DSP_BATTERY_POSITIVE_VOLTAGE_SIGNAL_DID_DATA_WRITE_DATA_ASYN_FNC                NULL


/*
 * Configuration parameter for DID Write_Data CallBack Func in case of SYN use port
 */
#define DCM_UDS_DSP_BATTERY_POSITIVE_VOLTAGE_SIGNAL_DID_DATA_WRITE_DATA_SYN_DYN_FNC             NULL

/*
 * Configuration parameter for DID Write_Data CallBack Func in case of ASYN use port
 */
#define DCM_UDS_DSP_BATTERY_POSITIVE_VOLTAGE_SIGNAL_DID_DATA_WRITE_DATA_ASYN_DYN_FNC            NULL

/*
 * Configuration parameter indicates the use port of this DID
 */
#define DCM_UDS_DSP_BATTERY_POSITIVE_VOLTAGE_SIGNAL_DID_DATA_USE_PORT                           DCM_USE_DATA_SYNCH_FNC


/***********
 * VIN-NUM *
 ***********/


/*
 * Configuration parameter indicates the usage of DID
 */
#define DCM_DSP_UDS_VIN_NUM_DID_USED                                                    TRUE

/*
 * Configuration parameter points to the size of this DID if existed
 */
#define DCM_DSP_UDS_VIN_NUM_DID_NUM_OF_CONFIGURED_SIGNALS                               ((uint8)1)


/*
 * Configuration parameter points to the size of this DID if existed
 */
#define DCM_DSP_UDS_VIN_NUM_SIGNAL_DID_DATA_BYTE_SIZE                                   ((uint16)1)

/*
 * Configuration parameter indicates the type of the DID data
 */
#define DCM_DSP_UDS_VIN_NUM_SIGNAL_DID_DATA_TYPE                                        DCM_UINT8_N


/*
 * Configuration parameter indicates the type of the DID data
 */
#define DCM_DSP_UDS_VIN_NUM_SIGANAL_DID_BYTE_OFFSET                                     ((uint16)0)

/*
 * Configuration parameter indicates the num of supported sessions to read this DID
 */
#define DCM_DSP_UDS_VIN_NUM_DID_READ_NUM_OF_SUPPORTED_SESSIONS                           ((uint8)0)

/*
 * Configuration parameter indicates the num of supported Sec. Lev to read this DID
 */
#define DCM_DSP_UDS_VIN_NUM_DID_READ_NUM_OF_SUPPORTED_SEC_LEV                            ((uint8)0)

/*
 * Configuration parameter indicates the num of supported sessions to write this DID
 */
#define DCM_DSP_UDS_VIN_NUM_DID_WRITE_NUM_OF_SUPPORTED_SESSIONS                          ((uint8)0)

/*
 * Configuration parameter indicates the num of supported Sec. Lev to write this DID
 */
#define DCM_DSP_UDS_VIN_NUM_DID_WRITE_NUM_OF_SUPPORTED_SEC_LEV                           ((uint8)0)

/*
 * Configuration parameter indicates the usage of check condition
 */
#define DCM_DSP_UDS_VIN_NUM_SIGNAL_DID_DATA_CONDITION_CHECK_READ_FNC_USED                 TRUE

/*
 * Configuration parameter for DID Check_Condition CallBack Func
 */
#define DCM_DSP_UDS_VIN_NUM_SIGNAL_DID_DATA_CONDITION_CHECK_READ_FNC                      NULL

/*
 * Configuration parameter for DID Check_Condition CallBack Func
 */
#define DCM_DSP_UDS_VIN_NUM_SIGNAL_DID_DATA_CONDITION_CHECK_READ_ASYN_FNC                 NULL

/*
 * Configuration parameter for DID Read_DataLength CallBack Func in case of SYN use port
 */
#define DCM_UDS_DSP_VIN_NUM_SIGNAL_DID_DATA_READ_DATA_LENGTH_SYN_FNC                       NULL

/*
 * Configuration parameter for DID Read_DataLength CallBack Func in case of ASYN use port
 */
#define DCM_UDS_DSP_VIN_NUM_SIGNAL_DID_DATA_READ_DATA_LENGTH_ASYN_FNC                      NULL

/*
 * Configuration parameter for DID Read_Data CallBack Func in case of SYN use port
 */
#define DCM_UDS_DSP_VIN_NUM_SIGNAL_DID_DATA_READ_DATA_SYN_FNC                              NULL

/*
 * Configuration parameter for DID Read_Data CallBack Func in case of ASYN use port
 */
#define DCM_UDS_DSP_VIN_NUM_SIGNAL_DID_DATA_READ_DATA_ASYN_FNC                             NULL

/*
 * Configuration parameter for DID Read_Data CallBack Func in case of ASYN error use port
 */
#define DCM_UDS_DSP_VIN_NUM_SIGNAL_DID_DATA_READ_DATA_ASYN_ERROR_FNC                       NULL
/*
 * Configuration parameter for DID Write_Data CallBack Func in case of SYN use port
 */
#define DCM_UDS_DSP_VIN_NUM_SIGNAL_DID_DATA_WRITE_DATA_SYN_FNC                             NULL

/*
 * Configuration parameter for DID Write_Data CallBack Func in case of ASYN use port
 */
#define DCM_UDS_DSP_VIN_NUM_SIGNAL_DID_DATA_WRITE_DATA_ASYN_FNC                            NULL

/*
 * Configuration parameter for DID Write_Data CallBack Func in case of SYN use port
 */
#define DCM_UDS_DSP_VIN_NUM_SIGNAL_DID_DATA_WRITE_DATA_SYN_DYN_FNC                         NULL

/*
 * Configuration parameter for DID Write_Data CallBack Func in case of ASYN use port
 */
#define DCM_UDS_DSP_VIN_NUM_SIGNAL_DID_DATA_WRITE_DATA_ASYN_DYN_FNC                        NULL

/*
 * Configuration parameter indicates the use port of this DID
 */
#define DCM_UDS_DSP_VIN_NUM_SIGNAL_DID_DATA_USE_PORT                                       DCM_USE_DATA_ASYNCH_FNC



/************
 * 010A-DID *
 ************/


/*
 * Configuration parameter indicates the usage of DID
 */
#define DCM_DSP_UDS_0X010A_DID_USED                                                     TRUE


/*
 * Configuration parameter indicates the num of supported sessions to read this DID
 */
#define DCM_DSP_UDS_0X010A_DID_READ_NUM_OF_SUPPORTED_SESSIONS                           ((uint8)0)

/*
 * Configuration parameter indicates the num of supported Sec. Lev to read this DID
 */
#define DCM_DSP_UDS_0X010A_DID_READ_NUM_OF_SUPPORTED_SEC_LEV                            ((uint8)0)

/*
 * Configuration parameter indicates the num of supported sessions to write this DID
 */
#define DCM_DSP_UDS_0X010A_DID_WRITE_NUM_OF_SUPPORTED_SESSIONS                          ((uint8)0)

/*
 * Configuration parameter indicates the num of supported Sec. Lev to write this DID
 */
#define DCM_DSP_UDS_0X010A_DID_WRITE_NUM_OF_SUPPORTED_SEC_LEV                           ((uint8)0)

/*
 * Configuration parameter points to the size of this DID if existed
 */
#define DCM_DSP_UDS_0X010A_DID_NUM_OF_CONFIGURED_SIGNALS                               ((uint8)3)


/*
 * Configuration parameter points to the size of this DID if existed
 */
#define DCM_DSP_UDS_0X010A_ENGINE_COOLANT_TEMP_SIGNAL_DID_DATA_BYTE_SIZE               ((uint16)5)

/*
 * Configuration parameter indicates the type of the DID data
 */
#define DCM_DSP_UDS_0X010A_ENGINE_COOLANT_TEMP_SIGNAL_DID_DATA_TYPE                    DCM_UINT8_N


/*
 * Configuration parameter indicates the type of the DID data
 */
#define DCM_DSP_UDS_0X010A_ENGINE_COOLANT_TEMP_SIGANAL_DID_BYTE_OFFSET                 ((uint16)0)

/*
 * Configuration parameter indicates the usage of check condition
 */
#define DCM_DSP_UDS_0X010A_ENGINE_COOLANT_TEMP_SIGNAL_DID_DATA_CONDITION_CHECK_READ_FNC_USED                     FALSE

/*
 * Configuration parameter for DID Check_Condition CallBack Func
 */
#define DCM_DSP_UDS_0X010A_ENGINE_COOLANT_TEMP_SIGNAL_DID_DATA_CONDITION_CHECK_READ_FNC                           NULL

/*
 * Configuration parameter for DID Check_Condition CallBack Func
 */
#define DCM_DSP_UDS_0X010A_ENGINE_COOLANT_TEMP_SIGNAL_DID_DATA_CONDITION_CHECK_READ_ASYN_FNC                      NULL

/*
 * Configuration parameter for DID Read_DataLength CallBack Func in case of SYN use port
 */
#define DCM_UDS_DSP_0X010A_ENGINE_COOLANT_TEMP_SIGNAL_DID_DATA_READ_DATA_LENGTH_SYN_FNC                           NULL

/*
 * Configuration parameter for DID Read_DataLength CallBack Func in case of ASYN use port
 */
#define DCM_UDS_DSP_0X010A_ENGINE_COOLANT_TEMP_SIGNAL_DID_DATA_READ_DATA_LENGTH_ASYN_FNC                          NULL

/*
 * Configuration parameter for DID Read_Data CallBack Func in case of SYN use port
 */
#define DCM_UDS_DSP_0X010A_ENGINE_COOLANT_TEMP_SIGNAL_DID_DATA_READ_DATA_SYN_FNC                                  NULL

/*
 * Configuration parameter for DID Read_Data CallBack Func in case of ASYN use port
 */
#define DCM_UDS_DSP_0X010A_ENGINE_COOLANT_TEMP_SIGNAL_DID_DATA_READ_DATA_ASYN_FNC                                 NULL

/*
 * Configuration parameter for DID Read_Data CallBack Func in case of ASYN error use port
 */
#define DCM_UDS_DSP_0X010A_ENGINE_COOLANT_TEMP_SIGNAL_DID_DATA_READ_DATA_ASYN_ERROR_FNC                           NULL
/*
 * Configuration parameter for DID Write_Data CallBack Func in case of SYN use port
 */
#define DCM_UDS_DSP_0X010A_ENGINE_COOLANT_TEMP_SIGNAL_DID_DATA_WRITE_DATA_SYN_FNC                                 NULL

/*
 * Configuration parameter for DID Write_Data CallBack Func in case of ASYN use port
 */
#define DCM_UDS_DSP_0X010A_ENGINE_COOLANT_TEMP_SIGNAL_DID_DATA_WRITE_DATA_ASYN_FNC                                NULL

/*
 * Configuration parameter for DID Write_Data CallBack Func in case of SYN use port
 */
#define DCM_UDS_DSP_0X010A_ENGINE_COOLANT_TEMP_SIGNAL_DID_DATA_WRITE_DATA_SYN_DYN_FNC                             NULL

/*
 * Configuration parameter for DID Write_Data CallBack Func in case of ASYN use port
 */
#define DCM_UDS_DSP_0X010A_ENGINE_COOLANT_TEMP_SIGNAL_DID_DATA_WRITE_DATA_ASYN_DYN_FNC                            NULL

/*
 * Configuration parameter indicates the use port of this DID
 */
#define DCM_UDS_DSP_0X010A_ENGINE_COOLANT_TEMP_SIGNAL_DID_DATA_USE_PORT                                           DCM_USE_DATA_SYNCH_FNC






/*
 * Configuration parameter points to the size of this DID if existed
 */
#define DCM_DSP_UDS_0X010A_THROTTLE_POSITION_SIGNAL_DID_DATA_BYTE_SIZE                  ((uint16)6)

/*
 * Configuration parameter indicates the type of the DID data
 */
#define DCM_DSP_UDS_0X010A_THROTTLE_POSITION_SIGNAL_DID_DATA_TYPE                       DCM_UINT8_N


/*
 * Configuration parameter indicates the type of the DID data
 */
#define DCM_DSP_UDS_0X010A_THROTTLE_POSITION_SIGANAL_DID_BYTE_OFFSET                    ((uint16)5)

/*
 * Configuration parameter indicates the usage of check condition
 */
#define DCM_DSP_UDS_0X010A_THROTTLE_POSITION_SIGNAL_DID_DATA_CONDITION_CHECK_READ_FNC_USED                     FALSE

/*
 * Configuration parameter for DID Check_Condition CallBack Func
 */
#define DCM_DSP_UDS_0X010A_THROTTLE_POSITION_SIGNAL_DID_DATA_CONDITION_CHECK_READ_FNC                          NULL

/*
 * Configuration parameter for DID Check_Condition CallBack Func
 */
#define DCM_DSP_UDS_0X010A_THROTTLE_POSITION_SIGNAL_DID_DATA_CONDITION_CHECK_READ_ASYN_FNC                     NULL

/*
 * Configuration parameter for DID Read_DataLength CallBack Func in case of SYN use port
 */
#define DCM_UDS_DSP_0X010A_THROTTLE_POSITION_SIGNAL_DID_DATA_READ_DATA_LENGTH_SYN_FNC                          NULL

/*
 * Configuration parameter for DID Read_DataLength CallBack Func in case of ASYN use port
 */
#define DCM_UDS_DSP_0X010A_THROTTLE_POSITION_SIGNAL_DID_DATA_READ_DATA_LENGTH_ASYN_FNC                         NULL

/*
 * Configuration parameter for DID Read_Data CallBack Func in case of SYN use port
 */
#define DCM_UDS_DSP_0X010A_THROTTLE_POSITION_SIGNAL_DID_DATA_READ_DATA_SYN_FNC                                 NULL

/*
 * Configuration parameter for DID Read_Data CallBack Func in case of ASYN use port
 */
#define DCM_UDS_DSP_0X010A_THROTTLE_POSITION_SIGNAL_DID_DATA_READ_DATA_ASYN_FNC                                NULL

/*
 * Configuration parameter for DID Read_Data CallBack Func in case of ASYN error use port
 */
#define DCM_UDS_DSP_0X010A_THROTTLE_POSITION_SIGNAL_DID_DATA_READ_DATA_ASYN_ERROR_FNC                          NULL
/*
 * Configuration parameter for DID Write_Data CallBack Func in case of SYN use port
 */
#define DCM_UDS_DSP_0X010A_THROTTLE_POSITION_SIGNAL_DID_DATA_WRITE_DATA_SYN_FNC                                NULL

/*
 * Configuration parameter for DID Write_Data CallBack Func in case of ASYN use port
 */
#define DCM_UDS_DSP_0X010A_THROTTLE_POSITION_SIGNAL_DID_DATA_WRITE_DATA_ASYN_FNC                               NULL


/*
 * Configuration parameter for DID Write_Data CallBack Func in case of SYN use port
 */
#define DCM_UDS_DSP_0X010A_THROTTLE_POSITION_SIGNAL_DID_DATA_WRITE_DATA_SYN_DYN_FNC                            NULL

/*
 * Configuration parameter for DID Write_Data CallBack Func in case of ASYN use port
 */
#define DCM_UDS_DSP_0X010A_THROTTLE_POSITION_SIGNAL_DID_DATA_WRITE_DATA_ASYN_DYN_FNC                           NULL

/*
 * Configuration parameter indicates the use port of this DID
 */
#define DCM_UDS_DSP_0X010A_THROTTLE_POSITION_SIGNAL_DID_DATA_USE_PORT                                           DCM_USE_DATA_ASYNCH_FNC_ERROR





/*
 * Configuration parameter points to the size of this DID if existed
 */
#define DCM_DSP_UDS_0X010A_ENGINE_SPEED_SIGNAL_DID_DATA_BYTE_SIZE                  ((uint16)4)

/*
 * Configuration parameter indicates the type of the DID data
 */
#define DCM_DSP_UDS_0X010A_ENGINE_SPEED_SIGNAL_DID_DATA_TYPE                       DCM_UINT8_DYN


/*
 * Configuration parameter indicates the type of the DID data
 */
#define DCM_DSP_UDS_0X010A_ENGINE_SPEED_SIGANAL_DID_BYTE_OFFSET                    ((uint16)11)

/*
 * Configuration parameter indicates the usage of check condition
 */
#define DCM_DSP_UDS_0X010A_ENGINE_SPEED_SIGNAL_DID_DATA_CONDITION_CHECK_READ_FNC_USED                     FALSE

/*
 * Configuration parameter for DID Check_Condition CallBack Func
 */
#define DCM_DSP_UDS_0X010A_ENGINE_SPEED_SIGNAL_DID_DATA_CONDITION_CHECK_READ_FNC                          NULL


/*
 * Configuration parameter for DID Check_Condition CallBack Func
 */
#define DCM_DSP_UDS_0X010A_ENGINE_SPEED_SIGNAL_DID_DATA_CONDITION_CHECK_READ_ASYN_FNC                     NULL

/*
 * Configuration parameter for DID Read_DataLength CallBack Func in case of SYN use port
 */
#define DCM_UDS_DSP_0X010A_ENGINE_SPEED_SIGNAL_DID_DATA_READ_DATA_LENGTH_SYN_FNC                          NULL

/*
 * Configuration parameter for DID Read_DataLength CallBack Func in case of ASYN use port
 */
#define DCM_UDS_DSP_0X010A_ENGINE_SPEED_SIGNAL_DID_DATA_READ_DATA_LENGTH_ASYN_FNC                         NULL

/*
 * Configuration parameter for DID Read_Data CallBack Func in case of SYN use port
 */
#define DCM_UDS_DSP_0X010A_ENGINE_SPEED_SIGNAL_DID_DATA_READ_DATA_SYN_FNC                                 NULL

/*
 * Configuration parameter for DID Read_Data CallBack Func in case of ASYN use port
 */
#define DCM_UDS_DSP_0X010A_ENGINE_SPEED_SIGNAL_DID_DATA_READ_DATA_ASYN_FNC                                NULL

/*
 * Configuration parameter for DID Read_Data CallBack Func in case of ASYN error use port
 */
#define DCM_UDS_DSP_0X010A_ENGINE_SPEED_SIGNAL_DID_DATA_READ_DATA_ASYN_ERROR_FNC                          NULL
/*
 * Configuration parameter for DID Write_Data CallBack Func in case of SYN use port
 */
#define DCM_UDS_DSP_0X010A_ENGINE_SPEED_SIGNAL_DID_DATA_WRITE_DATA_SYN_FNC                                NULL

/*
 * Configuration parameter for DID Write_Data CallBack Func in case of ASYN use port
 */
#define DCM_UDS_DSP_0X010A_ENGINE_SPEED_SIGNAL_DID_DATA_WRITE_DATA_ASYN_FNC                               NULL

/*
 * Configuration parameter for DID Write_Data CallBack Func in case of SYN use port
 */
#define DCM_UDS_DSP_0X010A_ENGINE_SPEED_SIGNAL_DID_DATA_WRITE_DATA_SYN_DYN_FNC                            NULL

/*
 * Configuration parameter for DID Write_Data CallBack Func in case of ASYN use port
 */
#define DCM_UDS_DSP_0X010A_ENGINE_SPEED_SIGNAL_DID_DATA_WRITE_DATA_ASYN_DYN_FNC                           NULL

/*
 * Configuration parameter indicates the use port of this DID
 */
#define DCM_UDS_DSP_0X010A_ENGINE_SPEED_SIGNAL_DID_DATA_USE_PORT                                           DCM_USE_DATA_SYNCH_FNC



/****************************
 *   UDS-SUPPORTED-RIDS     *
 ****************************/


/*
 * Configuration parameter indicates the num of supported RIDS
 */
#define DCM_NUM_OF_SUPPORTED_ROUTINES                           ((uint8)0x01)

/*
 * Configuration parameter indicates the usage of DID
 */
#define DCM_DSP_UDS_CHECK_MEMORY_ROUTINE_ROUTINE_USED           TRUE

/*
 * Configuration parameter for RID Start_Routine CallBack Func
 */
#define DCM_DSP_UDS_START_CHECK_MEMORY_ROUTINE_FNC              NULL

/*
 * Configuration parameter for RID Stop_Routine CallBack Func
 */
#define DCM_DSP_UDS_STOP_CHECK_MEMORY_ROUTINE_FNC               NULL

/*
 * Configuration parameter for RID RequestRoutine_Result CallBack Func
 */
#define DCM_DSP_UDS_REQUEST_CHECK_MEMORY_RESULTS_ROUTINE_FNC    NULL

/*
 * Configuration parameter indicates the num of supported sessions for this RID
 */
#define DCM_DSP_UDS_CHECK_MEMORY_NUM_RID_NUM_OF_SUPPORTED_SESSIONS          ((uint8)0)

/*
 * Configuration parameter indicates the num of supported Sec. Lev. for this RID
 */
#define DCM_DSP_UDS_CHECK_MEMORY_RID_NUM_OF_SUPPORTED_SEC_LEV               ((uint8)0)



/**********************************
 * DSD-UDS-SERVICES-CONFIGURATION *
 **********************************/


/*
 * Configuration parameter indicates the num of supported services
 */
#define DCM_DSD_UDS_NUM_OF_SUPPORTED_SERVICES                                ((uint8)6)

/*
 * Configuration parameter indicates the usage of Service
 */
#define DCM_DSD_UDS_SESSION_CONTROL_0X10_0X01_SERVICE_USED                   TRUE
#define DCM_DSD_UDS_TESTER_PRESENT_0X3E_0X01_SERVICE_USED                    TRUE
#define DCM_DSD_UDS_READ_DATA_BY_ID_0X22_0X01_SERVICE_USED                   TRUE
#define DCM_DSD_UDS_WRITE_DATA_BY_ID_0X2E_0X01_SERVICE_USED                  TRUE
#define DCM_DSD_UDS_SECURITY_ACCESS_0X27_0X01_SERVICE_USED                   TRUE
#define DCM_DSD_UDS_ROUTINE_CONTROL_0X31_0X01_SERVICE_USED                   TRUE

/*
 * Configuration parameter indicates the subFunc availability in the service
 */
#define DCM_DSD_UDS_SESSION_CONTROL_0X10_0X01_Sub_func_Avail                 TRUE
#define DCM_DSD_UDS_TESTER_PRESENT_0X3E_0X01_Sub_func_Avail                  TRUE
#define DCM_DSD_UDS_READ_DATA_BY_ID_0X22_0X01_Sub_func_Avail                 FALSE
#define DCM_DSD_UDS_WRITE_DATA_BY_ID_0X2E_0X01_Sub_func_Avail                FALSE
#define DCM_DSD_UDS_SECURITY_ACCESS_0X27_0X01_Sub_func_Avail                 TRUE
#define DCM_DSD_UDS_ROUTINE_CONTROL_0X31_0X01_Sub_func_Avail                 TRUE

/*
 * Configuration parameter indicates the num of supported sessions for the service
 */
#define DCM_DSD_UDS_SESSION_CONTROL_0X10_0X01_NUM_OF_SUPPORTED_SESSIONS      ((uint8)0)
#define DCM_DSD_UDS_TESTER_PRESENT_0X3E_0X01_NUM_OF_SUPPORTED_SESSIONS       ((uint8)2)
#define DCM_DSD_UDS_READ_DATA_BY_ID_0X22_0X01_NUM_OF_SUPPORTED_SESSIONS      ((uint8)2)
#define DCM_DSD_UDS_WRITE_DATA_BY_ID_0X2E_0X01_NUM_OF_SUPPORTED_SESSIONS     ((uint8)2)
#define DCM_DSD_UDS_SECURITY_ACCESS_0X27_0X01_NUM_OF_SUPPORTED_SESSIONS      ((uint8)2)
#define DCM_DSD_UDS_ROUTINE_CONTROL_0X31_0X01_NUM_OF_SUPPORTED_SESSIONS      ((uint8)1)

/*
 * Configuration parameter indicates the num of supported Sec. Lev. for the service
 */
#define DCM_DSD_UDS_SESSION_CONTROL_0X10_0X01_NUM_OF_SUPPORTED_SEC_LEV       ((uint8)0)
#define DCM_DSD_UDS_TESTER_PRESENT_0X3E_0X01_NUM_OF_SUPPORTED_SEC_LEV        ((uint8)2)
#define DCM_DSD_UDS_READ_DATA_BY_ID_0X22_0X01_NUM_OF_SUPPORTED_SEC_LEV       ((uint8)2)
#define DCM_DSD_UDS_WRITE_DATA_BY_ID_0X2E_0X01_NUM_OF_SUPPORTED_SEC_LEV      ((uint8)2)
#define DCM_DSD_UDS_SECURITY_ACCESS_0X27_0X01_NUM_OF_SUPPORTED_SEC_LEV       ((uint8)0)
#define DCM_DSD_UDS_ROUTINE_CONTROL_0X31_0X01_NUM_OF_SUPPORTED_SEC_LEV       ((uint8)0)

/*
 * Configuration parameter for Service External Processing handler
 */
#define DCM_DSD_0X10_0X01_EXTERNAL_FNC     NULL
#define DCM_DSD_0X3E_0X01_EXTERNAL_FNC     NULL
#define DCM_DSD_0X22_0X01_EXTERNAL_FNC     Dsp_UDS_ReadDataByIdentifier_0x22
#define DCM_DSD_0X2E_0X01_EXTERNAL_FNC     Dsp_UDS_WriteDataByIdentifier_0x2E
#define DCM_DSD_0X27_0X01_EXTERNAL_FNC     Dsp_UDS_SecurityAccess_0x27
#define DCM_DSD_0X31_0X01_EXTERNAL_FNC     Dsp_UDS_RoutineControl_0x31

/*
 * Configuration parameter for Service External Processing handler
 */
#define DCM_DSD_0X10_0X01_INTERNAL_FNC     Dsp_UDS_DiagnosticSessionControl0x10
#define DCM_DSD_0X3E_0X01_INTERNAL_FNC     Dsp_UDS_TesterPresent0x3E
#define DCM_DSD_0X22_0X01_INTERNAL_FNC     NULL
#define DCM_DSD_0X2E_0X01_INTERNAL_FNC     NULL
#define DCM_DSD_0X27_0X01_INTERNAL_FNC     NULL
#define DCM_DSD_0X31_0X01_INTERNAL_FNC     NULL

/*
 * Configuration parameter for Sub-Service Container pointer
 */
#define DCM_DSD_0X10_0X01_SUB_SERVICE      NULL
#define DCM_DSD_0X3E_0X01_SUB_SERVICE      NULL
#define DCM_DSD_0X22_0X01_SUB_SERVICE      NULL
#define DCM_DSD_0X2E_0X01_SUB_SERVICE      NULL
#define DCM_DSD_0X27_0X01_SUB_SERVICE      NULL
#define DCM_DSD_0X31_0X01_SUB_SERVICE      NULL

/*
 * Configuration parameter indicates service table ID
 */
#define DCM_DSD_UDS_ON_CAN_SERVICE_TABLE_ID                             ((uint8)0x01)


/*************************************
 * DSL-Used-Parameters-Configuration *
 *************************************/

/*
 * Configuration parameter for negative response code buffer size
 */
#define DCM_DSL_UDS_NEGATIVE_RES_CODE_BUFFER_SIZE              ((uint8)3)

/*
 * Configuration parameter for Local buffer size
 */
#define DCM_DSL_UDS_LOCAL_BUFFER_SIZE                          ((uint8)8)

/*
 * Configuration parameter for UDS Protocol buffer size
 */
#define DCM_DSL_UDS_MAIN_BUFFER_SIZE                           ((uint32)30)

/*
 * Configuration parameter for Non-Default session time out in seconds
 */
#define DCM_DSL_UDS_S_3_SERVER_MAX                             ((float32)5)

/*
 * Configuration parameter indicates the num of UDS_ON_CAN protocol Connections
 */
#define DCM_UDS_ON_CAN_NUM_OF_SUPPORTED_CONNECTIONS            ((uint8)0x01)

/*
 * Configuration parameter indicates UDS_ON_CAN protocol Connection one ID
 */
#define DCM_DSL_UDS_ON_CAN_CONNECTION_ONE_ID                   ((uint8)0x01)

/*
 * Configuration parameter indicates UDS_ON_CAN protocol Connection one Func RX Channel ID
 */
#define DCM_DSL_UDS_ON_CAN_CONNECTION_ONE_FUNC_RX_CH_ID        ((uint32)0x01)

/*
 *  Configuration parameter indicates UDS_ON_CAN protocol Connection one Phys RX Channel ID
 */
#define DCM_DSL_UDS_ON_CAN_CONNECTION_ONE_PHYS_RX_CH_ID        ((uint32)0x02)

/*
 * Configuration parameter indicates UDS_ON_CAN protocol Connection one TX Channel ID
 */
#define DCM_DSL_UDS_ON_CAN_CONNECTION_ONE_TX_CH_ID             ((uint32)0x03)

/*
 * Configuration parameter to enable the responding to requests
 */
#define DCM_RESPOND_ALL_REQUEST                    TRUE

/*
 *
 */
#define DCM_DEV_ERROR_DETECT                       FALSE


/*
 * Configuration parameter indicates the max num of responsePending response
 */
#define DCM_DSL_DIAG_RESP_MAX_NUM_RESP_PEND        ((uint8)4)

/*
 * Configuration parameter indicates the main func of DCM period which set in OS.
 */
#define DCM_MAIN_FUNCTION_PERIOD_TIME_MS                 50



#endif /* INCLUDE_DCM_CFG_H_ */
