/*
* Copyright (C) 2020, KylinSoft Co., Ltd.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, see <http://www.gnu.org/licenses/&gt;.
*
*/

#include "titleBar.h"
#include <QApplication>
#include <iostream>
#include <QDBusMessage>
#include <QDBusConnection>

using namespace std;

TitleBar::TitleBar(QWidget *parent)
    : QDialog (parent)
    , style_settings (new QGSettings(ORG_UKUI_STYLE))
    , icon_theme_settings (new QGSettings(ORG_UKUI_STYLE))
    , m_logo (new QLabel())
    , m_logoMsg (new QLabel())
    , m_pMinimizeButton (new QPushButton())
    , m_pMaximizeButton (new QPushButton())
    , m_pCloseButton (new QPushButton())
    , m_pMenuButton(new QToolButton(this))
    , m_pMenu(new QMenu(this))
    , m_pAbout(new KYCAboutDialog(this))
    , pTitleLayout (new QHBoxLayout())
    , pButtonLayout (new QHBoxLayout())
    , pLayout (new QHBoxLayout())
{
    setFixedHeight(36);
    setMinimumWidth(860);

    stylelist << STYLE_NAME_KEY_DARK << STYLE_NAME_KEY_BLACK;
    iconthemelist << ICON_THEME_KEY_BASIC << ICON_THEME_KEY_CLASSICAL << ICON_THEME_KEY_DEFAULT;

    flagMaxWindow = false;

    m_logo->setFixedSize (24, 24);
    m_logo->setPixmap(QIcon::fromTheme("kylin-scanner").pixmap(m_logo->size()));

    QFont fontLogoMsg;
    fontLogoMsg.setPixelSize(14);
    m_logoMsg->setFont(fontLogoMsg);
    m_logoMsg->setText (tr("kylin-scanner"));
    m_logoMsg->setScaledContents(true);
    m_logoMsg->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

#if 0
    if ("ukui-icon-theme-basic" == icon_theme_settings->get(ICON_THEME_NAME).toString()) {
        m_logo->setStyleSheet("QLabel{border-image:url(/usr/share/icons/ukui-icon-theme-basic/24x24/apps/kylin-scanner.png);border-radius:4px;}");
    } else if ("ukui-icon-theme-classical" == icon_theme_settings->get(ICON_THEME_NAME).toString()) {
        m_logo->setStyleSheet("QLabel{border-image:url(/usr/share/icons/ukui-icon-theme-classical/24x24/apps/kylin-scanner.png);border-radius:4px;}");
    } else if ("ukui-icon-theme-default" == icon_theme_settings->get(ICON_THEME_NAME).toString()) {
        m_logo->setStyleSheet("QLabel{border-image:url(/usr/share/icons/ukui-icon-theme-default/24x24/apps/kylin-scanner.png);border-radius:4px;}");
    }
#endif

    // Menu : Help F1 | About | Exit
    m_pMenu->setMinimumWidth(160);
    m_pMenu->addAction(tr("Help"), this, [=](){
        QDBusMessage msg = QDBusMessage::createMethodCall( "com.kylinUserGuide.hotel_1000",
                                                        "/",
                                                        "com.guide.hotel",
                                                        "showGuide");
        msg << "kylin-scanner";
        QDBusMessage response = QDBusConnection::sessionBus().call(msg);
    }, QKeySequence(Qt::Key_F1));
    m_pMenu->addAction(tr("About"), this, [=](){
        m_pAbout->show();
    });
    m_pMenu->addAction(tr("Exit"), [=](){exit(0);} );

    m_pMenuButton->setIcon (QIcon::fromTheme (ICON_THEME_MENU));
    m_pMenuButton->setToolTip(tr("Minimize"));
    m_pMenuButton->setFixedSize(30, 30);
    m_pMenuButton->setIconSize (QSize(16, 16));
    m_pMenuButton->setPopupMode(QToolButton::InstantPopup);
    // 自定义标题栏窗口三联按钮
    m_pMenuButton->setProperty("isWindowButton", 0x1);
    m_pMenuButton->setProperty("useIconHighlightEffect", 0x2);
    m_pMenuButton->setAutoRaise(true);
    m_pMenuButton->setMenu(m_pMenu);

    m_pMinimizeButton->setIcon (QIcon::fromTheme (ICON_THEME_MINIMIZE));
    m_pMinimizeButton->setToolTip(tr("Minimize"));
    m_pMinimizeButton->setFixedSize(30, 30);
    m_pMinimizeButton->setIconSize (QSize(16, 16));
    // 自定义标题栏窗口三联按钮
    m_pMinimizeButton->setProperty("isWindowButton", 0x1);
    m_pMinimizeButton->setProperty("useIconHighlightEffect", 0x2);
    m_pMinimizeButton->setFlat(true);

    m_pMaximizeButton->setIcon (QIcon::fromTheme (ICON_THEME_MAXIMAZE));
    m_pMaximizeButton->setToolTip(tr("Maximize"));
    m_pMaximizeButton->setFixedSize(30, 30);
    m_pMaximizeButton->setIconSize (QSize(16, 16));
    m_pMaximizeButton->setProperty("isWindowButton", 0x1);
    m_pMaximizeButton->setProperty("useIconHighlightEffect", 0x2);
    m_pMaximizeButton->setFlat(true);

    m_pCloseButton->setIcon (QIcon::fromTheme (ICON_THEME_CLOSE));
    m_pCloseButton->setToolTip(tr("Close"));
    m_pCloseButton->setFixedSize(30, 30);
    m_pCloseButton->setIconSize (QSize(16, 16));
    m_pCloseButton->setProperty("isWindowButton", 0x2);
    m_pCloseButton->setProperty("useIconHighlightEffect", 0x8);
    m_pCloseButton->setFlat(true);

    pTitleLayout->addSpacing(0);
    pTitleLayout->addSpacing(8);
    pTitleLayout->addWidget (m_logo);
    pTitleLayout->addSpacing(0);
    //pTitleLayout->addSpacing(8);
    pTitleLayout->addWidget (m_logoMsg);
    pTitleLayout->setContentsMargins(0,8,0,0);

    pButtonLayout->addStretch();
    pButtonLayout->setSpacing(0);
    pButtonLayout->addWidget(m_pMenuButton);
    pButtonLayout->addSpacing(4);
    pButtonLayout->addWidget(m_pMinimizeButton);
    pButtonLayout->addSpacing(4);
    pButtonLayout->addWidget(m_pMaximizeButton);
    pButtonLayout->addSpacing(4);
    pButtonLayout->addWidget(m_pCloseButton);
    pButtonLayout->setAlignment(Qt::AlignCenter);
    pButtonLayout->setContentsMargins(0, 4, 4, 4);

    pLayout->addLayout(pTitleLayout);
    pLayout->addLayout(pButtonLayout);
    pLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(pLayout);

    connect(m_pMinimizeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(m_pMaximizeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(m_pCloseButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(style_settings,SIGNAL(changed(QString)),this,SLOT(titlebar_style_changed(QString)));
    connect(icon_theme_settings,SIGNAL(changed(QString)), this, SLOT(titlebar_icon_theme_changed(QString)));
}

TitleBar::~TitleBar()
{

}

void TitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    emit m_pMaximizeButton->clicked();
}

void TitleBar::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && mMoving)
    {
        QWidget *pWindow = this->window();
        if (pWindow->isWindow ())
        {
            bool bMaximize = pWindow->isMaximized();

            //qDebug() << "pWindow->pos() = " << pWindow->pos ();
            //qDebug() << "event->globalPos() = " << event->globalPos ();
            //qDebug() << "mLastMousePosition = " << mLastMousePosition;

            if (bMaximize)
            {
                m_pMaximizeButton->setProperty("maximizeProperty", "maximize");
                m_pMaximizeButton->setToolTip(tr("Maximize"));
                m_pMaximizeButton->setIcon (QIcon::fromTheme (ICON_THEME_MAXIMAZE));
                flagMaxWindow = false;

                pWindow->showNormal (); // 最大化时拖拽，窗口显示正常大小
            }
            else
            {
                // 当前窗口不是最大化窗口
                pWindow->move(pWindow->pos() + (event->globalPos() - mLastMousePosition));
                mLastMousePosition = event->globalPos();
            }
        }
    }
}

