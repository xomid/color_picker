#include "UISidePalette.h"
#include "util.h"

void UISidePalette::on_init() {
	handle = parse_svg(R"(
		<svg width="24" height="24" viewBox="-2 -4 24 24" preserveAspectRatio="xMinYMin"><path fill="currentColor" d="m12.547 1.621 6.095 9.794A3 3 0 0 1 16.095 16H3.905a3 3 0 0 1-2.547-4.585L7.453 1.62a3 3 0 0 1 5.094 0z"/></svg>
	)");
	handle->m_colors = &colors;
	colors["currentColor"] = Color("#fff");

	handleY = 0;
	handleW = 10;
	marginH = 1;
	marginV = 10;
	barW = boxModel.width - 2 * (handleW + marginH);
	imgLeft = marginH + handleW;
	imgTop = marginV;
	img.create(barW, 256, 3);
	fullHueImg.create(1, 256, 3);
	_gen_hue(fullHueImg, false);
	value = 0;
	lmat *= agg::trans_affine_rotation(PI_D2);
	rmat *= agg::trans_affine_rotation(-PI_D2);
	if (parent) set_background_color(parent->backgroundColor);

	// test
	img.clear(0xff, 0xff, 0);
	Color c("#f00");
	//gen_bri(value, false, c);
}

void UISidePalette::_gen_hue(Sheet& sheet, bool bWebColor) {
	int width = sheet.w,
		height = sheet.h,
		pitch = sheet.pitch,
		x, y;
	byte* data = sheet.data, * d;
	if (height != 256) return;

	float r, g, b, im, max, min, h, ins;
	int val;
	byte bb, bg, br;

	ins = 1.00f;
	r = 255;
	g = 0;
	b = 0;
	im = 255;
	val = 0;
	ins = im / 42.5;
	max = 255 - ins, min = ins;

	for (y = 0; y < 256; y++)
	{
		d = data + (255 - y) * pitch;
		if (y > 0 && y < 44) {
			if (g <= max)
				g += ins;
			else
			{
				h = 255 - g;
				g = 255;
				r -= ins - h;
			}
		}
		else if (y < 86 && y >= 44) {
			if (r >= min)
				r -= ins;
			else
			{
				h = r - val;
				r = val;
				b += ins - h;
			}
		}
		else if (y < 129 && y >= 86) {
			if (b <= max)
				b += ins;
			else
			{
				h = 255 - b;
				b = 255;
				g -= ins - h;
			}
		}
		else if (y < 171 && y >= 129) {
			if (g >= min)
				g -= ins;
			else
			{
				h = g - val;
				g = val;
				r += ins - h;
			}
		}
		else if (y < 214 && y >= 171) {
			if (r <= max)
				r += ins;
			else
			{
				h = 255 - r;
				r = 255;
				b -= ins - h;
			}
		}
		else if (y < 256 && y >= 214) {
			if (b >= min)
				b -= ins;
			else
				b = val;
		}

		bb = CLAMP255(int(b));
		bg = CLAMP255(int(g));
		br = CLAMP255(int(r));

		if (bWebColor) {
			bb = lookupWebColor[bb];
			bg = lookupWebColor[bg];
			br = lookupWebColor[br];
		}

		for (x = 0; x < width; x++) {
			*d++ = bb;
			*d++ = bg;
			*d++ = br;
		}
	}
}

void UISidePalette::gen_hue(bool bWebColor) {
	_gen_hue(img, bWebColor);
}

void UISidePalette::gen_sat(const Color& hint, bool bWebColor) {
	int width = img.w,
		height = img.h,
		pitch = img.pitch,
		x, y;
	byte* data = img.data, * d;
	if (height != 256) return;

	float n2, n3, r, g, b, * max, * mean, * min, m2, m3;
	int ib, ig, ir;
	byte bb, bg, br;

	r = (float)hint.r, g = (float)hint.g, b = (float)hint.b;

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

	//set the value that will be substracted
	n2 = (*max - *mean) / 255;
	n3 = (*max - *min) / 255;

	//let's fill the image


	for (y = 0; y < 256; y++) {
		d = data + y * pitch;

		if (y > 0) {
			*mean += n2;
			*min += n3;
		}

		ib = int(b + 0.01);
		ig = int(g + 0.01);
		ir = int(r + 0.01);

		bb = CLAMP255(ib);
		bg = CLAMP255(ig);
		br = CLAMP255(ir);

		if (bWebColor) {
			bb = lookupWebColor[bb];
			bg = lookupWebColor[bg];
			br = lookupWebColor[br];
		}

		for (x = 0; x < width; x++) {
			*d++ = bb;
			*d++ = bg;
			*d++ = br;
		}
	}
}

