int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 0;
    if (A >= 128) {
     B = B_;
     goto loc_f5;
    }
   }
   if (nondet_bool()) {
    int C_ = nondet();
    int B_ = 0;
    if (127 >= A) {
     B = B_;
     C = C_;
     goto loc_f7;
    }
   }
  goto end;
 }
 loc_f7:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    int B_ = 1 + B;
    if (35 >= B) {
     B = B_;
     C = C_;
     goto loc_f7;
    }
   }
   if (nondet_bool()) {
    if (B >= 36) {
     goto loc_f5;
    }
   }
  goto end;
 }
 loc_f5:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet());
}
