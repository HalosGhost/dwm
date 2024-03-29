/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>
#include "push.c"

/* appearance */
static const char * fonts [] = {
    "Inconsolatazi4 for Powerline:size=10",
    "Symbola:style=Regular:size=10"
};

static const char menufnt         [] = "Fira Code 9";
static const unsigned int borderpx   = 1;        /* border pixel of windows */
static const unsigned int snap       = 32;       /* snap pixel */
static const int showbar             = 1;     /* False means no bar */
static const int topbar              = 1;     /* False means bottom bar */
static const char * colors [][3] = {
	/*                  fg         bg         border   */
	[SchemeNorm]    = { "#bbbbbb", "#222222", "#444444" },
	[SchemeSel]     = { "#eeeeee", "#4c7899", "#4c7899" },
	[SchemeWarn]    = { "#000000", "#c6cc66", "#c6cc66" },
	[SchemeUrgent]  = { "#000000", "#994c59", "#994c59" },
};

/* tagging */
static const char * tags [] = { ">_", "web", "docs", "comms", "play" };

static const Rule rules [] = {
    /* class            instance   title  tagmask  floating  monitor */
    { "st-256color",    NULL,      NULL,  1 << 0,  False,    -1      },
    { "LibreWolf",      NULL,      NULL,  1 << 1,  False,    -1      },
    { "Zathura",        NULL,      NULL,  1 << 2,  False,    -1      },
    { "Gimp",           NULL,      NULL,  1 << 2,  False,    -1      },
    { "Inkscape",       NULL,      NULL,  1 << 2,  False,    -1      },
    { "libreoffice",    NULL,      NULL,  1 << 2,  False,    -1      },
    { "Slack",          NULL,      NULL,  1 << 3,  False,    -1      },
    { "signal-desktop", NULL,      NULL,  1 << 3,  False,    -1      },
    { "Steam",          NULL,      NULL,  1 << 4,  False,    -1      },
    { "mpv",            NULL,      NULL,  1 << 4,  False,    -1      },

    { NULL,             ">_",      NULL,  1 << 0,  False,    -1      },
    { NULL,             "web",     NULL,  1 << 1,  False,    -1      },
    { NULL,             "docs",    NULL,  1 << 2,  False,    -1      },
    { NULL,             "comms",   NULL,  1 << 3,  False,    -1      },
    { NULL,             "play",    NULL,  1 << 4,  False,    -1      }
};

/* layout(s) */
static const float mfact        = 0.6;  /* factor of master area size [0.05..0.95] */
static const int nmaster        = 1;    /* number of clients in master area */
static const int resizehints    = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 0;    /* 1 will force focus on the fullscreen window */

static const Layout layouts [] = {
    /* symbol     arrange function */
    { "[]=",      tile    }, /* first entry is default */
    { "><>",      NULL    }, /* no layout function means floating behavior */
    { "[M]",      monocle }
};

/* key definitions */
#define MOD1 Mod1Mask
#define MOD2 ControlMask
#define MOD3 ShiftMask
#define MOD4 Mod4Mask

