int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = 1;
    int D_ = 1;
    int C_ = 0;
    int B_ = 1;
    int A_ = 1;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f8;
    }
   }
  goto end;
 }
 loc_f10:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = nondet();
    if (E >= 6 && D >= 1 + E) {
     E = E_;
     goto loc_f14;
    }
   }
   if (nondet_bool()) {
    int E_ = 2 + E;
    if (5 >= E && D >= 1 + E) {
     E = E_;
     goto loc_f14;
    }
   }
   if (nondet_bool()) {
    int E_ = -10 + E;
    int D_ = 2 + D;
    if (E >= D) {
     D = D_;
     E = E_;
     goto loc_f8;
    }
   }
  goto end;
 }
 loc_f14:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 10 + D;
    if (E >= 10 && 12 >= E) {
     D = D_;
     goto loc_f10;
    }
   }
   if (nondet_bool()) {
    int D_ = 1 + D;
    if (E >= 13) {
     D = D_;
     goto loc_f10;
    }
   }
   if (nondet_bool()) {
    int D_ = 1 + D;
    if (9 >= E) {
     D = D_;
     goto loc_f10;
    }
   }
  goto end;
 }
 loc_f8:
 {
 __cost++;
   if (nondet_bool()) {
    if (29 >= D) {
     goto loc_f10;
    }
   }
   if (nondet_bool()) {
    int F_ = 1;
    int C_ = 1;
    if (D >= 30) {
     C = C_;
     F = F_;
     goto loc_f28;
    }
   }
  goto end;
 }
 loc_f28:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
