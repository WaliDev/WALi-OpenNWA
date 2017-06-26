int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A) {
 goto loc_evaleasy2start;

 loc_evaleasy2bb1in:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 1) {
     goto loc_evaleasy2bbin;
    }
   }
   if (nondet_bool()) {
    if (0 >= A) {
     goto loc_evaleasy2returnin;
    }
   }
  goto end;
 }
 loc_evaleasy2bbin:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = -1 + A;
    if (1 >= 0) {
     A = A_;
     goto loc_evaleasy2bb1in;
    }
   }
  goto end;
 }
 loc_evaleasy2entryin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evaleasy2bb1in;
    }
   }
  goto end;
 }
 loc_evaleasy2returnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evaleasy2stop;
    }
   }
  goto end;
 }
 loc_evaleasy2start:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evaleasy2entryin;
    }
   }
  goto end;
 }
 loc_evaleasy2stop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet());
}
