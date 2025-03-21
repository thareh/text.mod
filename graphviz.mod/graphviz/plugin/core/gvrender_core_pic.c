/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * https://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/

#include "config.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <gvc/gvplugin_render.h>
#include <gvc/gvplugin_device.h>
#include <gvc/gvio.h>
#include <cgraph/strview.h>
#include <common/utils.h>
#include <common/color.h>
#include <common/colorprocs.h>

#include <common/const.h>
#include <util/agxbuf.h>

/* Number of points to split splines into */
#define BEZIERSUBDIVISION 6

enum {FORMAT_PIC};

static bool onetime = true;
static double Fontscale;

/* There are a couple of ways to generate output: 
    1. generate for whatever size is given by the bounding box
       - the drawing at its "natural" size might not fit on a physical page
         ~ dot size specification can be used to scale the drawing
         ~ and it's not difficult for user to scale the pic output to fit (multiply 4 (3 distinct) numbers on 3 lines by a scale factor)
       - some troff implementations may clip large graphs
         ~ handle by scaling to manageable size
       - give explicit width and height as parameters to .PS
       - pic scale variable is reset to 1.0
       - fonts are printed as size specified by caller, modified by user scaling
    2. scale to fit on a physical page
       - requires an assumption of page size (GNU pic assumes 8.5x11.0 inches)
         ~ any assumption is bound to be wrong more often than right
       - requires separate scaling of font point sizes since pic's scale variable doesn't affect text
         ~ possible, as above
       - likewise for line thickness
       - GNU pic does this (except for fonts) if .PS is used without explicit width or height; DWB pic does not
         ~ pic variants likely to cause trouble
  The first approach is used here.
*/

static const char pic_comments[] = "# ";       /* PIC comment */
static const char troff_comments[] = ".\\\" "; /* troff comment */
static const char picgen_msghdr[] = "dot pic plugin: ";

static void unsupported(char *s)
{
    agwarningf("%s%s unsupported\n", picgen_msghdr, s);
}

/* troff font mapping */
typedef struct {
    char trname[3], *psname;
} fontinfo;

static const fontinfo fonttab[] = {
    {"AB", "AvantGarde-Demi"},
    {"AI", "AvantGarde-BookOblique"},
    {"AR", "AvantGarde-Book"},
    {"AX", "AvantGarde-DemiOblique"},
    {"B ", "Times-Bold"},
    {"BI", "Times-BoldItalic"},
    {"CB", "Courier-Bold"},
    {"CO", "Courier"},
    {"CX", "Courier-BoldOblique"},
    {"H ", "Helvetica"},
    {"HB", "Helvetica-Bold"},
    {"HI", "Helvetica-Oblique"},
    {"HX", "Helvetica-BoldOblique"},
    {"Hb", "Helvetica-Narrow-Bold"},
    {"Hi", "Helvetica-Narrow-Oblique"},
    {"Hr", "Helvetica-Narrow"},
    {"Hx", "Helvetica-Narrow-BoldOblique"},
    {"I ", "Times-Italic"},
    {"KB", "Bookman-Demi"},
    {"KI", "Bookman-LightItalic"},
    {"KR", "Bookman-Light"},
    {"KX", "Bookman-DemiItalic"},
    {"NB", "NewCenturySchlbk-Bold"},
    {"NI", "NewCenturySchlbk-Italic"},
    {"NR", "NewCenturySchlbk-Roman"},
    {"NX", "NewCenturySchlbk-BoldItalic"},
    {"PA", "Palatino-Roman"},
    {"PB", "Palatino-Bold"},
    {"PI", "Palatino-Italic"},
    {"PX", "Palatino-BoldItalic"},
    {"R ", "Times-Roman"},
    {"S ", "Symbol"},
    {"ZD", "ZapfDingbats"},
};
static const size_t fonttab_size = sizeof(fonttab) / sizeof(fonttab[0]);

#ifdef HAVE_MEMRCHR
void *memrchr(const void *s, int c, size_t n);
#else
static const void *memrchr(const void *s, int c, size_t n) {
  const char *str = s;
  for (size_t i = n - 1; i != SIZE_MAX; --i) {
    if (str[i] == c) {
      return &str[i];
    }
  }
  return NULL;
}
#endif

