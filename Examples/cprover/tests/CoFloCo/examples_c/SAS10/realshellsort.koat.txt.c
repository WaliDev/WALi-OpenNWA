int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P) {
 goto loc_start0;

 loc_lbl101:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = E + -G;
    if (K == L && E >= 0 && 1 + 2 * O >= L && G >= 1 && L >= 1 + I && O >= G && I >= G + E && E >= G) {
     E = E_;
     goto loc_lbl101;
    }
   }
   if (nondet_bool()) {
    int M_ = I;
    int I_ = 1 + I;
    if (K == L && E >= 0 && 1 + 2 * O >= L && G >= 1 && L >= 1 + I && O >= G && I >= G + E) {
     I = I_;
     M = M_;
     goto loc_lbl53;
    }
   }
  goto end;
 }
 loc_lbl121:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = A;
    if (1 + M == L && I == L && K == L && L >= 1 + E && G >= 1 && E >= 0 && O >= G && 1 + 2 * O >= L && 1 + 2 * A >= G && G >= 2 * A) {
     G = G_;
     goto loc_lbl123;
    }
   }
  goto end;
 }
 loc_lbl123:
 {
 __cost++;
   if (nondet_bool()) {
    if (A == 0 && 1 + M == L && I == L && K == L && G == 0 && O >= 0 && L >= 1 + E && O >= 1 && 1 + 2 * O >= L && E >= 0) {
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int A_ = nondet();
    int I_ = 0;
    if (G == A && 1 + M == L && I == L && K == L && O >= 2 * A && L >= 1 + E && O >= 1 && 1 + 2 * O >= L && E >= 0 && A >= 0 && 0 >= L && A >= 1) {
     A = A_;
     I = I_;
     goto loc_lbl121;
    }
   }
   if (nondet_bool()) {
    int C_ = nondet();
    int I_ = 0;
    int E_ = 0;
    if (G == A && 1 + M == L && I == L && K == L && O >= 2 * A && L >= 1 + E && O >= 1 && 1 + 2 * O >= L && E >= 0 && A >= 0 && A >= 1 && L >= 1) {
     C = C_;
     E = E_;
     I = I_;
     goto loc_lbl71;
    }
   }
  goto end;
 }
 loc_lbl21:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = O;
    if (K == L && I == J && G == H && E == F && C == D && A == B && M == N && 1 + 2 * O >= L && 0 >= O && L >= 2 * O) {
     G = G_;
     goto loc_stop;
    }
   }
   if (nondet_bool()) {
    int C_ = nondet();
    int I_ = 0;
    int G_ = O;
    int E_ = 0;
    if (K == L && I == J && G == H && E == F && C == D && A == B && M == N && 1 + 2 * O >= L && O >= 1 && L >= 2 * O) {
     C = C_;
     E = E_;
     G = G_;
     I = I_;
     goto loc_lbl71;
    }
   }
  goto end;
 }
 loc_lbl53:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = nondet();
    if (1 + M == L && K == L && I == L && 1 + 2 * O >= L && O >= G && E >= 0 && G >= 1 && L >= 1 + E) {
     A = A_;
     goto loc_lbl121;
    }
   }
   if (nondet_bool()) {
    int C_ = nondet();
    int E_ = I;
    if (K == L && I == 1 + M && 1 + 2 * O >= L && O >= G && E >= 0 && G >= 1 && M >= E && L >= 1 + M && L >= 2 + M) {
     C = C_;
     E = E_;
     goto loc_lbl71;
    }
   }
  goto end;
 }
 loc_lbl71:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = E + -G;
    if (K == L && I == E && L >= 1 + E && E >= 0 && G >= 1 && O >= G && 1 + 2 * O >= L && E >= G) {
     E = E_;
     goto loc_lbl101;
    }
   }
   if (nondet_bool()) {
    int M_ = I;
    int I_ = 1 + I;
    if (K == L && I == E && L >= 1 + E && E >= 0 && G >= 1 && O >= G && 1 + 2 * O >= L) {
     I = I_;
     M = M_;
     goto loc_lbl53;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    int O_ = nondet();
    if (O == P && M == N && K == L && I == J && G == H && E == F && C == D && A == B) {
     O = O_;
     goto loc_lbl21;
    }
   }
  goto end;
 }
 loc_start0:
 {
 __cost++;
   if (nondet_bool()) {
    int O_ = P;
    int M_ = N;
    int K_ = L;
    int I_ = J;
    int G_ = H;
    int E_ = F;
    int C_ = D;
    int A_ = B;
    if (1 >= 0) {
     A = A_;
     C = C_;
     E = E_;
     G = G_;
     I = I_;
     K = K_;
     M = M_;
     O = O_;
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
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
