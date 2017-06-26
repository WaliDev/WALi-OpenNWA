int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 0) {
     goto loc_f2;
    }
   }
  goto end;
 }
 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    int B_ = -2 + B;
    int A_ = A + B;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int C_ = -2 + C;
    int A_ = A + C;
    if (1 >= 0) {
     A = A_;
     C = C_;
     goto loc_f2;
    }
   }
  goto end;
 }
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet());
}
