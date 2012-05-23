/****************************************************************************
*  Copyright (C) 2011 Rapha�l MARQUES <work.rmarques@gmail.com>
*
*  This program is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/

/** --------------------------------------------------------------------------------------------------------------- **/

#ifndef CONFIGPAGES_HPP
#define CONFIGPAGES_HPP

/** --------------------------------------------------------------------------------------------------------------- **/

#include <qevent.h>
#include <qstyle.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qwidget.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qgroupbox.h>
#include <qcheckbox.h>
#include <qtoolbutton.h>
#include <qpushbutton.h>
#include <qfiledialog.h>
#include <qcolordialog.h>
#include <qradiobutton.h>
#include <qtextstream.h>

#include <qdebug.h>

/** --------------------------------------------------------------------------------------------------------------- **/

class ConfigBar : public QLineEdit
{
    Q_OBJECT

public:

    ConfigBar(const QIcon & icon, const QString & lineText = "", const QString & tip = "", const QString & path = "", QWidget * lineParent = 0) : QLineEdit(lineText, lineParent) {
        this -> appPath = path;
        this -> clearButton = new QToolButton(this);
        this -> clearButton -> setIcon(QIcon(this -> appPath + "/utils/icons/clear.png"));
        this -> clearButton -> setIconSize(QSize(16, 16));
        this -> clearButton -> setCursor(Qt::PointingHandCursor);
        this -> clearButton -> setStyleSheet("QToolButton {border: none; padding: 0px;}");
        this -> clearButton -> hide();
        this -> clearButton -> setToolTip(tr("Clean the field"));

        this -> iconButton = new QToolButton(this);
        this -> iconButton -> setIcon(icon);
        this -> iconButton -> setCursor(Qt::PointingHandCursor);
        this -> iconButton -> setIconSize(QSize(16, 16));
        this -> iconButton -> setStyleSheet("QToolButton {border: none; padding: 0px;}");
        this -> iconButton -> setToolTip(tip);

        this -> connect(this -> clearButton, SIGNAL(clicked()), this, SLOT(clear()));
        this -> connect(this, SIGNAL(textChanged(const QString &)), this, SLOT(updateCloseButton(const QString &)));

        int frameWidth = style() -> pixelMetric(QStyle::PM_DefaultFrameWidth);
        this -> setStyleSheet(QString("QLineEdit {padding-right: %1px; padding-left: %1px;} ").arg(this -> clearButton -> sizeHint().width() + frameWidth + 1));

        QSize miniSize = minimumSizeHint();

        this -> setMinimumSize(qMax(miniSize.width(), this -> clearButton -> sizeHint().height() + frameWidth * 2 + 2),
                               qMax(miniSize.height(), this -> clearButton -> sizeHint().height() + frameWidth * 2 + 2));
    }

    ~ConfigBar() {}

    QToolButton * iconButton;
    QString appPath;

    /** --------------------------------------------------------------------------------------------------------------- **/

protected:

    void resizeEvent(QResizeEvent * event) {
        QSize size = this -> clearButton -> sizeHint();
        int frameWidth = style() -> pixelMetric(QStyle::PM_DefaultFrameWidth);
        this -> clearButton -> move(rect().right() - frameWidth - size.width(), (rect().bottom() + 1 - size.height()) / 2);
        this -> iconButton -> move(5, (rect().bottom() + 1 - size.height()) / 2);
    }

    /** --------------------------------------------------------------------------------------------------------------- **/

private slots:

    void updateCloseButton(const QString & text) {
        this -> clearButton -> setVisible(!text.isEmpty());
    }

    /** --------------------------------------------------------------------------------------------------------------- **/

private:

    QToolButton * clearButton;

};

/** --------------------------------------------------------------------------------------------------------------- **/
/** --------------------------------------------------------------------------------------------------------------- **/
/** --------------------------------------------------------------------------------------------------------------- **/

class GlobalPage : public QWidget
{

    Q_OBJECT

public:

    GlobalPage(QWidget * parent = 0) : QWidget(parent) {
        QGroupBox * configGroup = new QGroupBox(tr("Global Settings"), this);

        QLabel * lblang = new QLabel(tr("Language:"), this);
        this -> language = new QComboBox(this);
        language -> addItems(QStringList() << tr("English") << tr("French") << tr("Deutch") << tr("Spanish"));
        //this -> connect(language, SIGNAL(currentIndexChanged(int)), this, SLOT(selectLang(int)));

        QLabel * lbiphone = new QLabel(tr("iPhone version:"), this);
        this -> iphones = new QComboBox(this);
        iphones -> addItems(QStringList() << "3G" << "3GS" << "4" << "4S" << "5");

        QLabel * lbl  = new QLabel(tr("When application starts:"), this);
        this -> vault = new QRadioButton(tr("Load latest backup from the Vault"), this);
        this -> apple = new QRadioButton(tr("Load latest iTunes iPhone Backup"), this);
        this -> nada  = new QRadioButton(tr("Do nothing"), this);

        QVBoxLayout * loadLayout = new QVBoxLayout;
        loadLayout -> addWidget(lbl);
        loadLayout -> addWidget(vault);
        loadLayout -> addWidget(apple);
        loadLayout -> addWidget(nada);

        QGridLayout * configLayout = new QGridLayout();
        configLayout -> addWidget(lblang, 0, 0);
        configLayout -> addWidget(language, 0, 1);
        configLayout -> addWidget(lbiphone, 1, 0);
        configLayout -> addWidget(iphones, 1, 1);
        configLayout -> addLayout(loadLayout, 2, 0);
        configGroup -> setLayout(configLayout);

        configLayout -> setSpacing(20);
        loadLayout -> setSpacing(6);

        QVBoxLayout * mainLayout = new QVBoxLayout();
        mainLayout -> addWidget(configGroup);
        mainLayout -> addStretch(1);
        this -> setLayout(mainLayout);
    }

