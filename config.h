/* See LICENSE file for copyright and license details. */
#include "tcl.c"
/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "hack:size=10" };
static const char dmenufont[]       = "hack:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#18453b";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "WeB", "MaiL", "3-TerM", "4-ChaT", "5-IDE", "6-VM", "7-Tide", "Man", "Pwns" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Skype",	NULL,       NULL,       0,            1,           -1},
	{ "Firefox",  NULL,       NULL,       1,       0,           0 },
	{ "Brave-browser",  NULL,       NULL,       1,       0,           0 },
	{ "discord",	NULL,NULL, 1 << 3 ,0,0},
	{ "Signal",	NULL,NULL, 1 << 3 ,0,1},
	{ "Slack", NULL, NULL, 1 << 3 , 0 ,0},
	{ "Evolution", NULL, NULL, 1 << 1 , 0 , 0},
	{ "thunderbird", NULL, NULL, 1 << 1, 0,0},
	{ "Barrier", NULL, NULL, 1 << 8 , 0 , 1},
	{ "1Password", NULL, NULL, 1 << 4 , 1 , -1},
	{ "Vmware", NULL, NULL, 1 << 5 , 0 , 0},
	{ "org.remmina.Remmina",NULL, NULL, 1 << 6 , 0 , 0 },
	{ "Komodo", NULL, NULL, 1 << 4 , 0 , 0},
	{ "Atom", NULL, NULL, 1 << 4 , 0 , 0},
	{ "VSCodium", NULL, NULL, 1 << 4 , 0 , 0},
	{ "Xfce4-terminal", NULL, "ETH", 1<< 8, 0, 1},
	{ "Pavucontrol", NULL, NULL, 1 << 7, 0,1},
	{ "Spotify", NULL, NULL, 1 << 7, 0,1},
	{ "Nm-connection-editor", NULL, NULL, 1 << 6, 0,1},
	{ "Wicd-client.py", NULL, NULL, 1 << 6, 0,1},
	{ "Opera", NULL, NULL, 1, 0, 1},
	{ "Standard Notes", NULL, NULL, 1<< 3, 0, 1},
	{ "install4j-burp-StartBurp",     NULL,       NULL,       0,            1,           -1 }
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "|M|",      centeredmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[]=",      tile },    /* first entry is default */
	{ "|||",      tcl },
	{ "[M]",      monocle },
	{ ">M>",      centeredfloatingmaster },
};

/* key definitions */
#define MODKEY Mod4Mask
#define MODKEYALT Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },


/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "xfce4-terminal", NULL };
static const char *slockcmd[] = { "/usr/bin/slock", NULL };
static const char *spotcmd[] = { "/snap/bin/spotify", NULL };
static const char *barriercmd[] = { "/usr/bin/barrier", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
        { Mod1Mask,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ Mod1Mask,                       XK_space,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEYALT|ControlMask,             XK_b, spawn,          {.v = barriercmd } },
	{ MODKEYALT|ControlMask,			XK_l,	   spawn,	   {.v = slockcmd } },
	{ MODKEYALT|ControlMask,			XK_s,	   spawn,	   {.v = spotcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEYALT|ControlMask,             XK_1,      setlayout,      {.v = &layouts[0]} },
	{ MODKEYALT|ControlMask,             XK_2,      setlayout,      {.v = &layouts[1]} },
	{ MODKEYALT|ControlMask,             XK_3,      setlayout,      {.v = &layouts[2]} },
	{ MODKEYALT|ControlMask,             XK_4,      setlayout,      {.v = &layouts[3]} },
	{ MODKEYALT|ControlMask,             XK_5,      setlayout,      {.v = &layouts[4]} },
    { MODKEYALT|ControlMask,             XK_6,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
