#include "window_manager.hpp"
#include <glog/logging.h>

using ::std::unique_ptr;

unique_ptr<WindowManager> WindowManager::Create() {
	//open display
	Display* display = XOpenDisplay(nullptr);
	if (display == nullptr){
		LOG(ERROR) << "Failed to open X Disp" << XDisplayName(nullptr);
		return nullptr;

	}
	//Construct WM inst
	return unique_ptr<WindowManager>(new WindowManager(display));
}

WindowManager::WindowManager(Display* display)
	: display_(CHECK_NOTNULL(display)),
	root_(DefaultRootWindow(display_)) {
	}

WindowManager::~WindowManager() {
	XCloseDisplay(display_);
}


void WindowManager::Run() {
	wm_detected_ = false;
	XSetErrorHandler(&WindowManager::OnWMDetected);
	XSelectInput(
			display_,
			root_,
			SubstructureRedirectMask | SubstructureNotifyMask);
	
	XSync(display_, false);

	if (wm_detected_) {
	
		LOG(ERROR) << "Detected another WM on disp"
			<<XDisplayString(display_);
		return;

	}
	XSetErrorHandler(&WindowManager::OnXError);

	//main loop


}

int WindowManager::OnWMDetected(Display* display, XErrorEvent* e) {

	CHECK_EQ(static_cast<int>(e->error_code), BadAccess);
	wm_detected_ = true;
	return 0;

}

int WindowManager::OnXError(Display* display, XErrorEvent* e) {
	const int MAX_ERROR_TEXT_LENGTH = 1024;
	char error_text[MAX_ERROR_TEXT_LENGTH];
	XGetErrorText(display, e->error_code, error_text, sizeof(error_text));
	LOG(ERROR) << "Received X error:\n"
	           << "    Request: " << int(e->request_code)
	           << " - " << XRequestCodeToString(e->request_code) << "\n"
	           << "    Error code: " << int(e->error_code)
	           << " - " << error_text << "\n"
	           << "    Resource ID: " << e->resourceid;
	 // The return value is ignored.
	return 0;

}

