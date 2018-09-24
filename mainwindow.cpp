#include "mainwindow.h"
#include "define_dialog.h"
#include "ui_mainwindow.h"
#include "version.hpp"

#define DEFAULT_FONT_SIZE 9

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setup();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setup(void)
{
    loadFonts();
    loadFontsMeta();
    loadFontsPreview();
    ui->actionMakeDefine->setIcon(QIcon(BRANDS_MARKDOWN));
    ui->actionAbout->setIcon(QIcon(SOLID_INFO_CIRCLE));
    ui->actionAbout->connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(slotAbout()));
    ui->actionMakeDefine->connect(ui->actionMakeDefine, SIGNAL(triggered()), this, SLOT(slotMake()));
}

void MainWindow::loadFonts(void)
{
    static QList<QPair<QFont*, QString>> fontNames = {
        { &m_iconFontBrandsRegular, ":/otfs/Font Awesome 5 Brands-Regular-400.otf" },
        { &m_iconFontFreeRegular,   ":/otfs/Font Awesome 5 Free-Regular-400.otf"   },
        { &m_iconFontFreeSolid,     ":/otfs/Font Awesome 5 Free-Solid-900.otf"     },
    };

    enum {
        IconFontBrands,
        IconFontRegular,
        IconFontSolid,
        IconFontMax,
    };

    for(int i = 0; i < fontNames.length(); i++)
    {
        int fontId = QFontDatabase::addApplicationFont(fontNames.at(i).second);
        QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);

        if(fontFamilies.isEmpty())
        {
            continue;
        }
        if(i == IconFontBrands)
        {
            m_iconFontBrandsRegular = QFont(fontFamilies.first());
        }
        else if(i == IconFontRegular)
        {
            m_iconFontFreeRegular = QFont(fontFamilies.first());
        }
        else if(i == IconFontSolid)
        {
            m_iconFontFreeSolid = QFont(fontFamilies.first());
        }
        fontNames.at(i).first->setPointSize(DEFAULT_FONT_SIZE);
    }
}

void MainWindow::loadFontsMeta(const QString &a_filename)
{
    QString jsonString = Common::getFileText(a_filename);
    QJsonObject json = Common::getJsonFromString(jsonString);

    for(QJsonObject::iterator i = json.begin(); i != json.end(); i++)
    {
        static QMap<QString, QFont> fontstyles = {
            { "brands",  m_iconFontBrandsRegular },
            { "regular", m_iconFontFreeRegular   },
            { "solid",   m_iconFontFreeSolid     },
        };

        QString label = i.key();
        QString unicode = i.value().toObject().value("unicode").toString();

        for(QJsonValue arrayValue : i.value().toObject().value("styles").toArray())
        {
            QString style = arrayValue.toString();
            bool ok = false;
            QChar unicodeChar = unicode.toInt(&ok, 16);

            if(ok)
            {
                m_iconFontPreview.insert(QString("%1-%2").arg(style).arg(label), {style, label, unicode, unicodeChar, fontstyles[style]});
            }
            else
            {
                qDebug() << QString("Unicode(0x%1) convert to char error!").arg(unicode);
            }

            m_iconFontDefind.insert(QString("%1-%2").arg(style).arg(label), {style, label, unicode});
            qDebug() << QString("label=%1,unicode=0x%2,styles=%3").arg(label).arg(unicode).arg(style);
        }
    }
}

void MainWindow::loadFontsPreview(void)
{
    enum {
        PreviewStyle,
        PreviewLabel,
        PreviewUnicode,
        PreviewUnicodeChar,
        PreviewFont,
        PreviewMax,
    };

    qDeleteAll(labs);
    labs.clear();

    uint count = 0;
    for(QMap<QString, QList<QVariant>>::iterator i = m_iconFontPreview.begin(); i != m_iconFontPreview.end(); i++)
    {
        QString key = i.key();
        QList<QVariant> value = i.value();
        QLabel *lab = new QLabel();

        Q_UNUSED(key)
        value.at(PreviewFont).value<QFont>().setPixelSize(15);
        lab->setAlignment(Qt::AlignCenter);
        lab->setFont(value.at(PreviewFont).value<QFont>());
        lab->setText(value.at(PreviewUnicodeChar).toChar());
        lab->setToolTip(QString("Styles=%1\nLabel=%2\nUnicode=0x%3\nFont=%4\nCount=%5").arg(value.at(PreviewStyle).toString()).arg(value.at(PreviewLabel).toString()).arg(value.at(PreviewUnicode).toString()).arg(value.at(PreviewFont).value<QFont>().family()).arg(++count));
        lab->setMinimumSize(30, 30);
        lab->setMaximumSize(30, 30);
        labs.append(lab);
    }

    ui->widgetFontPanel->setWidget(labs, 50);
    ui->widgetFontPanel->setMargin(0);
    ui->widgetFontPanel->setSpacing(0);
}

void MainWindow::slotAbout(void)
{
    QMessageBox::about(this, tr("Font Awesome %1").arg(Version::c_font_version), Common::getFileText(":/LICENSE.txt"));
}

void MainWindow::slotMake(void)
{
    DefineDialog dialog;

    dialog.mainUi = this;
    dialog.exec();
}

void MainWindow::slotMetaDefine(void)
{
    enum {
        DefindStyle,
        DefindLabel,
        DefindUnicode,
        DefindMax,
    };

    static const char *c_fontawesome_unicode_define_header_filename = "fontawesome_define_unicode.h";

    QStringList texts;

    texts << "#ifndef FONTAWESOME_DEFINE_UNICODE_H";
    texts << "#define FONTAWESOME_DEFINE_UNICODE_H";
    texts << QString();

    uint count = 0;
    for(QMap<QString, QList<QString>>::iterator i = m_iconFontDefind.begin(); i != m_iconFontDefind.end(); i++)
    {
        QString key = i.key();
        QString unicode = i.value().at(DefindUnicode);

        texts << QString("#define %1_UNICODE 0x%2 // count=%3").arg(key.replace("-", "_").toUpper()).arg(unicode).arg(++count);
    }

    texts << QString();
    texts << "#endif // FONTAWESOME_DEFINE_UNICODE_H";

    if(!Common::setFileText(c_fontawesome_unicode_define_header_filename, texts.join("\r\n")))
    {
        qDebug() << QString("file: %1 saved!").arg(c_fontawesome_unicode_define_header_filename);
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    static const char *c_fontawesome_filename_define_header_filename = "fontawesome_define.h";

    texts.clear();

    texts << "#ifndef FONTAWESOME_DEFINE_H";
    texts << "#define FONTAWESOME_DEFINE_H";
    texts << QString();

    texts << "#define PREFIX \":/svgs/\"";
    texts << "#define CONCAT(__x1, __x2) __x1 ## __x2";
    texts << QString();

    count = 0;
    for(QMap<QString, QList<QString>>::iterator i = m_iconFontDefind.begin(); i != m_iconFontDefind.end(); i++)
    {
        QString key = i.key();
        QString style = i.value().at(DefindStyle);
        QString label = i.value().at(DefindLabel);

        texts << QString("#define %1 CONCAT(PREFIX, \"%2/%3.svg\") // count=%4").arg(key.replace("-", "_").toUpper()).arg(style).arg(label).arg(++count);
    }

    texts << QString();
    texts << "#endif // FONTAWESOME_DEFINE_H";

    if(!Common::setFileText(c_fontawesome_filename_define_header_filename, texts.join("\r\n")))
    {
        qDebug() << QString("file: %1 saved!").arg(c_fontawesome_filename_define_header_filename);
    }
}

