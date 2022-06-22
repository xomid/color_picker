#pragma once
#include "util.h"
#include "UIMainPalette.h"
#include "UISidePalette.h"
#include "UIColorPreview.h"
#include <oui_button.h>
#include <oui_group.h>
#include <oui_check.h>
#include <oui_number.h>

class UIColorPicker : public OUI
{
	UIButton btnOK, btnCancel, btnAdd;
	UIMainPalette mainPalette;
	UISidePalette sidePalette;
	UIColorPreview preview;
	UIGroup grpChannel;
	UIRadio rdHue, rdSat, rdBri, rdRed, rdGrn, rdBlu;
	UINumber numHue, numSat, numBri, numRed, numGrn, numBlu;
	UILabel lblNew, lblCurrent, lblHue, lblSat, lblBri, lblHash;
	UICheck chkWebColor;
	UIText txtHex;
	int channel, hue, sat, bri, red, grn, blu, sideValue;
	bool bWebColor;
	Color currentColor;

public:
	void on_init() override;
	void on_resize(int width, int height) override;
	void process_event(OUI* element, uint32_t message, uint64_t param, bool bubbleUp) override;
	void update_side_palette();
	void update_main_palette();
	void update_values(int im);
	void update_fields(int srcChannel);
	void webalize_color(Color& color);
	void update_view(int triggerChannel, bool reset);
	void select_channel(int channel);

	// interface
	void set_color(Color color);
	Color get_color();
};

