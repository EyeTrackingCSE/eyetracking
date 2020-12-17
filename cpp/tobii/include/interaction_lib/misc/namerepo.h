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

#include <unordered_map>


namespace IL
{



    template <typename TIdType, typename T>
    class NameRepo
    {
    public:
        TIdType AddOrGet(T idstr)
        {
            auto it = _nameIdMap.find(idstr);
            if (it != std::end(_nameIdMap))
            {
                return it->second;
            }

            auto id = NextId();
            _nameIdMap[idstr] = id;
            _idNameMap[id] = idstr;
            return id;
        }

        T& GetName(TIdType id)
        {
            auto it = _idNameMap.find(id);
            if (it != std::end(_idNameMap))
                return it->second;

            static T default_t;
            return default_t;
        }

        TIdType operator()(T idstr)
        {
            return AddOrGet(idstr);
        }

    private:
        static TIdType NextId()
        {
            static TIdType id = 0;
            return ++id;
        }

        std::unordered_map<TIdType, T> _idNameMap;
        std::unordered_map<T, TIdType> _nameIdMap;
    };


    template <typename T>
    using InteractorIdNameRepo = NameRepo<InteractorId, T>;

    template <typename T>
    using MaskIdNameRepo = NameRepo<MaskId, T>;

    template <typename T>
    using BaseScoreIdNameRepo = NameRepo<BaseScoreId, T>;

    template <typename T>
    using WeightDistributionIdNameRepo = NameRepo<WeightDistributionId, T>;

} // IL