#ifndef SKSAT_SHELL_HPP_
#define SKSAT_SHELL_HPP_

#include <sksat/common.hpp>
#include <sksat/cmdline.hpp>

namespace sksat {

class shell {
public:
	shell() : com_enable(false), in(stdin), out(stdout), default_func(nullptr) {}
	shell(std::FILE *in, std::FILE *out) : com_enable(false), default_func(nullptr) { set_file(in, out); }

	void set_infile(std::FILE *in){
		if(in != nullptr) this->in = in;
		else throw;
	}
	void set_outfile(std::FILE *out){
		if(out != nullptr) this->out = out;
		else throw;
	}
	void set_file(std::FILE *in, std::FILE *out){
		set_infile(in);
		set_outfile(out);
	}

	void set_prompt(sksat::string prompt){ this->prompt=prompt; }
	void set_comment(sksat::string com){ oneline_com = com; }
	void set_comment(sksat::string s1, sksat::string s2){
		com_start = s1;
		com_end   = s2;
		com_enable = true;
	}

	using str_func = int (*)(sksat::string&);
	str_func default_func;

	int command(sksat::string &str){
		if(default_func != nullptr) default_func(str);
		return 0;
	}

	void loop(){
		bool com_flg = true;
		buf.reserve(1000);
		for(;;){
			out_str(prompt);
			if(!in_str()) break;
			if(com_flg && !oneline_com.empty()) remove_oneline_com(buf);
			str += buf;

			if(com_enable){
				com_flg = remove_comment(str);
				if(com_flg){
					command(str);
					str.clear();
				}
			}else{
				command(str);
			}
		}
	}
protected:
	bool in_str(){
		char c;
		buf.clear();
		for(;;){
			c = fgetc(in);
			if(c == '\n') break;
			if(c == EOF) return false;
			buf.push_back(c);
		}
		return true;
	}

	void out_str(sksat::string &s){
		fprintf(out, "%s", s.c_str());
	}
	void out_str(const char *s){
		fprintf(out, "%s", s);
	}

	void remove_oneline_com(sksat::string &s){
		auto p = s.find(oneline_com);
		if(p != sksat::string::npos){
//			out_str("oneline comment found!\n");
			s.erase(s.begin()+p, s.begin()+s.length());
		}
	}

	bool remove_comment(sksat::string &s){
		auto p1 = s.find(com_start);
		if(p1 == sksat::string::npos) return true; // そもそもコメントが無かった
		auto p2 = s.find(com_end);
		if(p2 == sksat::string::npos) return false; // コメントが途中
//		out_str("comment found!");
		s.erase(s.begin()+p1, s.begin()+p2+com_end.length());
	}

	bool com_enable;
	std::FILE *in, *out;
	sksat::string prompt;
	sksat::string oneline_com;
	sksat::string com_start, com_end;
	sksat::string buf, str;
};

}

#endif
