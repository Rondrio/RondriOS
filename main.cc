#include <iostream>

#include <unistd.h>
#include <sys/types.h>

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h>
#include <X11/X.h>

#include <signal.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "lcars.hh"
#include "lcars_keylistener.hh"

#include <X11/XF86keysym.h>

/*
 * Round Button
 * Square Button
 * (Square-Round Button)
 *
 * Lables
 *
 * Checkbox / Checkslider
 * Radiobutton
 *
 * Text Box  \ TextArea
 * Input Box /
 *
 * Image View
 * Video View
 *
 * Progress Bar
 *
 * Border Blocks
 *
 * */

static void sigterm(int i) {
	exit(0);
}

static void quit(LCARS * lcars, Arg * arg) {
	lcars->Exit(arg->i);
}

static void close(LCARS * lcars, Arg * arg) {
	// lcars->CloseFocusedWindow();
	// lcars->GetScreen()->GetInterface()->SetNeedsRepaint(true);
	lcars->UnframeWindow(lcars->GetFocusedFrameWindowPair().window);
}

static void summon(LCARS * lcars, Arg * arg) {
	Display * dpy = lcars->GetDisplay();

	if(fork() == 0) {
		if(dpy)
			close(ConnectionNumber(dpy));

		int exitcode = execvp( ((char **) arg->v)[0], (char **) arg->v);
		lcars->Exit(exitcode);
	}
}

static char * terminalcmd[] = {(char *)"konsole", (char *)"konsole", NULL};
static char * pa_vol_down[] = {(char *)"pactl", (char *)"set-sink-volume", 	(char *)"@DEFAULT_SINK@", (char *)"-5%", 	NULL};
static char * pa_vol_rise[] = {(char *)"pactl", (char *)"set-sink-volume", 	(char *)"@DEFAULT_SINK@", (char *)"+5%", 	NULL};
static char * pa_vol_mute[] = {(char *)"pactl", (char *)"set-sink-mute", 	(char *)"@DEFAULT_SINK@", (char *)"toggle", NULL};


static KeyListener keylisteners[] = {
	{XK_q, 						ShiftMask|Mod4Mask, 	quit, 		{.i = 1}},
	{XK_c, 						ShiftMask|Mod4Mask, 	close, 		{.i = 0}},
	{XK_Return, 				ShiftMask|Mod4Mask, 	summon, 	{.v = terminalcmd}},
	{XF86XK_AudioRaiseVolume, 	None,					summon, 	{.v = pa_vol_rise}},
	{XF86XK_AudioLowerVolume,	None, 					summon, 	{.v = pa_vol_down}},
	{XF86XK_AudioMute,			None, 					summon, 	{.v = pa_vol_mute}}
};

int main(int argc, char ** argv) {

	//TODO: Improve
	signal(SIGTERM, sigterm);
	signal(SIGINT, sigterm);

	Display * dpy = XOpenDisplay(NULL);

	LCARS lcars(dpy);

	lcars.Init();

	lcars.AddKeyListener(&keylisteners[0]);
	lcars.AddKeyListener(&keylisteners[1]);
	lcars.AddKeyListener(&keylisteners[2]);
	lcars.AddKeyListener(&keylisteners[3]);
	lcars.AddKeyListener(&keylisteners[4]);
	lcars.AddKeyListener(&keylisteners[5]);

	lcars.Run();
	lcars.Exit(1);
}
