#ifndef DEFINE_DIALOG_H
#define DEFINE_DIALOG_H

#include <QDialog>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QString>
#include <QFileInfoList>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QProcess>
#include <QObject>
#include <QUuid>
#include <QTableWidget>
#include <QClipboard>
#include <QTextCursor>
#include <QPlainTextEdit>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QDateTime>
#include <QResource>
#include <QApplication>

namespace Ui {
class DefineDialog;
}

class MainWindow;

class DefineDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DefineDialog(QWidget *parent = 0);
    ~DefineDialog();
    MainWindow *mainUi;
    void reloadSource(const QString &a_filename);

private slots:
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);

    void on_buttonBrowseSource_clicked();
    void on_buttonMake_clicked();

private:
    Ui::DefineDialog *ui;
};

#endif // DEFINE_DIALOG_H
