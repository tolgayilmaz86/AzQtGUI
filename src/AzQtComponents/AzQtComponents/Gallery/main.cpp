/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <AzCore/PlatformIncl.h>

#include <AzQtComponents/Components/GlobalEventFilter.h>
#include <AzQtComponents/Components/StyledDockWidget.h>
#include <AzQtComponents/Components/O3DEStylesheet.h>
#include <AzQtComponents/Utilities/HandleDpiAwareness.h>
#include <AzQtComponents/Components/WindowDecorationWrapper.h>
#include "ComponentDemoWidget.h"

#include <QApplication>
#include <QMainWindow>
#include <QSettings>

#include <iostream>

int main(int argc, char **argv)
{
    //const AZ::Debug::Trace tracer;
    //ComponentApplicationWrapper componentApplicationWrapper;

    QApplication::setOrganizationName("O3DE");
    QApplication::setOrganizationDomain("o3de.org");
    QApplication::setApplicationName("O3DEWidgetGallery");

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    //qInstallMessageHandler(LogToDebug);

    AzQtComponents::Utilities::HandleDpiAwareness(AzQtComponents::Utilities::PerScreenDpiAware);
    QApplication app(argc, argv);

    auto globalEventFilter = new AzQtComponents::GlobalEventFilter(&app);
    app.installEventFilter(globalEventFilter);

    AzQtComponents::StyleManager styleManager(&app);
    std::string_view engineRootPath;
    //if (auto settingsRegistry = AZ::SettingsRegistry::Get(); settingsRegistry != nullptr)
    //{
    //    settingsRegistry->Get(engineRootPath, AZ::SettingsRegistryMergeUtils::FilePathKey_EngineRootFolder);
    //}
    styleManager.initialize(&app, engineRootPath);

    auto wrapper = new AzQtComponents::WindowDecorationWrapper(AzQtComponents::WindowDecorationWrapper::OptionNone);
    auto widget = new ComponentDemoWidget(wrapper);
    wrapper->setGuest(widget);
    widget->resize(550, 900);
    widget->show();

    wrapper->enableSaveRestoreGeometry("windowGeometry");
    wrapper->restoreGeometryFromSettings();

    QObject::connect(widget, &ComponentDemoWidget::refreshStyle, &styleManager, [&styleManager]() {
        styleManager.Refresh();
    });

    app.setQuitOnLastWindowClosed(true);

    app.exec();
}
