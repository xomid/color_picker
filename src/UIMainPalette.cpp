#include "UIMainPalette.h"
#include "UISidePalette.h"
#include "util.h"

void UIMainPalette::on_init() {
	line = new byte[256 * 3];
	img.create(256, 256, 3);
	circle = parse_svg(R"(
		<svg width="12" height="12"><circle cx="6" cy="6" r="5" stroke="currentColor" stroke-width="1.5" fill="none"/></svg>
	)");
	circle->m_colors = &colors;
	pxSelected.set(0, 0);
	currentColor = &colors["currentColor"];

	fullSatImg.create(256, 256, 3);
	fullBriImg.create(256, 256, 3);
	_gen_sat(fullSatImg, 255, false);
	_gen_bri(fullBriImg, 255, false);

	// test
	img.clear(0, 0xff, 0);
	Color c("#f00");
	gen_hue(c, 0);
}

void UIMainPalette::gen_hue(const Color& hint, bool bWebColor) {
	int width = img.w,
		height = img.h,
		pitch = img.pitch,
		x, y;
	byte* data = img.data, * d;
	if (height != 256 && width != 256) return;

	float n2, n3, r, g, b, * max, * min, * mean, m2, m3;
	byte br, bg, bb;
	r = hint.r, g = hint.g, b = hint.b;

	//Set max, mean and min pointer
	if (r > g)
		if (r > b)
		{
			max = &r;
			if (g > b) { mean = &g;  min = &b; }
			else { mean = &b;  min = &g; }
		}
		else { max = &b;  mean = &r;  min = &g; }

	else
		if (g > b)
		{
			max = &g;
			if (r > b) { mean = &r;  min = &b; }
			else { mean = &b;  min = &r; }
		}
		else { max = &b;  mean = &g;  min = &r; }

	//Save first value of mean and min;
	m2 = *mean;
	m3 = *min;

	//Start filling the image
	for (y = 0; y < 256; y++)
	{
		d = data + y * pitch;
		*max = 255.f - y;
		*mean = *max;
		*min = *max;

		//set the value that will be substracted in the horizontal directon
		n2 = (*max - (m2 - y * (m2 / 255.f))) / 255.f;
		n3 = (*max - (m3 - y * (m3 / 255.f))) / 255.f;

		//let's fill the image
		for (x = 0; x < 256; x++) {
			if (x > 0) {
				*mean -= n2;
				*min -= n3;
			}

			br = CLAMP255((int)(r + .5f));
			bg = CLAMP255((int)(g + .5f));
			bb = CLAMP255((int)(b + .5f));

			if (bWebColor) {
				br = lookupWebColor[br];
				bg = lookupWebColor[bg];
				bb = lookupWebColor[bb];
			}

			*d++ = bb;
			*d++ = bg;
			*d++ = br;
		}
	}

	get_pixel(img, pxSelected.x, pxSelected.y, crLast);
}

void UIMainPalette::gen_hue_line_ns(int val)
{
	int x;
	byte* d = line;

	float r, g, b, ins, im, max, min, h, fval;
	fval = (float)val;
	ins = 1.00f;
	r = 255.f;
	g = 255.f - fval;
	b = 255.f - fval;
	im = fval;
	ins = im / 42.5f;
	max = 255.f - ins, min = ins;

	for (x = 0; x < 256; x++) {
		if (x > 0 && x < 44)
		{
			if (g <= max)
				g += ins;
			else
			{
				h = 255.f - g;
				g = 255.f;
				r -= ins - h;
			}
		}
		else if (x < 86 && x >= 44)
		{
			if (r >= min)
				r -= ins;
			else
			{
				h = r - fval;
				r = fval;
				b += ins - h;
			}
		}
		else if (x < 129 && x >= 86)
		{
			if (b <= max)
				b += ins;
			else
			{
				h = 255.f - b;
				b = 255.f;
				g -= ins - h;
			}
		}
		else if (x < 171 && x >= 129)
		{
			if (g >= min)
				g -= ins;
			else
			{
				h = g - fval;
				g = fval;
				r += ins - h;
			}
		}
		else if (x < 214 && x >= 171)
		{
			if (r <= max)
				r += ins;
			else
			{
				h = 255.f - r;
				r = 255.f;
				b -= ins - h;
			}
		}
		else if (x < 256 && x >= 214)
		{
			if (b >= min)
				b -= ins;
			else
			{
				b = fval;
			}
		}

		/*if (!(r >= fval && r <= 255))
			if (r < fval) r = fval;
			else r = 255;
		if (!(g >= fval && g <= 255))
			if (g < fval) g = fval;
			else g = 255;
		if (!(b >= fval && b <= 255))
			if (b < fval) b = fval;
			else b = 255;*/

		*d++ = CLAMP255(int(b));
		*d++ = CLAMP255(int(g));
		*d++ = CLAMP255(int(r));
	}
}

