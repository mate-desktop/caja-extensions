/*
 *  Caja xattr tags property page extension
 *
 *  Copyright (C) 2017 Felipe Barriga Richards
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  Authors: Felipe Barriga Richards <spam@felipebarriga.cl>
 */

#ifndef CAJA_EXTENSIONS_TAGS_TREE_VIEW_H
#define CAJA_EXTENSIONS_TAGS_TREE_VIEW_H

enum
{
    COL_NAME = 0,
    NUM_COLS
};

GtkWidget * create_view_and_model (GList *tags);
GtkTreeModel * create_and_fill_model (GList *tags);

#endif //CAJA_EXTENSIONS_TAGS_TREE_VIEW_H
