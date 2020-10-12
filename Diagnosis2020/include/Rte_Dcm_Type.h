/******************************************************************************
 * Module      : RTE-DCM                                                      *
 * File Name   : Rte_Dcm_Type.h                                               *
 * Description : Header file of Shared Rte-Dcm Types                          *
 * Created on  : April 6, 2020                                                *
 ******************************************************************************/

#ifndef INCLUDE_RTE_DCM_TYPE_H_
#define INCLUDE_RTE_DCM_TYPE_H_


#include "Std_Types.h"

typedef uint8 Dcm_OpStatusType;

#define DCM_RTE_INITIAL         ((Dcm_OpStatusType)0x00)
#define DCM_RTE_PENDING         ((Dcm_OpStatusType)0x01)
#define DCM_RTE_CANCEL          ((Dcm_OpStatusType)0x02)
#define DCM_RTE_FORCE_RCRRP_OK  ((Dcm_OpStatusType)0x03)


#endif /* INCLUDE_RTE_DCM_TYPE_H_ */
