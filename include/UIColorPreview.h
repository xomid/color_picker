#pragma once
#include <oui.h>
class UIColorPreview :
    public OUI
{
private:
    Color crNew, crCurrent;
    Rect rcNew, rcCurrent;

public:
    void set_new(Color crNew);
    void set_current(Color crCurrent);
    Color get_current_color();

    void on_init() override;
    void on_update() override;
    void on_resize(int width, int height) override; 
    void on_mouse_down(int x, int y, uint32_t flags) override;
    void on_dbl_click(int x, int y, uint32_t flags) override;
};

