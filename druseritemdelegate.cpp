/*
    Deriv is a cross-platform client for th Wired 2.0 protocol
    Copyright (C) 2014  Rafael Warnault, rw@read-write.fr

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#include <QApplication>
#include "druseritemdelegate.h"


DRUserItemDelegate::DRUserItemDelegate()
{
}

DRUserItemDelegate::~DRUserItemDelegate()
{
// TODO Auto-generated destructor stub
}

//alocate each item size in listview.
QSize DRUserItemDelegate::sizeHint(const QStyleOptionViewItem &  option ,
                              const QModelIndex & index) const
{
    QIcon icon = qvariant_cast<QIcon>(index.data(IconRole));
    QSize iconsize = icon.actualSize(option.decorationSize);
    QFont font = QApplication::font();
    QFontMetrics fm(font);

    return(QSize(iconsize.width(),iconsize.height()+fm.height() +4 ));

}
void DRUserItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                           const QModelIndex &index) const
 {
    QStyledItemDelegate::paint(painter,option,index);

    painter->save();

    QFont font = QApplication::font();
    QFont SubFont = QApplication::font();
    //font.setPixelSize(font.weight()+);
    font.setBold(true);
    SubFont.setWeight(SubFont.weight()-2);
    QFontMetrics fm(font);

    QIcon icon = qvariant_cast<QIcon>(index.data(IconRole));
    QString headerText = qvariant_cast<QString>(index.data(headerTextRole));
    QString subText = qvariant_cast<QString>(index.data(subHeaderTextrole));

    QSize iconsize = icon.actualSize(option.decorationSize);

    QRect headerRect = option.rect;
    QRect subheaderRect = option.rect;
    QRect iconRect = subheaderRect;

    iconRect.setRight(iconsize.width()+15);
    iconRect.setTop(iconRect.top()-10);
    headerRect.setLeft(iconRect.right());
    subheaderRect.setLeft(iconRect.right());
    headerRect.setTop(headerRect.top()+5);
    headerRect.setBottom(headerRect.top()+fm.height());

    subheaderRect.setTop(headerRect.bottom()+2);

    //painter->drawPixmap(QPoint(iconRect.right()/2,iconRect.top()/2),icon.pixmap(iconsize.width(),iconsize.height()));
    painter->drawPixmap(QPoint(5,iconRect.top()+iconsize.height()/2+3),icon.pixmap(iconsize.width(),iconsize.height()));

    painter->setFont(font);
    painter->drawText(headerRect,headerText);


    painter->setFont(SubFont);
    painter->drawText(subheaderRect.left(),subheaderRect.top()+17,subText);

    painter->restore();

 }
