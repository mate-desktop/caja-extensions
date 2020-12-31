/*
 *  Caja SendTo
 *
 *  Copyright (C) 2005 Roberto Majadas
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
 *  Author: Roberto Majadas <roberto.majadas@openshine.com>
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <glib/gi18n-lib.h>

#include <libcaja-extension/caja-extension-types.h>
#include <libcaja-extension/caja-column-provider.h>

#include "caja-nste.h"

void
caja_module_initialize (GTypeModule*module)
{
	g_print ("Initializing caja-sendto extension\n");
	caja_nste_register_type (module);
}

void
caja_module_shutdown (void)
{
	g_print ("Shutting down caja-sendto extension\n");
}

void
caja_module_list_types (const GType **types,
			    int          *num_types)
{
	static GType type_list[1];

	type_list[0] = CAJA_TYPE_NSTE;
	*types = type_list;

	*num_types = 1;
}
