int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_start0;

 loc_a:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = nondet();
    int D_ = -1 + D;
    if (D >= 1 && C >= 0 && B >= 0 && A >= D) {
     C = C_;
     D = D_;
     goto loc_b;
    }
   }
  goto end;
 }
 loc_b:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 1 + D && B >= 0 && D >= 0 && C >= 0) {
     goto loc_start;
    }
   }
   if (nondet_bool()) {
    if (A >= 1 + D && B >= 0 && D >= 0 && 0 >= 1 + C) {
     goto loc_stop3;
    }
   }
  goto end;
 }
 loc_cont1:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = -1 + D;
    int C_ = 1;
    if (C == 0 && A >= D && B >= 0 && D >= 1) {
     C = C_;
     D = D_;
     goto loc_stop2;
    }
   }
   if (nondet_bool()) {
    int C_ = -1 + C;
    if (A >= D && B >= 0 && C >= 0 && D >= 1 && C >= 1) {
     C = C_;
     goto loc_a;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    if (D == 0 && C >= 0 && B >= 0 && A >= 0) {
     goto loc_stop1;
    }
   }
   if (nondet_bool()) {
    if (A >= D && D >= 0 && C >= 0 && B >= 0 && A >= 0 && D >= 1) {
     goto loc_cont1;
    }
   }
  goto end;
 }
 loc_start0:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = A;
    int C_ = B;
    if (B >= 0 && A >= 0) {
     C = C_;
     D = D_;
     goto loc_start;
    }
   }
  goto end;
 }
 loc_stop1:
 loc_stop2:
 loc_stop3:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet());
}
