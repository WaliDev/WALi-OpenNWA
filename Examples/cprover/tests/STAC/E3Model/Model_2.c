//package com.ainfosec.MalwareAnalyzer;

//import java.util.List;
//import java.util.Set;
//import java.util.Iterator;
//import java.util.NavigableSet;
//import java.util.Arrays;
//import java.io.Writer;
//import java.io.PrintWriter;
//import java.io.StringWriter;
//import java.io.UnsupportedEncodingException;
//import java.net.URLDecoder;
//import java.util.Map;
//import java.util.HashMap;
//import java.security.NoSuchAlgorithmException;
//import java.io.InputStream;
//import java.security.MessageDigest;
//import java.io.ByteArrayInputStream;
//import java.io.IOException;
//import fi.iki.elonen.NanoHTTPD;

//public class Server extends NanoHTTPD
////public class ModelOfServer
////{
	//
	// I use the prefix "M_" for variables that are part of the model,
	//   not the original program, i.e. abstractions of the original
	//   program's behavior.
	//
	
    int M_responseLength;

    //private static String response;
    //private static String M_Server_response;
    
    //public Server(final int p) throws IOException {
    //public ModelOfServer() {
    //    //super(p);
    //    //this.start(5000, false);
    //    //System.out.println("Server started, running on port " + p);
    //    //M_Server_response = "";
    //    M_responseLength = 0;
    //}
    
    //public static String featurize(final byte[] input) {
    //public static int featurize(final byte[] M_input) { // Model returns (upper-bound on) length of string output
        /*
        final int[] counts = new int[256];
        for (final byte b : input) {
            final int unsigned = b & 0xFF;
            final int[] array = counts;
            final int n = unsigned;
            ++array[n];
        }
        String fv_csv = "";
        Boolean first = true;
        for (final int c : counts) {
            if (!first) {
                fv_csv += ",";
            }
            fv_csv += c;
            first = false;
        }
        return fv_csv;
        */
    	//int M_maxStringLengthOfInput = 20;
        //return (M_maxStringLengthOfInput + 1) * 256;
    //}
    
    /*
    private static byte[] createChecksum(final byte[] input) throws NoSuchAlgorithmException, IOException {
        final InputStream fis = new ByteArrayInputStream(input);
        final byte[] buffer = new byte[1024];
        final MessageDigest complete = MessageDigest.getInstance("MD5");
        int numRead;
        do {
            numRead = fis.read(buffer);
            if (numRead > 0) {
                complete.update(buffer, 0, numRead);
            }
        } while (numRead != -1);
        fis.close();
        return complete.digest();
    }
    */
    
    /*
    public static String getMD5Checksum(final byte[] input) throws NoSuchAlgorithmException, IOException {
        final byte[] b = createChecksum(input);
        String result = "";
        for (int i = 0; i < b.length; ++i) {
            result += Integer.toString((b[i] & 0xFF) + 256, 16).substring(1);
        }
        return result;
    }
    */
    
    //int M_nd1, M_nd2;
    //private static boolean nondet() { 
    //	  return M_nd1 % M_nd2 == 0; // ICRA should interpret this as a nondeterministic boolean
    //}
