/*
 *  caja-image-rotator.h
 *
 *  Copyright (C) 2004-2006 Jürg Billeter
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *  Author: Jürg Billeter <j@bitron.ch>
 *
 */

#ifndef __CAJA_IMAGE_ROTATOR_H__
#define __CAJA_IMAGE_ROTATOR_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define CAJA_TYPE_IMAGE_ROTATOR         (caja_image_rotator_get_type ())
G_DECLARE_FINAL_TYPE (CajaImageRotator, caja_image_rotator, CAJA, IMAGE_ROTATOR, GObject)

CajaImageRotator *caja_image_rotator_new (GList *files);
void caja_image_rotator_show_dialog (CajaImageRotator *dialog);

G_END_DECLS

#endif /* __CAJA_IMAGE_ROTATOR_H__ */
