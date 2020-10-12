/*
 * Det.h
 *
 *  Created on: Jul 11, 2020
 *      Author: ahmed
 */

#ifndef INCLUDE_DET_H_
#define INCLUDE_DET_H_

#include "Std_Types.h"
Std_ReturnType Det_ReportError(uint16 ModuleId, uint8 InstanceId,uint8 ApiId,uint8 ErrorId);

Std_ReturnType Det_ReportRuntimeError(uint16 ModuleId, uint8 InstanceId, uint8 ApiId,uint8 ErrorId);

#endif /* INCLUDE_DET_H_ */
