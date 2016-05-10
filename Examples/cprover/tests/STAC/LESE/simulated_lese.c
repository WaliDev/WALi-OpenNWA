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

#define false 0
#define true 1

// Uncomment the definition below to test compilability with gcc */
// #define __VERIFIER_assume(x)    /* Omit x */

typedef unsigned int size_t;

// Taken from http://opensource.apple.com//source/Libc/Libc-167/gen.subproj/ppc.subproj/strncmp.c
int
strncmp(const char *s1, const char *s2, size_t n)
{
    for ( ; n > 0; s1++, s2++, --n)
	if (*s1 != *s2)
	    return ((*(unsigned char *)s1 < *(unsigned char *)s2) ? -1 : +1);
	else if (*s1 == '\0')
	    return 0;
    return 0;
}

void fatal(char *str) {
  __VERIFIER_assume(false);
}

void LogRequest(char *str) {
  return;
}

#define URI_DELIMITER ' '
#define VERSION_DELIMITER '\n'

void process_request(char * input)
{
  char URI[80], version[80], msgbuf[100];
  int ptr=0, uri_len=0, ver_len=0, i, j;

  if (strncmp(input, "GET ", 4) != 0) {
    fatal("Unsupported request");
  }
  ptr = 4;
  while (input[ptr] != URI_DELIMITER) {
    if (uri_len < 80) {
      URI[uri_len] = input[ptr];
    }
    uri_len++; ptr++;
  }
  ptr ++;
  while (input[ptr] != VERSION_DELIMITER) {
    if (ver_len < 80) {
      version[ver_len] = input[ptr];
    }
    ver_len++; ptr++;
  }
  if (ver_len < 8 || version[5] != '1') {
    fatal("Unsupported protocol version");
  }

  for (i=0,ptr=0; i < uri_len; i++, ptr++) {
    msgbuf[ptr] = URI[i];
  }
  msgbuf[ptr++] = ',';
  for (j = 0; j < ver_len; j++, ptr++) {
    msgbuf[ptr] = version[j];
  }
  msgbuf[ptr++] = '\0';
  LogRequest(msgbuf);
}

int main() {

  process_request("Now is the time for all good men to come to the aid of the party\n");
  return 0;
}
