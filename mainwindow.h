#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "common.h"
#include "form/panelwidget.h"
#include "fontawesome_define.h"
#include "fontawesome_define_unicode.h"

#include <QMainWindow>
#include <QDebug>
#include <QFontDatabase>
#include <QLabel>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}
class DefineDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    friend class DefineDialog;

public slots:
    void slotAbout(void);
    void slotMake(void);
    void slotMetaDefine(void);

private:
    void setup(void);
    void loadFonts(void);
    void loadFontsMeta(const QString &a_filename = ":/metadata/icons.json");
    void loadFontsPreview(void);

private:
    Ui::MainWindow *ui;
    QFont m_iconFontBrandsRegular;
    QFont m_iconFontFreeRegular;
    QFont m_iconFontFreeSolid;
    QMap<QString, QList<QString>> m_iconFontDefind;
    QMap<QString, QList<QVariant>> m_iconFontPreview;
    QList<QWidget *> labs;
};

#endif // MAINWINDOW_H
