#include <iostream>
#include "trax.h"

bool is_prohibited_3(const int x, const int y){
  int lc, rc, uc, dc;

  get_around_colors(x, y, lc, rc, uc, dc);
  if ( ( lc==rc && rc==uc && uc!=0 ) ||
       ( rc==uc && uc==dc && dc!=0 ) ||
       ( uc==dc && dc==lc && lc!=0 ) ||
       ( dc==lc && lc==rc && rc!=0 ) ) {

    std::cout << "**** 3 LINES WITH SAME COLOR! ****\n";
    return true;
  }
  return false;
}

bool is_consistent_placement(int x, int y, char tile){
  int lc, rc, uc, dc;

  get_around_colors(x, y, lc, rc, uc, dc);
  if (tile=='+'){
    if (lc!=rc && (lc!=0 && rc!=0)){
      std::cout << "**** DIFFERENT COLOR ON LEFT & RIGHT ON '+' ****\n";
      return false;
    }

    if (uc!=dc && (uc!=0 && dc!=0)){
      std::cout << "**** DIFFERENT COLOR ON UPPER & LOWER ON '+' ****\n";
      return false;
    }
  }

  if (tile=='/'){
    if (lc!=uc && (lc!=0 && uc!=0)){
      std::cout << "**** DIFFERENT COLOR ON LEFT & UPPER ON '/' ****\n";
      return false;
    }

    if (dc!=rc && (dc!=0 && rc!=0)){
      std::cout << "**** DIFFERENT COLOR ON LOWER & RIGHT ON '/' ****\n";
      return false;
    }
  }

  if (tile=='\\'){
    if (lc!=dc && (lc!=0 && dc!=0)){
      std::cout << "**** DIFFERENT COLOR ON LEFT & LOWER ON '\\' ****\n";
      return false;
    }

    if (uc!=rc && (uc!=0 && rc!=0)){
      std::cout << "**** DIFFERENT COLOR ON UPPER & RIGHT ON '\\' ****\n";
      return false;
    }
  }
  return true;
}
