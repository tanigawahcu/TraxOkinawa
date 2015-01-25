#include <iostream>
#include <iomanip>

#include "trax.h"

#define BOARD_MAX 100

const std::string esc_ul = "\033[4m";
const std::string esc_rev = "\033[7m";
const std::string esc_red = "\033[31m";
const std::string esc_normal = "\033[0m";

char board[BOARD_MAX][BOARD_MAX];
char board_color[BOARD_MAX][BOARD_MAX];
char board_marks[BOARD_MAX][BOARD_MAX];

int left, right, top, bottom;

void clear_marks(){
  for(int x=0; x<BOARD_MAX; x++)
    for(int y=0; y<BOARD_MAX; y++)
      board_marks[x][y] = 0; 
}

void clear_board(){
  for(int x=0; x<BOARD_MAX; x++){
    for(int y=0; y<BOARD_MAX; y++){
      board[x][y] = ' ';
      board_color[x][y] = 0;  // right hand side is 1: RED, 2: WHITE
      board_marks[x][y] = 0; 
    }
  }

  left = right = top = bottom = BOARD_MAX/2;
}


void show_board(){
  std::cout << "   |";
  for(int x=left; x<=right; x++)
    std::cout << (char)(x-left+'@') << ' ';
    std::cout << "|\n";
  
  for(int y=top; y<=bottom; y++){
    std::cout << std::setw(3) << y-top;
    for(int x=left; x<=right; x++){
      std::cout << (x==left ? "|" : "")
		<< (board_marks[x][y] == 1 ? esc_rev : "")
		<< (board_marks[x][y] == 2 ? esc_ul  : "")
		<< (board_color[x][y] == 1 ? esc_red : "")
		<< board[x][y] << esc_normal << ' '
		<< (x==right ? "|" : "");
    }
    std::cout << '\n';
  }
}


int opposite_color(const int c){
  if (c==0) return 0;
  if (c==1) return 2;
  return 1;
}

void get_around_colors(const int x, const int y,
		       int& lc, int& rc, int& uc, int& dc){
  // left color
  lc = board_color[x-1][y]; 

  // right color
  rc = board[x+1][y]=='+' ? board_color[x+1][y] :
    opposite_color(board_color[x+1][y]);

  // up color
  uc = board[x][y-1]=='+' ? opposite_color(board_color[x][y-1]) :
    board[x][y-1]=='/' ? board_color[x][y-1] :
    opposite_color(board_color[x][y-1]);

  // down color
   dc = board[x][y+1]=='+' ? opposite_color(board_color[x][y+1]) :
    board[x][y+1]=='\\' ? board_color[x][y+1] :
    opposite_color(board_color[x][y+1]);
}




bool trace_loop(const int x, const int y, const int d, const int mark=0){
  // dir: up(1) down(2) left(3) right(4)
  int xx=x; int yy=y; int dir=d;

  do {
    int next_dir;

    switch(dir){
    case 1: // up
      yy--;
      next_dir = board[xx][yy]=='/' ? 4 : board[xx][yy]=='\\' ? 3 : 1;
      break;
    case 2: // down
      yy++;
      next_dir = board[xx][yy]=='/' ? 3 : board[xx][yy]=='\\' ? 4 : 2;
      break;
    case 3: // left
      xx--;
      next_dir = board[xx][yy]=='/' ? 2 : board[xx][yy]=='\\' ? 1 : 3;
      break;
    case 4: // right
      xx++;
      next_dir = board[xx][yy]=='/' ? 1 : board[xx][yy]=='\\' ? 2 : 4;
      break;
    }
    dir=next_dir;

    //    if (!(xx==x && yy==y) && mark!=0) board_marks[xx][yy]=mark;

    if (mark!=0) board_marks[xx][yy]=mark;

    if (xx==x && yy==y){ // Loop found !
      return true;
    }
  } while(board[xx][yy] != ' ');
  return false;
}

bool trace_loop(const int x, const int y){
  bool found = false;
  for(int d=1; d<=4; d++)
    if (trace_loop(x, y, d)){
      trace_loop(x, y, d, 2);
      found = true;
    }
  return found;
}



bool trace_line(const int x, const int y, const int d, const int mark=0){
  // dir: up(1) down(2) left(3) right(4)
  int xx=x; int yy=y; int dir=d;
  
  if (mark!=0) board_marks[xx][yy]=mark;
  do {
    int next_dir;

    switch(dir){
    case 1: // up
      yy--;
      next_dir = board[xx][yy]=='/' ? 4 : board[xx][yy]=='\\' ? 3 : 1;
      break;
    case 2: // down
      yy++;
      next_dir = board[xx][yy]=='/' ? 3 : board[xx][yy]=='\\' ? 4 : 2;
      break;
    case 3: // left
      xx--;
      next_dir = board[xx][yy]=='/' ? 2 : board[xx][yy]=='\\' ? 1 : 3;
      break;
    case 4: // right
      xx++;
      next_dir = board[xx][yy]=='/' ? 1 : board[xx][yy]=='\\' ? 2 : 4;
      break;
    }
    dir=next_dir;

    if (mark!=0) board_marks[xx][yy]=mark;

    if ((d==4 && xx==right +1 && (right-left)>=8) ||
	(d==2 && yy==bottom+1 && (bottom-top)>=8)){ // Line found !
      return true;
    }
  } while(board[xx][yy] != ' ');

   return false;
}


