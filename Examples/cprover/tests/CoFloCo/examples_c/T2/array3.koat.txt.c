int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = nondet();
    int C_ = 0;
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_f9;
    }
   }
  goto end;
 }
 loc_f17:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (49 >= A) {
     A = A_;
     goto loc_f17;
    }
   }
   if (nondet_bool()) {
    if (A >= 50) {
     goto loc_f24;
    }
   }
  goto end;
 }
 loc_f9:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (49 >= C) {
     C = C_;
     goto loc_f9;
    }
   }
   if (nondet_bool()) {
    int A_ = 0;
    if (C >= 50) {
     A = A_;
     goto loc_f17;
    }
   }
  goto end;
 }
 loc_f24:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet());
}
