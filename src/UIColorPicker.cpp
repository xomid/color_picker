#include "UIColorPicker.h"

void UIColorPicker::on_init()
{
	bWebColor = false;
	channel = 0;
	hue = 0;
	sat = 0;
	bri = 0;
	red = 0;
	grn = 0;
	blu = 0;

	int h, w, marg, hSpace, l, t;

	int radioCheckH = 14;
	int radioCheckW = 20;
	int hashW = 14;
	int numW = 60;
	marg = 5;
	mainPalette.create(10, 30, 256, 256, this);
	sidePalette.create(mainPalette.boxModel.left + mainPalette.boxModel.width + 3,
		mainPalette.boxModel.top - 10, 40, 256 + 20, this);
	chkWebColor.create(mainPalette.boxModel.left, mainPalette.boxModel.top + mainPalette.boxModel.height + 2 * marg, 100, radioCheckH, this);

	int previewW = 60;
	int previewH = 60;
	l = sidePalette.boxModel.left + sidePalette.boxModel.width + 3;
	t = sidePalette.boxModel.top;
	h = 14;
	hSpace = h + 3;
	lblNew.create(l, t, previewW, h, this); t += hSpace;
	preview.create(l, t, previewW, previewH, this);
	t += previewH + 3;
	lblCurrent.create(l, t, previewW, h, this);

	// buttons
	h = 30, w = 120, hSpace = h + marg;
	l = boxModel.width - w - marg;
	t = 10;
	h = 30;
	btnOK.create(l, t, w, h, this); t += hSpace;
	btnCancel.create(l, t, w, h, this); t += hSpace;
	//btnAdd.create(l, t, w, h, this);

	// radio buttons
	l = sidePalette.boxModel.left + sidePalette.boxModel.width + 10;
	t = 150;
	w = 300;
	grpChannel.create(l, t, w, 250, this);
	int L = 2;
	int T = 10;
	l = L;
	t = T;
	marg = 15;
	hSpace = radioCheckH + marg;
	rdHue.create(l, t, radioCheckW, radioCheckH, &grpChannel); t += hSpace;
	rdSat.create(l, t, radioCheckW, radioCheckH, &grpChannel); t += hSpace;
	rdBri.create(l, t, radioCheckW, radioCheckH, &grpChannel); t += hSpace;
	rdRed.create(l, t, radioCheckW, radioCheckH, &grpChannel); t += hSpace;
	rdGrn.create(l, t, radioCheckW, radioCheckH, &grpChannel); t += hSpace;
	rdBlu.create(l, t, radioCheckW, radioCheckH, &grpChannel); t += hSpace;
	lblHash.create(l, t, hashW, radioCheckH, &grpChannel);

	// number fields
	int d = 10;
	h = radioCheckH + d;
	t = T - d / 2;
	w = numW;
	marg = 10;
	l += radioCheckW + marg;
	numHue.create(l, t, w, h, &grpChannel); t += hSpace;
	numSat.create(l, t, w, h, &grpChannel); t += hSpace;
	numBri.create(l, t, w, h, &grpChannel); t += hSpace;
	numRed.create(l, t, w, h, &grpChannel); t += hSpace;
	numGrn.create(l, t, w, h, &grpChannel); t += hSpace;
	numBlu.create(l, t, w, h, &grpChannel); t += hSpace;

	l += hashW - (radioCheckW + marg);
	w += marg + radioCheckW - hashW;
	marg = 5;
	l += marg;
	w -= marg;
	txtHex.create(l, t, w, h, &grpChannel);

	t = 10 - d / 2;
	l += w + 4;
	w = 10;
	lblHue.create(l, t, w, h, &grpChannel); t += hSpace;
	lblSat.create(l, t, w, h, &grpChannel); t += hSpace;
	lblBri.create(l, t, w, h, &grpChannel); t += hSpace;

	l = 20, t = mainPalette.boxModel.top + mainPalette.boxModel.height + 10;

	btnOK.set_text(L"OK");
	btnCancel.set_text(L"Cancel");
	btnAdd.set_text(L"Add...");

	rdHue.set_text(L"H");
	rdSat.set_text(L"S");
	rdBri.set_text(L"B");
	rdRed.set_text(L"R");
	rdGrn.set_text(L"G");
	rdBlu.set_text(L"B");

	numHue.set_text(L"0");
	numSat.set_text(L"0");
	numBri.set_text(L"0");
	numRed.set_text(L"0");
	numGrn.set_text(L"0");
	numBlu.set_text(L"0");
	txtHex.set_text(L"000000");

	numHue.config("0", "1.0", "0", "360");
	numSat.config("0", "1.0", "0", "100");
	numBri.config("0", "1.0", "0", "100");
	numRed.config("0", "1.0", "0", "255");
	numGrn.config("0", "1.0", "0", "255");
	numBlu.config("0", "1.0", "0", "255");

	lblNew.set_text(L"new");
	lblCurrent.set_text(L"current");
	lblHue.set_text(L"°");
	lblSat.set_text(L"%");
	lblBri.set_text(L"%");
	lblHash.set_text(L"#");
	chkWebColor.set_text(L"Only Web Color");

	// set channel
	select_channel(0);
	select_channel(2);
	set_color(Colors::red);
}

