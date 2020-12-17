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

#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif

#include "../api/typedefs.h" // Fixed for C-Binding

typedef enum IL_WeightDistributionType
{
    IL_WeightDistributionType_Flat                    = 0,        // default, means no standard weight distribution is applied
    IL_WeightDistributionType_CenterWeighted          = 1,        // center weighted

    IL_WeightDistributionType_LeftWeighted            = 2,        // NOT IMPLEMENTED YET, please do not use
    IL_WeightDistributionType_RightWeighted           = 4,        // NOT IMPLEMENTED YET, please do not use
    IL_WeightDistributionType_TopWeighted             = 8,        // NOT IMPLEMENTED YET, please do not use
    IL_WeightDistributionType_BottomWeighted          = 16,       // NOT IMPLEMENTED YET, please do not use
    IL_WeightDistributionType_TopLeftWeighted         = 32,       // NOT IMPLEMENTED YET, please do not use
    IL_WeightDistributionType_TopRightWeighted        = 64,       // NOT IMPLEMENTED YET, please do not use
    IL_WeightDistributionType_BottomLeftWeighted      = 128,      // NOT IMPLEMENTED YET, please do not use
    IL_WeightDistributionType_BottomRightWeighted     = 256,      // NOT IMPLEMENTED YET, please do not use

    IL_WeightDistributionType_HorizontalLineWeighted  = 512,      // NOT IMPLEMENTED YET, please do not use
    IL_WeightDistributionType_VerticalLineWeighted    = 1024,     // NOT IMPLEMENTED YET, please do not use

    IL_WeightDistributionType_Occluder                = 16384,    // means this interactor blocks other interactors behind it
                                                                  // (and does not get any events, it ONLY occludes)

    IL_WeightDistributionType_LastFlag                = 32768,    // INTERNAL from here,  please do not use any of the following values

    IL_WeightDistributionType_CustomBitmapWeighted    = 32769,
    IL_WeightDistributionType_EllipsoidOutline,
    IL_WeightDistributionType_CustomFunction,
} IL_WeightDistributionType;

#ifdef __cplusplus
namespace IL
{
    enum class WeightDistributionType
    {
        Flat                   = ::IL_WeightDistributionType_Flat,
        CenterWeighted         = ::IL_WeightDistributionType_CenterWeighted,

        LeftWeighted           = ::IL_WeightDistributionType_LeftWeighted,
        RightWeighted          = ::IL_WeightDistributionType_RightWeighted,
        TopWeighted            = ::IL_WeightDistributionType_TopWeighted,
        BottomWeighted         = ::IL_WeightDistributionType_BottomWeighted,
        TopLeftWeighted        = ::IL_WeightDistributionType_TopLeftWeighted,
        TopRightWeighted       = ::IL_WeightDistributionType_TopRightWeighted,
        BottomLeftWeighted     = ::IL_WeightDistributionType_BottomLeftWeighted,
        BottomRightWeighted    = ::IL_WeightDistributionType_BottomRightWeighted,

        HorizontalLineWeighted = ::IL_WeightDistributionType_HorizontalLineWeighted,
        VerticalLineWeighted   = ::IL_WeightDistributionType_VerticalLineWeighted,

        Occluder               = ::IL_WeightDistributionType_Occluder,

        LastFlag               = ::IL_WeightDistributionType_LastFlag,

        CustomBitmapWeighted   = ::IL_WeightDistributionType_CustomBitmapWeighted,
        EllipsoidOutline       = ::IL_WeightDistributionType_EllipsoidOutline,
        CustomFunction         = ::IL_WeightDistributionType_CustomFunction,
    };

    struct WeightDistributionData
    {
        WeightDistributionData(WeightDistributionId id, WeightDistributionType type)
            : id(id), iid(EmptyInteractorId()), type(type)
        {}

        WeightDistributionData(WeightDistributionId id, InteractorId iid, WeightDistributionType type)
            : id(id), iid(iid), type(type)
        {}

        WeightDistributionId id;
        InteractorId iid;
        WeightDistributionType type;
    };

    struct WeightDistributionBitmap
    {
        WeightDistributionBitmap(WeightDistributionId id, bool isOutline, uint8_t cols, uint8_t rows, uint8_t* bitmap)
            : id(id), isOutline(isOutline), cols(cols), rows(rows), bitmap(bitmap)
        {}

        WeightDistributionId id;
        bool isOutline;
        uint8_t cols;
        uint8_t rows;
        uint8_t* bitmap;
    };
}
#endif

