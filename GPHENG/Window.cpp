#include "Window.h"
#include "iostream"

Window::Window()
{
	m_hwnd = NULL;
	m_isRun = false;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_CREATE: {
		Window* window = (Window*)((LPCREATESTRUCT)lparam)->lpCreateParams;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
		window->setHWND(hwnd);
		window->onCreate();
		break;
	}
	case WM_DESTROY: {
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->onDestroy();
		::PostQuitMessage(0);
		break;
	}
	default: {
		return ::DefWindowProc(hwnd, msg, wparam, lparam);
	}
	}

	return NULL;
}

bool Window::init()
{
	const wchar_t CLASS_NAME[] = L"MyWindowClass";
	const wchar_t APP_DESCRIPTION[] = L"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";

	WNDCLASSEX wc;
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = NULL;
	wc.lpszClassName = CLASS_NAME;
	wc.lpszMenuName = APP_DESCRIPTION;
	wc.style = NULL;
	wc.lpfnWndProc = &WndProc;

	if (!::RegisterClassEx(&wc)) {
		return false;
	}

	m_hwnd = ::CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW, 
		CLASS_NAME,
		APP_DESCRIPTION,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		1024, 768,
		NULL,
		NULL, 
		NULL, 
		this);

	if (!m_hwnd) {
		return false;
	}

	::ShowWindow(m_hwnd, SW_SHOW);
	::UpdateWindow(m_hwnd);

	m_isRun = true;

	return true;
}

bool Window::broadcastWindow()
{
	MSG msg;

	this->onUpdate();

	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	Sleep(1);

	return true;
}

bool Window::isRunning() {
	return m_isRun;
}

bool Window::release()
{
	if (!::DestroyWindow(m_hwnd)) {
		return false;
	}

	return true;
}

void Window::onDestroy()
{
	m_isRun = false;
}

RECT Window::getClientWindowRect()
{
	RECT rc;
	::GetClientRect(this->m_hwnd, &rc);
	return rc;
}

void Window::setHWND(HWND hwnd)
{
	this->m_hwnd = hwnd;
}

void Window::onCreate()
{

}

void Window::onUpdate()
{

}