static const char *picfontname(strview_t psname) {
    for (size_t i = 0; i < fonttab_size; ++i)
        if (strview_str_eq(psname, fonttab[i].psname))
            return fonttab[i].trname;
    agerrorf("%s%.*s is not a troff font\n", picgen_msghdr,
          (int)psname.size, psname.data);
    /* try base font names, e.g. Helvetica-Outline-Oblique -> Helvetica-Outline -> Helvetica */
    const char *dash = memrchr(psname.data, '-', psname.size);
    if (dash != NULL) {
        strview_t prefix = {.data = psname.data,
                            .size = (size_t)(dash - psname.data)};
        return picfontname(prefix);
    }
    return "R";
}

static void picptarray(GVJ_t *job, pointf *A, size_t n, int close) {
    for (size_t i = 0; i < n; i++) {
        if (i == 0) {
            gvprintf(job, "move to (%.0f, %.0f)", A[i].x, A[i].y);
        } else {
            gvprintf(job, "; line to (%.0f, %.0f)", A[i].x, A[i].y);
        }
    }
    if (close) {
        gvprintf(job, "; line to (%.0f, %.0f)", A[0].x, A[0].y);
    }
    gvputs(job, "\n");
}

static void pic_comment(GVJ_t *job, char *str)
{
    gvprintf(job, "%s %s\n", pic_comments, str);
}

static void pic_begin_graph(GVJ_t * job)
{
    obj_state_t *obj = job->obj;

    gvprintf(job, "%s Creator: %s version %s (%s)\n",
	troff_comments, job->common->info[0], job->common->info[1], job->common->info[2]);
    gvprintf(job, "%s Title: %s\n", troff_comments, agnameof(obj->u.g));
    gvprintf(job,
            "%s save point size and font\n.nr .S \\n(.s\n.nr DF \\n(.f\n",
            troff_comments);
}

static void pic_end_graph(GVJ_t * job)
{
    gvprintf(job,
            "%s restore point size and font\n.ps \\n(.S\n.ft \\n(DF\n",
            troff_comments);
}

