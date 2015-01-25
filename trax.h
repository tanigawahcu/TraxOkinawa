#ifndef _TRAX_H_
#define _TRAX_H_

class move {
public:
  int x, y;
  char tile;
  move(const std::string);
};

std::ostream& operator<<(std::ostream&, const move&);


int opposite_color(const int);
void get_around_colors(const int x, const int y,
		       int& lc, int& rc, int& uc, int& dc);

bool is_prohibited_3(const int x, const int y);
bool is_consistent_placement(int x, int y, char tile);

bool place(const int, const int, const char);
bool scan_forced();


#endif

