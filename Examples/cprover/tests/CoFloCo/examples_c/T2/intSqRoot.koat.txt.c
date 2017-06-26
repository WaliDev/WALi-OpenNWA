int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = C;
    int A_ = 1;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_f6;
    }
   }
  goto end;
 }
 loc_f6:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = nondet();
    int B_ = nondet();
    if (D_*D_ >= 1 + C && B >= 2 + A) {
     B = B_;
     D = D_;
     goto loc_f6;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    int A_ = nondet();
    if (C >= D_*D_ && B >= 2 + A) {
     A = A_;
     D = D_;
     goto loc_f6;
    }
   }
   if (nondet_bool()) {
    if (1 + A >= B) {
     goto loc_f16;
    }
   }
  goto end;
 }
 loc_f16:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet());
}
