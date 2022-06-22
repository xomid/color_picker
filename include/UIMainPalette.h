#pragma once
#include <oui_label.h>

class UIMainPalette : public UILabel
{
private:
	Sheet img, fullSatImg, fullBriImg;
	PSVGShape circle;
	Point pxSelected;
	Color crNew, crLast, *currentColor;
	byte* line;
	bool bPressed;

	void gen_hue_line_ns(int val);
	void gen_hue_line_nb(int val);
	void _gen_sat(Sheet& sheet, int val, bool bWebColor);
	void _gen_bri(Sheet& sheet, int val, bool bWebColor);

public:
	void gen_hue(const Color& hint, bool bWebColor);
	void gen_sat(int sat, bool bWebColor);
	void gen_bri(int bri, bool bWebColor);
	void gen_red(int red, bool bWebColor);
	void gen_grn(int grn, bool bWebColor);
	void gen_blu(int blu, bool bWebColor);
	void pick(int x, int y, bool alert);
	Color get_picked_color() const;
	void get_full_sat(Color& res) const;
	void get_full_bri(Color& res);
	void get_selector(Point& selector) const;
	void set_selector(Point selector, bool alert, Color* calibrateColor);

	void on_init() override;
	void on_update() override;
	void on_mouse_move(int x, int y, uint32_t flags) override;
	void on_mouse_down(int x, int y, uint32_t flags) override;
	void on_mouse_up(int x, int y, uint32_t flags) override;
	void on_dbl_click(int x, int y, uint32_t flags) override;
};

