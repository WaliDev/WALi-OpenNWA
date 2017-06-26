int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = nondet();
    int E_ = nondet();
    int B_ = nondet();
    int F_ = 1 + 2 * G_;
    int C_ = 0;
    int A_ = 1 + 2 * G_;
    if (2 * G_ >= 0 && E_ >= 1) {
     A = A_;
     B = B_;
     C = C_;
     E = E_;
     F = F_;
     G = G_;
     goto loc_f14;
    }
   }
  goto end;
 }
 loc_f14:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = nondet();
    int C_ = 1 + C;
    int B_ = -1 + B;
    int A_ = -1 + A;
    if (D_ >= 1 && A >= 1) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     goto loc_f14;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    int A_ = -1 + A;
    if (A >= 1 + B && A >= 1 && 0 >= D_) {
     A = A_;
     D = D_;
     goto loc_f14;
    }
   }
   if (nondet_bool()) {
    if (0 >= A) {
     goto loc_f24;
    }
   }
  goto end;
 }
 loc_f24:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f24;
    }
   }
  goto end;
 }
 loc_f26:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f29;
    }
   }
  goto end;
 }
 loc_f29:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
