/* Template rsrc Script.r */
/* This file is the engine of the rsrc rez files. */
/* It does not need to be edited. */

#include <Types.r>
#include "BSNG Product.h"
#include "BSNG rsrc.h"

resource 'ALRT' (1000, purgeable) {
	{104, 130, 205, 488},	 /* 358 wide by 101 high */
	1000,					         /* DITL id */
	{	                     /* array: 4 elements */
		OK, visible, silent, /* [1] */
		OK, visible, silent, /* [2] */
		OK, visible, silent, /* [3] */
		OK, visible, silent  /* [4] */
	},

	alertPositionMainScreen
};

resource 'DITL' (1000, purgeable) {
  {                     /* array DITLarray: 2 elements */
	  {71, 288, 91, 348}, /* [1] */
		Button {
			enabled,
			"OK"
		},
	  {10,75,58,348},     /* [2] */
		StaticText {
			disabled ,
			"This " PROGRAM_NAME " plugin was written in "
			PLUGIN_YEAR PLUGIN_AUTHOR_DITL "."
		}
  }
};

resource 'STR ' (1000) {
	PROGRAM_NAME
};

resource 'vers' (1, purgeable) {
	PLUGIN_VERSION_MAJOR,
	(PLUGIN_VERSION_MINOR * 0x10) + PLUGIN_VERSION_BUGFIX,
	release,
	0x00,
	PLUGIN_AUTHOR_LOCALE,
	PLUGIN_VERSION_TEXT,
	PLUGIN_VERSION_TEXT "\nwritten " PLUGIN_YEAR PLUGIN_AUTHOR_VERS
};

resource 'vers' (2, purgeable) {
	PLUGIN_VERSION_MAJOR,
	(PLUGIN_VERSION_MINOR * 0x10) + PLUGIN_VERSION_BUGFIX,
	release,
	0x00,
	PLUGIN_AUTHOR_LOCALE,
	PLUGIN_VERSION_TEXT,
	"Plugin for BSNG 2.5"
};
