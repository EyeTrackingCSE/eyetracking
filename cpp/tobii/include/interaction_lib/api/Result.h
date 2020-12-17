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

typedef enum IL_Result
{
    IL_Result_Error_Unknown = -1000,
    IL_Result_Error_StreamTypeNotAvailable,
    IL_Result_Error_NotInTransaction,
    IL_Result_Error_UnknownInteractorId,
    IL_Result_Error_UnknownWeightDistributionId,
    IL_Result_Error_CustomWeightDistributionInUse,
    IL_Result_Error_InvalidParamValue,
    IL_Result_Error_NoAdapterSet,
    IL_Result_Error_AllocationFailed,
    IL_Result_Error_MAX,
    
    IL_Result_Ok = 0,

    IL_Result_Warning_NotInTransaction,
    IL_Result_Warning_AlreadyInTransaction,
    IL_Result_Warning_UnknownInteractorId,
    IL_Result_Warning_UnknownWeightDistributionId,
    IL_Result_Warning_InsufficientInformationForCoordinateTransform,
    IL_Result_Warning_NoAdapterSet,
    IL_Result_Warning_NoDeviceAvailable,
    IL_Result_Warning_Timeout,
    IL_Result_Warning_MAX,
} IL_Result;

#ifdef __cplusplus
namespace IL
{
    enum class Result
    {
        Error_Unknown =                                               ::IL_Result_Error_Unknown,
        Error_StreamTypeNotAvailable =                                ::IL_Result_Error_StreamTypeNotAvailable,
        Error_NotInTransaction =                                      ::IL_Result_Error_NotInTransaction,
        Error_UnknownInteractorId =                                   ::IL_Result_Error_UnknownInteractorId,
        Error_UnknownWeightDistributionId =                           ::IL_Result_Error_UnknownWeightDistributionId,
        Error_CustomWeightDistributionInUse =                         ::IL_Result_Error_CustomWeightDistributionInUse,
        Error_InvalidParamValue =                                     ::IL_Result_Error_InvalidParamValue,
        Error_NoAdapterSet =                                          ::IL_Result_Error_NoAdapterSet,
        Error_AllocationFailed =                                      ::IL_Result_Error_AllocationFailed,

        Ok =                                                         ::IL_Result_Ok,
        
        Warning_NotInTransaction =                                    ::IL_Result_Warning_NotInTransaction,
        Warning_AlreadyInTransaction =                                ::IL_Result_Warning_AlreadyInTransaction,
        Warning_UnknownInteractorId =                                 ::IL_Result_Warning_UnknownInteractorId,
        Warning_UnknownWeightDistributionId =                         ::IL_Result_Warning_UnknownWeightDistributionId,
        Warning_InsufficientInformationForCoordinateTransform =       ::IL_Result_Warning_InsufficientInformationForCoordinateTransform,
        Warning_NoAdapterSet =                                        ::IL_Result_Warning_NoAdapterSet,
        Warning_NoDeviceAvailable =                                   ::IL_Result_Warning_NoDeviceAvailable,
        Warning_Timeout =                                             ::IL_Result_Warning_Timeout,
    };
}
#endif
