// WARNING: The files in this directory have not been extensively tested
//   and may be incorrect. --JTB


int ik_105_x_lb;
int ik_105_x_ub;
int ik_105_y_lb;
int ik_105_y_ub;
int output_cost;

int getX(int v1_x, int v1_y) {
  __VERIFIER_assume(ik_105_x_lb <= v1_x && v1_x <= ik_105_x_ub);
  __VERIFIER_assume(ik_105_y_lb <= v1_y && v1_y <= ik_105_y_ub);
  return v1_x;
}

int getY(int v1_x, int v1_y) {
  __VERIFIER_assume(ik_105_x_lb <= v1_x && v1_x <= ik_105_x_ub);
  __VERIFIER_assume(ik_105_y_lb <= v1_y && v1_y <= ik_105_y_ub);
  return v1_y;
}

void ItemInit(int v2, int v3) {
  if (v2 < ik_105_x_lb) {
    ik_105_x_lb = v2;
  }
  if (v2 > ik_105_x_ub) {
    ik_105_x_ub = v2;
  }
  if (v3 < ik_105_y_lb) {
    ik_105_y_lb = v3;
  }
  if (v3 > ik_105_y_ub) {
    ik_105_y_ub = v3;
  }
}

void outputItem(int v1_x, int v1_y) {
  __VERIFIER_assume(ik_105_x_lb <= v1_x && v1_x <= ik_105_x_ub);
  __VERIFIER_assume(ik_105_y_lb <= v1_y && v1_y <= ik_105_y_ub);
  int x = getX(v1_x, v1_y);
  for (int i = 0; i < x; i++) {
    output_cost++;
  }  
  // adding these two assumptions slows down ICRA dramatically
  //__VERIFIER_assume(ik_105_x_lb <= v1_x && v1_x <= ik_105_x_ub);
  //__VERIFIER_assume(ik_105_y_lb <= v1_y && v1_y <= ik_105_y_ub);
  int y = getY(v1_x, v1_y);
  for (int i = 0; i < y; i++) {
    output_cost++;
  }
  output_cost++;
}

int init_globals() {
  ik_105_x_lb = 0;
  ik_105_x_ub = 0;
  ik_105_y_lb = 0;
  ik_105_y_ub = 0;
  output_cost = 0;
}

int main() {
  init_globals();
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
    ItemInit(x,y);
    int v33_x = __VERIFIER_nondet_int();
    int v33_y = __VERIFIER_nondet_int();
    __VERIFIER_assume(ik_105_x_lb <= v33_x && v33_x <= ik_105_x_ub);
    __VERIFIER_assume(ik_105_y_lb <= v33_y && v33_y <= ik_105_y_ub);
    outputItem(v33_x, v33_y);
  }
  __VERIFIER_print_hull(output_cost);
  return output_cost;
}