#define nondet __VERIFIER_nondet_int
    
    //public NanoHTTPD.Response serve(final NanoHTTPD.IHTTPSession session) {
    void serve(int M_total_input_size) {
        M_responseLength = 0;
    	//M_nd1 = M_nd1_; M_nd2 = M_nd2_;

        //Logger.initialize("response.log");
        //final Map<String, String> files = new HashMap<String, String>();
        //final NanoHTTPD.Method method = session.getMethod();
        //if (NanoHTTPD.Method.POST.equals((Object)method)) {
    	if ( nondet() ) {
        //if (M_method == "POST") {
        	/*
            try {
                session.parseBody((Map)files);
            }
            catch (IOException ioe) {
                return newFixedLengthResponse((NanoHTTPD.Response.IStatus)NanoHTTPD.Response.Status.INTERNAL_ERROR, "text/plain", "SERVER INTERNAL ERROR: IOException: " + ioe.getMessage());
            }
            catch (NanoHTTPD.ResponseException re) {
                return newFixedLengthResponse((NanoHTTPD.Response.IStatus)NanoHTTPD.Response.Status.INTERNAL_ERROR, "text/plain", re.getMessage());
            }
            */
        	/*
            String body = session.getQueryParameterString();
            try {
                body = URLDecoder.decode(body, "UTF-8");
            }
            catch (UnsupportedEncodingException e2) {
                return newFixedLengthResponse((NanoHTTPD.Response.IStatus)NanoHTTPD.Response.Status.INTERNAL_ERROR, "text/plain", "COULD NOT DECODE MESSAGE\n");
            }
            */
            //final String[] lines = body.split("\\r?\\n");
            //final String s2 = lines[0];
            //switch (s2) {
    		//if (nondet()) {
                //case "query": {
    			if (nondet()) {
                //if (lines.length != 2) {
    			    if (nondet()) {
                        //M_Server_response = "Could not parse input request\n";
                        M_responseLength = 30;
                        return; // FIXME
                        //break;
                    }
                    //final String query_sample_md5 = lines[1];
                    //final Sample query_sample = Database.get_sample_by_md5(query_sample_md5);
                    //if (query_sample == null) {
                    if (nondet()) {
                        //M_Server_response = "The supplied MD5 does not correspond to any sample in the database.\n";
                    	M_responseLength = 68;
                        return; // FIXME
                        //break;
                    }
                    //final NavigableSet<ComparisonResult> results = Database.compute_all_similarities(query_sample_md5);
                    //M_Server_response = "Querying " + query_sample_md5 + " " + (query_sample.is_packed() ? "(Packed)" : "(Not Packed)") + "\n";
                    //M_responseLength = 9 + 32 + (nondet() ? 8 : 12) + 1;
                    M_responseLength = 9 + 32 + 12 + 1;
                    //M_Server_response += "Top Five Most Similar:\n";
                    M_responseLength += 23;
                    //int ii = 0;
                    int M_ii = 4;
                    //String feature_vector_output = "";
                    int M_feature_vector_string_length = 0;
                    //for (final ComparisonResult cr : results.descendingSet()) {
                    //while (nondet()) {
                    for(int M_ii = 0; M_ii < 4; M_ii++) {
                        ////////////if (nondet()) { break; }
                        //if (cr.get_md5().equals(query_sample_md5)) {
                    	//if (nondet()) {
                        //    continue;
                        //}
                        //if (ii > 4) {
                        //    break;
                        //}
                        //final Sample s = Database.get_sample_by_md5(cr.get_md5());
                        //final String packed_status = s.is_packed() ? "(Packed)" : "(Not Packed)";
                        //M_Server_response += String.format("%s - Score: %.3f %s\n", cr.get_md5(), cr.get_score(), packed_status);
                        //M_responseLength += 12 + 32 + (24) + (nondet() ? 8 : 12);
                        //M_responseLength += 1; ///////////////////////////////// FIXME FIXME FIXME FIXME
                        //M_responseLength += (12 + 32 + 24 + 12);
                        M_responseLength += 1;
                        //M_responseLength += 80;
                        //feature_vector_output += String.format("%s = {%s}\n", cr.get_md5(), s.serialize_fv());
                        ///////////////////M_feature_vector_string_length += 32 + 4 + (256*20) + 2;
                        //break; // FIXME FIXME XXX FIXME FIXME
                    }
                    //M_Server_response = M_Server_response + "\n" + feature_vector_output;
                    ////////////////////M_responseLength += 1 + M_feature_vector_string_length;
                    //break;
                }
    		    
                //case "add": {
    		    else if (nondet()) {
                    //if (lines.length != 3) {
    		    	if (nondet()) {
                    	M_responseLength += 72;
                        return;
                        //M_Server_response = "Could not parse input request\nWrong number of lines for an add request.\n";
                        //break;
                    }
                    //final String new_sample_md5 = lines[1];
                    //if (new_sample_md5.length() != 32) {
                    if (nondet()) {
                    	M_responseLength += 39;
                        return;
                        //M_Server_response = "Could not parse input request\nBad MD5.\n";
                        //break;
                    }
                    //if (Database.is_unknown(new_sample_md5)) {
                    if (nondet()) {
                        //try {
                    	if (nondet()) {
                            //Database.see(new_sample_md5);
                            //final Sample new_sample = new Sample(lines[2]);
                            //Database.add_sample(new_sample_md5, new_sample);
                        //}
                        //catch (Exception e) {
                    	} else {
                            //if (e instanceof NumberFormatException) {
                        	if (nondet()) {                    		
                                //M_Server_response = "Could not parse input request\nUnparseable data in supplied feature vector\n";
                                M_responseLength += 74;
                                //break;
                            }
                            //if (e instanceof IllegalArgumentException) {
                        	else if (nondet()) {
                                //M_Server_response = "Could not parse input request\n" + e.toString() + "\n";
                                M_responseLength += 30 +  + 1;
                                //break;
                            }
                            //final StringWriter sw = new StringWriter();
                            //final PrintWriter pw = new PrintWriter(sw);
                            //e.printStackTrace(pw);
                            //M_Server_response = M_Server_response + "Could not parse input request\n" + sw.toString() + "\n";
                            M_responseLength += 1024; // FIXME assumption about maximum length of stack trace
                            //break;
                        }
                        //M_Server_response = "OK\n";
                        M_responseLength = 3;
                        //break;
                    }
                    //M_Server_response = "Already known\n";
                    M_responseLength = 14;
                    //break;
                }
                //case "add_dasm": {
    		    else if (nondet()) {
                    //if (lines.length <= 3) {
                	if (nondet()) {
                        //M_Server_response = "Could not parse input request\nNot enough lines for an add_dasm request.\n";
                        M_responseLength = 72;
                        return;
                        //break;
                    }
                    //final String new_dasm_md5 = lines[1];
                    //if (new_dasm_md5.length() != 32) {
                   	if (nondet()) {
                        //M_Server_response = "Could not parse input request\nBad MD5.\n";
                        M_responseLength = 39;
                        return;
                        //break;
                    }
                    //if (DasmDatabase.get_dasm(new_dasm_md5) != null) {
                   	if (nondet()) {
                        //M_Server_response = "Already known\n";
                        M_responseLength = 14;
                        return;
                        //break;
                    }
                    int start_of_disassembly = -1;
                    for (int jj = 2; jj <= 5; ++jj) {
                        //if (lines[jj].contains("pei-i386")) {
                        if (nondet()) {
                            start_of_disassembly = jj;
                        }
                    }
                    //if (start_of_disassembly != -1) {
                   	if (nondet()) {
                        //final String[] dasm_lines = Arrays.copyOfRange(lines, start_of_disassembly, lines.length);
                        //final Dasm new_dasm = DasmHelpers.build_dasm(dasm_lines);
                        //DasmDatabase.add_dasm(new_dasm_md5, new_dasm);
                        //M_Server_response = "OK\n";
                        M_responseLength = 3;
                        return;
                        //break;
                    }
                    //M_Server_response = "Could not parse input request\nDoes not appear to be an objdump disassembly of a PE32.\nUse \"objdump -d <input_exe>\"";
                    M_responseLength = 114;
                    //break;
                }
                //case "list_dasms": {
                else if (nondet()) {
                    //if (lines.length != 1) {
                   	if (nondet()) {
                        //M_Server_response = "Malformed list_dasms request\n";
                        M_responseLength = 29;
                        return;
                        //break;
                    }
                    //final Set<String> md5s = DasmDatabase.get_all_dasm_md5s();
                    //M_Server_response = "";
                   	M_responseLength = 0;
                   	// FIXME Here is a LOOP
                    // for (final String md5 : md5s) {
                   	while(M_total_input_size > 0) { M_total_input_size -= 32;
                        //M_Server_response = M_Server_response + md5 + "\n";
                       	M_responseLength += 32 + 1;
                    }
                    //break;
                }
                //case "get_function_entrypoints": {
                else if (nondet()) {
                    //if (lines.length != 2) {
                	if (nondet()) {
                        //M_Server_response = "Malformed get_function_entrypoints request\n";
                        M_responseLength = 43;
                        return;
                        //break;
                    }
                    //final String desired_md5 = lines[1];
                    //if (desired_md5.length() != 32) {
                    if (nondet()) {
                        //M_Server_response = "Malformed MD5\n";
                        M_responseLength = 14;
                        return;
                        //break;
                    }
                    //final Dasm dasm = DasmDatabase.get_dasm(desired_md5);
                    //if (dasm == null) {
                    if (nondet()) {
                        //M_Server_response = "Unknown MD5\n";
                        M_responseLength = 12;
                        return;
                        //break;
                    }
                    //final Set<CFG> cfgs = dasm.getCfgs();
                    //M_Server_response = "";
                    M_responseLength = 0;
                    //for (final CFG cfg : cfgs) {
                    while(M_total_input_size > 0) { M_total_input_size--;
                        // FIXME here is a LOOP
                        //M_Server_response = M_Server_response + "0x" + Integer.toHexString(cfg.getEntrypoint()) + "\n";
                        M_responseLength += 2 + 16 + 1;
                    }
                    //break;
                }
                //case "get_cfg": {
    	        else if (nondet()) {
    	        	//if (lines.length != 3) {
    	        	if (nondet()) {
                        //M_Server_response = "Malformed get_cfg request\n";
                        M_responseLength = 26;
                        return;
                        //break;
                    }
                    //final String cfg_md5 = lines[1];
                    //if (cfg_md5.length() != 32) {
                    if (nondet()) {
                        //M_Server_response = "Malformed MD5\n";
                        M_responseLength = 14;
                        return;
                        //break;
                    }
                    //Integer ep;
                    //try {
                    //    ep = Integer.decode(lines[2]);
                    //}
                    //catch (NumberFormatException e3) {
                    if (nondet()) {
                        //M_Server_response = "Could not decode desired function entry point.\n";
                        M_responseLength = 47;
                        return;
                        //break;
                    }
                    //final Dasm cfg_dasm = DasmDatabase.get_dasm(cfg_md5);
                    //final CFG cfg2 = cfg_dasm.getCFG(ep);
                    //if (cfg2 == null) {
                    if (nondet()) {
                        //M_Server_response = "Requested function does not exist.\n";
                        M_responseLength = 35;
                        return;
                        //break;
                    }
                    //M_Server_response = "";
                    M_responseLength = 0;
                    //for (final BasicBlock block : cfg2.get_all_blocks()) {
                    //    final List<Map.Entry<Integer, String>> instructions = block.getInstructions();
                    //    for (final Map.Entry<Integer, String> instr : instructions) {
                    while(M_total_input_size > 0) { M_total_input_size -= 5;
                        while(M_total_input_size > 0 && nondet()) { // FIXME??
                        	M_total_input_size -= 5;
                            // FIXME FIXME FIXME IN PROGRESS...
                            // FIXME here is a LOOP
                            //final int va = instr.getKey();
                            //final String op = instr.getValue();
                            //M_Server_response = M_Server_response + "0x" + Integer.toHexString(va) + " : " + op + "\n";
                            M_responseLength += 2 + 16 + 3 + 3000 + 1;
                        }
                        //M_Server_response += "SUCCESSORS: ";
                        M_responseLength += 12;
                        //for (final Integer succ : block.getSuccessors()) {
                        int M_nSuccessors = 4; //FIXME assumption about max number of successors
                        while(M_nSuccessors > 0) { M_nSuccessors--;
                            // FIXME here is a LOOP
                            //M_Server_response = M_Server_response + "0x" + Integer.toHexString(succ) + ",";
                            M_responseLength += 2 + 16 + 1;
                        }
                        //M_Server_response += "\n-----\n";
                        M_responseLength += 7;
                    }
                    //break;
                }

                // print_hull of the part up to here takes 1 minute 36 sec

                //case "query_dasms": {
                else if (nondet()) {
                    //if (lines.length != 2) {
                    if (nondet()) {
                        //M_Server_response = "Malformed query_dasms request\n";
                        M_responseLength = 30;
                        return;
                        //break;
                    }
                    //final String query_md5 = lines[1];
                    //if (query_md5.length() != 32) {
                    if (nondet()) {
                        //M_Server_response = "Malformed MD5";
                    	M_responseLength = 13;
                        return;
                        //break;
                    }
                    //final Dasm query_dasm = DasmDatabase.get_dasm(query_md5);
                    //if (query_dasm == null) {
                    if (nondet()) {
                        //M_Server_response = "Desired disassembly does not exist.";
                        M_responseLength = 35;
                        return;
                        //break;
                    }
                    //final NavigableSet<ComparisonResult> dasm_results = DasmDatabase.compute_all_similarities(query_md5);
                    //M_Server_response = "Querying " + query_md5 + "\n";
                    M_responseLength = 9 + 32 + 1;
                    //M_Server_response += "Top Five Most Similar:\n";
                    M_responseLength += 23;
                    int kk = 0;
                    //for (final ComparisonResult cr2 : dasm_results.descendingSet()) {
                    while(1) {
                        // FIXME here is a LOOP
                    	if (kk > 4) {
                            break;
                        }
                        ++kk;
                        //M_Server_response += String.format("%s - Score: %.3f\n", cr2.get_md5(), cr2.get_score());
                        M_responseLength += 11 + 32 + (24);
                    }
                    //M_Server_response += "\n";
                    M_responseLength += 1;
                    //break;
                } else {
                    //M_Server_response = "Could not parse input request\n";
                    M_responseLength = 30;
                    //break;
                }
            //} // end of switch (now an if..else if... ...else)
            
                // print_hull of the part up to here takes 4 minute 49 sec
        }
        //////// //else if (NanoHTTPD.Method.PUT.equals((Object)method)) {
        //////// //else if (M_method == "PUT") {
        //////// else if (nondet()) {
        ////////     //final Integer contentLength = Integer.parseInt(session.getHeaders().get("content-length"));
        ////////     //final byte[] buffer = new byte[(int)contentLength];
        ////////     //String new_sample_md6;
        ////////     //String fv_csv;
        ////////     //try {
        ////////     //    session.getInputStream().read(buffer, 0, contentLength);
        ////////     //    new_sample_md6 = getMD5Checksum(buffer);
        ////////     //    fv_csv = featurize(buffer);
        ////////     //}
        ////////     //catch (Exception e4) {
        ////////     //    return newFixedLengthResponse((NanoHTTPD.Response.IStatus)NanoHTTPD.Response.Status.INTERNAL_ERROR, "text/plain", "Error handling HTTP request.\n");
        ////////     //}
        //////// 	if (nondet()) {
        //////// 		return;
        //////// 	}
        ////////     //if (Database.is_unknown(new_sample_md6)) {
        ////////     if (nondet()) {
        ////////     	
        ////////         //try {
        ////////         //    Database.see(new_sample_md6);
        ////////         //    final Sample new_sample2 = new Sample(fv_csv);
        ////////         //    Database.add_sample(new_sample_md6, new_sample2);
        ////////         //}
        ////////         //catch (Exception e4) {
        ////////       	if (nondet()) {
        ////////             //M_Server_response = "Error, malformed binary input.";
        ////////             M_responseLength = 30;
        ////////         }
        ////////         //M_Server_response = "OK\n";
        ////////         M_responseLength = 3;
        ////////     }
        ////////     else {
        ////////         //M_Server_response = "Already known\n";
        ////////         M_responseLength = 14;
        ////////     }
        //////// }
        //////// else {
        ////////     //M_Server_response = "Unsupported HTTP request type.\n";
        ////////     M_responseLength = 31;
        //////// }
        //Logger.write_log_entry(M_Server_response);
        //Logger.close_log();
        //return newFixedLengthResponse((NanoHTTPD.Response.IStatus)NanoHTTPD.Response.Status.OK, "text/plain", M_Server_response.substring(0, Math.min(M_Server_response.length(), 16000)));
        return; // print_hull of the part up to here takes 15 minute 31 sec
    }
    void main(int M_total_input_size) {
        serve(M_total_input_size);
        __VERIFIER_print_hull(M_responseLength);
    }
//}
