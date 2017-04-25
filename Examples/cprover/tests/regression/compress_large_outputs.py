#!/usr/bin/python

import os.path, sys, zipfile

def usage() :
    print "USAGE: compress_large_outputs.py <base_directory>"
    print "  where <base_directory> is a directory containing some HTML files"
    print "  and a subdirectory named \"outputs\" that contains some "
    print "  \".out\" files that are referred to by those HTML files."
    sys.exit(0)

# Here is the template for the PHP files we'll create:
TEMPLATE="""
<?
// NOTE: If you are reading this file and you expected to
//   see some ICRA output instead, it is because the ICRA
//   output was too large and needed to be compressed; 
//   the ICRA output was replaced with a zipped version
//   and a PHP script that uncompresses the zipped version.
//   Apparently, though, you are seeing the source code for
//   the PHP version instead of running the PHP code.
//
//   To obtain the original output file, you can download
//   the zipped file and uncompress it manually.
//
//   The zipped output file has the same name as this
//   script, except with ".php" replaced with ".zip".
//
//   For more info, contact <jbreck@cs.wisc.edu>

$filename = "%s";

header("Content-Type: text/plain");

set_time_limit(300); // One really hopes it doesn't take that long

function not_eof($str) {
  if ($str === "0") { return TRUE; } // <s> great idea, PHP designers! </s>
  if ($str) { return TRUE; }
  return FALSE;
}

$zipfilename = $filename . ".zip";

$zip = zip_open($zipfilename);

if ($zip) {
  while ($zip_entry = zip_read($zip)) {
    if (zip_entry_open($zip, $zip_entry)) {
      while(not_eof($contents = zip_entry_read($zip_entry))) {
        echo "$contents";
      }
      zip_entry_close($zip_entry);
    }
    if (zip_entry_name($zip_entry) == $filename) { break; }
  }
  zip_close($zip);
} else {
  echo "ERROR while attempting to uncompress $zipfilename.";
}
?>
"""

if len(sys.argv) != 2 : usage()

base = sys.argv[1]

outputs = os.path.join(base, "outputs")

triples = list() # ( .out file, .zip file, .php file )

for filename in os.listdir(outputs) :
    fullPath = os.path.join(outputs, filename)
    if filename.startswith("__") : 
        # Don't compress __result.out and similar files; those are
        #   read by scripts, not by humans, so they should never
        #   be compressed.
        continue 
    if filename.endswith(".out.zip") :
        if os.path.exists(fullPath[:-4]) : continue # recompress if possible
        # Idea: We already zipped this file, and deleted its ".out",
        #   during some earlier run of this script; yet, we still may need to
        #   update some HTML files that refer to it.
        outfile = filename[:-4]
        #skip = False
        #for o, z, p in triples :
        #    if outfile == o : skip = True
        #if skip : continue
        triples.append((outfile, outfile + ".zip", outfile + ".php"))
        print "Saw already-compressed file: " + outfile
        continue
    if not filename.endswith(".out") : continue
    if os.path.getsize(fullPath) < 1024 * 1024 : continue
    print "Compressing: " + filename
    phpFilename = filename + ".php"
    with open(os.path.join(outputs,phpFilename), 'w') as F :
        F.write(TEMPLATE % filename)
    zipFilename = filename + ".zip"
    Z = zipfile.ZipFile(os.path.join(outputs,zipFilename), 'w', zipfile.ZIP_DEFLATED)
    Z.write(fullPath, os.path.join("outputs",filename))
    Z.close()
    os.remove(fullPath)
    triples.append((filename, zipFilename, phpFilename))

print "Done compressing files."
print "Now, I will replace references to the following files: "
for o,z,p in triples : print "  ",o

for filename in os.listdir(base) :
    fullPath = os.path.join(base, filename)
    if not fullPath.endswith(".html") : continue
    print "Processing HTML file: " + filename
    with open(fullPath, 'r') as F : contents = F.read() # slurp file to memory
    for outFile, zipFile, phpFile in triples :
        outputsOutFile = "\"" + os.path.join("outputs",outFile) + "\""
        outputsPhpFile = "\"" + os.path.join("outputs",phpFile) + "\""
        contents = contents.replace(outputsOutFile, outputsPhpFile)
    with open(fullPath, 'w') as F : F.write(contents)

