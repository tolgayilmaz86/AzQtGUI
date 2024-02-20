/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <AzCore/PlatformIncl.h>
#include <AzQtComponents/Utilities/QtPluginPaths.h>
#include <QApplication>
#include <QDir>
#include <QSettings>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QJsonValue>

#define AZ_MAX_PATH_LEN 1024
#define AZ_CORRECT_FILESYSTEM_SEPARATOR '\\'

#if defined(__APPLE__)
// needed for _NSGetExecutablePath
#include <mach-o/dyld.h>
#include <libgen.h>
#include <unistd.h>
#endif

#if defined(AZ_PLATFORM_LINUX)
#include <libgen.h>
#include <unistd.h>
#endif

namespace AzQtComponents
{
  //! Enum used for the GetExecutablePath return type which indicates
  //! whether the function returned with a success value or a specific error
  enum class ExecutablePathResult : int8_t
  {
      Success,
      BufferSizeNotLargeEnough,
      GeneralError
  };
  //! Structure used to encapsulate the return value of GetExecutablePath
  //! Two pieces of information is returned.
  //! 1. Whether the executable path was able to be stored in the buffer.
  //! 2. If the executable path that was returned includes the executable filename
  struct GetExecutablePathReturnType
  {
      ExecutablePathResult m_pathStored{ExecutablePathResult::Success};
      bool m_pathIncludesFilename{};
  };

  GetExecutablePathReturnType GetExecutablePath(size_t exeStorageSize)
  {
    GetExecutablePathReturnType result;
    result.m_pathIncludesFilename = true;
    result.m_pathStored = ExecutablePathResult::GeneralError;

    wchar_t pathBufferW[AZ_MAX_PATH_LEN] = {0};
    const DWORD pathLen = GetModuleFileNameW(nullptr,
                                            pathBufferW,
                                            static_cast<DWORD>(sizeof(pathBufferW)));
    const DWORD errorCode = GetLastError();
    if (pathLen == exeStorageSize && errorCode == ERROR_INSUFFICIENT_BUFFER)
    {
        result.m_pathStored = ExecutablePathResult::BufferSizeNotLargeEnough;
    }
    else if (pathLen == 0 && errorCode != ERROR_SUCCESS)
    {
        result.m_pathStored = ExecutablePathResult::GeneralError;
    }
    else
    {
        result.m_pathStored = ExecutablePathResult::Success;
    }

    return result;
  }

    // the purpose of this function is to set up the QT globals so that it finds its platform libraries and that kind of thing.
    // these paths have to be set up BEFORE you create the Qt application itself, otherwise it won't know how to work on your current platform
    // since it will be missing the plugin for your current platform (windows/osx/etc)
    void PrepareQtPaths()
    {
#if !defined(USE_DEFAULT_QT_LIBRARY_PATHS)
        char executablePath[AZ_MAX_PATH_LEN];
        GetExecutablePathReturnType result = GetExecutablePath(AZ_MAX_PATH_LEN);
        if (result.m_pathStored == ExecutablePathResult::Success)
        {
            if (result.m_pathIncludesFilename)
            {
                char* lastSlashAddress = strrchr(executablePath, AZ_CORRECT_FILESYSTEM_SEPARATOR);
                if (lastSlashAddress == executablePath)
                {
                    executablePath[1] = '\0'; //Executable directory is root, therefore set the following character to \0
                }
                else
                {
                    *lastSlashAddress = '\0';
                }
            }
            QApplication::addLibraryPath(executablePath);
        }
        else
        {
            QApplication::addLibraryPath(".");
        }
#endif
    }
} // namespace AzQtComponents

