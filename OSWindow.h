#pragma once
#include "OSAPI.h"
#include "NewUI/Core/UIControl.h"

class OSWindow: public UIControl
{
  public:
    static OSWindow *Create(const wchar_t *title, int window_width, int window_height);


    void SetFullscreenMode();
    void SetWindowedMode(int new_window_width, int new_window_height);
    void SetCursor(const char *cursor_name);

    inline HWND         GetApiHandle() const {return api_handle;}
    inline unsigned int GetWidth() const     {RECT rc; GetClientRect(api_handle, &rc); return rc.right - rc.left;}
    inline unsigned int GetHeight() const    {RECT rc; GetClientRect(api_handle, &rc); return rc.bottom - rc.top;}
    

    bool OnOSMenu(int item_id);

    // UIControl
    virtual void Show() override;
    
    // UIControl
    virtual bool Focused() override  {return GetFocus() == api_handle;}
    
    // UIControl
    //virtual bool OnKey(int key) override;
    // UIControl
    virtual bool OnMouseLeftClick(int x, int y) override;
    // UIControl
    virtual bool OnMouseRightClick(int x, int y) override;
    // UIControl
    //virtual bool OnMouseEnter() override;
    // UIControl
    //virtual bool OnMouseLeave() override;

  protected:
    bool Initialize(const wchar_t *title, int window_width, int window_height);
    bool WinApiMessageProc(UINT msg, WPARAM wparam, LPARAM lparam, LRESULT *result);

    HWND  api_handle;

  private:
    static LPARAM __stdcall WinApiMsgRouter(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    HMENU CreateDebugMenuPanel();
};