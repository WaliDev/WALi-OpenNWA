int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F) {
 goto loc_start0;

 loc_cut:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1;
    if (B == A && 1 + C == A && E >= 1 && A >= 2 + E && A >= 2) {
     C = C_;
     goto loc_lbl71;
    }
   }
   if (nondet_bool()) {
    int E_ = 1 + E;
    int C_ = 0;
    if (B == A && 1 + C == A && E >= 1 && A >= 2 + E && A >= 3 + E && 1 >= A) {
     C = C_;
     E = E_;
     goto loc_cut;
    }
   }
   if (nondet_bool()) {
    int E_ = 1 + E;
    int C_ = 0;
    if (B == A && 2 + E == A && 1 + C == A && A >= 3 && 1 >= A) {
     C = C_;
     E = E_;
     goto loc_stop;
    }
   }
  goto end;
 }
 loc_lbl71:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (B == A && E >= 0 && C >= 1 && A >= 2 + E && A >= 1 + C && A >= 2 + C) {
     C = C_;
     goto loc_lbl71;
    }
   }
   if (nondet_bool()) {
    int E_ = 1 + E;
    if (B == A && 1 + C == A && E >= 0 && A >= 2 && A >= 2 + E && A >= 3 + E) {
     E = E_;
     goto loc_cut;
    }
   }
   if (nondet_bool()) {
    int E_ = 1 + E;
    if (B == A && 1 + C == A && 2 + E == A && A >= 2) {
     E = E_;
     goto loc_stop;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = 0;
    int C_ = 1;
    if (E == F && C == D && B == A && A >= 2) {
     C = C_;
     E = E_;
     goto loc_lbl71;
    }
   }
   if (nondet_bool()) {
    int E_ = 1;
    int C_ = 0;
    if (E == F && C == D && B == A && 1 >= A) {
     C = C_;
     E = E_;
     goto loc_stop;
    }
   }
  goto end;
 }
 loc_start0:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = F;
    int C_ = D;
    int B_ = A;
    if (1 >= 0) {
     B = B_;
     C = C_;
     E = E_;
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
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
