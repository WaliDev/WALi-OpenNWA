int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = nondet();
    int B_ = 0;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_f10;
    }
   }
  goto end;
 }
 loc_f10:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (C >= 1 + B) {
     B = B_;
     goto loc_f10;
    }
   }
   if (nondet_bool()) {
    int E_ = 0;
    int D_ = C;
    if (B >= C) {
     D = D_;
     E = E_;
     goto loc_f18;
    }
   }
  goto end;
 }
 loc_f18:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = 1 + E;
    int F_ = E;
    if (D >= 2 + E) {
     F = F_;
     G = G_;
     goto loc_f22;
    }
   }
   if (nondet_bool()) {
    int E_ = 0;
    if (1 + E >= D) {
     E = E_;
     goto loc_f34;
    }
   }
  goto end;
 }
 loc_f22:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = 1 + G;
    if (D >= 1 + G) {
     G = G_;
     goto loc_f22;
    }
   }
   if (nondet_bool()) {
    int G_ = 1 + G;
    int F_ = G;
    if (D >= 1 + G) {
     F = F_;
     G = G_;
     goto loc_f22;
    }
   }
   if (nondet_bool()) {
    int H_ = nondet();
    int E_ = 1 + E;
    if (G >= D) {
     E = E_;
     H = H_;
     goto loc_f18;
    }
   }
  goto end;
 }
 loc_f34:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = 1 + E;
    if (D >= 2 + E) {
     E = E_;
     goto loc_f34;
    }
   }
   if (nondet_bool()) {
    if (1 + E >= D) {
     goto loc_f43;
    }
   }
  goto end;
 }
 loc_f43:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
