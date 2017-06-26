int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 1 + B;
    if (C >= B && B >= 0) {
     D = D_;
     goto loc_f4;
    }
   }
  goto end;
 }
 loc_f4:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = nondet();
    if (B >= 1 + D) {
     A = A_;
     goto loc_f6;
    }
   }
   if (nondet_bool()) {
    int A_ = nondet();
    if (D >= 1 + B) {
     A = A_;
     goto loc_f6;
    }
   }
   if (nondet_bool()) {
    int D_ = B;
    if (B == D) {
     D = D_;
     goto loc_f14;
    }
   }
  goto end;
 }
 loc_f6:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= 1 + A) {
     goto loc_f7;
    }
   }
   if (nondet_bool()) {
    if (A >= 1) {
     goto loc_f7;
    }
   }
   if (nondet_bool()) {
    int A_ = 0;
    if (A == 0) {
     A = A_;
     goto loc_f14;
    }
   }
  goto end;
 }
 loc_f7:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 1 + D;
    if (C >= D) {
     D = D_;
     goto loc_f4;
    }
   }
   if (nondet_bool()) {
    int D_ = 0;
    if (D >= 1 + C) {
     D = D_;
     goto loc_f4;
    }
   }
  goto end;
 }
 loc_f14:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet());
}
