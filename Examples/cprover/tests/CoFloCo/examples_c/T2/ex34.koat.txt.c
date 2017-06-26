int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R, int S, int T, int U, int V, int W, int X, int Y, int Z, int A1, int B1, int C1, int D1, int E1, int F1, int G1, int H1, int I1, int J1, int K1, int L1, int M1, int N1, int O1, int P1, int Q1, int R1, int S1, int T1, int U1, int V1) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int V1_ = nondet();
    int U1_ = nondet();
    int T1_ = nondet();
    int S1_ = nondet();
    int R1_ = nondet();
    int P1_ = nondet();
    int O1_ = nondet();
    int N1_ = nondet();
    int M1_ = nondet();
    int L1_ = nondet();
    int J1_ = nondet();
    int I1_ = nondet();
    int H1_ = nondet();
    int G1_ = nondet();
    int F1_ = nondet();
    int D1_ = nondet();
    int C1_ = nondet();
    int B1_ = nondet();
    int A1_ = nondet();
    int Z_ = nondet();
    int X_ = nondet();
    int W_ = nondet();
    int V_ = nondet();
    int U_ = nondet();
    int T_ = nondet();
    int R_ = nondet();
    int Q_ = nondet();
    int P_ = nondet();
    int O_ = nondet();
    int N_ = nondet();
    int L_ = nondet();
    int K_ = nondet();
    int J_ = nondet();
    int I_ = nondet();
    int H_ = nondet();
    int F_ = nondet();
    int E_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    int B_ = nondet();
    int Q1_ = 2;
    int K1_ = 2;
    int E1_ = 2;
    int Y_ = 2;
    int S_ = 2;
    int M_ = 2;
    int G_ = 2;
    int A_ = 2;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     N = N_;
     O = O_;
     P = P_;
     Q = Q_;
     R = R_;
     S = S_;
     T = T_;
     U = U_;
     V = V_;
     W = W_;
     X = X_;
     Y = Y_;
     Z = Z_;
     A1 = A1_;
     B1 = B1_;
     C1 = C1_;
     D1 = D1_;
     E1 = E1_;
     F1 = F1_;
     G1 = G1_;
     H1 = H1_;
     I1 = I1_;
     J1 = J1_;
     K1 = K1_;
     L1 = L1_;
     M1 = M1_;
     N1 = N1_;
     O1 = O1_;
     P1 = P1_;
     Q1 = Q1_;
     R1 = R1_;
     S1 = S1_;
     T1 = T1_;
     U1 = U1_;
     V1 = V1_;
     goto loc_f75;
    }
   }
  goto end;
 }
 loc_f75:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