void UIMainPalette::gen_hue_line_nb(int val)
{
	int x;
	byte* d = line;
	float r, g, b, ins, im, max, min, h, fval;

	fval = (float)val;
	ins = 1.00f;
	r = fval; g = b = 0.f;
	im = fval;
	ins = im / 42.5f;
	max = fval - ins, min = ins;

	for (x = 0; x < 256; x++) {
		if (x > 0 && x < 44)
		{
			if (g <= max)
				g += ins;
			else
			{
				h = fval - g;
				g = fval;
				r -= ins - h;
			}
		}
		else if (x < 86 && x >= 44)
		{
			if (r >= min)
				r -= ins;
			else
			{
				h = r;
				r = 0.f;
				b += ins - h;
			}
		}
		else if (x < 129 && x >= 86)
		{
			if (b <= max)
				b += ins;
			else
			{
				h = fval - b;
				b = fval;
				g -= ins - h;
			}
		}
		else if (x < 171 && x >= 129)
		{
			if (g >= min)
				g -= ins;
			else
			{
				h = g;
				g = 0.f;
				r += ins - h;
			}
		}
		else if (x < 214 && x >= 171)
		{
			if (r <= max)
				r += ins;
			else
			{
				h = fval - r;
				r = fval;
				b -= ins - h;
			}
		}
		else if (x < 256 && x >= 214)
		{
			if (b >= min)
				b -= ins;
			else
				b = 0.f;
		}

		*d++ = CLAMP255(int(b));
		*d++ = CLAMP255(int(g));
		*d++ = CLAMP255(int(r));
	}
}

void UIMainPalette::_gen_sat(Sheet& sheet, int val, bool bWebColor) {

	int width = sheet.w,
		height = sheet.h,
		pitch = sheet.pitch,
		x, y;
	byte* data = sheet.data, * d;
	if (height != 256 && width != 256) return;

	gen_hue_line_ns(val);

	float b, g, r;
	float nr, ng, nb;
	byte* s;

	//fill the image
	s = line;
	for (x = 0; x < 256; x++) {
		d = data + 3 * x;

		b = *s++;
		g = *s++;
		r = *s++;

		nr = r / 255;
		ng = g / 255;
		nb = b / 255;

		for (y = 0; y < 256; y++) {
			if (bWebColor) {
				d[0] = CLAMP255(lookupWebColor[(int)(b + .5f)]);
				d[1] = CLAMP255(lookupWebColor[(int)(g + .5f)]);
				d[2] = CLAMP255(lookupWebColor[(int)(r + .5f)]);
			}
			else {
				d[0] = CLAMP255((int)(b + .5f));
				d[1] = CLAMP255((int)(g + .5f));
				d[2] = CLAMP255((int)(r + .5f));
			}

			r -= nr;
			g -= ng;
			b -= nb;

			d += pitch;
		}
	}
}

void UIMainPalette::_gen_bri(Sheet& sheet, int val, bool bWebColor) {

	int width = sheet.w,
		height = sheet.h,
		pitch = sheet.pitch,
		x, y;
	byte* data = sheet.data, * d;
	if (height != 256 && width != 256) return;

	float b, g, r;
	float nr, ng, nb, max;
	byte* s;

	max = (float)val;
	gen_hue_line_nb(val);

	//fill the image
	s = line;
	for (x = 0; x < 256; x++) {
		d = data + 3 * x;

		b = *s++;
		g = *s++;
		r = *s++;

		nr = r / 255.f;
		ng = g / 255.f;
		nb = b / 255.f;

		nr = (max - r) / 255.f;
		ng = (max - g) / 255.f;
		nb = (max - b) / 255.f;

		for (y = 0; y < 256; y++) {
			if (bWebColor) {
				d[0] = lookupWebColor[CLAMP255(int(b + .5f))];
				d[1] = lookupWebColor[CLAMP255(int(g + .5f))];
				d[2] = lookupWebColor[CLAMP255(int(r + .5f))];
			}
			else {
				d[0] = CLAMP255((int)(b + .5f));
				d[1] = CLAMP255((int)(g + .5f));
				d[2] = CLAMP255((int)(r + .5f));
			}

			r += nr;
			g += ng;
			b += nb;

			d += pitch;
		}
	}
}

void UIMainPalette::gen_sat(int sat, bool bWebColor) {
	_gen_sat(img, sat, bWebColor);
	get_pixel(img, pxSelected.x, pxSelected.y, crLast);
}

void UIMainPalette::gen_bri(int bri, bool bWebColor) {
	_gen_bri(img, bri, bWebColor);
	get_pixel(img, pxSelected.x, pxSelected.y, crLast);
}

void UIMainPalette::gen_red(int red, bool bWebColor) {
	int width = img.w,
		height = img.h,
		pitch = img.pitch,
		g, b;
	byte* data = img.data, * d;
	if (height != 256 && width != 256) return;

	if (bWebColor) {
		for (g = 255; g >= 0; g--) {
			d = data + (255 - g) * pitch;
			for (b = 0; b < 256; b++) {
				*d++ = lookupWebColor[b];
				*d++ = lookupWebColor[g];
				*d++ = red;
			}
		}
	}
	else {
		for (g = 255; g >= 0; g--) {
			d = data + (255 - g) * pitch;
			for (b = 0; b < 256; b++) {
				*d++ = b;
				*d++ = g;
				*d++ = red;
			}
		}
	}

	get_pixel(img, pxSelected.x, pxSelected.y, crLast);
}

