#ifndef GUI_H_
#define GUI_H_

#include <vector>
#include <thread>

#define DEFAULT_SCRNX 320
#define DEFAULT_SCRNY 200

class Gui {
public:
	Gui();
	~Gui();

	void Init();
	int Start();

	void gui_proc();
private:
	static std::vector<Gui*> instances;
	std::thread *gui_th;
	int scrnx, scrny;
};

#endif
