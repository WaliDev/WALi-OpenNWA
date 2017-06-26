int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_f3;

 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = -1 + B;
    if (29 >= A) {
     B = B_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int B_ = -1 + B;
    if (A >= 30) {
     B = B_;
     goto loc_f300;
    }
   }
  goto end;
 }
 loc_f3:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f300;
    }
   }
  goto end;
 }
 loc_f300:
 {
 __cost++;
   if (nondet_bool()) {
    if (19 >= B) {
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int C_ = nondet();
    if (B >= 20) {
     C = C_;
     goto loc_f1;
    }
   }
  goto end;
 }
 loc_f1:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet());
}
