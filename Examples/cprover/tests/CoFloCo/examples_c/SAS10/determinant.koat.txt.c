int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H) {
 goto loc_start0;

 loc_a:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1;
    if (B == A && C == D && E == F && G == H && A >= 1) {
     C = C_;
     goto loc_d;
    }
   }
  goto end;
 }
 loc_b:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (B == A && G == 1 + A && A >= C && C >= 1 && A >= 1 + C) {
     C = C_;
     goto loc_d;
    }
   }
   if (nondet_bool()) {
    int E_ = B;
    if (B == A && G >= 1 + C && C >= 1 && A >= 1 + C && 1 + A >= G && A >= G) {
     E = E_;
     goto loc_c;
    }
   }
  goto end;
 }
 loc_c:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = -1 + E;
    if (B == A && G >= 1 + C && E >= C && C >= 1 && A >= G && A >= E && E >= 1 + C) {
     E = E_;
     goto loc_c;
    }
   }
   if (nondet_bool()) {
    int G_ = 1 + G;
    if (B == A && C == E && G >= 1 + E && E >= 1 && A >= G && A >= E) {
     G = G_;
     goto loc_b;
    }
   }
  goto end;
 }
 loc_d:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = 1 + C;
    if (B == A && C >= 1 && A >= 1 && A >= C && A >= 1 + C) {
     G = G_;
     goto loc_b;
    }
   }
   if (nondet_bool()) {
    if (B == A && C == A && A >= 1) {
     goto loc_halt;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    if (G == H && E == F && C == D && B == A && A >= 1) {
     goto loc_a;
    }
   }
  goto end;
 }
 loc_start0:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = H;
    int E_ = F;
    int C_ = D;
    int B_ = A;
    if (1 >= 0) {
     B = B_;
     C = C_;
     E = E_;
     G = G_;
     goto loc_start;
    }
   }
  goto end;
 }
 loc_halt:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
