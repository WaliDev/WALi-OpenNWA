// WARNING: The files in this directory have not been extensively tested
//   and may be incorrect. --JTB

int ik_105_x_lb;
int ik_105_x_ub;
int ik_105_y_lb;
int ik_105_y_ub;
int output_cost;

void init_globals() {
  ik_105_x_lb = 0;
  ik_105_x_ub = 0;
  ik_105_y_lb = 0;
  ik_105_y_ub = 0;
  output_cost = 0;
}

int main() {
  //init_globals();
  // if we don't inline init_globals(), we don't get an upper bound
  ik_105_x_lb = 0;
  ik_105_x_ub = 0;
  ik_105_y_lb = 0;
  ik_105_y_ub = 0;
  output_cost = 0;
  //
  int i;
  for (i = 0; i < 10; i++) {
    int length = __VERIFIER_nondet_int();
    __VERIFIER_assume(0 <= length && length <= 10);
    int x = 0;
    int y = 0;
    int j;
    for (j = 0; j < length; j++) {
      if (__VERIFIER_nondet_int() == 0) {
        x++;
      }
      if (__VERIFIER_nondet_int() == 1) {
        y++;
      }  
    }
    //inlining ItemInit(x,y);
    if (x < ik_105_x_lb) {
      ik_105_x_lb = x;
    }
    if (x > ik_105_x_ub) {
      ik_105_x_ub = x;
    }
    if (y < ik_105_y_lb) {
      ik_105_y_lb = y;
    }
    if (y > ik_105_y_ub) {
      ik_105_y_ub = y;
    }
    int v33_x = __VERIFIER_nondet_int();
    int v33_y = __VERIFIER_nondet_int();
    __VERIFIER_assume(ik_105_x_lb <= v33_x && v33_x <= ik_105_x_ub);
    __VERIFIER_assume(ik_105_y_lb <= v33_y && v33_y <= ik_105_y_ub);
    //inlining outputItem(v33_x, v33_y);
    __VERIFIER_assume(ik_105_x_lb <= v33_x && v33_x <= ik_105_x_ub);
    __VERIFIER_assume(ik_105_y_lb <= v33_y && v33_y <= ik_105_y_ub);
    //inlining int x = getX(v33_x, v33_y);
    __VERIFIER_assume(ik_105_x_lb <= v33_x && v33_x <= ik_105_x_ub);
    __VERIFIER_assume(ik_105_y_lb <= v33_y && v33_y <= ik_105_y_ub);
    int x = v33_x;
    int k;
    for (k = 0; k < x; k++) {
      output_cost++;
    }  
    __VERIFIER_assume(ik_105_x_lb <= v33_x && v33_x <= ik_105_x_ub);
    __VERIFIER_assume(ik_105_y_lb <= v33_y && v33_y <= ik_105_y_ub);
    //inlining int y = getY(v33_x, v33_y);
    __VERIFIER_assume(ik_105_x_lb <= v33_x && v33_x <= ik_105_x_ub);
    __VERIFIER_assume(ik_105_y_lb <= v33_y && v33_y <= ik_105_y_ub);
    int y = v33_y;
    for (k = 0; k < y; k++) {
      output_cost++;
    }
    output_cost++;
  }
  __VERIFIER_print_hull(output_cost);
  return output_cost;
}
