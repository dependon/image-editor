/*
 * Copyright (C) 2020 ~ 2021 Uniontech Software Technology Co., Ltd.
 *
 * Author:     LiuMingHang <liuminghang@uniontech.com>
 *
 * Maintainer: ZhangYong <ZhangYong@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "toast.h"



#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QHBoxLayout>
#include <QLabel>
#include <QIcon>
#include <DIconButton>
#include "imagebutton.h"
#include <dimagebutton.h>
#include "dthememanager.h"
#include "dgraphicsgloweffect.h"
#include "dhidpihelper.h"
#include "accessibility/ac-desktop-define.h"
DWIDGET_BEGIN_NAMESPACE



Toast::Toast(QWidget *parent) :
    QFrame(parent), DObject(*new ToastPrivate(this))
{
    D_D(Toast);
    setObjectName(TOAST_OBJECT);
    DThemeManager::registerWidget(this);

    d->initUI();
}

Toast::~Toast()
{

}

QString Toast::text() const
{
    D_DC(Toast);
    return d->textLabel->text();
}

QIcon Toast::icon() const
{
    D_DC(Toast);
    return d->icon;
}

qreal Toast::opacity() const
{
    D_DC(Toast);
    return d->effect->opacity();
}

void Toast::setText(QString text)
{
    D_D(Toast);
    d->textLabel->setVisible(true);
    d->textLabel->setText(text);
}

void Toast::setIcon(QString iconfile)
{
    D_D(Toast);
    d->icon = QIcon(iconfile);
    d->iconLabel->setVisible(true);
    d->iconLabel->setPixmap(DHiDPIHelper::loadNxPixmap(iconfile));
}

void Toast::setIcon(QIcon icon, QSize defaultSize)
{
    D_D(Toast);
    d->icon = icon;
    d->iconLabel->setVisible(true);
    d->iconLabel->setPixmap(d->icon.pixmap(icon.actualSize(defaultSize)));
}

void Toast::setOpacity(qreal opacity)
{
    D_D(Toast);
    d->effect->setOpacity(opacity);
    update();
}


ToastPrivate::ToastPrivate(Toast *qq)
    : DObjectPrivate(qq)
{

}

void ToastPrivate::initUI()
{
    D_Q(Toast);
    q->setWindowFlags(q->windowFlags() | Qt::WindowStaysOnTopHint);

    auto layout = new QHBoxLayout(q);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(12);

    iconLabel = new QLabel;
    iconLabel->setVisible(false);
    textLabel = new QLabel;
    textLabel->setVisible(false);
    //由于qrc路径变更,代码中使用也得变更
    closeBt = new ImageButton(":/common/images/input_clear_normal.svg",
                              ":/common/images/input_clear_hover.svg",
                              ":/common/images/input_clear_press.svg",
                              ":/common/images/input_clear_normal.svg");

    q->connect(closeBt, &ImageButton::clicked, q, [ = ]() {
        q->hide();
        q->setProperty("hide_by_user", true);
    });

    layout->addWidget(iconLabel);
    layout->addWidget(textLabel);
    layout->addStretch();
    layout->addWidget(closeBt);

    effect = new DGraphicsGlowEffect(q);
    effect->setBlurRadius(20.0);
    effect->setColor(QColor(0, 0, 0, 255 / 10));
    effect->setOffset(0, 0);
    q->setGraphicsEffect(effect);
    q->hide();
}


DWIDGET_END_NAMESPACE
