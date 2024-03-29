/* modifier 0 means no modifier */
static char *useragent      = "Mozilla/5.0 (X11; U; Unix; en-US) "
	"AppleWebKit/537.15 (KHTML, like Gecko) Chrome/24.0.1295.0 "
	"Safari/537.15 Surf/"VERSION;
static char *stylefile      = "~/.surf/style.css";
static char *scriptfile     = "~/.surf/script.js";

static bool kioskmode       = false; /* Ignore shortcuts */
static bool showindicators  = true;  /* Show indicators in window title */
static bool zoomto96dpi     = true;  /* Zoom pages to always emulate 96dpi */
static bool runinfullscreen = false; /* Run in fullscreen mode by default */

static guint defaultfontsize = 12;   /* Default font size */
static gfloat zoomlevel = 1.0;       /* Default zoom level */

/* Soup default features */
static char *cookiefile     = "~/.surf/cookies.txt";
static char *cookiepolicies = "Aa@"; /* A: accept all; a: accept nothing,
                                        @: accept no third party */
static char *cafile         = "/etc/ssl/certs/ca-certificates.crt";
static char *strictssl      = FALSE; /* Refuse untrusted SSL connections */
static time_t sessiontime   = 3600;

/* Webkit default features */
static bool enablescrollbars = true;
static bool enablespatialbrowsing = true;
static bool enableplugins = true;
static bool enablescripts = true;
static bool enableinspector = true;
static bool loadimages = true;
static bool hidebackground  = false;
static bool allowgeolocation = true;

#define SETPROP(p, q) { \
	.v = (char *[]){ "/bin/sh", "-c", \
		"dmenu < $0/$1 > $0/$2", ctl, p, q, NULL \
	} \
}

/* DOWNLOAD(URI, referer) */
#define DOWNLOAD(d, r) { \
	.v = (char *[]){ "/bin/sh", "-c", \
		"st -e /bin/sh -c \"curl -L -J -O --user-agent '$1'" \
		" --referer '$2' -b $3 -c $3 '$0';" \
		" sleep 5;\"", \
		d, useragent, r, cookiefile, NULL \
	} \
}

#define MODKEY GDK_CONTROL_MASK

/* hotkeys */
/*
 * If you use anything else but MODKEY and GDK_SHIFT_MASK, don't forget to
 * edit the CLEANMASK() macro.
 */
static Key keys[] = {
    /* modifier	            keyval          function    arg             Focus */
    { MODKEY|GDK_SHIFT_MASK,GDK_KEY_r,      reload,     { .b = TRUE } },
    { MODKEY,               GDK_KEY_r,      reload,     { .b = FALSE } },
    { MODKEY|GDK_SHIFT_MASK,GDK_KEY_p,      print,      { 0 } },

    { MODKEY,               GDK_KEY_p,      clipboard,  { .b = TRUE } },
    { MODKEY,               GDK_KEY_y,      clipboard,  { .b = FALSE } },

    { MODKEY|GDK_SHIFT_MASK,GDK_KEY_j,      zoom,       { .i = -1 } },
    { MODKEY|GDK_SHIFT_MASK,GDK_KEY_k,      zoom,       { .i = +1 } },
    { MODKEY|GDK_SHIFT_MASK,GDK_KEY_q,      zoom,       { .i = 0  } },
    { MODKEY,               GDK_KEY_minus,  zoom,       { .i = -1 } },
    { MODKEY,               GDK_KEY_plus,   zoom,       { .i = +1 } },

    { MODKEY,               GDK_KEY_l,      navigate,   { .i = +1 } },
    { MODKEY,               GDK_KEY_h,      navigate,   { .i = -1 } },

    { MODKEY,               GDK_KEY_j,      scroll_v,   { .i = +1 } },
    { MODKEY,               GDK_KEY_k,      scroll_v,   { .i = -1 } },
    { MODKEY,               GDK_KEY_b,      scroll_v,   { .i = -10000 } },
    { MODKEY,               GDK_KEY_space,  scroll_v,   { .i = +10000 } },
    { MODKEY,               GDK_KEY_i,      scroll_h,   { .i = +1 } },
    { MODKEY,               GDK_KEY_u,      scroll_h,   { .i = -1 } },

    { 0,                    GDK_KEY_F11,    fullscreen, { 0 } },
    { 0,                    GDK_KEY_Escape, stop,       { 0 } },
    { MODKEY,               GDK_KEY_o,      source,     { 0 } },
    { MODKEY|GDK_SHIFT_MASK,GDK_KEY_o,      inspector,  { 0 } },

    { MODKEY,               GDK_KEY_g,      spawn,      SETPROP("uri", "go") },
    { MODKEY,               GDK_KEY_f,      spawn,      SETPROP("query", "find") },
    { MODKEY,               GDK_KEY_slash,  spawn,      SETPROP("query", "find") },

    { MODKEY,               GDK_KEY_n,      find,       { .b = TRUE } },
    { MODKEY|GDK_SHIFT_MASK,GDK_KEY_n,      find,       { .b = FALSE } },

    { MODKEY|GDK_SHIFT_MASK,GDK_KEY_c,      toggle,     { .v = "enable-caret-browsing" } },
    { MODKEY|GDK_SHIFT_MASK,GDK_KEY_i,      toggle,     { .v = "auto-load-images" } },
    { MODKEY|GDK_SHIFT_MASK,GDK_KEY_s,      toggle,     { .v = "enable-scripts" } },
    { MODKEY|GDK_SHIFT_MASK,GDK_KEY_v,      toggle,     { .v = "enable-plugins" } },
    { MODKEY|GDK_SHIFT_MASK,GDK_KEY_a,      togglecookiepolicy, { 0 } },
    { MODKEY|GDK_SHIFT_MASK,GDK_KEY_m,      togglestyle, { 0 } },
    { MODKEY|GDK_SHIFT_MASK,GDK_KEY_b,      togglescrollbars, { 0 } },
    { MODKEY|GDK_SHIFT_MASK,GDK_KEY_g,      togglegeolocation, { 0 } },
};

