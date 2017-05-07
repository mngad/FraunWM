#include <X11/Xlib.h>
#include <stdio.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

int main(void)
{

	printf("for loop");
    Display * dpy;
    XWindowAttributes attr;
    XButtonEvent start;
    XEvent ev;
	Window returnedroot;
	Window returnedparent;
	Window * children;
	unsigned int numchildren;
	
    if(!(dpy = XOpenDisplay(0x0))) return 1;
	int F1 = XKeysymToKeycode(dpy, XStringToKeysym("F1"));
	int XK_TAB = XKeysymToKeycode(dpy, XStringToKeysym("XK_TAB"));

    XGrabKey(dpy, F1, Mod1Mask,
            DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync);
    XGrabButton(dpy, 1, Mod1Mask, DefaultRootWindow(dpy), True,
            ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);
    XGrabButton(dpy, 3, Mod1Mask, DefaultRootWindow(dpy), True,
            ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);
	XGrabKey(dpy, XK_TAB, ModMask,
			DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync);
	
	XQueryTree(dpy,DefaultRootWindow(dpy),&returnedroot,&returnedparent,&children, &numchildren);
    start.subwindow = None;
    for(;;)
    {
	
		XQueryTree(dpy,DefaultRootWindow(dpy),&returnedroot,&returnedparent,&children, &numchildren);


//		printf("num children = %u \n", numchildren);
//		printf("for loop \n");
        XNextEvent(dpy, &ev);
		if(ev.type == KeyPress && ev.xkey.keycode == XK_TAB)
		{
            XRaiseWindow(dpy, 1);
			printf("Tab pressed");
		}
        else if(ev.type == ButtonPress && ev.xbutton.subwindow != None)
        {
            XGetWindowAttributes(dpy, ev.xbutton.subwindow, &attr);
            start = ev.xbutton;
        }
        else if(ev.type == MotionNotify && start.subwindow != None)
        {
            int xdiff = ev.xbutton.x_root - start.x_root;
            int ydiff = ev.xbutton.y_root - start.y_root;
            XMoveResizeWindow(dpy, start.subwindow,
                attr.x + (start.button==1 ? xdiff : 0),
                attr.y + (start.button==1 ? ydiff : 0),
                MAX(1, attr.width + (start.button==3 ? xdiff : 0)),
                MAX(1, attr.height + (start.button==3 ? ydiff : 0)));
        }
        else if(ev.type == ButtonRelease)
            start.subwindow = None;
    }
}