#define TAGKEYS(KEY,TAG) \
    { MOD1,            KEY,  toggleview,  { .ui = 1 << TAG } }, \
    { MOD1|MOD2,       KEY,  view,        { .ui = 1 << TAG } }, \
    { MOD1|MOD3,       KEY,  tag,         { .ui = 1 << TAG } }, \
    { MOD1|MOD2|MOD3,  KEY,  toggletag,   { .ui = 1 << TAG } }

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char *[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon [2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char * dmenucmd  [] = { '\0' };
static const char * roficmd   [] = { "rofi", "-show", "run", NULL };
static const char * termcmd   [] = { "st", "-e", "tmux", NULL };
static const char * volumeInc [] = { "pactl", "--", "set-sink-volume", "@DEFAULT_SINK@",   "+5%",    NULL };
static const char * volumeDec [] = { "pactl", "--", "set-sink-volume", "@DEFAULT_SINK@",   "-5%",    NULL };
static const char * volumeTog [] = { "pactl", "--", "set-sink-mute",   "@DEFAULT_SINK@",   "toggle", NULL };
static const char * micTog    [] = { "pactl", "--", "set-source-mute", "@DEFAULT_SOURCE@", "toggle", NULL };
static const char * lockscrn  [] = { "loginctl", "--no-ask-password", "lock-session", NULL };
static const char * bl_up     [] = { "enlighten", "+5%", NULL };
static const char * bl_dn     [] = { "enlighten", "-5%", NULL };

static Key keys [] = {
    /* modifier   key                        function         argument             */
    { MOD1,       XK_d,                      spawn,           { .v = roficmd     } },
    { MOD1,       XK_Return,                 spawn,           { .v = termcmd     } },
    { 0,          XF86XK_AudioRaiseVolume,   spawn,           { .v = volumeInc   } },
    { 0,          XF86XK_AudioLowerVolume,   spawn,           { .v = volumeDec   } },
    { 0,          XF86XK_AudioMute,          spawn,           { .v = volumeTog   } },
    { 0,          XF86XK_AudioMicMute,       spawn,           { .v = micTog      } },
    { 0,          XF86XK_MonBrightnessDown,  spawn,           { .v = bl_dn       } },
    { 0,          XF86XK_MonBrightnessUp,    spawn,           { .v = bl_up       } },
    { MOD1,       XK_b,                      togglebar,       { 0                } },
    { MOD1,       XK_j,                      focusstack,      { .i = +1          } },
    { MOD1,       XK_k,                      focusstack,      { .i = -1          } },
    { MOD1|MOD3,  XK_j,                      pushdown,        { 0                } },
    { MOD1|MOD3,  XK_k,                      pushup,          { 0                } },
    { MOD1|MOD2,  XK_l,                      spawn,           { .v = lockscrn    } },
    { MOD1|MOD3,  XK_l,                      incnmaster,      { .i = +1          } },
    { MOD1|MOD3,  XK_h,                      incnmaster,      { .i = -1          } },
    { MOD1,       XK_h,                      setmfact,        { .f = -0.05       } },
    { MOD1,       XK_l,                      setmfact,        { .f = +0.05       } },
    { MOD1,       XK_Tab,                    view,            { 0                } },
    { MOD1|MOD3,  XK_c,                      killclient,      { 0                } },
    { MOD1,       XK_t,                      setlayout,       { .v = &layouts[0] } },
    { MOD1,       XK_f,                      setlayout,       { .v = &layouts[1] } },
    { MOD1,       XK_m,                      setlayout,       { .v = &layouts[2] } },
    { MOD1,       XK_space,                  setlayout,       { 0                } },
    { MOD1|MOD3,  XK_space,                  togglefloating,  { 0                } },
    { MOD1,       XK_0,                      view,            { .ui = ~0         } },
    { MOD1|MOD3,  XK_0,                      tag,             { .ui = ~0         } },
    { MOD1,       XK_comma,                  focusmon,        { .i = -1          } },
    { MOD1,       XK_period,                 focusmon,        { .i = +1          } },
    { MOD1|MOD3,  XK_comma,                  tagmon,          { .i = -1          } },
    { MOD1|MOD3,  XK_period,                 tagmon,          { .i = +1          } },
    TAGKEYS(      XK_1,                                         0                  ),
    TAGKEYS(      XK_2,                                         1                  ),
    TAGKEYS(      XK_3,                                         2                  ),
    TAGKEYS(      XK_4,                                         3                  ),
    TAGKEYS(      XK_5,                                         4                  ),
    { MOD1|MOD3,  XK_q,                      quit,            { 0                } }
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons [] = {
    /* click          event mask  button    function         argument             */
    { ClkWinTitle,    0,          Button2,  zoom,            { 0                } },
    { ClkClientWin,   MOD1,       Button1,  movemouse,       { 0                } },
    { ClkClientWin,   MOD1,       Button2,  togglefloating,  { 0                } },
    { ClkClientWin,   MOD1,       Button3,  resizemouse,     { 0                } },
};

