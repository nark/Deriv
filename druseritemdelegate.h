/*
   Copyright 2014, Rafaël Warnault <rw@read-write.fr>

   Clementine is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Clementine is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Clementine.  If not, see <http://www.gnu.org/licenses/>.
*/



#ifndef DRUSERITEMDELEGATE_H
#define DRUSERITEMDELEGATE_H

#include <QtGui>
#include <QStyledItemDelegate>


class DRUserItemDelegate : public QStyledItemDelegate
{
public:
    DRUserItemDelegate();
    virtual ~DRUserItemDelegate();

    enum datarole {headerTextRole = Qt::UserRole + 100,subHeaderTextrole = Qt::UserRole+101,IconRole = Qt::UserRole+102};

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
           const QModelIndex &index) const;

    QSize sizeHint(const QStyleOptionViewItem &option,
               const QModelIndex &index ) const;

};

#endif // DRUSERITEMDELEGATE_H
