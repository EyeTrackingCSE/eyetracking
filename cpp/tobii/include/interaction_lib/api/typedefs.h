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
#include <limits>
#else
#include <stdint.h>
#endif

typedef uint64_t IL_InteractorId;
typedef uint64_t IL_MaskId;
typedef uint64_t IL_BaseScoreId;
typedef uint64_t IL_WeightDistributionId;
typedef int64_t IL_Timestamp;


#define IL_EMPTYINTERACTORID 0xffffffffffffffff-1
#define IL_EMPTYBASESCOREID 0xffffffffffffffff-1
#define IL_EMPTYWEIGHTDISTRIBUTIONID 0xffffffffffffffff-1 

#ifdef __cplusplus
    namespace IL
    {
        using InteractorId = ::IL_InteractorId;
        using MaskId = ::IL_MaskId;
        using BaseScoreId = ::IL_BaseScoreId;
        using WeightDistributionId = ::IL_WeightDistributionId;
        using Timestamp = ::IL_Timestamp;

        constexpr InteractorId EmptyInteractorId() { return (std::numeric_limits<InteractorId>::max)() - 1; }
        constexpr BaseScoreId EmptyBaseScoreId() { return (std::numeric_limits<BaseScoreId>::max)() - 1; }
        constexpr WeightDistributionId EmptyWeightDistributionId() { return (std::numeric_limits<WeightDistributionId>::max)() - 1; }

        template <typename T>
        const T& Empty();

        static_assert(IL_EMPTYINTERACTORID == EmptyInteractorId(), "EmptyInteractorId val mismatch C <-> C++");
        static_assert(IL_EMPTYBASESCOREID == EmptyBaseScoreId(), "EmptyInteractorId val mismatch C <-> C++");
        static_assert(IL_EMPTYWEIGHTDISTRIBUTIONID == EmptyWeightDistributionId(), "EmptyInteractorId val mismatch C <-> C++");

    }

#endif
