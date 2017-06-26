int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E) {
 goto loc_f28;

 loc_f12:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f11;
    }
   }
  goto end;
 }
 loc_f28:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = A;
    int D_ = 1;
    int C_ = A;
    int B_ = 1;
    if (A >= 1) {
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f20;
    }
   }
  goto end;
 }
 loc_f11:
 loc_f20:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet());
}
