int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C) {
 goto loc_evalexministart;

 loc_evalexminibb1in:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= C && 100 >= B) {
     goto loc_evalexminibbin;
    }
   }
   if (nondet_bool()) {
    if (B >= 101) {
     goto loc_evalexminireturnin;
    }
   }
   if (nondet_bool()) {
    if (C >= 1 + A) {
     goto loc_evalexminireturnin;
    }
   }
  goto end;
 }
 loc_evalexminibbin:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 1 + B;
    int B_ = C;
    int A_ = -1 + A;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     goto loc_evalexminibb1in;
    }
   }
  goto end;
 }
 loc_evalexminientryin:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = A;
    int A_ = B;
    if (1 >= 0) {
     A = A_;
     B = B_;
     goto loc_evalexminibb1in;
    }
   }
  goto end;
 }
 loc_evalexminireturnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalexministop;
    }
   }
  goto end;
 }
 loc_evalexministart:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalexminientryin;
    }
   }
  goto end;
 }
 loc_evalexministop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet());
}
