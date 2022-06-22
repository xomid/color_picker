#include "UIColorPreview.h"

void UIColorPreview::on_init() {
	set_new(Colors::red);
	set_current(Colors::white);
}

void UIColorPreview::set_new(Color crNew) {
	this->crNew.set(crNew);
	invalidate();
}

void UIColorPreview::set_current(Color crCurrent) {
	this->crCurrent.set(crCurrent);
	invalidate();
}

Color UIColorPreview::get_current_color() {
	return this->crCurrent;
}

void UIColorPreview::on_resize(int width, int height) {
	rcNew.set(0, 0, contentArea.width, contentArea.height / 2);
	rcCurrent.set(0, rcNew.height, rcNew.width, contentArea.height - rcNew.height);
}

void UIColorPreview::on_update() {
	canvas.clear(&rcNew, &crNew);
	canvas.clear(&rcCurrent, &crCurrent);
}

void UIColorPreview::on_mouse_down(int x, int y, uint32_t flags) {
	if (rcCurrent.is_inside(x, y)) {
		set_new(crCurrent);
		if (parent) parent->process_event(this, Event::Update, 0, true);
	}
}

void UIColorPreview::on_dbl_click(int x, int y, uint32_t flags) {
	on_mouse_down(x, y, flags);
}
