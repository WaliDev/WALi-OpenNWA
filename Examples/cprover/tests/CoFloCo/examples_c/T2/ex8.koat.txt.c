int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = nondet();
    int B_ = nondet();
    int A_ = nondet();
    int D_ = 0;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     goto loc_f9;
    }
   }
  goto end;
 }
 loc_f24:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    if (0 >= 1 + Y_0) {
     goto loc_f24;
    }
   }
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f24;
    }
   }
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f37;
    }
   }
  goto end;
 }
 loc_f37:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    if (Y_0 >= 1 + Y_1) {
     goto loc_f37;
    }
   }
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f37;
    }
   }
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f48;
    }
   }
  goto end;
 }
 loc_f9:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = nondet();
    int D_ = 1 + D;
    if (C >= 1 + D) {
     D = D_;
     E = E_;
     goto loc_f9;
    }
   }
   if (nondet_bool()) {
    if (D >= C) {
     goto loc_f24;
    }
   }
  goto end;
 }
 loc_f48:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet());
}
