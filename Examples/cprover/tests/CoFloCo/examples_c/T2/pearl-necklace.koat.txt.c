int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f1;
    }
   }
  goto end;
 }
 loc_f1:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (A >= 1 + B) {
     B = B_;
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    int C_ = B;
    if (B >= A) {
     C = C_;
     goto loc_f2;
    }
   }
  goto end;
 }
 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = -1 + C;
    if (C >= 1) {
     C = C_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int D_ = C;
    if (0 >= C) {
     D = D_;
     goto loc_f3;
    }
   }
  goto end;
 }
 loc_f3:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 1 + D;
    if (A >= 1 + D) {
     D = D_;
     goto loc_f3;
    }
   }
   if (nondet_bool()) {
    int E_ = D;
    if (D >= A) {
     E = E_;
     goto loc_f4;
    }
   }
  goto end;
 }
 loc_f4:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = -1 + E;
    if (E >= 1) {
     E = E_;
     goto loc_f4;
    }
   }
  goto end;
 }
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet());
}