void UIMainPalette::gen_grn(int grn, bool bWebColor) {
	int width = img.w,
		height = img.h,
		pitch = img.pitch,
		r, b;
	byte* data = img.data, * d;
	if (height != 256 && width != 256) return;

	if (bWebColor) {
		for (r = 255; r >= 0; r--) {
			d = data + (255 - r) * pitch;
			for (b = 0; b < 256; b++) {
				*d++ = lookupWebColor[b];
				*d++ = grn;
				*d++ = lookupWebColor[r];
			}
		}
	}
	else {
		for (r = 255; r >= 0; r--) {
			d = data + (255 - r) * pitch;
			for (b = 0; b < 256; b++) {
				*d++ = b;
				*d++ = grn;
				*d++ = r;
			}
		}
	}

	get_pixel(img, pxSelected.x, pxSelected.y, crLast);
}

void UIMainPalette::gen_blu(int blu, bool bWebColor) {
	int width = img.w,
		height = img.h,
		pitch = img.pitch,
		r, g;
	byte* data = img.data, * d;
	if (height != 256 && width != 256) return;

	if (bWebColor) {
		for (g = 255; g >= 0; g--) {
			d = data + (255 - g) * pitch;
			for (r = 0; r < 256; r++) {
				*d++ = blu;
				*d++ = lookupWebColor[g];
				*d++ = lookupWebColor[r];
			}
		}
	}
	else {
		for (g = 255; g >= 0; g--) {
			d = data + (255 - g) * pitch;
			for (r = 0; r < 256; r++) {
				*d++ = blu;
				*d++ = g;
				*d++ = r;
			}
		}
	}

	get_pixel(img, pxSelected.x, pxSelected.y, crLast);
}

void UIMainPalette::on_update() {
	canvas.bit_blt(img, 0, 0, img.w, img.h, 0, 0, true);
	godSheet->setclip(&absContentShape, 0xff);
	canvas.render_svg_origin(circle, pxSelected.x, pxSelected.y, 1.0, 0xff);
	godSheet->unclip();
}

void UIMainPalette::pick(int x, int y, bool alert) {

	x = CLAMP3(0, x, 255);
	y = CLAMP3(0, y, 255);
	pxSelected.set(x, y);

	if (pxSelected.x < 0 || pxSelected.y < 0 || pxSelected.x > 255 || pxSelected.y > 255
		|| img.w != 256 || img.h != 256) {
		color.set(0, 0, 0, 0xff);
		return;
	}
	byte* d = img.data + pxSelected.y * img.pitch + 3 * pxSelected.x;
	crLast.b = d[0];
	crLast.g = d[1];
	crLast.r = d[2];

	if (((crLast.r + crLast.g + crLast.b) / 3) >= 70) currentColor->set(0, 0, 0);
	else currentColor->set(0xff, 0xff, 0xff);

	invalidate();
	if (parent && alert) parent->process_event(this, Event::Update, 0, true);
}

void UIMainPalette::on_mouse_move(int x, int y, uint32_t flags) {
	if (bPressed) pick(x, y, true);
}

void UIMainPalette::on_mouse_down(int x, int y, uint32_t flags) {
	bPressed = true;
	set_capture(this);
	pick(x, y, true);
}

void UIMainPalette::on_mouse_up(int x, int y, uint32_t flags) {
	bPressed = false;
	if (get_capture() == this)
		release_capture();
}

void UIMainPalette::on_dbl_click(int x, int y, uint32_t flags) {
	on_mouse_down(x, y, flags);
}

Color UIMainPalette::get_picked_color() const {
	return crLast;
}

void UIMainPalette::get_full_sat(Color& res) const {
	get_pixel(fullSatImg, pxSelected.x, pxSelected.y, res);
}

void UIMainPalette::get_full_bri(Color& res) {
	get_pixel(fullBriImg, pxSelected.x, pxSelected.y, res);
}

void UIMainPalette::get_selector(Point& selector) const {
	selector = pxSelected;
}

void UIMainPalette::set_selector(Point selector, bool alert, Color* calibrateColor) {

	if (calibrateColor) {
		int width = img.w,
			height = img.h,
			pitch = img.pitch,
			x, y;
		byte* data = img.data, * d;

		for (y = 0; y < height; y++) {
			d = data + y * pitch;
			for (x = 0; x < width; ++x) {
				if (*d++ == calibrateColor->b &&
					*d++ == calibrateColor->g &&
					*d++ == calibrateColor->r) {
					selector.set(x, y);
					break;
				}
			}
		}

		invalidate();
	}

	pick(selector.x, selector.y, alert);
}
