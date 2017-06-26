int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H) {
 goto loc_start;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = B;
    int C_ = 0;
    if (A >= 1 + B) {
     C = C_;
     D = D_;
     goto loc_f12;
    }
   }
   if (nondet_bool()) {
    if (B >= A) {
     goto loc_f58;
    }
   }
  goto end;
 }
 loc_f12:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = nondet();
    int F_ = nondet();
    int E_ = 1 + E;
    if (G_ >= F_ && A >= E) {
     E = E_;
     F = F_;
     G = G_;
     goto loc_f12;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int F_ = nondet();
    int C_ = nondet();
    int E_ = 1 + E;
    int D_ = E;
    if (F_ >= 1 + G_ && A >= E) {
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     G = G_;
     goto loc_f12;
    }
   }
   if (nondet_bool()) {
    int D_ = B;
    if (B == D && E >= 1 + A) {
     D = D_;
     goto loc_f35;
    }
   }
   if (nondet_bool()) {
    if (E >= 1 + A && B >= 1 + D) {
     goto loc_f22;
    }
   }
   if (nondet_bool()) {
    if (E >= 1 + A && D >= 1 + B) {
     goto loc_f22;
    }
   }
  goto end;
 }
 loc_f22:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = nondet();
    int E_ = 1 + E;
    if (A >= E) {
     E = E_;
     H = H_;
     goto loc_f22;
    }
   }
   if (nondet_bool()) {
    if (E >= 1 + A) {
     goto loc_f29;
    }
   }
  goto end;
 }
 loc_f29:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = nondet();
    int E_ = 1 + E;
    if (A >= E) {
     E = E_;
     H = H_;
     goto loc_f29;
    }
   }
   if (nondet_bool()) {
    if (E >= 1 + A) {
     goto loc_f35;
    }
   }
  goto end;
 }
 loc_f35:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= 1 + C) {
     goto loc_f37;
    }
   }
   if (nondet_bool()) {
    if (C >= 1) {
     goto loc_f37;
    }
   }
   if (nondet_bool()) {
    int C_ = 0;
    int B_ = 1 + B;
    if (C == 0) {
     B = B_;
     C = C_;
     goto loc_f0;
    }
   }
  goto end;
 }
 loc_f37:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int H_ = nondet();
    if (0 >= 1 + Y_0 && A >= D) {
     H = H_;
     goto loc_f43;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int H_ = nondet();
    if (Y_0 >= 1 && A >= D) {
     H = H_;
     goto loc_f43;
    }
   }
   if (nondet_bool()) {
    int H_ = 0;
    int D_ = 1 + D;
    if (A >= D) {
     D = D_;
     H = H_;
     goto loc_f37;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (D >= 1 + A) {
     B = B_;
     goto loc_f0;
    }
   }
  goto end;
 }
 loc_f43:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = 1 + E;
    if (A >= E) {
     E = E_;
     goto loc_f43;
    }
   }
   if (nondet_bool()) {
    if (E >= 1 + A) {
     goto loc_f48;
    }
   }
  goto end;
 }
 loc_f48:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = 1 + E;
    if (A >= E) {
     E = E_;
     goto loc_f48;
    }
   }
   if (nondet_bool()) {
    int D_ = 1 + D;
    if (E >= 1 + A) {
     D = D_;
     goto loc_f37;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f0;
    }
   }
  goto end;
 }
 loc_f58:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
