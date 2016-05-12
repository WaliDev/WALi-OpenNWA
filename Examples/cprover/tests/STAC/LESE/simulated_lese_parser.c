// This program is based on an example from
//
//   P. Saxena, P. Poosankam, S. McCamant, and D. Song,
//   Loop-extended symbolic execution on binary programs.
//   ISSTA 2009: 225-236
//
// which motived them to define loop-extended symbolic execution.
// The essence of their technique is to create a "stretchy"
// formula for the path condition to a point of interest.
// The path formula captures an arbitrary unspecified
// number of iterations for each loop by expressing the
// state transformation in terms of a "loop trip-count
// variable". In essence, the loop trip-count variable
// is an existentially quantified variable.
//
// 1  #define URI_DELIMITER ' '
// 2  #define VERSION_DELIMITER '\n'
// 3
// 4  void process_request(char * input)
// 5  {
// 6    char URI[80], version[80], msgbuf[100];
// 7    int ptr=0, uri_len=0, ver_len=0, i, j;
// 8
// 9    if (strncmp(input, "GET ", 4) != 0) {
// 10     fatal("Unsupported request");
//      }
// 11   ptr = 4;
// 12   while (input[ptr] != URI_DELIMITER) {
// 13     if (uri_len < 80) {
// 14       URI[uri_len] = input[ptr];
//        }
// 15     uri_len++; ptr++;
// 16   }
// 17   ptr ++;
// 18   while (input[ptr] != VERSION_DELIMITER) {
// 19     if (ver_len < 80) {
// 20       version[ver_len] = input[ptr];
//        }
// 21     ver_len++; ptr++;
// 22   }
// 23   if (ver_len < 8 || version[5] != '1') {
// 24     fatal("Unsupported protocol version");
//      }
// 25
// 26   for (i=0,ptr=0; i < uri_len; i++, ptr++) {
// 27     msgbuf[ptr] = URI[i];
//      }
// 28   msgbuf[ptr++] = ',';
// 29   for (j = 0; j < ver_len; j++, ptr++) {
// 30     msgbuf[ptr] = version[j];
//      }
// 31   msgbuf[ptr++] = '\0';
// 32   LogRequest(msgbuf);
// 33 }
//
// Figure 1: A simplified example from an HTTP server that handles
// requests of the form: "GET " URI " " Version "\n"


void simulated_parser(int N) {
  int i = 0;
  int ver_len = 0;

  // Comment out next line to simulate (2) the construction of an overapproximating path formula 
  __VERIFIER_assume(N == 20);  // Simulates (1) exploration with a concrete state in which N is 20

  while (i < N) {
    ver_len++; i++;
  }
  assert(ver_len >= 8);  // Should SUCCEED for exploration (1); should FAIL when path-formula (2) is used
}

int main(){

  simulated_parser(__VERIFIER_nondet_int());

  return(0);
}