static void pic_begin_page(GVJ_t * job)
{
    box pbr = job->pageBoundingBox;

    if (onetime && job->rotation && job->rotation != 90) {
        unsupported("rotation");
        onetime = false;
    }
    double height = PS2INCH((double)pbr.UR.y - (double)pbr.LL.y);
    double width = PS2INCH((double)pbr.UR.x - (double)pbr.LL.x);
    if (job->rotation == 90) {
        double temp = width;
        width = height;
        height = temp;
    }
    gvprintf(job, ".PS %.5f %.5f\n", width, height);
    gvprintf(job,
            "%s to change drawing size, multiply the width and height on the .PS line above and the number on the two lines below (rounded to the nearest integer) by a scale factor\n",
            pic_comments);
    if (width > 0.0) {
        Fontscale = log10(width);
        Fontscale += 3.0 - (int) Fontscale;     /* between 3.0 and 4.0 */
    } else
        Fontscale = 3.0;
    Fontscale = pow(10.0, Fontscale);   /* a power of 10 times width, between 1000 and 10000 */
    gvprintf(job, ".nr SF %.0f\nscalethickness = %.0f\n", Fontscale,
            Fontscale);
    gvprintf(job,
            "%s don't change anything below this line in this drawing\n",
            pic_comments);
    gvprintf(job,
            "%s non-fatal run-time pic version determination, version 2\n",
            pic_comments);
    gvprintf(job,
            "boxrad=2.0 %s will be reset to 0.0 by gpic only\n",
            pic_comments);
    gvprintf(job, "scale=1.0 %s required for comparisons\n",
            pic_comments);
    gvprintf(job,
            "%s boxrad is now 0.0 in gpic, else it remains 2.0\n",
            pic_comments);
    gvprintf(job,
            "%s dashwid is 0.1 in 10th Edition, 0.05 in DWB 2 and in gpic\n",
            pic_comments);
    gvprintf(job,
            "%s fillval is 0.3 in 10th Edition (fill 0 means black), 0.5 in gpic (fill 0 means white), undefined in DWB 2\n",
            pic_comments);
    gvprintf(job,
            "%s fill has no meaning in DWB 2, gpic can use fill or filled, 10th Edition uses fill only\n",
            pic_comments);
    gvprintf(job,
            "%s DWB 2 doesn't use fill and doesn't define fillval\n",
            pic_comments);
    gvprintf(job,
            "%s reset works in gpic and 10th edition, but isn't defined in DWB 2\n",
            pic_comments);
    gvprintf(job, "%s DWB 2 compatibility definitions\n",
            pic_comments);
    gvprintf(job,
            "if boxrad > 1.0 && dashwid < 0.075 then X\n\tfillval = 1;\n\tdefine fill Y Y;\n\tdefine solid Y Y;\n\tdefine reset Y scale=1.0 Y;\nX\n");
    gvprintf(job, "reset %s set to known state\n", pic_comments);
    gvprintf(job, "%s GNU pic vs. 10th Edition d\\(e'tente\n",
            pic_comments);
    gvprintf(job,
            "if fillval > 0.4 then X\n\tdefine setfillval Y fillval = 1 - Y;\n\tdefine bold Y thickness 2 Y;\n");
    gvprintf(job,
            "\t%s if you use gpic and it barfs on encountering \"solid\",\n",
            pic_comments);
    gvprintf(job,
            "\t%s\tinstall a more recent version of gpic or switch to DWB or 10th Edition pic;\n",
            pic_comments);
    gvprintf(job,
            "\t%s\tsorry, the groff folks changed gpic; send any complaint to them;\n",
            pic_comments);
    gvprintf(job,
            "X else Z\n\tdefine setfillval Y fillval = Y;\n\tdefine bold Y Y;\n\tdefine filled Y fill Y;\nZ\n");
    gvprintf(job,
            "%s arrowhead has no meaning in DWB 2, arrowhead = 7 makes filled arrowheads in gpic and in 10th Edition\n",
            pic_comments);
    gvprintf(job,
            "%s arrowhead is undefined in DWB 2, initially 1 in gpic, 2 in 10th Edition\n",
            pic_comments);
    gvprintf(job, "arrowhead = 7 %s not used by graphviz\n",
            pic_comments);
    gvprintf(job,
            "%s GNU pic supports a boxrad variable to draw boxes with rounded corners; DWB and 10th Ed. do not\n",
            pic_comments);
    gvprintf(job, "boxrad = 0 %s no rounded corners in graphviz\n",
            pic_comments);
    gvprintf(job,
            "%s GNU pic supports a linethick variable to set line thickness; DWB and 10th Ed. do not\n",
            pic_comments);
    gvprintf(job, "linethick = 0; oldlinethick = linethick\n");
    gvprintf(job,
            "%s .PS w/o args causes GNU pic to scale drawing to fit 8.5x11 paper; DWB does not\n",
            pic_comments);
    gvprintf(job,
            "%s maxpsht and maxpswid have no meaning in DWB 2.0, set page boundaries in gpic and in 10th Edition\n",
            pic_comments);
    gvprintf(job,
            "%s maxpsht and maxpswid are predefined to 11.0 and 8.5 in gpic\n",
            pic_comments);
    gvprintf(job, "maxpsht = %f\nmaxpswid = %f\n", height, width);
    gvprintf(job, "Dot: [\n");
    gvprintf(job,
            "define attrs0 %% %%; define unfilled %% %%; define rounded %% %%; define diagonals %% %%\n");
}

static void pic_end_page(GVJ_t * job)
{
    gvprintf(job,
	"]\n.PE\n");
}

static void pic_textspan(GVJ_t * job, pointf p, textspan_t * span)
{
    static char *lastname;
    static double lastsize;

    switch (span->just) {
    case 'l': 
        break;
    case 'r': 
        p.x -= span->size.x;
        break;
    default:
    case 'n': 
        p.x -= span->size.x / 2;
        break;
    }
    /* Why on earth would we do this. But it works. SCN 2/26/2002 */
    p.y += span->font->size / (3.0 * POINTS_PER_INCH);
    p.x += span->size.x / (2.0 * POINTS_PER_INCH);

    if (span->font->name && (!lastname || strcmp(lastname, span->font->name))) {
        gvprintf(job, ".ft %s\n", picfontname(strview(span->font->name, '\0')));
	lastname = span->font->name;
    }
    double sz = fmax(span->font->size, 1);
    if (fabs(sz - lastsize) > 0.5) {
        gvprintf(job, ".ps %.0f*\\n(SFu/%.0fu\n", sz, Fontscale);
	lastsize = sz;
    }
    gvputc(job, '"');
    gvputs_nonascii(job, span->str);
    gvprintf(job, "\" at (%.5f,%.5f);\n", p.x, p.y);
}

