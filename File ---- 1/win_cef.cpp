#include "stdafx.h"
#include "win_cef.h"

const char kProcessType[] = "type";
const char kRendererProcess[] = "renderer";

enum ProcessType {
    PROCESS_TYPE_BROWSER,
    PROCESS_TYPE_RENDERER,
    PROCESS_TYPE_OTHER,
};

class Client :public CefClient {
public:
    Client() {};
    virtual ~Client() {}

private:
    IMPLEMENT_REFCOUNTING(Client);
    DISALLOW_COPY_AND_ASSIGN(Client);
};


CefRefPtr<CefApp> CreateBrowserProcessApp() {
    return CefRefPtr<CefApp>();
}

CefRefPtr<CefApp> CreateRendererProcessApp() {
    return CefRefPtr<CefApp>();
}

CefRefPtr<CefApp> CreateOtherProcessApp() {
    return CefRefPtr<CefApp>();
}

CefRefPtr<CefClient> CreateClient() {
    return new Client();
}

CefRefPtr<CefCommandLine> CreateCommandLine(const CefMainArgs& main_args) {
    CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
    command_line->InitFromString(::GetCommandLineW());
    return command_line;
}

ProcessType GetProcessType(const CefRefPtr<CefCommandLine>& command_line) {
    if (!command_line->HasSwitch(kProcessType))
        return PROCESS_TYPE_BROWSER;

    const std::string& process_type = command_line->GetSwitchValue(kProcessType);
    if (process_type == kRendererProcess)
        return PROCESS_TYPE_RENDERER;

    return PROCESS_TYPE_OTHER;

}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
    {
        RECT rcClient;
        ::GetClientRect(hWnd, &rcClient);

        if (!::IsRectEmpty(&rcClient)) {

            CefWindowInfo info;
            info.SetAsChild(hWnd, rcClient);
            CefBrowserHost::CreateBrowser(info, CreateClient(), "https://www.baidu.com/", CefBrowserSettings(), nullptr);
        }
        return 0;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

ATOM CefRegisterClass(HINSTANCE hInstance) {
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINCEF));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"CefWindowClass";
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR    lpCmdLine,
                      _In_ int       nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    CefEnableHighDPISupport();
    CefMainArgs main_args(hInstance);
    CefRefPtr<CefCommandLine> command_line = CreateCommandLine(main_args);
    CefRefPtr<CefApp> app;
    switch (GetProcessType(command_line)) {
    case PROCESS_TYPE_BROWSER:
        app = CreateBrowserProcessApp();
        break;
    case PROCESS_TYPE_RENDERER:
        app = CreateRendererProcessApp();
        break;
    case PROCESS_TYPE_OTHER:
        app = CreateOtherProcessApp();
        break;
    }

    int exit_code = CefExecuteProcess(main_args, app, nullptr);
    if (exit_code >= 0) {
        return exit_code;
    }

    CefSettings settings;
    CefInitialize(main_args, settings, app, nullptr);

    CefRegisterClass(hInstance);

    HWND hWnd = CreateWindowW(L"CefWindowClass",
                              L"CefBrowser",
                              WS_OVERLAPPEDWINDOW,
                              CW_USEDEFAULT,
                              0,
                              CW_USEDEFAULT,
                              0,
                              nullptr, nullptr, hInstance, nullptr);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    CefRunMessageLoop();
    CefShutdown();
    return 0;
}