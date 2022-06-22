#include "util.h"

void get_pixel(const Sheet& img, int x, int y, Color& res) {
	if (x < 0 || y < 0 || x >= img.w || y >= img.h) {
		res.set(0, 0, 0, 0xff);
		return;
	}
	byte* d = img.data + y * img.pitch + 3 * x;
	res.b = d[0];
	res.g = d[1];
	res.r = d[2];
}

void rgb_to_hsb(int red, int green, int blue, int& hue, int& sat, int& bri)
{
	double maxRGB, minRGB, Delta;
	double h, s, b;

	//hue = 100;
	minRGB = Min(Min(red, green), blue);
	maxRGB = Max(Max(red, green), blue);
	Delta = maxRGB - minRGB;

	b = maxRGB;
	if (maxRGB > 0)
		s = 255 * Delta / maxRGB;
	else
		s = 0;

	if (s > 0)
	{
		if (red == maxRGB)
			h = (green - blue) / Delta;
		else
			if (green == maxRGB)
				h = 2 + (blue - red) / Delta;
			else
				if (blue == maxRGB)
					h = 4 + (red - green) / Delta;
	}
	else
		h = -1;

	h = h * 60;
	h = (h < 0) ? h + 360 : h;

	if (red == green && green == blue)
		h = 0;

	hue = (int)(h + 0.5);
	sat = (int)(((s * 100.000) / 255.000) + 0.5);
	bri = (int)(((b * 100.000 / 255.00)) + 0.5);
}

void hsb_to_rgb(int hue, int sat, int bri, byte& red, byte& green, byte& blue) {
	double      hh, p, q, t, ff, b, s;
	long        i;
	double ir, ig, ib;

	if (sat <= 0) {// < is bogus, just shuts up warnings
		ir = bri;
		ig = bri;
		ib = bri;
	}
	else {
		if (hue >= 360) hue = 0;

		hh = double(hue) / 60.0;
		s = double(sat) / 100.0;
		b = double(bri) / 100.0;

		i = (long)hh;
		ff = hh - i;
		p = b * (1.0 - s);
		q = b * (1.0 - (s * ff));
		t = b * (1.0 - (s * (1.0 - ff)));

		switch (i) {
		case 0:
			ir = b;
			ig = t;
			ib = p;
			break;
		case 1:
			ir = q;
			ig = b;
			ib = p;
			break;
		case 2:
			ir = p;
			ig = b;
			ib = t;
			break;

		case 3:
			ir = p;
			ig = q;
			ib = b;
			break;
		case 4:
			ir = t;
			ig = p;
			ib = b;
			break;
		case 5:
		default:
			ir = b;
			ig = p;
			ib = q;
			break;
		}
	}

	red = CLAMP255((int)(ir * 255 + 0.5));
	green = CLAMP255((int)(ig * 255 + 0.5));
	blue = CLAMP255((int)(ib * 255 + 0.5));
}

std::wstring to_whex(Color& color) {
	static wchar_t hexcol[16];
	memset(hexcol, 0, sizeof(hexcol));
	_snwprintf_s(hexcol, sizeof hexcol, L"%02x%02x%02x", color.r, color.g, color.b);
	return std::wstring(hexcol);
}