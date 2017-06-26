int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 0;
    if (1 >= 0) {
     A = A_;
     goto loc_f5;
    }
   }
  goto end;
 }
 loc_f17:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f17;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int B_ = 1 + B;
    if (0 >= 1 + Y_0) {
     B = B_;
     goto loc_f17;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (1 >= 0) {
     B = B_;
     goto loc_f17;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int G_ = nondet();
    int F_ = B;
    int D_ = B;
    int C_ = B;
    if (0 >= 1 + Y_0) {
     C = C_;
     D = D_;
     F = F_;
     G = G_;
     goto loc_f32;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int F_ = B;
    int D_ = B;
    int C_ = B;
    if (1 >= 0) {
     C = C_;
     D = D_;
     F = F_;
     G = G_;
     goto loc_f32;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int F_ = B;
    int D_ = B;
    if (1 >= 0) {
     D = D_;
     F = F_;
     G = G_;
     goto loc_f13;
    }
   }
  goto end;
 }
 loc_f32:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f32;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int C_ = 1 + C;
    if (0 >= 1 + Y_0) {
     C = C_;
     goto loc_f32;
    }
   }
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (1 >= 0) {
     C = C_;
     goto loc_f32;
    }
   }
   if (nondet_bool()) {
    int E_ = C;
    int D_ = C;
    if (1 >= 0) {
     D = D_;
     E = E_;
     goto loc_f13;
    }
   }
  goto end;
 }
 loc_f5:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (99 >= A) {
     A = A_;
     goto loc_f5;
    }
   }
   if (nondet_bool()) {
    int D_ = -2 + A;
    if (A >= 100) {
     D = D_;
     goto loc_f13;
    }
   }
   if (nondet_bool()) {
    int D_ = -2 + A;
    int B_ = -2 + A;
    if (A >= 100 && 0 >= 1 + A) {
     B = B_;
     D = D_;
     goto loc_f17;
    }
   }
  goto end;
 }
 loc_f13:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