    QRadioButton * vault, * apple, * nada;
    QComboBox * iphones, * language;
};

/** --------------------------------------------------------------------------------------------------------------- **/
/** --------------------------------------------------------------------------------------------------------------- **/
/** --------------------------------------------------------------------------------------------------------------- **/

class SMSPage : public QWidget
{

    Q_OBJECT

public:

    ConfigBar * BGWay, * BGC;

    SMSPage(QWidget * parent = 0, const QString & path = "") : QWidget(parent) {
        QGroupBox * configGroup = new QGroupBox(tr("SMS Settings"), this);

        QLabel * BGILbl = new QLabel(tr("Background image:"), this),
               * BGCLbl = new QLabel(tr("Background color:"), this),
               * info   = new QLabel(tr("The background image is prior than the color.\nIf you only want the color, reset the image field."), this),
               * pix    = new QLabel(this);

        pix -> setPixmap(QPixmap(path + "/utils/icons/information.png"));

        this -> BGWay = new ConfigBar(QIcon(path + "/utils/icons/field.png"), "", tr("Select a file on your computer"), path, this);
        this -> BGC   = new ConfigBar(QIcon(path + "/utils/icons/color.png"), "", tr("Select a color"),                 path, this);

        this -> connect(this -> BGWay -> iconButton, SIGNAL(clicked()), this, SLOT(getPath()));
        this -> connect(this -> BGC -> iconButton,   SIGNAL(clicked()), this, SLOT(getColor()));

        QGridLayout * configLayout = new QGridLayout();
        configLayout -> addWidget(BGCLbl, 0, 0);
        configLayout -> addWidget(BGC, 0, 1);
        configLayout -> addWidget(BGILbl, 1, 0);
        configLayout -> addWidget(BGWay, 1, 1);
        configLayout -> addWidget(pix, 2, 0);
        configLayout -> addWidget(info, 2, 1);
        configLayout -> setSpacing(20);
        configGroup -> setLayout(configLayout);

        QVBoxLayout * mainLayout = new QVBoxLayout();
        mainLayout -> addWidget(configGroup);
        mainLayout -> addStretch(1);
        this -> setLayout(mainLayout);
    }

    /** --------------------------------------------------------------------------------------------------------------- **/

private slots:

    void getPath(){
        QString fileName(QFileDialog::getOpenFileName(this, tr("Open a background image"), QDir::homePath(), tr("Images (*.png *.jpg *.bmp)")));
        if(!fileName.isEmpty()) this -> BGWay -> setText(fileName);
    }

    /** --------------------------------------------------------------------------------------------------------------- **/

    void getColor(){
        QColor color(QColorDialog::getColor(QColor("#dbe2ed"), this, tr("Select a background color")));
        if (color.isValid()) {
            this -> BGC -> setText(color.name());
            this -> BGC -> setPalette(QPalette(color));
            this -> BGC -> setAutoFillBackground(true);
        }
    }
};

/** --------------------------------------------------------------------------------------------------------------- **/
/** --------------------------------------------------------------------------------------------------------------- **/
/** --------------------------------------------------------------------------------------------------------------- **/

class ContactsPage : public QWidget
{

    Q_OBJECT

public:

    QCheckBox * box;
    QRadioButton * _first_last, * _last_first;

    ContactsPage(QWidget * parent = 0) : QWidget(parent) {
        QGroupBox * configGroup = new QGroupBox(tr("Contacts Settings"), this);

        QLabel * Lbl        = new QLabel(tr("Sort contacts by:"),    this);
        this -> box         = new QCheckBox(tr("Display research bar"),  this);
        this -> _first_last = new QRadioButton(tr("Firstname Lastname"), this);
        this -> _last_first = new QRadioButton(tr("Lastname Firstname"), this);

        QVBoxLayout * loadLayout = new QVBoxLayout;
        loadLayout -> addWidget(Lbl);
        loadLayout -> addWidget(_last_first);
        loadLayout -> addWidget(_first_last);

        QGridLayout * configLayout = new QGridLayout();
        configLayout -> addWidget(box, 0, 0);
        configLayout -> addLayout(loadLayout, 1, 0);
        configGroup -> setLayout(configLayout);

        configLayout -> setSpacing(20);
        loadLayout -> setSpacing(6);

        QVBoxLayout * mainLayout = new QVBoxLayout();
        mainLayout -> addWidget(configGroup);
        mainLayout -> addStretch(1);
        this -> setLayout(mainLayout);
    }
};

