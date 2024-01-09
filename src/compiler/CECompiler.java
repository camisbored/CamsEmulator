import java.io.*;
import java.util.*;

public class CECompiler {

    private static Map<String, String> symbolTable = new HashMap<>();
    private static final Map<String, String> functionTable = new HashMap<>();
    private static int labelIndex = 0;
    private static int memoryAddress = 0;

    public static void main(String[] args) {

        if (args.length < 1) {
            System.out.println("Usage: java CamsCompiler <inputFile> <outputFile>");
            return;
        }

        String inputFile = args[0];

        String outputFile = "a.asm";

        if (args.length > 1){
            outputFile = args[1];
        }

        try {
            String sourceCode = readFile(inputFile);

            String compiledCode = compileSourceCode(sourceCode);

            writeFile(outputFile, compiledCode);
            System.out.println(inputFile + " compilation successful. Wrote to " +outputFile + ", size: " + compiledCode.length());

        } catch (IOException e) {
            System.out.println("Compilation Error: " + e.getMessage());
            System.exit(1);
        }
    }

    private static String readFile(String filePath) throws IOException {
        BufferedReader reader = new BufferedReader(new FileReader(filePath));
        StringBuilder stringBuilder = new StringBuilder();
        String line;
        while ((line = reader.readLine()) != null) {
            stringBuilder.append(line).append("\\n");
        }
        reader.close();
        return stringBuilder.toString();
    }

    private static void writeFile(String filePath, String content) throws IOException {
        BufferedWriter writer = new BufferedWriter(new FileWriter(filePath));
        writer.write(content);
        writer.close();
    }

    private static String compileSourceCode(String sourceCode) {
        StringBuilder compiledCode = new StringBuilder();

        ArrayList<String> statements = preprocess(sourceCode);

        for (String statement : statements) {
            try {
                String[] tokens = lexer(statement);
                List<String> assemblyCodes = parserAndCodeGenerator(tokens);
                assemblyCodes.forEach(code -> compiledCode.append(code).append("\n"));
            } catch (Exception e) {
                System.out.println("Error in line '"+"': "+e.getMessage()+"\\n");
                System.exit(1);
            }
        }
        return compiledCode.toString();
    }

    private static ArrayList<String> preprocess(String sourceCode) {

        String[] lines = sourceCode.split("\\\\n");

        ArrayList<String> statements = new ArrayList<>();
        StringBuilder statement = new StringBuilder();
        int braceLevel = 0;

        for (String line : lines) {
            if (line.trim().equals("//")){
                continue;
            }
            line = line.split("//")[0].trim();

            if (!line.isEmpty()) {
                    statement.append(line).append(" ");

                    for (char ch : line.toCharArray()) {
                        if (ch == '{') {
                            braceLevel++;
                        } else if (ch == '}') {
                            braceLevel--;
                        }
                    }

                    if (braceLevel == 0 && (line.endsWith(";") || line.endsWith("}"))) {
                        statements.add(statement.toString().trim());
                        statement = new StringBuilder();
                    }
            }
        }

        return statements;
    }

