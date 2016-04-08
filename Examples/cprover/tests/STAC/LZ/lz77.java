
import java.io.*;

/* C4B_plugin_ex class */
class ex {
	public static byte[] compress(byte[] in) throws IOException {
    	StringBuffer mSearchBuffer = new StringBuffer(1024);
    
    	ByteArrayInputStream stream = new ByteArrayInputStream(in);
    	InputStreamReader reader = new InputStreamReader(stream);
    	Reader mIn = new BufferedReader(reader);
    
    	ByteArrayOutputStream oStream = new ByteArrayOutputStream();
    	OutputStreamWriter writer = new OutputStreamWriter(oStream);
    	PrintWriter mOut = new PrintWriter(new BufferedWriter(writer));
    
    	String currentMatch = "";
    	int matchIndex = 0;
    	int tempIndex = 0;
    	int nextChar;
    
    	while ((nextChar = mIn.read()) != -1) {
      		tempIndex = mSearchBuffer.indexOf(currentMatch + (char)nextChar);
        
      		if (tempIndex != -1) {
        		currentMatch = currentMatch + (char)nextChar;
        		matchIndex = tempIndex;
      		}
      		else {
        		String codedString = "~" + matchIndex + "~" + currentMatch.length() + "~" + (char)nextChar;
        
        		String concat = currentMatch + (char)nextChar;
        		if (codedString.length() <= concat.length()) {
          			mOut.print(codedString);
          			mSearchBuffer.append(concat);
          			currentMatch = "";
          			matchIndex = 0;
        		}
        		else {
          			currentMatch = concat;matchIndex = -1;
          			while ((currentMatch.length() > 1) && (matchIndex == -1)) {
            			mOut.print(currentMatch.charAt(0));
            			mSearchBuffer.append(currentMatch.charAt(0));
            			currentMatch = currentMatch.substring(1, currentMatch.length());
            			matchIndex = mSearchBuffer.indexOf(currentMatch);
          			}
        		}
        		if (mSearchBuffer.length() > 1024) {
          			mSearchBuffer = mSearchBuffer.delete(0, mSearchBuffer.length() - 1024);
        		}
      		}
        }
        if (matchIndex != -1) {
            String codedString = "~" + matchIndex + "~" + currentMatch.length();
            if (codedString.length() <= currentMatch.length()) {
                mOut.print("~" + matchIndex + "~" + currentMatch.length());
            } else {
                mOut.print(currentMatch);
      		}
    	}
    	mIn.close();
    	mOut.flush();
    
   	 	byte[] bytes = oStream.toByteArray();
    
    	mOut.close();
    
    	return bytes;
	}
    
    public static void main(String[] args) throws IOException {
		File file = new File("input.txt");
        FileInputStream fin = null;
        try {
            // create FileInputStream object
            fin = new FileInputStream(file);
            byte[] fileContent = new byte[(int)file.length()];
            fin.read(fileContent);
            
            try {
            	// compress
            	byte[] compressedfile = compress(fileContent);
            	//create string from byte array
            	String s = new String(compressedfile);
            	System.out.println("Compressed file content: " + s);
            }
            catch (IOException ioe) {
            	System.out.println("Exception while compressing file " + ioe);
        	}
            
        }
        catch (FileNotFoundException e) {
            System.out.println("File not found" + e);
        }
        
        finally {
            // close the streams using close method
            try {
                if (fin != null) {
                    fin.close();
                }
            }
            catch (IOException ioe) {
                System.out.println("Error while closing stream: " + ioe);
            }
        }
	}
}