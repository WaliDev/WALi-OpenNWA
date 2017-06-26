int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F) {
 goto loc_start0;

 loc_lbl71:
 {
 __cost++;
   if (nondet_bool()) {
    if (A + C == D && E + C == F + D && B == 0 && D >= 1 + C) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int E_ = 1 + E;
    int C_ = -1 + C;
    int B_ = -1 + B;
    if (B + D == A + C && E + C == D + F && A + C >= D && D >= 1 + C && A + C >= 1 + D) {
     B = B_;
     C = C_;
     E = E_;
     goto loc_lbl71;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    if (E == F && C == D && B == A && 0 >= A) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int E_ = 1 + E;
    int C_ = -1 + C;
    int B_ = -1 + B;
    if (E == F && C == D && B == A && A >= 1) {
     B = B_;
     C = C_;
     E = E_;
     goto loc_lbl71;
    }
   }
  goto end;
 }
 loc_start0:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = F;
    int C_ = D;
    int B_ = A;
    if (1 >= 0) {
     B = B_;
     C = C_;
     E = E_;
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
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
