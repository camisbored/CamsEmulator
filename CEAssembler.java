
import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map.Entry;

public class CEAssembler {
	
    public static void main(String[] args) throws IOException{
    	File inFile = null;
    	File outFile = new File("a.ce");
        if (args.length == 1 || args.length == 2 ) {
           inFile = new File(args[0]);
           if (args.length == 2) {
        	   outFile = new File(args[1]);
           }
        } else {
           System.err.println("Invalid usage: usage- <inputFile> <outputFile>");
           System.exit(-1);
        }
        String content = new String(Files.readAllBytes(Paths.get(inFile.getAbsolutePath())));
        byte[] outputFile = new byte[256];
        int index = 0;
        List<String> lines = Arrays.asList(content.split("\n"));
        
        int lineNumber = 1;
        HashMap<Integer, String> lineMap = new HashMap<Integer, String>();
        for (String line : lines) {
        	lineMap.put(lineNumber++, line);
        }
        
        HashMap<String, Integer> loopLabels = new HashMap<String, Integer>();
        HashMap<Integer, String> jmpHitMap = new HashMap<Integer, String>();
        List<Integer> removalKeys = new ArrayList<Integer>();
        for (Entry<Integer, String> entry : lineMap.entrySet()) {
        	entry.setValue(entry.getValue().split(";")[0]);
        	entry.setValue(entry.getValue().replaceAll("\\s+", ""));
        	entry.setValue(entry.getValue().toLowerCase());
        	if (entry.getValue().isEmpty())
        		removalKeys.add(entry.getKey());
        	if (entry.getValue().endsWith(":")) {
        		String labelName = entry.getValue().split(":")[0];
        		loopLabels.put(labelName, -1);
        		entry.setValue(labelName);
        	}

        }
        removalKeys.forEach(lineMap::remove);
       
        for (Entry<Integer, String> entry : lineMap.entrySet()) {
        	String operation = entry.getValue();
        	String[] operands = null;
        	if (loopLabels.containsKey(operation)) {
        		loopLabels.put(operation, index);
        		continue;
        	}
        	if (operation.contains(",")) {
        		operands = operation.split(",");
        		operation = operands[0];
        	}
        	
        	boolean isJmp = false;
        	for (String instruc : jmpInstructions) {
        		if (operation.startsWith(instruc)) {
        			String[] jmpData = operation.split(instruc);
        			String labelName = jmpData[1];
        			operation = instruc;
                	byte[] opData = opCodeMap.get(operation);
                	outputFile[index++] = new Byte(opData[0]).byteValue();
                	jmpHitMap.put(index, labelName);
                	outputFile[index++] = 0x00;
        			isJmp = true;
        			break;
        		}
        	}
        	if (isJmp) {
        		continue;
        	}
        	
        	byte[] opData = opCodeMap.get(operation);
        	if (opData == null) {
        		throw new IllegalArgumentException("Unknown instruction on line " + entry.getKey() +": "+ entry.getValue());
        	}
        	outputFile[index++] = new Byte(opData[0]).byteValue();
        	byte numOperands = opData[1];
        	if ((operands != null && numOperands == 0) || numOperands > 0 && operands == null) {
        		throw new IllegalArgumentException("Incorrect number of parameters provided on line " + entry.getKey() +": "+ entry.getValue() + " takes " + numOperands + " parameters");
        	}

        	if (operands != null && numOperands > 0) {
            	if (operands.length-1 != numOperands) {
            		throw new IllegalArgumentException("Incorrect number of parameters provided on line " + entry.getKey() +": "+ entry.getValue() + " takes " + numOperands + " parameters");
            	}
            	for (int i=1;i<operands.length;i++) {
            		outputFile[index++] = (byte) (Integer.parseInt(operands[i]) & 0xFF);
            	}
        	}
        }

        //replace loop variables
        for (Entry<Integer, String> entry : jmpHitMap.entrySet()) {
        	Integer value = loopLabels.get(entry.getValue());
        	outputFile[entry.getKey()] = (byte) (value.byteValue() & 0xFF);
        }
        
        byte[] trimmedOutput = new byte[index];
        System.arraycopy(outputFile, 0, trimmedOutput, 0, index);
        
        Files.write(Paths.get(outFile.getAbsolutePath()), trimmedOutput);
        System.out.println("Output wrote successfully to : "+ outFile.getAbsolutePath()+ ", size: " +index +" bytes");
    }
    