    private static String[] lexer(String statement) throws Exception {
        statement = statement.trim();
        for (Map.Entry<String, String> entry : functionTable.entrySet()){
            if (statement.contains(entry.getKey())){
                return new String[]{"call", entry.getValue()};
            }
        }
        if (statement.startsWith("if")){

            boolean hasElseClause = false;

            if (statement.contains(" else ")){
                hasElseClause = true;
            }

            String conditionalType = null;
            String[] params = null;
            String condition = statement.split("\\(")[1].split("\\)")[0];
            if (condition.contains("==")){
                conditionalType = "==";
                params = condition.split(conditionalType);
            } else if (condition.contains("!=")){
                conditionalType = "!=";
                params = condition.split(conditionalType);
            } else {
                throw new IllegalArgumentException("Invalid conditional type: " + condition);
            }
            String statement1 = statement.split("\\)")[1];
            String statement2 = null;
            if (hasElseClause){
                statement2 = statement1.split(" else ")[1];
                statement2 = statement2.trim().substring(1, statement2.length()-1).trim();
                statement1 = statement1.split(" else ")[0];
            }
            statement1 = statement1.trim().substring(1, statement1.length()-2).trim();

            String[] ifClause = statement1.split(";");
            String[] elseClause = null;
            if (hasElseClause){
                elseClause = statement2.split(";");
            }

            List<String> ifResults = new ArrayList<>();
            List<String> elseResults = new ArrayList<>();

            for (int i = 0; i < ifClause.length; i++) {
                String[] a = lexer(ifClause[i].trim());
                ifResults.add(Arrays.toString(a)); // Collect results for ifClause
            }

            if (hasElseClause) {
                for (int i = 0; i < elseClause.length; i++) {
                    String[] b = lexer(elseClause[i].trim());
                    elseResults.add(Arrays.toString(b)); // Collect results for elseClause
                }
            }

            String ifResultString = String.join(", ", ifResults);
            String elseResultString =  null;

            if (hasElseClause){
                elseResultString = String.join(", ", elseResults);
            }

            return new String[]{"conditional", params[0], params[1], ifResultString, elseResultString, conditionalType};
        } else if (statement.startsWith("asm")){
            String asm = statement.split("\\(")[1].split("\\)")[0].trim();
            asm = asm.substring(1, asm.length()-1);
            return new String[]{"asm", asm};
        } else if (statement.startsWith("sleep")){
            String sleepLength = statement.split("\\(")[1].split("\\)")[0].trim();
            return new String[]{"sleep", sleepLength};
        } else if (statement.startsWith("sound")){
            String args = statement.split("\\(")[1].split("\\)")[0].trim();
            String[] args2 = args.split(",");
            String beep = args2[0].trim();
            String length = args2[1].trim();
            return new String[]{"sound", beep, length};
        } else if (statement.startsWith("def")){
            String functionName = statement.split("\\(")[0].split("def")[1].trim();
            String statements = statement.split("\\{")[1].split("}")[0].trim();
            String[] statementsArr = statements.split(";");
            List<String> statementResults = new ArrayList<>();

            for (int i = 0; i < statementsArr.length; i++) {
                String[] a = lexer(statementsArr[i].trim());
                statementResults.add(Arrays.toString(a)); // Collect results for ifClause
            }

            String statementResultString = String.join(", ", statementResults);


            return new String[]{"def", functionName, statementResultString};
        }else if (statement.startsWith("while")){
            String conditionalType = null;
            String[] params = null;
            String condition = statement.split("\\(")[1].split("\\)")[0];
            if (condition.contains("==")){
                conditionalType = "==";
                params = condition.split(conditionalType);
            } else if (condition.contains("!=")){
                conditionalType = "!=";
                params = condition.split(conditionalType);
            } else {
                throw new IllegalArgumentException("Invalid conditional type: " + condition);
            }

            String statements = statement.split("\\)\\{")[1];
            statements = statements.substring(0, statements.length()-1).trim();
            String[] statementsArr = statements.split(";");

            List<String> statementResults = new ArrayList<>();

            for (int i = 0; i < statementsArr.length; i++) {
                String[] a = lexer(statementsArr[i].trim());
                statementResults.add(Arrays.toString(a)); // Collect results for ifClause
            }

            String statementResultString = String.join(", ", statementResults);

            return new String[]{"while", params[0], params[1], statementResultString, conditionalType};

        } else if (statement.contains("input()")){
            String varName = statement.split("=")[0].trim();
            return new String[]{"input", varName};
        }
        else if (statement.contains("random()")){
            String varName = statement.split("=")[0].trim();
            return new String[]{"random", varName};
        }
        else if (statement.contains("=")) {
            statement = statement.trim().replaceAll(";", "");
            String[] parts = statement.split("=", 2);
            String varName = parts[0].trim();
            String expression = parts[1].trim();
            return new String[]{"assignment", varName, expression};
        } else if (statement.contains("draw(")){
            String params = statement.split("\\(")[1];
            params = params.split("\\)")[0];
            String x = params.split(",")[0].trim();
            String y = params.split(",")[1].trim();
            return new String[]{"draw", x, y};
        } else if (statement.startsWith("drawRGB")){
            String params = statement.split("\\(")[1];
            params = params.split("\\)")[0];
            String x = params.split(",")[0].trim();
            String y = params.split(",")[1].trim();
            String r = params.split(",")[2].trim();
            String g = params.split(",")[3].trim();
            String b = params.split(",")[4].trim();

            return new String[]{"drawRGB", x, y, r, g, b};
        } else {
            throw new Exception("Invalid statement format.");
        }
    }