static void pic_ellipse(GVJ_t * job, pointf * A, int filled)
{
    /* A[] contains 2 points: the center and corner. */

    gvprintf(job,
		"ellipse attrs0 %swid %.5f ht %.5f at (%.5f,%.5f);\n",
		filled ? "fill " : "",
		PS2INCH(2*(A[1].x - A[0].x)),
		PS2INCH(2*(A[1].y - A[0].y)),
		PS2INCH(A[0].x),
		PS2INCH(A[0].y));
}

static void pic_bezier(GVJ_t *job, pointf *A, size_t n, int filled) {
    (void)filled;

    pointf V[4];

    V[3].x = A[0].x;
    V[3].y = A[0].y;
    /* Write first point in line */
    gvprintf(job, "move to (%.0f, %.0f)", A[0].x, A[0].y);
    /* write subsequent points */
    for (size_t i = 0; i + 3 < n; i += 3) {
        V[0] = V[3];
        for (size_t j = 1; j <= 3; j++) {
            V[j].x = A[i + j].x;
            V[j].y = A[i + j].y;
        }
        for (int step = 1; step <= BEZIERSUBDIVISION; step++) {
            pointf pf = Bezier(V, (double)step / BEZIERSUBDIVISION, NULL, NULL);
            gvprintf(job, "; spline to (%.0f, %.0f)", pf.x, pf.y);
        }
    }

    gvputs(job, "\n");
}

static void pic_polygon(GVJ_t *job, pointf *A, size_t n, int filled) {
    (void)filled;
    picptarray(job, A, n, 1); // closed shape
}

static void pic_polyline(GVJ_t *job, pointf *A, size_t n) {
  picptarray(job, A, n, 0); // open shape
}

gvrender_engine_t pic_engine = {
    0,				/* pic_begin_job */
    0,				/* pic_end_job */
    pic_begin_graph,
    pic_end_graph,
    0,				/* pic_begin_layer */
    0,				/* pic_end_layer */
    pic_begin_page,
    pic_end_page,
    0,				/* pic_begin_cluster */
    0,				/* pic_end_cluster */
    0,				/* pic_begin_nodes */
    0,				/* pic_end_nodes */
    0,				/* pic_begin_edges */
    0,				/* pic_end_edges */
    0,				/* pic_begin_node */
    0,				/* pic_end_node */
    0,				/* pic_begin_edge */
    0,				/* pic_end_edge */
    0,				/* pic_begin_anchor */
    0,				/* pic_end_anchor */
    0,				/* pic_begin_label */
    0,				/* pic_end_label */
    pic_textspan,
    0,				/* pic_resolve_color */
    pic_ellipse,
    pic_polygon,
    pic_bezier,
    pic_polyline,
    pic_comment,
    0,				/* pic_library_shape */
};


static gvrender_features_t render_features_pic = {
    0,				/* flags */
    4.,                         /* default pad - graph units */
    NULL,			/* knowncolors */
    0,				/* sizeof knowncolors */
    HSVA_DOUBLE,		/* color_type */
};

static gvdevice_features_t device_features_pic = {
    0,				/* flags */
    {0.,0.},			/* default margin - points */
    {0.,0.},			/* default page width, height - points */
    {72.,72.},			/* default dpi */
};

gvplugin_installed_t gvrender_pic_types[] = {
    {FORMAT_PIC, "pic", -1, &pic_engine, &render_features_pic},
    {0, NULL, 0, NULL, NULL}
};

gvplugin_installed_t gvdevice_pic_types[] = {
    {FORMAT_PIC, "pic:pic", -1, NULL, &device_features_pic},
    {0, NULL, 0, NULL, NULL}
};
