/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */
#pragma once

#if !defined(Q_MOC_RUN)
#include <AzQtComponents/AzQtComponentsAPI.h>
#include <chrono>
//#include <AzCore/Memory/SystemAllocator.h>
//#include <AzCore/std/chrono/chrono.h>
#include <QString>
#endif

namespace AzQtComponents
{
    enum class ToastType
    {
        Information,
        Warning,
        Error,
        Custom
    };

    class AZ_QT_COMPONENTS_API ToastConfiguration
    {
    public:
        ToastConfiguration(ToastType toastType, const QString& title, const QString& description);

        bool m_closeOnClick = true;
        bool m_allowDuplicateNotifications = false;

        ToastType m_toastType = ToastType::Information;

        QString m_title;
        QString m_description;
        QString m_customIconImage;
        uint32_t m_borderRadius = 0;

        AZ_PUSH_DISABLE_DLL_EXPORT_MEMBER_WARNING
        std::chrono::milliseconds m_duration = std::chrono::milliseconds(5000);
        std::chrono::milliseconds m_fadeDuration = std::chrono::milliseconds(250);
        AZ_POP_DISABLE_DLL_EXPORT_MEMBER_WARNING
    };
} // namespace AzQtComponents
