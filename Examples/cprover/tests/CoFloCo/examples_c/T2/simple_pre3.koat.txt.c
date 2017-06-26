int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1;
    int B_ = 1;
    int A_ = 1;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_f12;
    }
   }
  goto end;
 }
 loc_f8:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1;
    int A_ = 2;
    if (A == 2) {
     A = A_;
     B = B_;
     goto loc_f12;
    }
   }
   if (nondet_bool()) {
    int B_ = 0;
    if (1 >= A) {
     B = B_;
     goto loc_f12;
    }
   }
   if (nondet_bool()) {
    int B_ = 0;
    if (A >= 3) {
     B = B_;
     goto loc_f12;
    }
   }
  goto end;
 }
 loc_f12:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet());
}