/** --------------------------------------------------------------------------------------------------------------- **/
/** --------------------------------------------------------------------------------------------------------------- **/
/** --------------------------------------------------------------------------------------------------------------- **/

class PicturesPage : public QWidget
{

    Q_OBJECT

public:

    QCheckBox * box;
    QRadioButton * _first_last, * _last_first;
    ConfigBar * BGC;

    PicturesPage(QWidget * parent = 0, const QString & path = "") : QWidget(parent) {
        QGroupBox * configGroup = new QGroupBox(tr("Pictures Settings"), this);

        QLabel * Lbl        = new QLabel(tr("Sort contacts by:"),    this),
               * BGCLbl     = new QLabel(tr("Background color:"),    this);
        this -> box         = new QCheckBox(tr("Display research bar"),  this);
        this -> _first_last = new QRadioButton(tr("Firstname Lastname"), this);
        this -> _last_first = new QRadioButton(tr("Lastname Firstname"), this);
        this -> BGC         = new ConfigBar(QIcon(path + "/utils/icons/color.png"), "", tr("Select a color"), path, this);

        this -> connect(this -> BGC -> iconButton, SIGNAL(clicked()), this, SLOT(getColor()));

        QVBoxLayout * loadLayout = new QVBoxLayout;
        loadLayout -> addWidget(Lbl);
        loadLayout -> addWidget(_last_first);
        loadLayout -> addWidget(_first_last);

        QGridLayout * configLayout = new QGridLayout();
        configLayout -> addWidget(BGCLbl, 0, 0);
        configLayout -> addWidget(BGC, 0, 1);
        configLayout -> addWidget(box, 1, 0, 1, 2);
        configLayout -> addLayout(loadLayout, 2, 0, 1, 2);
        configGroup -> setLayout(configLayout);

        configLayout -> setSpacing(20);
        loadLayout -> setSpacing(6);

        QVBoxLayout * mainLayout = new QVBoxLayout();
        mainLayout -> addWidget(configGroup);
        mainLayout -> addStretch(1);
        this -> setLayout(mainLayout);
    }

    /** --------------------------------------------------------------------------------------------------------------- **/

private slots:

    void getColor(){
        QColor color(QColorDialog::getColor(QColor("#dbe2ed"), this, tr("Select a background color")));
        if (color.isValid()) {
            this -> BGC -> setText(color.name());
            this -> BGC -> setPalette(QPalette(color));
            this -> BGC -> setAutoFillBackground(true);
        }
    }
};

/** --------------------------------------------------------------------------------------------------------------- **/
/** --------------------------------------------------------------------------------------------------------------- **/
/** --------------------------------------------------------------------------------------------------------------- **/

class ThemePage : public QWidget
{

    Q_OBJECT

public:

    QString appPath;
    QComboBox * select;

    ThemePage(QWidget * parent = 0, const QString & path = "") : QWidget(parent) {
        this -> appPath = path;
        QGroupBox * configGroup = new QGroupBox(tr("Themes Settings"), this);

        QLabel * Lbl = new QLabel(tr("Select a theme:"), this);
        this -> select = new QComboBox(this);
        this -> select -> addItems(QStringList()
                                   << "None"
                                   << "CDE"
                                   << "Cleanlooks"
                                   << "Darkness"
                                   << "Motif"
                                   << "Plastic"
                                   << "Windows"
                                   << "Windows Vista/7"
                                   << "WindowsXP");

        QGridLayout * configLayout = new QGridLayout();
        configLayout -> addWidget(Lbl, 0, 0);
        configLayout -> addWidget(select, 0, 1);
        configGroup -> setLayout(configLayout);

        QVBoxLayout * mainLayout = new QVBoxLayout();
        mainLayout -> addWidget(configGroup);
        mainLayout -> addStretch(1);
        this -> setLayout(mainLayout);
    }

    /** --------------------------------------------------------------------------------------------------------------- **/

    QString getStyleSheet() {
        QString content(""), path(this -> qss(this -> select -> currentText()));
        QFile css(this -> appPath + "/utils/" + path, this);
        if(css.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream stream(&css);
            while(!stream.atEnd())
                content += stream.readLine();
            css.close();
        } else if(!path.isEmpty())
            qDebug() << tr("Cannot open ") + path + " !";
        return content;
    }

    /** --------------------------------------------------------------------------------------------------------------- **/

    QString qss(const QString & skin) {
        if(skin == "Darkness")
            return "dark.qss";
        else return "";
    }
};

/** --------------------------------------------------------------------------------------------------------------- **/

#endif // CONFIGPAGES_HPP

/** --------------------------------------------------------------------------------------------------------------- **/
