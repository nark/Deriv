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



#ifndef MAIN_H
#define MAIN_H

#include <QtGlobal>

#ifdef Q_OS_LINUX
#include "config/linux/config.h"
#endif

#ifdef Q_OS_MAC
#include "config/osx/config.h"
#endif

#ifdef Q_OS_WIN32
#include "config/win/config.h"
#endif


#include <wired/wired.h>



/**
 * A global pointer to the Wired 2.0 specification class of libwired.
 */
extern wi_p7_spec_t *wc_spec;



#endif // MAIN_H