void TitleBar::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        QCursor cursor;
        cursor.setShape (Qt::ClosedHandCursor);
        QApplication::setOverrideCursor (cursor); // 使鼠标指针暂时改变形状

        mMoving = true;
        mLastMousePosition = event->globalPos();
    }
}

void TitleBar::mouseReleaseEvent(QMouseEvent *event)
{
    QApplication::restoreOverrideCursor (); // 恢复鼠标指针形状
    if (event->button() == Qt::LeftButton)
    {
        mMoving = false;
    }
}

bool TitleBar::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type())
    {
        case QEvent::WindowStateChange:
        case QEvent::Resize:
            updateMaximize();
            return true;
        default:
            break;
    }
    return QWidget::eventFilter(obj, event);
}

void TitleBar::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Escape:
        qDebug() << "3333333333333333" ;
        break;
    default:
        break;
    }
}

void TitleBar::onClicked()
{
    QPushButton *pButton = qobject_cast<QPushButton *>(sender());
    QWidget *pWindow = this->window();
    if (pWindow->isTopLevel())
    {
        if (pButton == m_pMinimizeButton)
        {
            pWindow->showMinimized();
        }
        else if (pButton == m_pMaximizeButton)
        {
            pWindow->isMaximized() ? pWindow->showNormal() : pWindow->showMaximized();
        }
        else if (pButton == m_pCloseButton)
        {
            pWindow->close();
        }
    }
}

