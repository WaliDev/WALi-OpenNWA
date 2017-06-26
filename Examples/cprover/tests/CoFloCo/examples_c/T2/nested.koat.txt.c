int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G) {
 goto loc_f3;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= A) {
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int D_ = A;
    int C_ = -1 + C;
    int B_ = C;
    int A_ = -1 + A;
    if (A >= 1) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     goto loc_f0;
    }
   }
  goto end;
 }
 loc_f1:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = A;
    int E_ = C;
    int C_ = -1 + C;
    int A_ = -1 + A;
    if (C >= 1 && A >= 1) {
     A = A_;
     C = C_;
     E = E_;
     F = F_;
     goto loc_f0;
    }
   }
  goto end;
 }
 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = nondet();
    if (0 >= C) {
     G = G_;
     goto loc_f4;
    }
   }
   if (nondet_bool()) {
    int A_ = nondet();
    if (C >= 1 && A_ >= 1) {
     A = A_;
     goto loc_f0;
    }
   }
  goto end;
 }
 loc_f3:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = nondet();
    int A_ = nondet();
    if (1 >= 0) {
     A = A_;
     C = C_;
     goto loc_f2;
    }
   }
  goto end;
 }
 loc_f4:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
