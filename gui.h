#ifndef GUI_H_
#define GUI_H_

#include <thread>
#include <functional>
#include "device/display.h"

class Gui {
public:
	Gui();
	~Gui();

	void Init();
	void SetDisplay(Device::Display &d){ this->disp = &d; }
	void Start();
	void Start(Device::Display &d){
		SetDisplay(d);
		Start();
	}
	void End();

	std::function<void(void)> onExit;
private:
	void proc();
	bool flg;
	std::thread th;
	Device::Display *disp;
	std::vector<uint8_t> font;
};

#endif
