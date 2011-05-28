/*
 *  Hamlib Rotator backend - IF-100 parallel port
 *  Copyright (c) 2011 by Stephane Fillod
 *
 *
 *   This library is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2 of
 *   the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Library General Public License for more details.
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this library; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif

#include "hamlib/rotator.h"
#include "parallel.h"
#include "misc.h"
#include "register.h"


static int
if100_set_position(ROT *rot, azimuth_t az, elevation_t el)
{
    hamlib_port_t *port = &rot->state.rotport;
    int retval;
    int az_i;
    int el_i;
    int dataout, i;

    rig_debug(RIG_DEBUG_TRACE, "%s called: %f %f\n", __func__, az, el);

    if (rot->state.max_az - rot->state.min_az >= 450)
        az_i = az * 0.70833;
    else
        az_i = (az + 45) * 0.56667;

    el_i = el * 1.4166;

    dataout = ((el_i & 0xff) << 8) + (az_i & 0xff);

#define DAT0  0x01
#define CLK   0x02
#define TRACK 0x08

    rig_debug(RIG_DEBUG_TRACE, "%s: shifting dataout 0x%04x to parallel port\n", __func__, dataout);

    retval = par_lock (port);
    if (retval != RIG_OK)
        return retval;

    for (i = 0; i < 16; i++) {
        if (dataout & 0x8000) {
            par_write_data(port, TRACK|DAT0);
            par_write_data(port, TRACK|DAT0|CLK);
            par_write_data(port, TRACK|DAT0);
        } else {
            par_write_data(port, TRACK);
            par_write_data(port, TRACK|CLK);
            par_write_data(port, TRACK);
        }
        dataout = (dataout << 1) & 0xffff;
    }

    par_write_data(port, TRACK);
    par_unlock (port);

    return RIG_OK;
}


/** IF-100 implements essentially only the set position function.
 */
const struct rot_caps if100_rot_caps = {
  .rot_model =      ROT_MODEL_IF100,
  .model_name =     "IF-100",
  .mfg_name =       "AMSAT",
  .version =        "0.1",
  .copyright = 	    "LGPL",
  .status =         RIG_STATUS_UNTESTED,
  .rot_type =       ROT_TYPE_OTHER,
  .port_type =      RIG_PORT_PARALLEL,
  .write_delay =  0,
  .post_write_delay =  0,
  .timeout =  200,
  .retry =  3,

  .min_az = 	0,
  .max_az =  	360,
  .min_el = 	0,
  .max_el =  	180,

  .set_position =  if100_set_position,
};


/* ************************************************************************* */

DECLARE_INITROT_BACKEND(amsat)
{
	rig_debug(RIG_DEBUG_VERBOSE, "%s called\n", __func__);

	rot_register(&if100_rot_caps);

	return RIG_OK;
}

