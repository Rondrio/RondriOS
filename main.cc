#include <iostream>

#include <unistd.h>
#include <sys/types.h>

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h>
#include <X11/X.h>
#include <X11/XF86keysym.h>

#include <signal.h>

#include "lcars.hh"
#include "lcars_keylistener.hh"
#include "lcars_error.hh"
#include "lcars_parser.hh"

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

static void quit(LCARS::LCARS * lcars, LCARS::Arg * arg) {
	lcars->Exit(arg->i);
}

static void close(LCARS::LCARS * lcars, LCARS::Arg * arg) {
	// lcars->CloseFocusedWindow();
	// lcars->GetScreen()->GetInterface()->SetNeedsRepaint(true);
	lcars->UnframeWindow(lcars->GetFocusedWindow());
}

static void summon(LCARS::LCARS * lcars, LCARS::Arg * arg) {
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


static LCARS::KeyListener keylisteners[] = {
	{XK_q, 						ShiftMask|Mod4Mask, 	quit, 		{.i = 1}},
	{XK_c, 						ShiftMask|Mod4Mask, 	close, 		{.i = 0}},
	{XK_Return, 				ShiftMask|Mod4Mask, 	summon, 	{.v = terminalcmd}},
	{XF86XK_AudioRaiseVolume, 	None,					summon, 	{.v = pa_vol_rise}},
	{XF86XK_AudioLowerVolume,	None, 					summon, 	{.v = pa_vol_down}},
	{XF86XK_AudioMute,			None, 					summon, 	{.v = pa_vol_mute}}
};

int main(int argc, char ** argv) {

	if(argc >= 2) {
		LCARS::LOG_Init(argv[1]);
	} else {
		LCARS::LOG("ERROR in main()", "Could not initialize LOG because of too few arguments.");
	}

	//TODO: Improve
	signal(SIGTERM, sigterm);
	signal(SIGINT, sigterm);

	Display * dpy = XOpenDisplay(NULL);

	if(dpy) {

		LCARS::LCARS lcars(dpy);

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

	LCARS::LOG("FATAL ERROR in main()", "Could not display.");

	// LCARS::Parser parser("/home/cediw/Downloads/lcars_reference/interface.xml");
	// parser.Parse();
}
