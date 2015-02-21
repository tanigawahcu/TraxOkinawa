#include <ostream>

#ifndef _TRAX_H_
#define _TRAX_H_

class move {
public:
  int x, y;
  char tile;
  move(const std::string);
};

class trax {
public:
  static const int BOARD_MAX = 100;

  void clear_marks();
  void clear_board();

  bool is_board_consistent();
  bool place(move);
  bool place(const int, const int, const char);

  bool trace_loop();
  bool trace_line();

  bool loop(){ return got_loop; }
  bool line(){ return got_line; }

protected:
  bool scan_forced();

  void get_around_colors(const int, const int, int&, int&, int&, int&);
  bool trace_loop(const int, const int, const int, const int);
  bool trace_loop(const int, const int);
  bool trace_line(const int, const int, const int, const int);

  bool is_prohibited_3(const int x, const int y);
  bool is_consistent_placement(int x, int y, char tile);
  bool is_line_color_connected(int x, int y);

  int opposite_color(const int);
  
  char board[BOARD_MAX][BOARD_MAX];
  char board_color[BOARD_MAX][BOARD_MAX];
  char board_marks[BOARD_MAX][BOARD_MAX];

  int left, right, top, bottom;
  bool got_loop, got_line;

  friend std::ostream& operator<<(std::ostream&, const trax&);
};


std::ostream& operator<<(std::ostream&, const move&);
std::ostream& operator<<(std::ostream&, const trax&);

#endif

