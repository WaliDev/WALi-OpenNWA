lz77.java is a Java implementation of LZ77 text compression.  It was obtained by
starting with the DARPA-provided bytecode and decompiling.  (A few comments were
added by hand.)

lz77.c is a *model* of lz77.java in which int-valued variables are used to model
the string-valued quantities:

+ In general, we model the input stream by its number of characters.
+ Model search buffer by its size.
+ Model match substring read from the input stream with the current search buffer by its length.
+ Appending a string str1 to another string str2 is modeled by increasing the length of str2 with the length of str1. Trimming a string is modeled in a similar way.
+ Output characters are modeled by increasing the size of the output (m) by the number of characters.
+ str1.indexOf(str2) is O(str1 length).
+ Appending and trimming are O(1).

matchIndex = nondet(searchBuffer,currentMatch) models the indexOf
function of the StringBuffer class in Java.
Here, it is assumed that the complexity of indexOf is O(m), where m is the size of
searchBuffer. nondet(searchBuffer,currentMatch) only models the
functionality of the indexOf function, meaning return 0 (matched) or 1
(matched):

+ It generates a random number in the range [0 .. searchBuffer]
+ If this random number is bigger than currentMatch + 1, return 1
+ Otherwise return 0

Hence, matchIndex is 0 or 1.

The complexity of indexOf is defined by:
// tick(searchBuffer);
tempBuffer = searchBuffer;
while (tempBuffer > 0) {
  tempBuffer = tempBuffer - 1;
  // tick(1);
}
