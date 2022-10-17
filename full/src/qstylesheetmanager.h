#pragma once

#include <QWidget>

class QStyleSheetManager
{
public:
        QStyleSheetManager(void);
        ~QStyleSheetManager(void);

    static void loadStyleSheet(const QString& sheet_name);
    static void loadStyleSheet(const QString& sheet_name, QWidget* widget);
};
