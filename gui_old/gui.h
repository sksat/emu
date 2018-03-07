#ifndef GUI_H_
#define GUI_H_

#include <vector>
#include <thread>
#include "../device/display.h"

#define DEFAULT_SCRNX 320
#define DEFAULT_SCRNY 200

class Gui {
public:
	Gui();
	~Gui();

	void Init();
	int Start();

	void SetDisplay(Display *disp){ this->disp = disp; }

	void gui_proc();
private:
	static std::vector<Gui*> instances;
	std::thread *gui_th;
	bool th_flg;
	int scrnx, scrny;
	Display *disp;
};

#endif
