int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D) {
 goto loc_start0;

 loc_cut:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = nondet();
    int C_ = A;
    if (A >= 1 + C && C >= 0 && 9 >= A && 2 + C >= A) {
     A = A_;
     C = C_;
     goto loc_lbl51;
    }
   }
  goto end;
 }
 loc_lbl51:
 {
 __cost++;
   if (nondet_bool()) {
    if (9 >= C && C >= 0 && C >= A) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    if (9 >= C && C >= 0 && A >= 3 + C) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    if (9 >= C && C >= 0 && 9 >= A && 2 + C >= A && A >= 1 + C) {
     goto loc_cut;
    }
   }
   if (nondet_bool()) {
    if (9 >= C && C >= 0 && 2 + C >= A && A >= 1 + C && A >= 10) {
     goto loc_stop;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = nondet();
    int C_ = 0;
    if (C == D && A == B) {
     A = A_;
     C = C_;
     goto loc_lbl51;
    }
   }
  goto end;
 }
 loc_start0:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = D;
    int A_ = B;
    if (1 >= 0) {
     A = A_;
     C = C_;
     goto loc_start;
    }
   }
  goto end;
 }
 loc_stop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet());
}
