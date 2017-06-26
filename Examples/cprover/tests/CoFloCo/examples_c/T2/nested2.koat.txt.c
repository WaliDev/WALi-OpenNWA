int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F) {
 goto loc_f3;

 loc_f0:
 {
 __cost++;
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
   if (nondet_bool()) {
    int A_ = 5000;
    if (C >= 1 && 0 >= A) {
     A = A_;
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
 loc_f3:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = nondet();
    int A_ = 5000;
    if (C_ >= 1) {
     A = A_;
     C = C_;
     goto loc_f0;
    }
   }
  goto end;
 }
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