    private static List<String> parserAndCodeGenerator(String[] tokens) throws Exception {
        String operation = tokens[0];

        List<String> assemblyCodes = new ArrayList<>();

        if (operation.equals("assignment")) {

            String varName = tokens[1];
            String expression = tokens[2];

            assemblyCodes.addAll(evaluateExpression(expression));
            assemblyCodes.addAll(storeInMemory(varName));

        } else if (operation.equalsIgnoreCase("asm")){
            String asm = tokens[1];
            assemblyCodes.add(asm);
        } else if (operation.equalsIgnoreCase("sleep")){
            String sleepLength = tokens[1];
            assemblyCodes.add("sleep, " + sleepLength);

        } else if (operation.equalsIgnoreCase("sound")){
            String pitch = tokens[1];
            String length = tokens[2];
            assemblyCodes.add("beep,"+pitch+","+length);

        }else if (operation.equalsIgnoreCase("def")){
            String functionName = tokens[1].trim();
            assemblyCodes.add(functionName+":");
            String arrayAsString = tokens[2];
            String[] reconstructedArr = arrayAsString.split("], ");
            for (int i = 0; i < reconstructedArr.length; i++){
                reconstructedArr[i]=reconstructedArr[i].replaceAll("\\[", "").replaceAll("\\]","");
                String[] reconstructedIf = reconstructedArr[i].substring(0, reconstructedArr[i].length()).split(", ");
                assemblyCodes.addAll(parserAndCodeGenerator(reconstructedIf));
            }
            assemblyCodes.add("ret");
            functionTable.put(functionName+"()", functionName);
        }else if (operation.equalsIgnoreCase("conditional")){
            assemblyCodes.add(evaluateToken(tokens[1].trim()));
            assemblyCodes.add("push r1");
            assemblyCodes.add(evaluateToken(tokens[2].trim()));
            assemblyCodes.add("cmpStackTop r1");

            String elseClause = tokens[4];
            String condition = tokens[5];


            String currentLabelIf = "label"+labelIndex++;
            String currentDoneLabel = currentLabelIf+"done";
            String currentLabelElse = "label" + labelIndex++;


//            if (elseClause != null) {
////                currentLabelElse = "label" + labelIndex++;
//                if (condition.equals("==")) {
//                    assemblyCodes.add("jne " + currentLabelElse);
//                } else {
//                    assemblyCodes.add("je" + currentLabelElse);
//                }
//            }

            if (condition.equals("==")) {
                assemblyCodes.add("je " + currentLabelIf);
                if (elseClause != null){
                    assemblyCodes.add("je " + currentLabelIf);
                    assemblyCodes.add("jne " + currentLabelElse);
                } else {
                    assemblyCodes.add("je " + currentLabelIf);
                    assemblyCodes.add("jne " + currentDoneLabel);
                }
            } else {
                assemblyCodes.add("jne " + currentLabelIf);
                if (elseClause != null){
                    assemblyCodes.add("jne " + currentLabelIf);
                    assemblyCodes.add("je " + currentLabelElse);
                } else {
                    assemblyCodes.add("jne " + currentLabelIf);
                    assemblyCodes.add("je " + currentDoneLabel);
                }
            }

            assemblyCodes.add(currentLabelIf+":");

            String arrayAsString = tokens[3];
            String[] reconstructedIfArr = arrayAsString.split("], ");
            for (int i = 0; i < reconstructedIfArr.length; i++){
                reconstructedIfArr[i]=reconstructedIfArr[i].replaceAll("\\[", "").replaceAll("\\]","");
                String[] reconstructedIf = reconstructedIfArr[i].substring(0, reconstructedIfArr[i].length()).split(", ");
                assemblyCodes.addAll(parserAndCodeGenerator(reconstructedIf));
            }

            assemblyCodes.add("jmp " + currentDoneLabel);


            if (elseClause != null){
                assemblyCodes.add(currentLabelElse+":");
                String elseArrayAsString = tokens[4];
                String[] reconstructedElseArr = elseArrayAsString.split("], ");
                for (int i = 0; i < reconstructedElseArr.length; i++){
                    reconstructedElseArr[i]=reconstructedElseArr[i].replaceAll("\\[", "").replaceAll("\\]","");
                    String[] reconstructedIElse = reconstructedElseArr[i].substring(0, reconstructedElseArr[i].length()).split(", ");
                    assemblyCodes.addAll(parserAndCodeGenerator(reconstructedIElse));
                }
                //assemblyCodes.add("jmp " + currentDoneLabel);
            }

            assemblyCodes.add(currentDoneLabel+":");

        } else if (operation.equalsIgnoreCase("while")){

            String arg1 = tokens[1].trim();
            String arg2 = tokens[2].trim();

            String loopStartLabel = "label"+labelIndex++;
            String loopDoneLabel = "label" + labelIndex++ +"done";

            assemblyCodes.add(loopStartLabel+":");

            String condition = tokens[4];

            assemblyCodes.add(evaluateToken(arg1));
            assemblyCodes.add("push r1");
            assemblyCodes.add(evaluateToken(arg2));
            assemblyCodes.add("cmpStackTop r1");
            if (condition.equals("==")) {
                assemblyCodes.add("jne " + loopDoneLabel);
            } else {
                assemblyCodes.add("je " + loopDoneLabel);
            }


            String arrayAsString = tokens[3];
            String[] reconstructedArr = arrayAsString.split("], ");
            for (int i = 0; i < reconstructedArr.length; i++){
                reconstructedArr[i]=reconstructedArr[i].replaceAll("\\[", "").replaceAll("\\]","");
                String[] reconstructedIf = reconstructedArr[i].substring(0, reconstructedArr[i].length()).split(", ");
                assemblyCodes.addAll(parserAndCodeGenerator(reconstructedIf));
            }

            assemblyCodes.add("jmp " + loopStartLabel);

            assemblyCodes.add(loopDoneLabel+":");

        } else if (operation.equalsIgnoreCase("draw")){
            assemblyCodes.add(evaluateToken(tokens[2].trim()));
            assemblyCodes.add("push r1");
            assemblyCodes.add("pop r2");
            assemblyCodes.add(evaluateToken(tokens[1].trim()));
            assemblyCodes.add("mov r3, 0");
            assemblyCodes.add("mov r4, 0");
            assemblyCodes.add("mov r5, 0");
            assemblyCodes.add("setPixelRedraw");
        } else if (operation.equalsIgnoreCase("drawRGB")){
            assemblyCodes.add(evaluateToken(tokens[5].trim()));
            assemblyCodes.add("push r1");
            assemblyCodes.add("pop r5");
            assemblyCodes.add(evaluateToken(tokens[4].trim()));
            assemblyCodes.add("push r1");
            assemblyCodes.add("pop r4");
            assemblyCodes.add(evaluateToken(tokens[3].trim()));
            assemblyCodes.add("push r1");
            assemblyCodes.add("pop r3");
            assemblyCodes.add(evaluateToken(tokens[2].trim()));
            assemblyCodes.add("push r1");
            assemblyCodes.add("pop r2");
            assemblyCodes.add(evaluateToken(tokens[1].trim()));
            assemblyCodes.add("setPixelRedraw");
        } else if (operation.equalsIgnoreCase("call")){
            assemblyCodes.add("call " + tokens[1].trim());
        } else if (operation.equalsIgnoreCase("input")){
            String param1 = tokens[1];
            assemblyCodes.add("getch");
            assemblyCodes.add("push r5");
            assemblyCodes.add("pop r1");
            List<String> localAsm = storeInMemory(param1);
            assemblyCodes.addAll(localAsm);
        } else if (operation.equalsIgnoreCase("random")){
            String param1 = tokens[1];
            assemblyCodes.add("rand");
            assemblyCodes.add("push r5");
            assemblyCodes.add("pop r1");
            List<String> localAsm = storeInMemory(param1);
            assemblyCodes.addAll(localAsm);
        } else {
            throw new Exception("Unsupported operation.");
        }
        return assemblyCodes;
    }

