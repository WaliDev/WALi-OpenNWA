int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E) {
 goto loc_evalcomplexstart;

 loc_evalcomplexbb10in:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = B;
    int C_ = A;
    if (29 >= B) {
     C = C_;
     D = D_;
     goto loc_evalcomplexbb8in;
    }
   }
   if (nondet_bool()) {
    if (B >= 30) {
     goto loc_evalcomplexreturnin;
    }
   }
  goto end;
 }
 loc_evalcomplexbb1in:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = 7 + C;
    if (2 >= C && C >= 6) {
     E = E_;
     goto loc_evalcomplexbb7in;
    }
   }
   if (nondet_bool()) {
    int E_ = 7 + C;
    if (C >= 6) {
     E = E_;
     goto loc_evalcomplexbb7in;
    }
   }
   if (nondet_bool()) {
    int E_ = 7 + C;
    if (5 >= C && C >= 3 && C >= 6) {
     E = E_;
     goto loc_evalcomplexbb6in;
    }
   }
   if (nondet_bool()) {
    int E_ = 2 + C;
    if (7 >= C && 5 >= C) {
     E = E_;
     goto loc_evalcomplexbb7in;
    }
   }
   if (nondet_bool()) {
    int E_ = 2 + C;
    if (C >= 11 && 5 >= C) {
     E = E_;
     goto loc_evalcomplexbb7in;
    }
   }
   if (nondet_bool()) {
    int E_ = 2 + C;
    if (10 >= C && C >= 8 && 5 >= C) {
     E = E_;
     goto loc_evalcomplexbb6in;
    }
   }
  goto end;
 }
 loc_evalcomplexbb6in:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 10 + D;
    int C_ = E;
    if (1 >= 0) {
     C = C_;
     D = D_;
     goto loc_evalcomplexbb8in;
    }
   }
  goto end;
 }
 loc_evalcomplexbb7in:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 1 + D;
    int C_ = E;
    if (1 >= 0) {
     C = C_;
     D = D_;
     goto loc_evalcomplexbb8in;
    }
   }
  goto end;
 }
 loc_evalcomplexbb8in:
 {
 __cost++;
   if (nondet_bool()) {
    if (D >= 1 + C) {
     goto loc_evalcomplexbb1in;
    }
   }
   if (nondet_bool()) {
    if (C >= D) {
     goto loc_evalcomplexbb9in;
    }
   }
  goto end;
 }
 loc_evalcomplexbb9in:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 2 + D;
    int A_ = -10 + C;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_evalcomplexbb10in;
    }
   }
  goto end;
 }
 loc_evalcomplexentryin:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = A;
    int A_ = B;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_evalcomplexbb10in;
    }
   }
  goto end;
 }
 loc_evalcomplexreturnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalcomplexstop;
    }
   }
  goto end;
 }
 loc_evalcomplexstart:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalcomplexentryin;
    }
   }
  goto end;
 }
 loc_evalcomplexstop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet());
}