	//key is opName, value is byte array where first index is opCode and second is number of parameters
	public static HashMap<String, byte[]> opCodeMap = new HashMap<String, byte[]>();
	public static List<String> jmpInstructions = new ArrayList<String>();
	static {
		opCodeMap.put("hlt", new byte[]{0x00, 0x00});
		opCodeMap.put("movr1",new byte[]{0x01, 0x01});
		opCodeMap.put("movr2",new byte[]{0x02, 0x01});
		opCodeMap.put("movr3",new byte[]{0x03, 0x01});
		opCodeMap.put("movr4",new byte[]{0x04, 0x01});
		opCodeMap.put("movr5",new byte[]{0x05, 0x01});
		opCodeMap.put("ldrr1",new byte[]{0x06, 0x01});
		opCodeMap.put("ldrr2",new byte[]{0x07, 0x01});
		opCodeMap.put("ldrr3",new byte[]{0x08, 0x01});
		opCodeMap.put("ldrr4",new byte[]{0x09, 0x01});
		opCodeMap.put("ldrr5",new byte[]{0x0A, 0x01});
		opCodeMap.put("strr1",new byte[]{0x0B, 0x01});
		opCodeMap.put("strr2",new byte[]{0x0C, 0x01});
		opCodeMap.put("strr3",new byte[]{0x0D, 0x01});
		opCodeMap.put("strr4",new byte[]{0x0E, 0x01});
		opCodeMap.put("strr5",new byte[]{0x0F, 0x01});
		opCodeMap.put("addr1",new byte[]{0x10, 0x01});
		opCodeMap.put("addr2",new byte[]{0x11, 0x01});
		opCodeMap.put("addr3",new byte[]{0x12, 0x01});
		opCodeMap.put("addr4",new byte[]{0x13, 0x01});
		opCodeMap.put("addr5",new byte[]{0x14, 0x01});
		opCodeMap.put("subr1",new byte[]{0x15, 0x01});
		opCodeMap.put("subr2",new byte[]{0x16, 0x01});
		opCodeMap.put("subr3",new byte[]{0x17, 0x01});
		opCodeMap.put("subr4",new byte[]{0x18, 0x01});
		opCodeMap.put("subr5",new byte[]{0x19, 0x01});
		opCodeMap.put("mulr1",new byte[]{0x1B, 0x01});
		opCodeMap.put("mulr2",new byte[]{0x1C, 0x01});
		opCodeMap.put("mulr3",new byte[]{0x1D, 0x01});
		opCodeMap.put("mulr4",new byte[]{0x1E, 0x01});
		opCodeMap.put("mulr5",new byte[]{0x1F, 0x01});
		opCodeMap.put("divr1",new byte[]{0x20, 0x01});
		opCodeMap.put("divr2",new byte[]{0x21, 0x01});
		opCodeMap.put("divr3",new byte[]{0x22, 0x01});
		opCodeMap.put("divr4",new byte[]{0x23, 0x01});
		opCodeMap.put("divr5",new byte[]{0x24, 0x01});
		opCodeMap.put("pushr1",new byte[]{0x25, 0x00});
		opCodeMap.put("pushr2",new byte[]{0x26, 0x00});
		opCodeMap.put("pushr3",new byte[]{0x27, 0x00});
		opCodeMap.put("pushr4",new byte[]{0x28, 0x00});
		opCodeMap.put("pushr5",new byte[]{0x29, 0x00});
		opCodeMap.put("popr1",new byte[]{0x2A, 0x00});
		opCodeMap.put("popr2",new byte[]{0x2B, 0x00});
		opCodeMap.put("popr3",new byte[]{0x2C, 0x00});
		opCodeMap.put("popr4",new byte[]{0x2D, 0x00});
		opCodeMap.put("popr5",new byte[]{0x2E, 0x00});
		opCodeMap.put("cmpr1",new byte[]{0x2F, 0x01});
		opCodeMap.put("cmpr2",new byte[]{0x30, 0x01});
		opCodeMap.put("cmpr3",new byte[]{0x31, 0x01});
		opCodeMap.put("cmpr4",new byte[]{0x32, 0x01});
		opCodeMap.put("cmpr5",new byte[]{0x33, 0x01});
		opCodeMap.put("incr1",new byte[]{0x34, 0x00});
		opCodeMap.put("incr2",new byte[]{0x35, 0x00});
		opCodeMap.put("incr3",new byte[]{0x36, 0x00});
		opCodeMap.put("incr4",new byte[]{0x37, 0x00});
		opCodeMap.put("incr5",new byte[]{0x38, 0x00});
		opCodeMap.put("decr1",new byte[]{0x39, 0x00});
		opCodeMap.put("decr2",new byte[]{0x3A, 0x00});
		opCodeMap.put("decr3",new byte[]{0x3B, 0x00});
		opCodeMap.put("decr4",new byte[]{0x3C, 0x00});
		opCodeMap.put("decr5",new byte[]{0x3D, 0x00});
		opCodeMap.put("shlr1",new byte[]{0x3F, 0x01});
		opCodeMap.put("shlr2",new byte[]{0x40, 0x01});
		opCodeMap.put("shlr3",new byte[]{0x41, 0x01});
		opCodeMap.put("shlr4",new byte[]{0x42, 0x01});
		opCodeMap.put("shlr5",new byte[]{0x43, 0x01});
		opCodeMap.put("shrr1",new byte[]{0x44, 0x01});
		opCodeMap.put("shrr2",new byte[]{0x45, 0x01});
		opCodeMap.put("shrr3",new byte[]{0x46, 0x01});
		opCodeMap.put("shrr4",new byte[]{0x47, 0x01});
		opCodeMap.put("shrr5",new byte[]{0x48, 0x01});
		opCodeMap.put("andr1",new byte[]{0x49, 0x01});
		opCodeMap.put("andr2",new byte[]{0x4A, 0x01});
		opCodeMap.put("andr3",new byte[]{0x4B, 0x01});
		opCodeMap.put("andr4",new byte[]{0x4C, 0x01});
		opCodeMap.put("andr5",new byte[]{0x4D, 0x01});
		opCodeMap.put("orr1",new byte[]{0x4E, 0x01});
		opCodeMap.put("orr2",new byte[]{0x4F, 0x01});
		opCodeMap.put("orr3",new byte[]{0x50, 0x01});
		opCodeMap.put("orr4",new byte[]{0x51, 0x01});
		opCodeMap.put("orr5",new byte[]{0x52, 0x01});
		opCodeMap.put("xorr1",new byte[]{0x53, 0x01});
		opCodeMap.put("xorr2",new byte[]{0x54, 0x01});
		opCodeMap.put("xorr3",new byte[]{0x55, 0x01});
		opCodeMap.put("xorr4",new byte[]{0x56, 0x01});
		opCodeMap.put("xorr5",new byte[]{0x57, 0x01});
		opCodeMap.put("notr1",new byte[]{0x58, 0x01});
		opCodeMap.put("notr2",new byte[]{0x59, 0x01});
		opCodeMap.put("notr3",new byte[]{0x5A, 0x01});
		opCodeMap.put("notr4",new byte[]{0x5B, 0x01});
		opCodeMap.put("notr5",new byte[]{0x5C, 0x01});
		opCodeMap.put("addstacktopr1",new byte[]{0x5D, 0x00});
		opCodeMap.put("addstacktopr2",new byte[]{0x5E, 0x00});
		opCodeMap.put("addstacktopr3",new byte[]{0x5F, 0x00});
		opCodeMap.put("addstacktopr4",new byte[]{0x60, 0x00});
		opCodeMap.put("addstacktopr5",new byte[]{0x61, 0x00});
		opCodeMap.put("substacktopr1",new byte[]{0x62, 0x00});
		opCodeMap.put("substacktopr2",new byte[]{0x63, 0x00});
		opCodeMap.put("substacktopr3",new byte[]{0x64, 0x00});
		opCodeMap.put("substacktopr4",new byte[]{0x65, 0x00});
		opCodeMap.put("substacktopr5",new byte[]{0x66, 0x00});
		opCodeMap.put("mulstacktopr1",new byte[]{0x67, 0x00});
		opCodeMap.put("mulstacktopr2",new byte[]{0x68, 0x00});
		opCodeMap.put("mulstacktopr3",new byte[]{0x69, 0x00});
		opCodeMap.put("mulstacktopr4",new byte[]{0x6A, 0x00});
		opCodeMap.put("mulstacktopr5",new byte[]{0x6B, 0x00});
		opCodeMap.put("divstacktopr1",new byte[]{0x6C, 0x00});
		opCodeMap.put("divstacktopr2",new byte[]{0x6D, 0x00});
		opCodeMap.put("divstacktopr3",new byte[]{0x6E, 0x00});
		opCodeMap.put("divstacktopr4",new byte[]{0x6F, 0x00});
		opCodeMap.put("divstacktopr5",new byte[]{0x70, 0x00});
		opCodeMap.put("cmpstacktopr1",new byte[]{0x71, 0x00});
		opCodeMap.put("cmpstacktopr2",new byte[]{0x72, 0x00});
		opCodeMap.put("cmpstacktopr3",new byte[]{0x73, 0x00});
		opCodeMap.put("cmpstacktopr4",new byte[]{0x74, 0x00});
		opCodeMap.put("cmpstacktopr5",new byte[]{0x75, 0x00});
		opCodeMap.put("shlstacktopr1",new byte[]{0x76, 0x00});
		opCodeMap.put("shlstacktopr2",new byte[]{0x77, 0x00});
		opCodeMap.put("shlstacktopr3",new byte[]{0x78, 0x00});
		opCodeMap.put("shlstacktopr4",new byte[]{0x79, 0x00});
		opCodeMap.put("shlstacktopr5",new byte[]{0x7A, 0x00});
		opCodeMap.put("shrstacktopr1",new byte[]{0x7B, 0x00});
		opCodeMap.put("shrstacktopr2",new byte[]{0x7C, 0x00});
		opCodeMap.put("shrstacktopr3",new byte[]{0x7D, 0x00});
		opCodeMap.put("shrstacktopr4",new byte[]{0x7E, 0x00});
		opCodeMap.put("shrstacktopr5",new byte[]{0x7F, 0x00});
		opCodeMap.put("andstacktopr1",new byte[]{(byte) 0x80, 0x00});
		opCodeMap.put("andstacktopr2",new byte[]{(byte) 0x81, 0x00});
		opCodeMap.put("andstacktopr3",new byte[]{(byte) 0x82, 0x00});
		opCodeMap.put("andstacktopr4",new byte[]{(byte) 0x83, 0x00});
		opCodeMap.put("andstacktopr5",new byte[]{(byte) 0x84, 0x00});
		opCodeMap.put("orstacktopr1",new byte[]{(byte) 0x85, 0x00});
		opCodeMap.put("orstacktopr2",new byte[]{(byte) 0x86, 0x00});
		opCodeMap.put("orstacktopr3",new byte[]{(byte) 0x87, 0x00});
		opCodeMap.put("orstacktopr4",new byte[]{(byte) 0x88, 0x00});
		opCodeMap.put("orstacktopr5",new byte[]{(byte) 0x89, 0x00});
		opCodeMap.put("xorstacktopr1",new byte[]{(byte) 0x8A, 0x00});
		opCodeMap.put("xorstacktopr2",new byte[]{(byte) 0x8B, 0x00});
		opCodeMap.put("xorstacktopr3",new byte[]{(byte) 0x8C, 0x00});
		opCodeMap.put("xorstacktopr4",new byte[]{(byte) 0x8D, 0x00});
		opCodeMap.put("xorstacktopr5",new byte[]{(byte) 0x8E, 0x00});
		opCodeMap.put("pusha",new byte[]{(byte) 0x8F, 0x00});
		opCodeMap.put("popa",new byte[]{(byte) 0x90, 0x00});
		opCodeMap.put("jmp",new byte[]{(byte) 0x91, 0x01});
		opCodeMap.put("je",new byte[]{(byte) 0x92, 0x01});
		opCodeMap.put("jne",new byte[]{(byte) 0x93, 0x01});
		opCodeMap.put("call",new byte[]{(byte) 0x94, 0x01});
		opCodeMap.put("ret",new byte[]{(byte) 0x95, 0x00});
		opCodeMap.put("jmpstacktop",new byte[]{(byte) 0x96, 0x00});
		opCodeMap.put("getch",new byte[]{(byte) 0x97, 0x00});
		opCodeMap.put("checkkeypress",new byte[]{(byte) 0x98, 0x00});
		opCodeMap.put("setpixel",new byte[]{(byte) 0x99, 0x00});
		opCodeMap.put("setpixelredraw",new byte[]{(byte) 0x9A, 0x00});
		opCodeMap.put("redraw",new byte[]{(byte) 0x9B, 0x00});
		opCodeMap.put("rand",new byte[]{(byte) 0x9C, 0x00});
		opCodeMap.put("beep",new byte[]{(byte) 0x9D, 0x02});
		opCodeMap.put("sleep",new byte[]{(byte) 0x9E, 0x01});
		opCodeMap.put("clrflag",new byte[]{(byte) 0x9F, 0x01});
		opCodeMap.put("setflag",new byte[]{(byte) 0xA0, 0x01});
		opCodeMap.put("nop",new byte[]{(byte) 0xA1, 0x01});
		
		jmpInstructions.add("jmp");
		jmpInstructions.add("je");
		jmpInstructions.add("jne");
		jmpInstructions.add("call");
	}
    
}