void TitleBar::titlebar_icon_theme_changed(QString)
{
    qDebug() << "titlebar icon-theme: " << icon_theme_settings->get(ICON_THEME_NAME).toString();

    if ("ukui-icon-theme-basic" == icon_theme_settings->get(ICON_THEME_NAME).toString()) {
        m_logo->setStyleSheet("QLabel{border-image:url(/usr/share/icons/ukui-icon-theme-basic/24x24/apps/kylin-scanner.png);border-radius:4px;}");
    } else if ("ukui-icon-theme-classical" == icon_theme_settings->get(ICON_THEME_NAME).toString()) {
        m_logo->setStyleSheet("QLabel{border-image:url(/usr/share/icons/ukui-icon-theme-classical/24x24/apps/kylin-scanner.png);border-radius:4px;}");
    } else if ("ukui-icon-theme-default" == icon_theme_settings->get(ICON_THEME_NAME).toString()) {
        m_logo->setStyleSheet("QLabel{border-image:url(/usr/share/icons/ukui-icon-theme-default/24x24/apps/kylin-scanner.png);border-radius:4px;}");
    }
}

void TitleBar::titlebar_style_changed(QString)
{
    qDebug() << "titlebar_style_changed = " << style_settings->get (STYLE_NAME).toString ();
}

void TitleBar::updateMaximize()
{
    QWidget *pWindow = this->window();
    if (pWindow->isWindow ())
    {
        bool bMaximize = pWindow->isMaximized();
        if (bMaximize)
        {
            m_pMaximizeButton->setToolTip(tr("Restore"));
            m_pMaximizeButton->setProperty("maximizeProperty", "restore");
            m_pMaximizeButton->setIcon (QIcon::fromTheme (ICON_THEME_RESTORE));
            flagMaxWindow = true;
            emit isMax();
        }
        else
        {
            m_pMaximizeButton->setProperty("maximizeProperty", "maximize");
            m_pMaximizeButton->setToolTip(tr("Maximize"));
            m_pMaximizeButton->setIcon (QIcon::fromTheme (ICON_THEME_MAXIMAZE));
            flagMaxWindow = false;
            emit isNormal();
        }
    }
}
