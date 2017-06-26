int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J) {
 goto loc_start0;

 loc_lbl13:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 + D == A && I == A && F == A && A >= 2 + G && 1 + G >= 0 && G + A >= 2) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    if (I == 1 + D && F == A && D >= 1 + G && A >= 1 + D && 1 + G >= 0 && G + D >= 1 && A >= 2 + D) {
     B = B_;
     goto loc_lbl31;
    }
   }
  goto end;
 }
 loc_lbl31:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = -2 + I;
    if (F == A && A >= 1 + I && I >= 1) {
     G = G_;
     goto loc_lbl43;
    }
   }
   if (nondet_bool()) {
    int I_ = 1 + I;
    int G_ = -1 + I;
    int D_ = I;
    if (F == A && A >= 1 + I && I >= 1) {
     D = D_;
     G = G_;
     I = I_;
     goto loc_lbl13;
    }
   }
  goto end;
 }
 loc_lbl43:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = -1 + G;
    if (F == A && A >= 1 + I && 1 + G >= 0 && I >= 2 + G && G >= 0) {
     G = G_;
     goto loc_lbl43;
    }
   }
   if (nondet_bool()) {
    int I_ = 1 + I;
    int D_ = I;
    if (F == A && A >= 1 + I && 1 + G >= 0 && I >= 2 + G) {
     D = D_;
     I = I_;
     goto loc_lbl13;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = 1;
    if (I == J && G == H && F == A && D == E && B == C && 1 >= A) {
     I = I_;
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int B_ = nondet();
    int I_ = 1;
    if (I == J && G == H && F == A && D == E && B == C && A >= 2) {
     B = B_;
     I = I_;
     goto loc_lbl31;
    }
   }
  goto end;
 }
 loc_start0:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = J;
    int G_ = H;
    int F_ = A;
    int D_ = E;
    int B_ = C;
    if (1 >= 0) {
     B = B_;
     D = D_;
     F = F_;
     G = G_;
     I = I_;
     goto loc_start;
    }
   }
  goto end;
 }
 loc_stop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