void UIColorPicker::update_side_palette()
{
	if (channel == 0) {
		sidePalette.set_value(sideValue);
		sidePalette.gen_hue(bWebColor);
	}
	else if (channel == 1) {
		Color cr;
		mainPalette.get_full_sat(cr);
		sidePalette.gen_sat(cr, bWebColor);
	}
	else if (channel == 2) {
		Color cr;
		mainPalette.get_full_bri(cr);
		sidePalette.gen_bri(cr, bWebColor);
	}
	else if (channel == 3)
		sidePalette.gen_red(currentColor, bWebColor);
	else if (channel == 4)
		sidePalette.gen_grn(currentColor, bWebColor);
	else if (channel == 5)
		sidePalette.gen_blu(currentColor, bWebColor);
}

void UIColorPicker::update_main_palette()
{
	if (channel == 0) {
		int i = (sideValue > 254) ? 0 : sideValue;
		Color cr;
		sidePalette.get_full_hue(cr, i);
		mainPalette.gen_hue(cr, bWebColor);
	}
	else if (channel == 1)
		mainPalette.gen_sat(sideValue, bWebColor);
	else if (channel == 2)
		mainPalette.gen_bri(sideValue, bWebColor);
	else if (channel == 3)
		mainPalette.gen_red(bWebColor ? lookupWebColor[sideValue] : sideValue, bWebColor);
	else if (channel == 4)
		mainPalette.gen_grn(bWebColor ? lookupWebColor[sideValue] : sideValue, bWebColor);
	else if (channel == 5)
		mainPalette.gen_blu(bWebColor ? lookupWebColor[sideValue] : sideValue, bWebColor);
}

void UIColorPicker::select_channel(int channel) {
	switch (channel) {
	case 1: 
		rdSat.select(true);
		this->channel = 1;
		break;
	case 2:
		rdBri.select(true);
		this->channel = 2;
		break;
	case 3:
		rdRed.select(true);
		this->channel = 3;
		break;
	case 4:
		rdGrn.select(true);
		this->channel = 4;
		break;
	case 5:
		rdBlu.select(true);
		this->channel = 5;
		break;
	default:
		rdHue.select(true);
		this->channel = 0;
		break;
	}

}

void UIColorPicker::on_resize(int width, int height) {

}

void UIColorPicker::webalize_color(Color& color) {
	if (bWebColor) {
		color.r = lookupWebColor[color.r];
		color.g = lookupWebColor[color.g];
		color.b = lookupWebColor[color.b];
	}
}

void UIColorPicker::process_event(OUI* element, uint32_t message, uint64_t param, bool bubbleUp) {

	int srcChannel = -1;
	if (message == Event::Select ||
		(message == Event::Update && (
			element == &numHue || element == &numSat || element == &numBri ||
			element == &numRed || element == &numGrn || element == &numBlu || element == &txtHex)) ||
		element == &chkWebColor || element == &preview
		)
	{
		bool reset = false;

		if (element == &rdHue || element == &rdSat || element == &rdBri ||
			element == &rdRed || element == &rdGrn || element == &rdBlu || element == &chkWebColor)
			element->select(message == Event::Select);

		if (element == &numHue || element == &numSat || element == &numBri) {
			srcChannel = 0;
			hue = atoi(numHue.get_number().c_str());
			sat = atoi(numSat.get_number().c_str());
			bri = atoi(numBri.get_number().c_str());
			hsb_to_rgb(hue, sat, bri, currentColor.r, currentColor.g, currentColor.b);
			reset = true;
		}
		else if (element == &numRed || element == &numGrn || element == &numBlu || element == &txtHex) {
			if (element == &txtHex) {
				currentColor.set(txtHex.text);
				srcChannel = 6;
			}
			else srcChannel = 3;
			currentColor.r = atoi(numRed.get_number().c_str());
			currentColor.g = atoi(numGrn.get_number().c_str());
			currentColor.b = atoi(numBlu.get_number().c_str());
			reset = true;
		}

		if (bWebColor != chkWebColor.isSelected) {
			bWebColor = chkWebColor.isSelected;
			reset = true;
		}

		if (element == &preview) {
			currentColor = preview.get_current_color();
			srcChannel = 8;
			reset = true;
		}

		int curChannel = -1;
		if (element == &rdHue || (reset && channel == 0))
			curChannel = 0;
		else if (element == &rdSat || (reset && channel == 1))
			curChannel = 1;
		else if (element == &rdBri || (reset && channel == 2))
			curChannel = 2;
		else if (element == &rdRed || (reset && channel == 3))
			curChannel = 3;
		else if (element == &rdGrn || (reset && channel == 4))
			curChannel = 4;
		else if (element == &rdBlu || (reset && channel == 5))
			curChannel = 5;

		update_view(curChannel, reset);
	}
	else if (message == Event::Update) {
		if (element == &mainPalette) {
			currentColor = mainPalette.get_picked_color();
			update_side_palette();
		}
		else if (element == &sidePalette) {
			sideValue = sidePalette.get_value();
			update_main_palette();
			currentColor = mainPalette.get_picked_color();
		}
	}
	else if (message == Event::Click) {
		if (element == &btnOK) {

		}
		else if (element == &btnCancel) {

		}
	}

	update_fields(srcChannel);
	invalidate();

}

