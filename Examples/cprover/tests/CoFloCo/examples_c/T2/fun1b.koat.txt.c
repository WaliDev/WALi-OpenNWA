int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f1;
    }
   }
  goto end;
 }
 loc_f1:
 {
 __cost++;
   if (nondet_bool()) {
    int L_ = nondet();
    int K_ = nondet();
    int H_ = nondet();
    int G_ = nondet();
    int M_ = 7;
    int J_ = 1;
    int I_ = 0;
    int E_ = F;
    int C_ = D;
    int B_ = 1;
    int A_ = -1 + A;
    if (K_ >= 1 && L_ >= 1 && 3 >= L_ && 7 >= K_ && 7 >= L_ && A >= 1) {
     A = A_;
     B = B_;
     C = C_;
     E = E_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     goto loc_f7;
    }
   }
   if (nondet_bool()) {
    int L_ = nondet();
    int K_ = nondet();
    int H_ = nondet();
    int G_ = nondet();
    int M_ = 7;
    int J_ = 1;
    int I_ = 0;
    int E_ = F;
    int C_ = D;
    int B_ = 1;
    if (K_ >= 1 && L_ >= 5 && 7 >= K_ && 7 >= L_) {
     B = B_;
     C = C_;
     E = E_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     goto loc_f7;
    }
   }
   if (nondet_bool()) {
    int K_ = nondet();
    int G_ = nondet();
    int M_ = 7;
    int L_ = 4;
    int J_ = 1;
    int I_ = 1;
    int H_ = 4;
    int F_ = 1 + F;
    int E_ = 1 + F;
    int D_ = 1 + D;
    int C_ = 1 + D;
    int B_ = 1;
    if (K_ >= 1 && 7 >= K_) {
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
     goto loc_f7;
    }
   }
   if (nondet_bool()) {
    int L_ = nondet();
    int H_ = nondet();
    int M_ = 2;
    int K_ = 3;
    int J_ = 0;
    int I_ = 0;
    int G_ = 3;
    int E_ = F;
    int C_ = D;
    int B_ = 0;
    if (L_ >= 1 && 3 >= L_ && 7 >= L_) {
     B = B_;
     C = C_;
     E = E_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int L_ = nondet();
    int H_ = nondet();
    int M_ = 2;
    int K_ = 3;
    int J_ = 0;
    int I_ = 0;
    int G_ = 3;
    int E_ = F;
    int C_ = D;
    int B_ = 0;
    if (L_ >= 5 && 7 >= L_) {
     B = B_;
     C = C_;
     E = E_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int M_ = 2;
    int L_ = 4;
    int K_ = 3;
    int J_ = 0;
    int I_ = 1;
    int H_ = 4;
    int G_ = 3;
    int F_ = 1 + F;
    int E_ = 1 + F;
    int D_ = 1 + D;
    int C_ = 1 + D;
    int B_ = 0;
    if (1 >= 0) {
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
     goto loc_f2;
    }
   }
  goto end;
 }
 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int L_ = nondet();
    int K_ = nondet();
    int H_ = nondet();
    int G_ = nondet();
    int M_ = 7;
    int J_ = 1;
    int E_ = F;
    int C_ = D;
    int B_ = 1;
    if (K_ >= 1 && L_ >= 1 && 3 >= L_ && 7 >= K_ && 7 >= L_) {
     B = B_;
     C = C_;
     E = E_;
     G = G_;
     H = H_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     goto loc_f7;
    }
   }
   if (nondet_bool()) {
    int L_ = nondet();
    int K_ = nondet();
    int H_ = nondet();
    int G_ = nondet();
    int M_ = 7;
    int J_ = 1;
    int E_ = F;
    int C_ = D;
    int B_ = 1;
    if (K_ >= 1 && L_ >= 5 && 7 >= K_ && 7 >= L_) {
     B = B_;
     C = C_;
     E = E_;
     G = G_;
     H = H_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     goto loc_f7;
    }
   }
   if (nondet_bool()) {
    int K_ = nondet();
    int G_ = nondet();
    int M_ = 7;
    int L_ = 4;
    int J_ = 1;
    int I_ = 1;
    int H_ = 4;
    int F_ = 1 + F;
    int E_ = 1 + F;
    int D_ = 1 + D;
    int C_ = 1 + D;
    int B_ = 1;
    if (K_ >= 1 && 7 >= K_) {
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
     goto loc_f7;
    }
   }
   if (nondet_bool()) {
    int L_ = nondet();
    int K_ = nondet();
    int H_ = nondet();
    int G_ = nondet();
    int M_ = 3;
    int J_ = 0;
    int E_ = F;
    int C_ = D;
    int B_ = 0;
    if (K_ >= 1 && L_ >= 1 && 3 >= L_ && 7 >= K_ && 7 >= L_) {
     B = B_;
     C = C_;
     E = E_;
     G = G_;
     H = H_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     goto loc_f3;
    }
   }
   if (nondet_bool()) {
    int L_ = nondet();
    int K_ = nondet();
    int H_ = nondet();
    int G_ = nondet();
    int M_ = 3;
    int J_ = 0;
    int E_ = F;
    int C_ = D;
    int B_ = 0;
    if (K_ >= 1 && L_ >= 5 && 7 >= K_ && 7 >= L_) {
     B = B_;
     C = C_;
     E = E_;
     G = G_;
     H = H_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     goto loc_f3;
    }
   }
   if (nondet_bool()) {
    int K_ = nondet();
    int G_ = nondet();
    int M_ = 3;
    int L_ = 4;
    int J_ = 0;
    int I_ = 1;
    int H_ = 4;
    int F_ = 1 + F;
    int E_ = 1 + F;
    int D_ = 1 + D;
    int C_ = 1 + D;
    int B_ = 0;
    if (K_ >= 1 && 7 >= K_) {
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
     goto loc_f3;
    }
   }
  goto end;
 }
 loc_f3:
 {
 __cost++;
   if (nondet_bool()) {
    int L_ = nondet();
    int K_ = nondet();
    int H_ = nondet();
    int G_ = nondet();
    int M_ = 6;
    int J_ = 1;
    int E_ = F;
    int C_ = D;
    int B_ = 1;
    if (K_ >= 1 && L_ >= 1 && 3 >= L_ && 7 >= K_ && 7 >= L_) {
     B = B_;
     C = C_;
     E = E_;
     G = G_;
     H = H_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     goto loc_f6;
    }
   }
   if (nondet_bool()) {
    int L_ = nondet();
    int K_ = nondet();
    int H_ = nondet();
    int G_ = nondet();
    int M_ = 6;
    int J_ = 1;
    int E_ = F;
    int C_ = D;
    int B_ = 1;
    if (K_ >= 1 && L_ >= 5 && 7 >= K_ && 7 >= L_) {
     B = B_;
     C = C_;
     E = E_;
     G = G_;
     H = H_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     goto loc_f6;
    }
   }
   if (nondet_bool()) {
    int K_ = nondet();
    int G_ = nondet();
    int M_ = 6;
    int L_ = 4;
    int J_ = 1;
    int I_ = 1;
    int H_ = 4;
    int F_ = 1 + F;
    int E_ = 1 + F;
    int D_ = 1 + D;
    int C_ = 1 + D;
    int B_ = 1;
    if (K_ >= 1 && 7 >= K_) {
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
     goto loc_f6;
    }
   }
  goto end;
 }
 loc_f4:
 {
 __cost++;
   if (nondet_bool()) {
    int L_ = nondet();
    int K_ = nondet();
    int H_ = nondet();
    int G_ = nondet();
    int M_ = 2;
    int J_ = 0;
    int I_ = 1;
    int E_ = F;
    int C_ = D;
    int B_ = 0;
    if (I == 1 && K_ >= 1 && L_ >= 1 && 3 >= L_ && 7 >= K_ && 7 >= L_ && O >= 1 + D && O >= 1 && N >= 1 + F && N >= 1) {
     B = B_;
     C = C_;
     E = E_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int L_ = nondet();
    int K_ = nondet();
    int H_ = nondet();
    int G_ = nondet();
    int M_ = 2;
    int J_ = 0;
    int I_ = 1;
    int E_ = F;
    int C_ = D;
    int B_ = 0;
    if (I == 1 && K_ >= 1 && L_ >= 5 && 7 >= K_ && 7 >= L_ && O >= 1 + D && O >= 1 && N >= 1 + F && N >= 1) {
     B = B_;
     C = C_;
     E = E_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int K_ = nondet();
    int G_ = nondet();
    int M_ = 2;
    int L_ = 4;
    int J_ = 0;
    int I_ = 1;
    int H_ = 4;
    int F_ = 1 + F;
    int E_ = 1 + F;
    int D_ = 1 + D;
    int C_ = 1 + D;
    int B_ = 0;
    if (K_ >= 1 && 7 >= K_ && O >= 1 && N >= 1 && O >= 2 + D && N >= 2 + F) {
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
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int L_ = nondet();
    int K_ = nondet();
    int H_ = nondet();
    int G_ = nondet();
    int M_ = 7;
    int J_ = 0;
    int E_ = F;
    int C_ = D;
    int B_ = 0;
    if (K_ >= 1 && L_ >= 1 && 3 >= L_ && 7 >= K_ && 7 >= L_ && D >= O && F >= N) {
     B = B_;
     C = C_;
     E = E_;
     G = G_;
     H = H_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     goto loc_f7;
    }
   }
   if (nondet_bool()) {
    int L_ = nondet();
    int K_ = nondet();
    int H_ = nondet();
    int G_ = nondet();
    int M_ = 7;
    int J_ = 0;
    int E_ = F;
    int C_ = D;
    int B_ = 0;
    if (K_ >= 1 && L_ >= 5 && 7 >= K_ && 7 >= L_ && D >= O && F >= N) {
     B = B_;
     C = C_;
     E = E_;
     G = G_;
     H = H_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     goto loc_f7;
    }
   }
   if (nondet_bool()) {
    int K_ = nondet();
    int G_ = nondet();
    int M_ = 7;
    int L_ = 4;
    int J_ = 0;
    int I_ = 1;
    int H_ = 4;
    int F_ = 1 + F;
    int E_ = 1 + F;
    int D_ = 1 + D;
    int C_ = 1 + D;
    int B_ = 0;
    if (K_ >= 1 && 7 >= K_ && 1 + D >= O && 1 + F >= N) {
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
     goto loc_f7;
    }
   }
   if (nondet_bool()) {
    int L_ = nondet();
    int K_ = nondet();
    int H_ = nondet();
    int G_ = nondet();
    int M_ = 7;
    int J_ = 1;
    int E_ = F;
    int C_ = D;
    int B_ = 1;
    if (K_ >= 1 && L_ >= 1 && 3 >= L_ && 7 >= K_ && 7 >= L_) {
     B = B_;
     C = C_;
     E = E_;
     G = G_;
     H = H_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     goto loc_f7;
    }
   }
   if (nondet_bool()) {
    int L_ = nondet();
    int K_ = nondet();
    int H_ = nondet();
    int G_ = nondet();
    int M_ = 7;
    int J_ = 1;
    int E_ = F;
    int C_ = D;
    int B_ = 1;
    if (K_ >= 1 && L_ >= 5 && 7 >= K_ && 7 >= L_) {
     B = B_;
     C = C_;
     E = E_;
     G = G_;
     H = H_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     goto loc_f7;
    }
   }
   if (nondet_bool()) {
    int K_ = nondet();
    int G_ = nondet();
    int M_ = 7;
    int L_ = 4;
    int J_ = 1;
    int I_ = 1;
    int H_ = 4;
    int F_ = 1 + F;
    int E_ = 1 + F;
    int D_ = 1 + D;
    int C_ = 1 + D;
    int B_ = 1;
    if (K_ >= 1 && 7 >= K_) {
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
     goto loc_f7;
    }
   }
  goto end;
 }
 loc_f6:
 {
 __cost++;
   if (nondet_bool()) {
    int K_ = nondet();
    int G_ = nondet();
    int M_ = 4;
    int L_ = 2;
    int J_ = 1;
    int I_ = 0;
    int H_ = 2;
    int E_ = F;
    int C_ = D;
    int B_ = 1;
    if (7 >= K_ && K_ >= 1) {
     B = B_;
     C = C_;
     E = E_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     goto loc_f4;
    }
   }
   if (nondet_bool()) {
    int K_ = nondet();
    int J_ = nondet();
    int G_ = nondet();
    int B_ = nondet();
    int M_ = 4;
    int L_ = 7;
    int I_ = 1;
    int H_ = 7;
    int E_ = F;
    int C_ = D;
    if (I == 1 && K_ >= 1 && J_ >= 0 && 1 >= J_ && 7 >= K_) {
     B = B_;
     C = C_;
     E = E_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     goto loc_f4;
    }
   }
  goto end;
 }
 loc_f7:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