void UISidePalette::gen_bri(const Color& hint, bool bWebColor) {
	int width = img.w,
		height = img.h,
		pitch = img.pitch,
		x, y;
	byte* data = img.data, * d;
	if (height != 256) return;

	float nr, ng, nb, r, g, b;
	r = (float)hint.r, g = (float)hint.g, b = (float)hint.b;
	nr = r / 255;
	ng = g / 255;
	nb = b / 255;
	byte bb, bg, br;

	for (y = 0; y < 256; y++) {
		d = data + y * pitch;

		bb = int(b);
		bg = int(g);
		br = int(r);

		if (bWebColor) {
			bb = lookupWebColor[CLAMP255(bb)];
			bg = lookupWebColor[CLAMP255(bg)];
			br = lookupWebColor[CLAMP255(br)];
		}

		for (x = 0; x < width; x++) {
			*d++ = bb;
			*d++ = bg;
			*d++ = br;
		}

		r -= nr;
		g -= ng;
		b -= nb;
	}
}

void UISidePalette::gen_red(Color& hint, bool bWebColor) {
	int width = img.w,
		height = img.h,
		pitch = img.pitch,
		x, y;
	byte* data = img.data, * d, r, g, b;
	if (height != 256) return;

	r = hint.r;
	g = hint.g;
	b = hint.b;

	if (bWebColor) {
		b = lookupWebColor[b];
		g = lookupWebColor[g];
	}

	for (y = 0; y < 256; y++) {
		r = 255 - y;
		if (bWebColor)
			r = lookupWebColor[r];
		d = data + y * pitch;
		for (x = 0; x < width; x++) {
			*d++ = b;
			*d++ = g;
			*d++ = r;
		}
	}
}

void UISidePalette::gen_grn(Color& hint, bool bWebColor) {
	int width = img.w,
		height = img.h,
		pitch = img.pitch,
		x, y;
	byte* data = img.data, * d, r, g, b;
	if (height != 256) return;

	r = hint.r;
	g = hint.g;
	b = hint.b;

	if (bWebColor) {
		b = lookupWebColor[b];
		r = lookupWebColor[r];
	}

	for (y = 0; y < 256; y++) {
		g = 255 - y;
		if (bWebColor)
			g = lookupWebColor[g];
		d = data + y * pitch;
		for (x = 0; x < width; x++) {
			*d++ = b;
			*d++ = g;
			*d++ = r;
		}
	}
}

void UISidePalette::gen_blu(Color& hint, bool bWebColor) {
	int width = img.w,
		height = img.h,
		pitch = img.pitch,
		x, y;
	byte* data = img.data, * d, r, g, b;
	if (height != 256) return;

	r = hint.r;
	g = hint.g;
	b = hint.b;

	if (bWebColor) {
		r = lookupWebColor[r];
		g = lookupWebColor[g];
	}

	for (y = 0; y < 256; y++) {
		b = 255 - y;
		if (bWebColor)
			b = lookupWebColor[b];
		d = data + y * pitch;
		for (x = 0; x < width; x++) {
			*d++ = b;
			*d++ = g;
			*d++ = r;
		}
	}
}

int UISidePalette::get_value() const {
	return 255 - handleY;
}

void UISidePalette::set_value(int newValue) {
	handleY = 255 - newValue;
}

void UISidePalette::on_update() {
	OUI::on_update();
	canvas.bit_blt(img, imgLeft, imgTop, img.w, img.h, 0, 0, true);

	int dx = handleW / 2;
	int l = dx, t = handleY + marginV;

	canvas.render_svg_origin(handle, l, t, 0.4, 0xff, &lmat);
	l = imgLeft + barW + dx;
	canvas.render_svg_origin(handle, l, t, 0.4, 0xff, &rmat);
}

void UISidePalette::pick(int x, int y) {

	handleY = CLAMP3(0, y - marginV, 255);
	invalidate();
	if (parent) parent->process_event(this, Event::Update, 0, true);
}

void UISidePalette::on_mouse_move(int x, int y, uint32_t flags) {
	if (bPressed) pick(x, y);
}

void UISidePalette::on_mouse_down(int x, int y, uint32_t flags) {
	bPressed = true;
	set_capture(this);
	pick(x, y);
}

void UISidePalette::on_mouse_up(int x, int y, uint32_t flags) {
	bPressed = false;
	if (get_capture() == this)
		release_capture();
}

void UISidePalette::on_dbl_click(int x, int y, uint32_t flags) {
	on_mouse_down(x, y, flags);
}

void UISidePalette::get_full_hue(Color& res, int hue) const {
	get_pixel(fullHueImg, 0, 255 - hue, res);
}

