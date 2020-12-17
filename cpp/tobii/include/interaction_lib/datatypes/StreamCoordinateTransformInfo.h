/*
COPYRIGHT 2018-2020  - PROPERTY OF TOBII AB
-------------------------------------
2018-2020 TOBII AB - KARLSROVAGEN 2D, DANDERYD 182 53, SWEDEN - All Rights Reserved.

NOTICE:  All information contained herein is, and remains, the property of Tobii AB and its suppliers, if any.
The intellectual and technical concepts contained herein are proprietary to Tobii AB and its suppliers and may be
covered by U.S.and Foreign Patents, patent applications, and are protected by trade secret or copyright law.
Dissemination of this information or reproduction of this material is strictly forbidden unless prior written
permission is obtained from Tobii AB.
*/

#pragma once

#include "StreamInputType.h"
#include "StreamCoordinateSystemType.h"

typedef struct IL_StreamCoordinateTransformInfo
{
    IL_StreamInputType streamType;
    float unitsPerMm_xy[2];
    float rangeUnits_xy[2];
    float offsetUnits_xy[2];
    IL_StreamCoordinateSystemType coordinateSystemType;
    const char* displayAreaId;
    bool usesAbsoluteCoordinates;
} IL_StreamCoordinateTransformInfo;

#ifdef __cplusplus
namespace IL
{
    using StreamCoordinateTransformInfo = ::IL_StreamCoordinateTransformInfo;
}
#endif