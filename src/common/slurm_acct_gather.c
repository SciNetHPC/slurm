/*****************************************************************************\
 *  slurm_acct_gather.c - generic interface needed for some
 *                        acct_gather plugins.
 *****************************************************************************
 *  Copyright (C) 2013 SchedMD LLC.
 *  Written by Danny Auble <da@schedmd.com>
 *
 *  This file is part of SLURM, a resource management program.
 *  For details, see <http://slurm.schedmd.com/>.
 *  Please also read the included file: DISCLAIMER.
 *
 *  SLURM is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your option)
 *  any later version.
 *
 *  In addition, as a special exception, the copyright holders give permission
 *  to link the code of portions of this program with the OpenSSL library under
 *  certain conditions as described in each individual source file, and
 *  distribute linked combinations including the two. You must obey the GNU
 *  General Public License in all respects for all of the code used other than
 *  OpenSSL. If you modify file(s) with this exception, you may extend this
 *  exception to your version of the file(s), but you are not obligated to do
 *  so. If you do not wish to do so, delete this exception statement from your
 *  version.  If you delete this exception statement from all source files in
 *  the program, then also delete it here.
 *
 *  SLURM is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with SLURM; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA.
\*****************************************************************************/

#include <sys/stat.h>

#include "slurm_acct_gather.h"
#include "xstring.h"

static bool inited = 0;

extern int acct_gather_conf_init(void)
{
	s_p_hashtbl_t *tbl = NULL;
	char *conf_path = NULL;
	s_p_options_t *full_options = NULL;
	int full_options_cnt = 0;
	struct stat buf;
	if (inited)
		return SLURM_SUCCESS;

	/* get options from plugins using acct_gather.conf */

//	acct_gather_energy_g_conf_options(&full_options, &full_options_cnt);
	acct_gather_profile_g_conf_options(&full_options, &full_options_cnt);
	/* ADD MORE HERE */

	/* for the NULL at the end */
	xrealloc(full_options,
	((full_options_cnt + 1) * sizeof(s_p_options_t)));

	/**************************************************/

	/* Get the acct_gather.conf path and validate the file */
	conf_path = get_extra_conf_path("acct_gather.conf");
	if ((conf_path == NULL) || (stat(conf_path, &buf) == -1)) {
		debug2("No acct_gather.conf file (%s)", conf_path);
	} else {
		debug("Reading acct_gather.conf file %s", conf_path);

		tbl = s_p_hashtbl_create(full_options);
		if (s_p_parse_file(tbl, NULL, conf_path, false) ==
		    SLURM_ERROR) {
			fatal("Could not open/read/parse "
			      "acct_gather.conf file %s",
			      conf_path);
		}
	}

	xfree(full_options);
	xfree(conf_path);

	/* handle acct_gather.conf in each plugin */
//	acct_gather_energy_g_conf_set(tbl);
	acct_gather_profile_g_conf_set(tbl);
	/* ADD MORE HERE */
	/******************************************/

	s_p_hashtbl_destroy(tbl);

	inited = 1;

	return SLURM_SUCCESS;
}

extern int acct_gather_conf_destroy(void)
{
	if (!inited)
		return SLURM_SUCCESS;

	return SLURM_SUCCESS;
}
