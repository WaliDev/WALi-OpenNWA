int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A) {
 goto loc_evalndecrstart;

 loc_evalndecrbb1in:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 2) {
     goto loc_evalndecrbbin;
    }
   }
   if (nondet_bool()) {
    if (1 >= A) {
     goto loc_evalndecrreturnin;
    }
   }
  goto end;
 }
 loc_evalndecrbbin:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = -1 + A;
    if (1 >= 0) {
     A = A_;
     goto loc_evalndecrbb1in;
    }
   }
  goto end;
 }
 loc_evalndecrentryin:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = -1 + A;
    if (1 >= 0) {
     A = A_;
     goto loc_evalndecrbb1in;
    }
   }
  goto end;
 }
 loc_evalndecrreturnin:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalndecrstop;
    }
   }
  goto end;
 }
 loc_evalndecrstart:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_evalndecrentryin;
    }
   }
  goto end;
 }
 loc_evalndecrstop:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet());
}
