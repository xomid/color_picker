#pragma once
#include <oui_label.h>

class UISidePalette :public UILabel
{
private:
	Sheet img, fullHueImg;
	int value;
	agg::trans_affine rmat, lmat;
	int imgLeft, imgTop, barW, handleW, handleY, marginH, marginV, hue, sat, bri;
	PSVGShape handle;
	bool bPressed;

	void _gen_hue(Sheet& sheet, bool bWebColor);

public:
	void gen_hue(bool bWebColor);
	void gen_sat(const Color& hint, bool bWebColor);
	void gen_bri(const Color& hint, bool bWebColor);
	void gen_red(Color& hint, bool bWebColor);
	void gen_grn(Color& hint, bool bWebColor);
	void gen_blu(Color& hint, bool bWebColor);
	void get_full_hue(Color& res, int hue) const;
	int get_value() const;
	void set_value(int newValue);
	void pick(int x, int y);

	void on_init() override;
	void on_update() override;
	void on_mouse_move(int x, int y, uint32_t flags) override;
	void on_mouse_down(int x, int y, uint32_t flags) override;
	void on_mouse_up(int x, int y, uint32_t flags) override;
	void on_dbl_click(int x, int y, uint32_t flags) override;
};

