//// ============================================================================
////  PART (b): Testing Rules of Inference
//// ============================================================================
//#include <iostream>
//#include <string>
//#ifdef _WIN32
//#include <windows.h>
//#endif
//using namespace std;
//
//// The logic symbols we accept and print
//const string NOT_SYM = "¬";
//const string AND_SYM = "∧";
//const string OR_SYM = "∨";
//const string IMP_SYM = "→";
//const string IFF_SYM = "↔";
//
//// ---------------------------------------------------------------------------
////  Expression tree
//// ---------------------------------------------------------------------------
//const int NODE_VAR = 0;
//const int NODE_T = 1;
//const int NODE_F = 2;
//const int NODE_NOT = 3;
//const int NODE_AND = 4;
//const int NODE_OR = 5;
//const int NODE_IMP = 6;
//const int NODE_IFF = 7;
//
//struct Node {
//    int type;
//    char name;      // only used when type == NODE_VAR
//    Node* left;
//    Node* right;
//};
//
//Node* makeNode(int type, char name, Node* left, Node* right) {
//    Node* n = new Node;
//    n->type = type;
//    n->name = name;
//    n->left = left;
//    n->right = right;
//    return n;
//}
//
//Node* makeVar(char c) { return makeNode(NODE_VAR, c, NULL, NULL); }
//Node* makeConst(bool value) { return makeNode(value ? NODE_T : NODE_F, ' ', NULL, NULL); }
//Node* makeNot(Node* a) { return makeNode(NODE_NOT, ' ', a, NULL); }
//Node* makeBin(int type, Node* a, Node* b) { return makeNode(type, ' ', a, b); }
//
//bool isBinary(Node* n) {
//    return n->type == NODE_AND || n->type == NODE_OR || n->type == NODE_IMP || n->type == NODE_IFF;
//}
//
//void freeTree(Node* n) {
//    if (n == NULL) return;
//    freeTree(n->left);
//    freeTree(n->right);
//    delete n;
//}
//
//string operatorSymbol(int type) {
//    if (type == NODE_AND) return AND_SYM;
//    if (type == NODE_OR)  return OR_SYM;
//    if (type == NODE_IMP) return IMP_SYM;
//    return IFF_SYM;
//}
//
//string treeToString(Node* n);
//
//string childToString(Node* child) {
//    if (isBinary(child)) return "(" + treeToString(child) + ")";
//    return treeToString(child);
//}
//
//string treeToString(Node* n) {
//    if (n->type == NODE_VAR) return string(1, n->name);
//    if (n->type == NODE_T) return "T";
//    if (n->type == NODE_F) return "F";
//    if (n->type == NODE_NOT) return NOT_SYM + childToString(n->left);
//    return childToString(n->left) + " " + operatorSymbol(n->type) + " " + childToString(n->right);
//}
//
//bool evaluate(Node* n, bool values[256]) {
//    if (n->type == NODE_VAR) return values[(unsigned char)n->name];
//    if (n->type == NODE_T) return true;
//    if (n->type == NODE_F) return false;
//    if (n->type == NODE_NOT) return !evaluate(n->left, values);
//    bool a = evaluate(n->left, values);
//    bool b = evaluate(n->right, values);
//    if (n->type == NODE_AND) return a && b;
//    if (n->type == NODE_OR)  return a || b;
//    if (n->type == NODE_IMP) return (!a) || b;
//    return a == b;
//}
//
//void markVariables(Node* n, bool seen[256]) {
//    if (n == NULL) return;
//    if (n->type == NODE_VAR) seen[(unsigned char)n->name] = true;
//    markVariables(n->left, seen);
//    markVariables(n->right, seen);
//}
//
//// ---------------------------------------------------------------------------
////  Tokenizer
//// ---------------------------------------------------------------------------
//const int TOK_VAR = 0;
//const int TOK_CONST = 1;
//const int TOK_NOT = 2;
//const int TOK_AND = 3;
//const int TOK_OR = 4;
//const int TOK_IMP = 5;
//const int TOK_IFF = 6;
//const int TOK_OPEN = 7;
//const int TOK_CLOSE = 8;
//
//const int MAX_TOKENS = 300;
//
//struct Token {
//    int type;
//    string text;
//};
//
//Token tokens[MAX_TOKENS];
//int tokenCount = 0;
//
//bool isLetter(char c) {
//    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
//}
//
//bool isLetterOrDigit(char c) {
//    return isLetter(c) || (c >= '0' && c <= '9') || c == '_';
//}
//
//bool isBinaryToken(int type) {
//    return type == TOK_AND || type == TOK_OR || type == TOK_IMP || type == TOK_IFF;
//}
//
//bool startsAt(const string& text, int pos, const string& symbol) {
//    int len = symbol.length();
//    if (pos + len > (int)text.length()) return false;
//    for (int i = 0; i < len; i++) {
//        if (text[pos + i] != symbol[i]) return false;
//    }
//    return true;
//}
//
//bool readSymbol(const string& text, int pos, int& type, int& length) {
//    if (startsAt(text, pos, "<->")) { type = TOK_IFF;   length = 3; return true; }
//    if (startsAt(text, pos, IFF_SYM)) { type = TOK_IFF;   length = IFF_SYM.length(); return true; }
//    if (startsAt(text, pos, "->")) { type = TOK_IMP;   length = 2; return true; }
//    if (startsAt(text, pos, IMP_SYM)) { type = TOK_IMP;   length = IMP_SYM.length(); return true; }
//    if (startsAt(text, pos, NOT_SYM)) { type = TOK_NOT;   length = NOT_SYM.length(); return true; }
//    if (startsAt(text, pos, "~")) { type = TOK_NOT;   length = 1; return true; }
//    if (startsAt(text, pos, "!")) { type = TOK_NOT;   length = 1; return true; }
//    if (startsAt(text, pos, AND_SYM)) { type = TOK_AND;   length = AND_SYM.length(); return true; }
//    if (startsAt(text, pos, "&")) { type = TOK_AND;   length = 1; return true; }
//    if (startsAt(text, pos, "^")) { type = TOK_AND;   length = 1; return true; }
//    if (startsAt(text, pos, OR_SYM)) { type = TOK_OR;    length = OR_SYM.length(); return true; }
//    if (startsAt(text, pos, "|")) { type = TOK_OR;    length = 1; return true; }
//    if (startsAt(text, pos, "v")) { type = TOK_OR;    length = 1; return true; }
//    if (startsAt(text, pos, "V")) { type = TOK_OR;    length = 1; return true; }
//    if (startsAt(text, pos, "(")) { type = TOK_OPEN;  length = 1; return true; }
//    if (startsAt(text, pos, ")")) { type = TOK_CLOSE; length = 1; return true; }
//    return false;
//}
//
//string tokenize(const string& text) {
//    tokenCount = 0;
//    int pos = 0;
//    int total = text.length();
//    while (pos < total) {
//        char c = text[pos];
//        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') { pos++; continue; }
//
//        if (tokenCount >= MAX_TOKENS) return "Rejected: the formula is too long.";
//
//        int type = 0, length = 0;
//        if (readSymbol(text, pos, type, length)) {
//            tokens[tokenCount].type = type;
//            tokens[tokenCount].text = text.substr(pos, length);
//            tokenCount++;
//            pos = pos + length;
//            continue;
//        }
//
//        if (isLetterOrDigit(c)) {
//            int end = pos;
//            while (end < total && isLetterOrDigit(text[end])) end++;
//            string word = text.substr(pos, end - pos);
//            if (!isLetter(word[0]))
//                return "Rejected: invalid or unsupported symbol \"" + word + "\".";
//            if (word.length() > 1)
//                return "Rejected: variable \"" + word + "\" is longer than 1 character.";
//            if (word == "T" || word == "F") tokens[tokenCount].type = TOK_CONST;
//            else tokens[tokenCount].type = TOK_VAR;
//            tokens[tokenCount].text = word;
//            tokenCount++;
//            pos = end;
//            continue;
//        }
//
//        int len = 1;
//        if ((c & 0xE0) == 0xC0) len = 2;
//        else if ((c & 0xF0) == 0xE0) len = 3;
//        else if ((c & 0xF8) == 0xF0) len = 4;
//        return "Rejected: invalid or unsupported symbol \"" + text.substr(pos, len) + "\".";
//    }
//    return "";
//}
//
//// ---------------------------------------------------------------------------
////  Syntax & Constraint Checks
//// ---------------------------------------------------------------------------
//string checkParentheses() {
//    int open = 0;
//    for (int i = 0; i < tokenCount; i++) {
//        if (tokens[i].type == TOK_OPEN) open++;
//        if (tokens[i].type == TOK_CLOSE) {
//            open--;
//            if (open < 0) return "Rejected: unbalanced parentheses (a ')' has no matching '(').";
//        }
//    }
//    if (open > 0) return "Rejected: unbalanced parentheses (" + to_string(open) + " '(' not closed).";
//    return "";
//}
//
//string checkSyntax() {
//    if (tokenCount == 0) return "Rejected: the formula is empty.";
//    bool waitingForOperand = true;
//    for (int i = 0; i < tokenCount; i++) {
//        int type = tokens[i].type;
//        if (waitingForOperand) {
//            if (type == TOK_VAR || type == TOK_CONST) {
//                waitingForOperand = false;
//            }
//            else if (type == TOK_NOT || type == TOK_OPEN) {
//                // still waiting for an operand
//            }
//            else if (isBinaryToken(type)) {
//                if (i > 0 && (isBinaryToken(tokens[i - 1].type) || tokens[i - 1].type == TOK_NOT))
//                    return "Rejected: two operators in a row (\"" + tokens[i - 1].text + "\" followed by \"" + tokens[i].text + "\").";
//                return "Rejected: missing operand before operator \"" + tokens[i].text + "\".";
//            }
//            else {
//                if (i > 0 && tokens[i - 1].type == TOK_OPEN) return "Rejected: empty parentheses.";
//                return "Rejected: missing operand before ')'.";
//            }
//        }
//        else {
//            if (isBinaryToken(type)) {
//                waitingForOperand = true;
//            }
//            else if (type == TOK_CLOSE) {
//                // fine
//            }
//            else if (type == TOK_NOT) {
//                return "Rejected: unary operator \"" + tokens[i].text + "\" cannot follow an operand (a binary operator is missing).";
//            }
//            else {
//                return "Rejected: missing operator between \"" + tokens[i - 1].text + "\" and \"" + tokens[i].text + "\".";
//            }
//        }
//    }
//    if (waitingForOperand)
//        return "Rejected: missing operand after the last operator \"" + tokens[tokenCount - 1].text + "\".";
//    return "";
//}
//
//int countDistinctVariables() {
//    bool seen[256];
//    for (int i = 0; i < 256; i++) seen[i] = false;
//    int count = 0;
//    for (int i = 0; i < tokenCount; i++) {
//        if (tokens[i].type == TOK_VAR) {
//            unsigned char c = (unsigned char)tokens[i].text[0];
//            if (!seen[c]) { seen[c] = true; count++; }
//        }
//    }
//    return count;
//}
//
//// ---------------------------------------------------------------------------
////  Parser
//// ---------------------------------------------------------------------------
//int parsePos = 0;
//
//Node* parseBiconditional();
//
//Node* parsePrimary() {
//    if (tokens[parsePos].type == TOK_OPEN) {
//        parsePos++;
//        Node* inside = parseBiconditional();
//        parsePos++;
//        return inside;
//    }
//    Token t = tokens[parsePos];
//    parsePos++;
//    if (t.type == TOK_CONST) return makeConst(t.text == "T");
//    return makeVar(t.text[0]);
//}
//
//Node* parseUnary() {
//    if (parsePos < tokenCount && tokens[parsePos].type == TOK_NOT) {
//        parsePos++;
//        return makeNot(parseUnary());
//    }
//    return parsePrimary();
//}
//
//Node* parseAnd() {
//    Node* left = parseUnary();
//    while (parsePos < tokenCount && tokens[parsePos].type == TOK_AND) {
//        parsePos++;
//        left = makeBin(NODE_AND, left, parseUnary());
//    }
//    return left;
//}
//
//Node* parseOr() {
//    Node* left = parseAnd();
//    while (parsePos < tokenCount && tokens[parsePos].type == TOK_OR) {
//        parsePos++;
//        left = makeBin(NODE_OR, left, parseAnd());
//    }
//    return left;
//}
//
//Node* parseImplies() {
//    Node* left = parseOr();
//    if (parsePos < tokenCount && tokens[parsePos].type == TOK_IMP) {
//        parsePos++;
//        return makeBin(NODE_IMP, left, parseImplies());
//    }
//    return left;
//}
//
//Node* parseBiconditional() {
//    Node* left = parseImplies();
//    while (parsePos < tokenCount && tokens[parsePos].type == TOK_IFF) {
//        parsePos++;
//        left = makeBin(NODE_IFF, left, parseImplies());
//    }
//    return left;
//}
//
//Node* parseTokens() {
//    parsePos = 0;
//    return parseBiconditional();
//}
//
//string trimText(const string& s) {
//    int start = 0;
//    int end = (int)s.length() - 1;
//    while (start <= end && (s[start] == ' ' || s[start] == '\t' || s[start] == '\r' || s[start] == '\n')) start++;
//    while (end >= start && (s[end] == ' ' || s[end] == '\t' || s[end] == '\r' || s[end] == '\n')) end--;
//    if (start > end) return "";
//    return s.substr(start, end - start + 1);
//}
//
//// ============================================================================
////  PART (b): TESTING AN ARGUMENT WITH A TRUTH TABLE
//// ============================================================================
//const int MAX_VARS = 8;
//const int MAX_PREMISES = 10;
//
//string readFormula(const string& text, Node*& tree) {
//    string error = tokenize(text);
//    if (error == "") error = checkParentheses();
//    if (error == "") error = checkSyntax();
//    if (error == "" && countDistinctVariables() > MAX_VARS)
//        error = "Rejected: more than " + to_string(MAX_VARS) + " distinct variables in one formula.";
//    if (error != "") return error;
//    tree = parseTokens();
//    return "";
//}
//
//bool checkArgument(int premiseCount, string premiseTexts[], const string& conclusionText, bool& parsedOk) {
//    parsedOk = true;
//    Node* premises[MAX_PREMISES];
//    Node* conclusion = NULL;
//    int made = 0;
//
//    string error = "";
//    for (int i = 0; i < premiseCount && error == ""; i++) {
//        error = readFormula(premiseTexts[i], premises[i]);
//        if (error != "") cout << "Premise " << (i + 1) << ": " << error << endl;
//        else made++;
//    }
//    if (error == "") {
//        error = readFormula(conclusionText, conclusion);
//        if (error != "") cout << "Conclusion: " << error << endl;
//    }
//    if (error != "") {
//        for (int i = 0; i < made; i++) freeTree(premises[i]);
//        parsedOk = false;
//        return false;
//    }
//
//    bool seen[256];
//    for (int i = 0; i < 256; i++) seen[i] = false;
//    for (int i = 0; i < premiseCount; i++) markVariables(premises[i], seen);
//    markVariables(conclusion, seen);
//
//    char vars[MAX_VARS];
//    int varCount = 0;
//    for (int c = 0; c < 256; c++) {
//        if (seen[c] && varCount < MAX_VARS) { vars[varCount] = (char)c; varCount++; }
//    }
//
//    cout << "Argument:" << endl;
//    for (int i = 0; i < premiseCount; i++)
//        cout << "  P" << (i + 1) << " = " << treeToString(premises[i]) << endl;
//    cout << "  C  = " << treeToString(conclusion) << "   (Conclusion)" << endl << endl;
//
//    int rows = 1;
//    for (int i = 0; i < varCount; i++) rows *= 2;
//
//    bool valid = true;
//    int criticalRows = 0;
//    int counterRowIndex = -1;
//
//    for (int row = 0; row < rows; row++) {
//        bool values[256] = { false };
//        for (int i = 0; i < varCount; i++) {
//            int bitFromLeft = varCount - 1 - i;
//            values[(unsigned char)vars[i]] = (((row >> bitFromLeft) & 1) == 0);
//        }
//
//        bool allPremisesTrue = true;
//        for (int i = 0; i < premiseCount; i++) {
//            if (!evaluate(premises[i], values)) allPremisesTrue = false;
//        }
//        bool c = evaluate(conclusion, values);
//
//        if (allPremisesTrue) {
//            criticalRows++;
//            if (!c) {
//                valid = false;
//                if (counterRowIndex == -1) counterRowIndex = row;
//            }
//        }
//    }
//
//    if (valid) {
//        cout << "Result: Valid" << endl;
//        cout << "Explanation: In all " << criticalRows << " critical row(s) where every premise is true, the conclusion is also true.\n" << endl;
//
//        cout << "  ";
//        for (int i = 0; i < varCount; i++) cout << vars[i] << " ";
//        cout << "|";
//        for (int i = 0; i < premiseCount; i++) cout << " P" << (i + 1) << (i + 1 >= 10 ? "" : " ");
//        cout << "| C  |" << endl;
//
//        cout << "  ";
//        for (int i = 0; i < varCount; i++) cout << "--";
//        cout << "+";
//        for (int i = 0; i < premiseCount; i++) cout << "----";
//        cout << "+----+" << endl;
//
//        for (int row = 0; row < rows; row++) {
//            bool values[256] = { false };
//            cout << "  ";
//            for (int i = 0; i < varCount; i++) {
//                int bitFromLeft = varCount - 1 - i;
//                bool isTrue = ((row >> bitFromLeft) & 1) == 0;
//                values[(unsigned char)vars[i]] = isTrue;
//                cout << (isTrue ? "T" : "F") << " ";
//            }
//            cout << "|";
//
//            bool allPremisesTrue = true;
//            for (int i = 0; i < premiseCount; i++) {
//                bool v = evaluate(premises[i], values);
//                if (!v) allPremisesTrue = false;
//                cout << " " << (v ? "T" : "F") << (i + 1 >= 10 ? "" : " ") << " ";
//            }
//            bool c = evaluate(conclusion, values);
//            cout << "| " << (c ? "T" : "F") << "  |";
//
//            if (allPremisesTrue) {
//                cout << "  <-- Critical Row (All premises true)";
//            }
//            cout << endl;
//        }
//    }
//    else {
//        cout << "Result: False" << endl;
//        cout << "Explanation: The argument is invalid. Counterexample row (all premises true, conclusion false):\n" << endl;
//
//        cout << "  ";
//        for (int i = 0; i < varCount; i++) cout << vars[i] << " ";
//        cout << "|";
//        for (int i = 0; i < premiseCount; i++) cout << " P" << (i + 1) << (i + 1 >= 10 ? "" : " ");
//        cout << "| C  |" << endl;
//
//        cout << "  ";
//        for (int i = 0; i < varCount; i++) cout << "--";
//        cout << "+";
//        for (int i = 0; i < premiseCount; i++) cout << "----";
//        cout << "+----+" << endl;
//
//        bool values[256] = { false };
//        cout << "  ";
//        for (int i = 0; i < varCount; i++) {
//            int bitFromLeft = varCount - 1 - i;
//            bool isTrue = ((counterRowIndex >> bitFromLeft) & 1) == 0;
//            values[(unsigned char)vars[i]] = isTrue;
//            cout << (isTrue ? "T" : "F") << " ";
//        }
//        cout << "|";
//        for (int i = 0; i < premiseCount; i++) {
//            bool v = evaluate(premises[i], values);
//            cout << " " << (v ? "T" : "F") << (i + 1 >= 10 ? "" : " ") << " ";
//        }
//        bool c = evaluate(conclusion, values);
//        cout << "| " << (c ? "T" : "F") << "  |  <-- Counterexample Row" << endl;
//    }
//    cout << endl;
//
//    for (int i = 0; i < premiseCount; i++) freeTree(premises[i]);
//    freeTree(conclusion);
//    return valid;
//}
//
//// ---------------------------------------------------------------------------
////  Predefined Rules & Test Cases
//// ---------------------------------------------------------------------------
//struct ArgumentDef {
//    string title;
//    int premiseCount;
//    string premises[2];
//    string conclusion;
//    bool expectedValid;
//};
//
//ArgumentDef nineRules[9] = {
//    {"Modus Ponens",           2, {"p → q", "p"},                "q",       true},
//    {"Modus Tollens",          2, {"p → q", "¬q"},               "¬p",      true},
//    {"Hypothetical Syllogism", 2, {"p → q", "q → r"},            "p → r",   true},
//    {"Disjunctive Syllogism",  2, {"p ∨ q", "¬p"},               "q",       true},
//    {"Addition",               1, {"p", ""},                     "p ∨ q",   true},
//    {"Simplification",         1, {"p ∧ q", ""},                 "p",       true},
//    {"Conjunction",            2, {"p", "q"},                    "p ∧ q",   true},
//    {"Resolution",             2, {"p ∨ q", "¬p ∨ r"},           "q ∨ r",   true},
//    {"Constructive Dilemma",   2, {"(p → q) ∧ (r → s)", "p ∨ r"}, "q ∨ s",  true}
//};
//
//ArgumentDef requiredTests[6] = {
//    {"Test Case 1 - Modus Ponens",           2, {"p → q", "p"},      "q",     true},
//    {"Test Case 2 - Hypothetical Syllogism", 2, {"p → q", "q → r"},  "p → r", true},
//    {"Test Case 3 - Disjunctive Syllogism",  2, {"p ∨ q", "¬p"},     "q",     true},
//    {"Test Case 4 - Fallacy of Affirming Consequent", 2, {"p → q", "q"}, "p", false},
//    {"Test Case 5 - Fallacy of Denying Antecedent",   2, {"p → q", "¬p"}, "¬q", false},
//    {"Test Case 6 - Constructive Dilemma",   2, {"(p → q) ∧ (r → s)", "p ∨ r"}, "q ∨ s", true}
//};
//
//void runList(ArgumentDef list[], int count) {
//    int matches = 0;
//    for (int i = 0; i < count; i++) {
//        cout << "======================================================================" << endl;
//        cout << "  " << list[i].title << endl;
//        cout << "======================================================================" << endl;
//        bool parsedOk = true;
//        bool valid = checkArgument(list[i].premiseCount, list[i].premises, list[i].conclusion, parsedOk);
//        bool agrees = parsedOk && (valid == list[i].expectedValid);
//        if (agrees) matches++;
//        cout << "Expected: " << (list[i].expectedValid ? "Valid" : "False")
//            << "   |   Computed: " << (valid ? "Valid" : "False")
//            << "   |   Status: " << (agrees ? "PASS" : "FAIL") << "\n" << endl;
//    }
//    cout << "Summary: " << matches << " of " << count << " test cases passed.\n" << endl;
//}
//
//// ---------------------------------------------------------------------------
////  Interactive Custom Argument Entry
//// ---------------------------------------------------------------------------
//bool readLineWithPrompt(const string& prompt, string& line) {
//    cout << prompt;
//    if (!getline(cin, line)) return false;
//    line = trimText(line);
//    return true;
//}
//
//void enterOwnArgument() {
//    string text;
//    int count = 0;
//
//    while (true) {
//        if (!readLineWithPrompt("Number of premises (1-" + to_string(MAX_PREMISES) + "): ", text)) return;
//        count = 0;
//        bool onlyDigits = (text.length() > 0 && text.length() <= 3);
//        for (int i = 0; i < (int)text.length(); i++) {
//            if (text[i] < '0' || text[i] > '9') onlyDigits = false;
//            else count = count * 10 + (text[i] - '0');
//        }
//        if (onlyDigits && count >= 1 && count <= MAX_PREMISES) break;
//        cout << "Please enter a whole number between 1 and " << MAX_PREMISES << "." << endl;
//    }
//
//    string premises[MAX_PREMISES];
//    for (int i = 0; i < count; i++) {
//        while (true) {
//            if (!readLineWithPrompt("Premise " + to_string(i + 1) + ": ", text)) return;
//            Node* tree = NULL;
//            string error = readFormula(text, tree);
//            if (error == "") { freeTree(tree); break; }
//            cout << error << " Try again." << endl;
//        }
//        premises[i] = text;
//    }
//
//    string conclusion;
//    while (true) {
//        if (!readLineWithPrompt("Conclusion: ", conclusion)) return;
//        Node* tree = NULL;
//        string error = readFormula(conclusion, tree);
//        if (error == "") { freeTree(tree); break; }
//        cout << error << " Try again." << endl;
//    }
//
//    cout << endl;
//    bool parsedOk = true;
//    checkArgument(count, premises, conclusion, parsedOk);
//}
//
//int main() {
//#ifdef _WIN32
//    SetConsoleCP(65001);       // Set Windows console input to UTF-8
//    SetConsoleOutputCP(65001); // Set Windows console output to UTF-8
//#endif
//
//    cout << "======================================================================" << endl;
//    cout << "                 Rules of Inference Tester (Q1 Part b)                " << endl;
//    cout << "======================================================================" << endl;
//    cout << "Operators: " << NOT_SYM << " (NOT)  " << AND_SYM << " (AND)  " << OR_SYM << " (OR)  "
//        << IMP_SYM << " (IMPLIES)  " << IFF_SYM << " (BICONDITIONAL)   constants: T F" << endl;
//    cout << "Aliases allowed: ~ or !   & or ^   | or v   ->   <->" << endl;
//    cout << "Note: Variables must be single letters (e.g., p, q, r, s)." << endl;
//
//    string choice;
//    while (true) {
//        cout << "\nMenu:" << endl;
//        cout << "  1. Enter my own argument (premises + conclusion)" << endl;
//        cout << "  2. Test all 9 standard rules of inference" << endl;
//        cout << "  3. Run the 6 required test cases" << endl;
//        cout << "  4. Exit" << endl;
//        if (!readLineWithPrompt("Choice: ", choice)) break;
//        if (choice == "1") { cout << endl; enterOwnArgument(); }
//        else if (choice == "2") { cout << endl; runList(nineRules, 9); }
//        else if (choice == "3") { cout << endl; runList(requiredTests, 6); }
//        else if (choice == "4") break;
//        else cout << "Invalid choice. Please enter 1, 2, 3, or 4." << endl;
//    }
//    system("pause");
//    return 0;
//}
