int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J) {
 goto loc_start0;

 loc_lbl71:
 {
 __cost++;
   if (nondet_bool()) {
    if (G == C && H == I && J == A && E == 1 && D == 1 + C && A >= 1 && C >= B) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int D_ = E + D;
    if (G == C && H == I && J == A && E == 1 && 1 + C >= D && D >= 1 + B && C >= D && A >= 1) {
     D = D_;
     goto loc_lbl71;
    }
   }
   if (nondet_bool()) {
    int D_ = D + -E;
    if (G == C && H == I && J == A && E == 1 && A >= 1 && 1 + C >= D && D >= 1 + B && 0 >= A && C >= D) {
     D = D_;
     goto loc_lbl81;
    }
   }
  goto end;
 }
 loc_lbl81:
 {
 __cost++;
   if (nondet_bool()) {
    if (G == C && H == I && J == A && 1 + E == 0 && D == 1 + C && C >= B && 0 >= A) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int D_ = E + D;
    if (G == C && H == I && J == A && 1 + E == 0 && 1 + C >= D && D >= 1 + B && 0 >= A && C >= D && A >= 1) {
     D = D_;
     goto loc_lbl71;
    }
   }
   if (nondet_bool()) {
    int D_ = D + -E;
    if (G == C && H == I && J == A && 1 + E == 0 && 1 + C >= D && D >= 1 + B && 0 >= A && C >= D) {
     D = D_;
     goto loc_lbl81;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = 1;
    if (J == A && H == I && G == C && E == F && D == B && B >= 1 + C && A >= 1) {
     E = E_;
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int E_ = 1;
    int D_ = 1 + D;
    if (J == A && H == I && G == C && E == F && D == B && C >= B && A >= 1) {
     D = D_;
     E = E_;
     goto loc_lbl71;
    }
   }
   if (nondet_bool()) {
    int E_ = -1;
    if (J == A && H == I && G == C && E == F && D == B && 0 >= A && B >= 1 + C) {
     E = E_;
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int E_ = -1;
    int D_ = 1 + D;
    if (J == A && H == I && G == C && E == F && D == B && 0 >= A && C >= B) {
     D = D_;
     E = E_;
     goto loc_lbl81;
    }
   }
  goto end;
 }
 loc_start0:
 {
 __cost++;
   if (nondet_bool()) {
    int J_ = A;
    int H_ = I;
    int G_ = C;
    int E_ = F;
    int D_ = B;
    if (1 >= 0) {
     D = D_;
     E = E_;
     G = G_;
     H = H_;
     J = J_;
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
