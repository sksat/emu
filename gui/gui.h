#ifndef GUI_H_
#define GUI_H_

#include <thread>

class Gui {
public:
	Gui();
	~Gui();

	void Init();
private:
	std::thread *gui_th;
	int scrnx, scrny;
	void gui_proc();
};

#endif
