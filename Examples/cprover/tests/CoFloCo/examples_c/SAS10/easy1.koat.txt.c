int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F) {
 goto loc_start0;

 loc_lbl111:
 {
 __cost++;
   if (nondet_bool()) {
    if (A == B && C == 100 && 41 >= E && E >= 2 && E >= 40) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int E_ = 1 + E;
    if (B == 0 && C == 100 && A == 0 && 41 >= E && E >= 2 && 39 >= E) {
     E = E_;
     goto loc_lbl91;
    }
   }
   if (nondet_bool()) {
    int E_ = 2 + E;
    if (A == B && C == 100 && 41 >= E && E >= 2 && 39 >= E && 0 >= 1 + B) {
     E = E_;
     goto loc_lbl111;
    }
   }
   if (nondet_bool()) {
    int E_ = 2 + E;
    if (A == B && C == 100 && 41 >= E && E >= 2 && 39 >= E && B >= 1) {
     E = E_;
     goto loc_lbl111;
    }
   }
  goto end;
 }
 loc_lbl91:
 {
 __cost++;
   if (nondet_bool()) {
    if (B == 0 && A == 0 && C == 100 && E == 40) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int E_ = 1 + E;
    if (B == 0 && C == 100 && A == 0 && 40 >= E && E >= 1 && 39 >= E) {
     E = E_;
     goto loc_lbl91;
    }
   }
   if (nondet_bool()) {
    int E_ = 2 + E;
    if (B == 0 && A == 0 && C == 100 && 40 >= E && E >= 1 && 39 >= E && 0 >= 1) {
     E = E_;
     goto loc_lbl111;
    }
   }
   if (nondet_bool()) {
    int E_ = 2 + E;
    if (B == 0 && A == 0 && C == 100 && 40 >= E && E >= 1 && 39 >= E && 0 >= 1) {
     E = E_;
     goto loc_lbl111;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = 1;
    int C_ = 100;
    if (E == F && C == D && B == 0 && A == 0) {
     C = C_;
     E = E_;
     goto loc_lbl91;
    }
   }
   if (nondet_bool()) {
    int E_ = 2;
    int C_ = 100;
    if (E == F && C == D && A == B && 0 >= 1 + B) {
     C = C_;
     E = E_;
     goto loc_lbl111;
    }
   }
   if (nondet_bool()) {
    int E_ = 2;
    int C_ = 100;
    if (E == F && C == D && A == B && B >= 1) {
     C = C_;
     E = E_;
     goto loc_lbl111;
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
    int A_ = B;
    if (1 >= 0) {
     A = A_;
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