    //takes an expression as a string and will generate code to have the result be in r1
    private static List<String> evaluateExpression(String expression) {
        List<String> expressionCode = new ArrayList<>();

//        String[] expressionTokens = expression.replaceAll("\\s+", "").split("\\+");
        String[] expressionTokens = expression.split("\\s+");
        if (expressionTokens.length == 0){
            throw new IllegalArgumentException("Expression with no arguments should never exist: " + expression);
        }

        else if (expressionTokens.length == 1){

            expressionCode.add(evaluateToken(expressionTokens[0]));

        } else {

            expressionCode.add(evaluateToken(expressionTokens[0]));
            expressionCode.add("push r1");

//            for (int i = 1; i < expressionTokens.length; i++){
//                expressionCode.add(evaluateToken(expressionTokens[i]));
//                expressionCode.add("addStackTop r1");
//            }
            for (int i = 1; i < expressionTokens.length; i++){
                String operator = expressionTokens[i++];
                expressionCode.add(evaluateToken(expressionTokens[i]));
                if (operator.equals("+")) {
                    expressionCode.add("addStackTop r1");
                } else if (operator.equals("-")) {
                    expressionCode.add("subStackTop r1");
                } else if (operator.equals("*")) {
                    expressionCode.add("mulStackTop r1");
                } else if (operator.equals("/")) {
                    expressionCode.add("divStackTop r1");
                } else if (operator.equals("|")) {
                    expressionCode.add("orStackTop r1");
                }else if (operator.equals("&")) {
                    expressionCode.add("andStackTop r1");
                } else if (operator.equals("<")) {
                    expressionCode.add("shlStackTop r1");
                } else if (operator.equals(">")) {
                    expressionCode.add("shrStackTop r1");
                } else if (operator.equals("^")){
                    expressionCode.add("xorStackTop r1");
                } else {
                    throw new IllegalArgumentException("Invalid operator: " + operator);
                }
            }

            expressionCode.add("pop r1");

        }

        return expressionCode;
    }

