int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_f1;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = nondet();
    if (0 >= A) {
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int C_ = -1 + C;
    int A_ = A + C;
    if (A >= 1) {
     A = A_;
     C = C_;
     goto loc_f0;
    }
   }
  goto end;
 }
 loc_f1:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f0;
    }
   }
  goto end;
 }
 loc_f2:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet());
}
