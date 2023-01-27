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

std::wstring to_whex(Color& color) {
	static wchar_t hexcol[16];
	memset(hexcol, 0, sizeof(hexcol));
	_snwprintf_s(hexcol, sizeof hexcol, L"%02x%02x%02x", color.r, color.g, color.b);
	return std::wstring(hexcol);
}