bool trace_line(){
  bool found = false;
  
  for(int y=top; y<=bottom; y++){
    if(board[left+1][y]!=' '){ // hotizontal candidate!
      if(trace_line(left+1, y, 4)){
	trace_line(left+1, y, 4, 2);
	found = true;
      }
    }
  }
    
  for(int x=left; x<=right; x++){
    if(board[x][top+1]!=' '){ // vertical candidate!
      if(trace_line(x, top+1, 2)){
	trace_line(x, top+1, 2, 2);
	found = true;
      }
    }
  }
    
  return found;
}


bool place(const int x, const int y, const char tile){
  board[x][y] = tile;
  int color = 0;

  // check left, right, up, down
  int lc, rc, uc, dc;
  get_around_colors(x, y, lc, rc, uc, dc);

  // Isolated
  if ( lc==rc && rc==uc && uc==dc && dc == 0 ){
    std::cout << "**** ISOLATED ****\n";
    color = 1; // For 1st move only. FIXME!
  }

  // Tiles around
  std::cout << (lc!=0 ? ((lc==1 ? esc_red : "") + "L" + esc_normal) : "")
	    << (rc!=0 ? ((rc==1 ? esc_red : "") + "R" + esc_normal) : "")
	    << (uc!=0 ? ((uc==1 ? esc_red : "") + "U" + esc_normal) : "")
	    << (dc!=0 ? ((dc==1 ? esc_red : "") + "D" + esc_normal) : "")
	    << "\n";
  
  // 3 same color check
  is_prohibited_3(x, y); // if true, it's prohibited pattern
  
  // tile requirement check
  is_consistent_placement(x, y, tile);

  // color the tile
  if (color==0){ // if not already colored
    if (tile=='+')  color = (lc==1 || rc==1 || uc==2 || dc==2) ? 1 : 2;
    if (tile=='/')  color = (lc==1 || uc==1 || dc==2 || rc==2) ? 2 : 1;
    if (tile=='\\') color = (lc==1 || dc==1 || uc==2 || rc==2) ? 2 : 1;
  }

  bool got_a_loop = false;
    
  if (color==0) std::cout << "**** SOMETHING IS WRONG ****\n";
  board_color[x][y]=color;
  if(trace_loop(x, y))    got_a_loop = true;
  if(scan_forced()) got_a_loop = true;

  return got_a_loop;
}

bool scan_forced(){
  for(int y=top; y<=bottom; y++){
    for(int x=left; x<=right; x++){
      if(board_color[x][y] != 0) continue;

      int lc, rc, uc, dc;
      get_around_colors(x, y, lc, rc, uc, dc);

      if ( (lc==uc && uc!=0 ) || (dc==rc && rc!=0 ) ){
	std::cout << "Forced play: "; return place(x, y, '/');  }
      if ( (rc==uc && uc!=0 ) || (dc==lc && lc!=0 ) ){
	std::cout << "Forced play: "; return place(x, y, '\\'); }
      if ( (lc==rc && rc!=0 && rc!=dc && rc!=uc) ||
	   (uc==dc && dc!=0 && dc!=lc && dc!=lc) ){
	std::cout << "Forced play: "; return place(x, y, '+');  }
    }
  }
  // No more forced play here
  return false;
}

// ----------------------------------------------------------------------

int main(){
  clear_board();

  // first move is either "@0/" or "@0+"
  std::string m;
  bool preamble = true;

  int turn = 0;
  
  show_board();
  while ( std::cin >> m ){
    if (preamble && m[0]=='@') preamble=false;

    if (!preamble){
      std::cout << "Turn " << ++turn << ": ";
      
      move mo = move(m);
      std::cout << m << "[" << mo << "]\n";

      // place a move
      int x = left +mo.x;
      int y = top  +mo.y;
      if (mo.x == 0){ left--; }
      if (mo.y == 0){ top--;   }

      if (right < x) right = x;
      bottom = (bottom < y) ? y : bottom;

      if (board[x][y] != ' ') std::cout << "**** ALREADY OCCUPIED! ****\n";

      if(place(x, y, mo.tile)) std::cout << "---- GOT A LOOP ! ----\n";
      if(trace_line()) std::cout << "---- GOT A LINE ! ----\n";
    
      board_marks[x][y] = 1;
      show_board();

      clear_marks();
	    
    }
  }
}
