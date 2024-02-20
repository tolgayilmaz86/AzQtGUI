/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <AzQtComponents/Utilities/HandleDpiAwareness.h>

#include <AzCore/PlatformIncl.h>
#include <array>
#include <string>

namespace AzQtComponents
{
    namespace Utilities
    {
        namespace Platform
        {
            void HandleDpiAwareness(DpiAwareness dpiAwareness)
            {
                // We need to force the dpi awareness settings on Windows based on the version.
                // Qt doesn't currently expose a mechanism to do that other than via command line arguments

                std::string qpaArg = "QT_QPA_PLATFORM=windows:fontengine=freetype,";

                switch (dpiAwareness)
                {
                    case Unaware:
                        qpaArg += std::to_string(qpaArg.size()) + "dpiawareness=0";
                        break;
                    case SystemDpiAware:
                        qpaArg +=  std::to_string(qpaArg.size()) + "dpiawareness=1";
                        break;
                    case PerScreenDpiAware:     // intentional fall-through (setting defaults to 2)
                    case Unset:                 // intentional fall-through
                    default:
                        qpaArg += std::to_string(qpaArg.size()) + "dpiawareness=2";
                        break;
                }

                _putenv(qpaArg.c_str());
            }
        } // namespace Platform
    } // namespace Utilities
} // namespace AzQtComponents
