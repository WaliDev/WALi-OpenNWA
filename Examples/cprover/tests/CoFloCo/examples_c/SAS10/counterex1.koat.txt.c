int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H) {
 goto loc_start0;

 loc_cut:
 {
 __cost++;
   if (nondet_bool()) {
    if (G == H && 1 + D == 0 && A >= 0) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int B_ = -1 + B;
    if (G == H && A >= 1 + D && 1 + D >= 0 && B >= 0 && D >= 0) {
     B = B_;
     goto loc_lbl42;
    }
   }
   if (nondet_bool()) {
    int D_ = -1 + D;
    if (G == H && A >= 1 + D && 1 + D >= 0 && D >= 0) {
     D = D_;
     goto loc_cut;
    }
   }
   if (nondet_bool()) {
    int E_ = B;
    int D_ = -1 + D;
    int B_ = 1 + B;
    if (G == H && A >= 1 + D && 1 + D >= 0 && D >= 0 && H >= B) {
     B = B_;
     D = D_;
     E = E_;
     goto loc_lbl72;
    }
   }
  goto end;
 }
 loc_lbl42:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = -1 + B;
    if (G == H && A >= D && D >= 0 && 1 + B >= 0 && B >= 0) {
     B = B_;
     goto loc_lbl42;
    }
   }
   if (nondet_bool()) {
    int D_ = -1 + D;
    if (G == H && A >= D && D >= 0 && 1 + B >= 0) {
     D = D_;
     goto loc_cut;
    }
   }
   if (nondet_bool()) {
    int E_ = B;
    int D_ = -1 + D;
    int B_ = 1 + B;
    if (G == H && A >= D && D >= 0 && 1 + B >= 0 && H >= B) {
     B = B_;
     D = D_;
     E = E_;
     goto loc_lbl72;
    }
   }
  goto end;
 }
 loc_lbl72:
 {
 __cost++;
   if (nondet_bool()) {
    if (G == H && 1 + E == B && 1 + H >= B && 1 + D >= 0 && A >= 1 + D) {
     goto loc_cut;
    }
   }
   if (nondet_bool()) {
    int E_ = B;
    int B_ = 1 + B;
    if (G == H && 1 + E == B && 1 + H >= B && 1 + D >= 0 && A >= 1 + D && H >= B) {
     B = B_;
     E = E_;
     goto loc_lbl72;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    if (G == H && E == F && D == A && B == C && 0 >= 1 + A) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int B_ = -1 + B;
    if (G == H && E == F && D == A && B == C && C >= 0 && A >= 0) {
     B = B_;
     goto loc_lbl42;
    }
   }
   if (nondet_bool()) {
    int D_ = -1 + D;
    if (G == H && E == F && D == A && B == C && A >= 0) {
     D = D_;
     goto loc_cut;
    }
   }
   if (nondet_bool()) {
    int E_ = B;
    int D_ = -1 + D;
    int B_ = 1 + B;
    if (G == H && E == F && D == A && B == C && A >= 0 && H >= C) {
     B = B_;
     D = D_;
     E = E_;
     goto loc_lbl72;
    }
   }
  goto end;
 }
 loc_start0:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = H;
    int E_ = F;
    int D_ = A;
    int B_ = C;
    if (1 >= 0) {
     B = B_;
     D = D_;
     E = E_;
     G = G_;
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
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
