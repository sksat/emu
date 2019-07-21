#ifndef SKSAT_WINDOW_HPP
#define SKSAT_WINDOW_HPP

#include <sksat/common.hpp>
#include <sksat/platform.hpp>
#include <sksat/color.hpp>

namespace sksat {

class window_base {
public:
	window_base() : opend(false), xsize(default_xsize), ysize(default_ysize), xpos(default_xpos), ypos(default_ypos) {}
	window_base(size_t x, size_t y) : opend(false), xsize(x), ysize(y), xpos(default_xpos), ypos(default_ypos) {}
	window_base(const sksat::string &t, size_t x, size_t y) : opend(false), title(t), xsize(x), ysize(y), xpos(default_xpos), ypos(default_ypos) {}

	void open(){ opend = api_open(); if(opend) clear(); }
	void open(size_t x, size_t y){ open(); set_size(x,y); }
	void open(const sksat::string &t){ set_title(t); open(); }
	void open(const sksat::string &t, size_t x, size_t y){ set_title(t); open(x,y); }

	void close(){ if(opend) api_close(); opend=false; }

	void show(){ if(opend) api_show(); }

	void set_title(const sksat::string &t){ title = t; set_title(t.c_str()); }
	void set_title(const char *t){ title = t; api_set_title(t);  }
	sksat::string get_title() const { return title; }

	virtual void set_size(size_t x, size_t y){
		xsize=x;
		ysize=y;
		if(opend)
			api_set_size(x,y);
	}
	void set_xsize(size_t x){ set_size(x, ysize); }
	void set_ysize(size_t y){ set_size(xsize, y); }
	size_t get_xsize() const { return xsize; }
	size_t get_ysize() const { return ysize; }

	void move(size_t x, size_t y){ xpos=x; ypos=y; api_move(x,y); }
	void set_pos(size_t x, size_t y){ move(x,y); }
	void set_xpos(size_t x){ move(x,ypos); }
	void set_ypos(size_t y){ move(xpos,y); }

	operator bool () const { return opend; }

	// 描画関数
	inline void draw_point(const sksat::color &col, size_t x, size_t y){ api_draw_point(col, x, y); }
	virtual void draw_line(const sksat::color &col, size_t x0, size_t y0, size_t x1, size_t y1){
		int x, y, len, dx, dy;
		dx = x1-x0; dy = y1-y0;
		x = x0<<10; y = y0<<10;
		if(dx < 0) dx = -dx;
		if(dy < 0) dy = -dy;
		if(dx >= dy){
			len = dx+1;
			dx = 1024;
			if(x0 > x1) dx = -1 * dx;
			int tmp=-1;
			if(y0 <= y1) tmp=1;
			dy = ((y1-y0+tmp) << 10) / len;
		}else{
			len = dy+1;
			dy = 1024;
			if(y0 > y1) dy = -1 * dy;
			int tmp=-1;
			if(x0 <= x1) tmp=1;
			dx = ((x1-x0+tmp) << 10) / len;
		}
		for(int i=0;i<len;i++){
			draw_point(col, x>>10, y>>10);
			x+=dx; y+=dy;
		}
	}
	virtual void draw_rect(const sksat::color &col, size_t x0, size_t y0, size_t x1, size_t y1, bool fill){
		if(fill){
			fill_rect(col, x0, y0, x1, y1);
		}else{
			draw_line(col, x0, y0, x1, y0);
			draw_line(col, x1, y0, x1, y1);
			draw_line(col, x1, y1, x0, y1);
			draw_line(col, x0, y1, x0, y0);
		}
	}
	virtual void fill_rect(const sksat::color &col, size_t x0, size_t y0, size_t x1, size_t y1){
		if(x0 > x1){
			size_t tmp = x1; x1 = x0; x0 = tmp; // swapとかでやるとよさそう
		}
		if(y0 > y1){
			size_t tmp = y1; y1 = y0; y0 = tmp;
		}
		for(size_t x=x0;x<=x1;x++){
			for(size_t y=y0;y<=y1;y++)
				draw_point(col, x, y);
		}
	}

	// colの色で描画
	void draw_point(size_t x, size_t y){ draw_point(col, x, y); }
	void draw_line(size_t x0, size_t y0, size_t x1, size_t y1){ draw_line(col, x0, y0, x1, y1);}
	void draw_rect(size_t x0, size_t y0, size_t x1, size_t y1, bool fill){ draw_rect(col, x0, y0, x1, y1, fill); }

	void clear(const sksat::color &col){ fill_rect(col, 0, 0, xsize, ysize); }
	void clear(){ clear(col_back); }

	inline void flush(){ if(opend) api_flush(); }

	inline bool step_loop(){ if(opend) return api_step_loop(); }
	inline void loop(){ while(api_step_loop()); }

protected: // 環境依存部(純粋仮想関数)
	virtual bool api_open() = 0;
	virtual void api_close() = 0;
	virtual void api_show() = 0;
	virtual void api_set_title(const char *t) = 0;
	virtual void api_set_size(size_t x, size_t y) = 0;
	virtual void api_flush() = 0;
	virtual void api_move(size_t x, size_t y) = 0;

	// 描画
	virtual void api_draw_point(const sksat::color &col, size_t x, size_t y) = 0;

	virtual bool api_step_loop() = 0;
public:
	sksat::color col;
	sksat::color col_back;

	static size_t default_xsize, default_ysize;
	static size_t default_xpos, default_ypos;
protected:
	bool opend;
	size_t xsize, ysize;
	size_t xpos, ypos;
	sksat::string title;
};

size_t window_base::default_xsize = 100;
size_t window_base::default_ysize = 100;
size_t window_base::default_xpos  = 0;
size_t window_base::default_ypos  = 0;

}


#if defined(OS_WIN32)
	#include <sksat/win32/window.hpp>
	namespace sksat{ using sksat::win32::window; }
#elif defined(OS_LINUX)
	#include <sksat/linux/window.hpp>
	namespace sksat{ using sksat::linux::window; }
#else
	#error not implemented.
#endif

#endif
