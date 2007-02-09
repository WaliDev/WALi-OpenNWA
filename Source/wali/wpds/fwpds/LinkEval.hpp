#ifndef __LINK_EVAL_H_
#define __LINK_EVAL_H_

class LinkEval {
 private:
  int n;
  int *ancestor;
  int *label;
  int *semi;
 public:
  LinkEval(int _n, int *s);
  ~LinkEval();
  void link(int,int);
  int eval(int);
 private:
  void compress(int);
};

#endif