    //will load token or literal value into r1
    private static String evaluateToken(String expressionToken) {
        boolean isVariable = isVarOrValue(expressionToken);
        if (isVariable){

            return loadVariable(expressionToken);

        } else {

            return "mov r1, " + expressionToken;

        }
    }

    private static String loadVariable(String token) {
        String memoryLocation;

        if (symbolTable.containsKey(token)){
            memoryLocation = symbolTable.get(token);
        } else {
            throw new IllegalArgumentException("Attempted to load unknown variable: " + token);
        }

        return "ldr r1, " + memoryLocation;
    }

    private static boolean isVarOrValue(String token) {
        String variableRegex = "^[a-zA-Z][a-zA-Z0-9_]*$";
        String numberRegex = "^[0-9]+$";
        if (token.matches(variableRegex)){
            return true;
        }
        if (token.matches(numberRegex)){
            return false;
        }
        throw new IllegalArgumentException("Illegal token detected: " + token);
    }

    private static List<String> storeInMemory(String varName, String value) {
        List<String> assemblyCodes = new ArrayList<>();
        String memoryLocation;

        if (symbolTable.containsKey(varName)){
            memoryLocation = symbolTable.get(varName);
        } else {
            memoryLocation = Integer.toString(memoryAddress++);
            symbolTable.put(varName, memoryLocation);
        }

        assemblyCodes.add("push r1");
        assemblyCodes.add("mov r1, " + value);
        assemblyCodes.add("str r1, " + memoryLocation);
        assemblyCodes.add("pop r1");
        return assemblyCodes;
    }

    private static List<String> storeInMemory(String varName) {
        List<String> assemblyCodes = new ArrayList<>();
        String memoryLocation;
        if (symbolTable.containsKey(varName)){
            memoryLocation = symbolTable.get(varName);
        } else {
            memoryLocation = Integer.toString(memoryAddress++);
            symbolTable.put(varName, memoryLocation);
        }
        symbolTable.put(varName, memoryLocation);
        assemblyCodes.add("str r1, " + memoryLocation);
        return assemblyCodes;
    }

    private static String loadFromMemory(String varName) {
        return symbolTable.get(varName);
    }

    public static int countSemicolons(String input) {
        int count = 0;
        for (int i = 0; i < input.length(); i++) {
            if (input.charAt(i) == ';') {
                count++;
            }
        }
        return count;
    }

}
