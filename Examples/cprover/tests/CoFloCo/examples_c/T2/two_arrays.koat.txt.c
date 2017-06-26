int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = nondet();
    int B_ = nondet();
    int D_ = 0;
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     goto loc_f15;
    }
   }
  goto end;
 }
 loc_f15:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 1 + D;
    if (49 >= D) {
     D = D_;
     goto loc_f15;
    }
   }
   if (nondet_bool()) {
    int E_ = 0;
    if (D >= 50) {
     E = E_;
     goto loc_f25;
    }
   }
  goto end;
 }
 loc_f25:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = 1 + E;
    if (49 >= E) {
     E = E_;
     goto loc_f25;
    }
   }
   if (nondet_bool()) {
    int A_ = 0;
    if (E >= 50) {
     A = A_;
     goto loc_f33;
    }
   }
  goto end;
 }
 loc_f33:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (49 >= A) {
     A = A_;
     goto loc_f33;
    }
   }
   if (nondet_bool()) {
    int F_ = 0;
    if (A >= 50) {
     F = F_;
     goto loc_f42;
    }
   }
  goto end;
 }
 loc_f42:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = 1 + F;
    if (49 >= F) {
     F = F_;
     goto loc_f42;
    }
   }
   if (nondet_bool()) {
    int G_ = 0;
    if (F >= 50) {
     G = G_;
     goto loc_f52;
    }
   }
  goto end;
 }
 loc_f52:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = 1 + G;
    if (49 >= G) {
     G = G_;
     goto loc_f52;
    }
   }
   if (nondet_bool()) {
    int A_ = 0;
    if (G >= 50) {
     A = A_;
     goto loc_f60;
    }
   }
  goto end;
 }
 loc_f60:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (49 >= A) {
     A = A_;
     goto loc_f60;
    }
   }
   if (nondet_bool()) {
    if (A >= 50) {
     goto loc_f69;
    }
   }
  goto end;
 }
 loc_f69:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
