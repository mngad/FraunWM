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


void WindowManager::Run() {//TODO}