void UIColorPicker::update_fields(int srcChannel) {
	if (srcChannel != 8)
		preview.set_new(currentColor);

	if ((srcChannel >= 3 && srcChannel < 6) || srcChannel < 0) {
		rgb_to_hsb(currentColor.r, currentColor.g, currentColor.b, hue, sat, bri);

		auto shue = std::to_wstring(hue);
		numHue.set_text(shue);

		auto ssat = std::to_wstring(sat);
		numSat.set_text(ssat);

		auto sbri = std::to_wstring(bri);
		numBri.set_text(sbri);
	}

	if ((srcChannel < 3) || srcChannel < 0) {
		if (srcChannel >= 0)
			hsb_to_rgb(hue, sat, bri, currentColor.r, currentColor.g, currentColor.b);

		auto sred = std::to_wstring(currentColor.r);
		numRed.set_text(sred);

		auto sgrn = std::to_wstring(currentColor.g);
		numGrn.set_text(sgrn);

		auto sblu = std::to_wstring(currentColor.b);
		numBlu.set_text(sblu);

		auto hex = to_whex(currentColor);
		txtHex.set_text(hex);
	}
}

void UIColorPicker::update_view(int triggerChannel, bool reset) {
	if (bWebColor)
		webalize_color(currentColor);

	int currChannel = channel;
	int val, max;
	rgb_to_hsb(currentColor.r, currentColor.g, currentColor.b, hue, sat, bri);

	if (triggerChannel == 0 || (reset && channel == 0)) {
		channel = 0;
		val = hue >= 360 || hue == 0 ? 360 : hue;
		max = 359;
	}
	else if (triggerChannel == 1 || (reset && channel == 1)) {
		channel = 1;
		val = sat;
		max = 100;
	}
	else if (triggerChannel == 2 || (reset && channel == 2)) {
		channel = 2;
		val = bri;
		max = 100;
	}
	else if (triggerChannel == 3 || (reset && channel == 3)) {
		channel = 3;
		val = currentColor.r;
		max = 255;
	}
	else if (triggerChannel == 4 || (reset && channel == 4)) {
		channel = 4;
		val = currentColor.g;
		max = 255;
	}
	else if (triggerChannel == 5 || (reset && channel == 5)) {
		channel = 5;
		val = currentColor.b;
		max = 255;
	}
	else return;

	if (currChannel != channel || reset) {
		sideValue = CLAMP3(0, val, max);
		sideValue = int(sideValue / double(max) * 255.0 + 0.5);
		sidePalette.set_value(sideValue);
		update_main_palette();

		Point sel;
		if (channel == 0) {
			sel.x = (int)((sat * 2.5500) + 0.5);
			sel.y = 255 - (int)((bri * 2.55) + 0.5);
		}
		else if (channel == 1) {
			sel.x = (int)(((hue / 360.5) * 255.00) + 0.5);
			sel.y = 255 - (int)((bri * 2.55) + 0.5);
		}
		else if (channel == 2) {
			sel.x = (int)(((hue / 360.5) * 255.00) + 0.5);
			sel.y = 255 - (int)((sat * 2.55) + 0.5);
		}
		else if (channel == 3) {
			sel.x = currentColor.b;
			sel.y = 255 - currentColor.g;
		}
		else if (channel == 4) {
			sel.x = currentColor.b;
			sel.y = 255 - currentColor.r;
		}
		else if (channel == 5)
		{
			sel.x = currentColor.r;
			sel.y = 255 - currentColor.g;
		}

		mainPalette.set_selector(sel, false, 0);
		update_side_palette();
	}
}

void UIColorPicker::set_color(Color color) {
	currentColor.set(color);
	rgb_to_hsb(currentColor.r, currentColor.g, currentColor.b, hue, sat, bri);
	update_fields(-1);
	preview.set_current(color);
	update_view(-1, true);
}

Color UIColorPicker::get_color() {
	return currentColor;
}