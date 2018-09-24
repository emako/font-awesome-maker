#include "define_dialog.h"
#include "mainwindow.h"
#include "ui_define_dialog.h"

DefineDialog::DefineDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DefineDialog)
{
    ui->setupUi(this);
    ui->buttonMake->setIcon(QIcon(BRANDS_MARKDOWN));
}

DefineDialog::~DefineDialog()
{
    delete ui;
}

void DefineDialog::on_buttonBrowseSource_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select File"), NULL, "Json File (*.json)");

    if(fileName.isEmpty())
    {
        return;
    }
    reloadSource(fileName);
}

void DefineDialog::on_buttonMake_clicked()
{
    mainUi->loadFontsMeta(ui->editSource->text());
    mainUi->slotMetaDefine();
}

void DefineDialog::dragEnterEvent(QDragEnterEvent* e)
{
    if(e->mimeData()->hasText())
    {
        e->acceptProposedAction();
    }
}

void DefineDialog::dropEvent(QDropEvent* e)
{
    const QMimeData* mime = e->mimeData();

    if(mime->hasUrls())
    {
        foreach(QUrl url, e->mimeData()->urls())
        {
            QFileInfo file(url.toLocalFile());
            QString filename = file.canonicalFilePath();
            filename = QDir::toNativeSeparators(filename);

            if(!QFileInfo(filename).isFile())
            {
                break;
            }
            reloadSource(filename);
            break;
        }
    }
    e->acceptProposedAction();
}

void DefineDialog::reloadSource(const QString &a_filename)
{
    ui->editSource->setText(a_filename);
}
