//// ============================================================================
////  PART (a): Propositional Logic Simplification Using Logic Laws
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
//// Delete a whole tree
//void freeTree(Node* n) {
//    if (n == NULL) return;
//    freeTree(n->left);
//    freeTree(n->right);
//    delete n;
//}
//
//// Make a full copy of a tree
//Node* cloneTree(Node* n) {
//    if (n == NULL) return NULL;
//    return makeNode(n->type, n->name, cloneTree(n->left), cloneTree(n->right));
//}
//
//// Are two trees exactly the same?
//bool sameTree(Node* a, Node* b) {
//    if (a == NULL && b == NULL) return true;
//    if (a == NULL || b == NULL) return false;
//    if (a->type != b->type) return false;
//    if (a->type == NODE_VAR && a->name != b->name) return false;
//    return sameTree(a->left, b->left) && sameTree(a->right, b->right);
//}
//
//int countNodes(Node* n) {
//    if (n == NULL) return 0;
//    return 1 + countNodes(n->left) + countNodes(n->right);
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
//// Work out the truth value of a tree
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
//    if (startsAt(text, pos, "^")) { type = TOK_AND;   length = 1; return true; } // Added ^ for AND
//    if (startsAt(text, pos, OR_SYM)) { type = TOK_OR;    length = OR_SYM.length(); return true; }
//    if (startsAt(text, pos, "|")) { type = TOK_OR;    length = 1; return true; }
//    if (startsAt(text, pos, "V")) { type = TOK_OR;    length = 1; return true; } // Added V for OR
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
//        if (tokenCount >= MAX_TOKENS) return "Rejected: the equation is far too long (maximum is 20 tokens).";
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
//    if (tokenCount == 0) return "Rejected: the equation is empty.";
//    bool waitingForOperand = true;
//    for (int i = 0; i < tokenCount; i++) {
//        int type = tokens[i].type;
//        if (waitingForOperand) {
//            if (type == TOK_VAR || type == TOK_CONST) {
//                waitingForOperand = false;
//            }
//            else if (type == TOK_NOT || type == TOK_OPEN) {
//                // still waiting
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
//int countNonBracketTokens() {
//    int count = 0;
//    for (int i = 0; i < tokenCount; i++) {
//        if (tokens[i].type != TOK_OPEN && tokens[i].type != TOK_CLOSE) count++;
//    }
//    return count;
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
//// ---------------------------------------------------------------------------
////  Logic Laws & Simplifier Implementation
//// ---------------------------------------------------------------------------
//const int MAX_STEPS = 3000;
//const int MAX_PLAN_NODES = 50;
//const int MAX_PLAN_TRIES = 8000;
//const int MAX_PLAN_LENGTH = 3;
//
//const int RULE_CONTRAPOSITIVE = 1;
//const int RULE_BICONDITIONAL = 2;
//const int RULE_CONDITIONAL = 3;
//const int RULE_DOUBLE_NEG = 4;
//const int RULE_NEG_CONSTANT = 5;
//const int RULE_IDENTITY = 6;
//const int RULE_DOMINATION = 7;
//const int RULE_IDEMPOTENT = 8;
//const int RULE_COMPLEMENT = 9;
//const int RULE_ABSORPTION = 10;
//const int RULE_DEMORGAN = 11;
//
//string lawName(int rule) {
//    if (rule == RULE_CONTRAPOSITIVE) return "Contrapositive Law";
//    if (rule == RULE_BICONDITIONAL)  return "Biconditional Law";
//    if (rule == RULE_CONDITIONAL)    return "Conditional Law";
//    if (rule == RULE_DOUBLE_NEG)     return "Double Negation Law";
//    if (rule == RULE_NEG_CONSTANT)   return "Negation Law (complement of a constant)";
//    if (rule == RULE_IDENTITY)       return "Identity Law";
//    if (rule == RULE_DOMINATION)     return "Domination Law";
//    if (rule == RULE_IDEMPOTENT)     return "Idempotent Law";
//    if (rule == RULE_COMPLEMENT)     return "Negation (Complement) Law";
//    if (rule == RULE_ABSORPTION)     return "Absorption Law";
//    return "De Morgan's Law";
//}
//
//int oppositeOperator(int type) {
//    if (type == NODE_AND) return NODE_OR;
//    return NODE_AND;
//}
//
//bool isNegationOf(Node* a, Node* b) {
//    return a->type == NODE_NOT && sameTree(a->left, b);
//}
//
//bool areComplements(Node* a, Node* b) {
//    return isNegationOf(a, b) || isNegationOf(b, a);
//}
//
//void detach(Node* n, Node* keep) {
//    if (n == NULL) return;
//    if (n->left == keep) n->left = NULL;
//    else detach(n->left, keep);
//    if (n->right == keep) n->right = NULL;
//    else detach(n->right, keep);
//}
//
//Node* keepOnly(Node* n, Node* keep) {
//    detach(n, keep);
//    freeTree(n);
//    return keep;
//}
//
//Node* replaceByConstant(Node* n, bool value) {
//    freeTree(n);
//    return makeConst(value);
//}
//
//bool isContrapositiveOf(Node* y, Node* x) {
//    if (x->type != NODE_IMP || y->type != NODE_IMP) return false;
//    return areComplements(y->left, x->right) && areComplements(y->right, x->left);
//}
//
//bool ruleContrapositive(Node*& n) {
//    if (n->type != NODE_AND && n->type != NODE_OR) return false;
//    Node* l = n->left;
//    Node* r = n->right;
//    if (l->type != NODE_IMP || r->type != NODE_IMP) return false;
//    if (sameTree(l, r)) return false;
//    if (isContrapositiveOf(r, l)) {
//        freeTree(n->right);
//        n->right = cloneTree(l);
//        return true;
//    }
//    if (isContrapositiveOf(l, r)) {
//        freeTree(n->left);
//        n->left = cloneTree(r);
//        return true;
//    }
//    return false;
//}
//
//bool ruleBiconditional(Node*& n) {
//    if (n->type != NODE_IFF) return false;
//    Node* p = n->left;
//    Node* q = n->right;
//    Node* result = makeBin(NODE_AND, makeBin(NODE_IMP, p, q),
//        makeBin(NODE_IMP, cloneTree(q), cloneTree(p)));
//    n->left = NULL;
//    n->right = NULL;
//    delete n;
//    n = result;
//    return true;
//}
//
//bool ruleConditional(Node*& n) {
//    if (n->type != NODE_IMP) return false;
//    Node* p = n->left;
//    Node* q = n->right;
//    Node* result = makeBin(NODE_OR, makeNot(p), q);
//    n->left = NULL;
//    n->right = NULL;
//    delete n;
//    n = result;
//    return true;
//}
//
//bool ruleDoubleNegation(Node*& n) {
//    if (n->type == NODE_NOT && n->left->type == NODE_NOT) {
//        n = keepOnly(n, n->left->left);
//        return true;
//    }
//    return false;
//}
//
//bool ruleNegateConstant(Node*& n) {
//    if (n->type == NODE_NOT && n->left->type == NODE_T) { n = replaceByConstant(n, false); return true; }
//    if (n->type == NODE_NOT && n->left->type == NODE_F) { n = replaceByConstant(n, true);  return true; }
//    return false;
//}
//
//bool ruleIdentity(Node*& n) {
//    if (n->type == NODE_AND) {
//        if (n->right->type == NODE_T) { n = keepOnly(n, n->left);  return true; }
//        if (n->left->type == NODE_T) { n = keepOnly(n, n->right); return true; }
//    }
//    if (n->type == NODE_OR) {
//        if (n->right->type == NODE_F) { n = keepOnly(n, n->left);  return true; }
//        if (n->left->type == NODE_F) { n = keepOnly(n, n->right); return true; }
//    }
//    return false;
//}
//
//bool ruleDomination(Node*& n) {
//    if (n->type == NODE_OR && (n->left->type == NODE_T || n->right->type == NODE_T)) {
//        n = replaceByConstant(n, true);
//        return true;
//    }
//    if (n->type == NODE_AND && (n->left->type == NODE_F || n->right->type == NODE_F)) {
//        n = replaceByConstant(n, false);
//        return true;
//    }
//    return false;
//}
//
//bool ruleIdempotent(Node*& n) {
//    if ((n->type == NODE_AND || n->type == NODE_OR) && sameTree(n->left, n->right)) {
//        n = keepOnly(n, n->left);
//        return true;
//    }
//    return false;
//}
//
//bool ruleComplement(Node*& n) {
//    if ((n->type == NODE_AND || n->type == NODE_OR) && areComplements(n->left, n->right)) {
//        n = replaceByConstant(n, n->type == NODE_OR);
//        return true;
//    }
//    return false;
//}
//
//bool ruleAbsorption(Node*& n) {
//    if (n->type != NODE_AND && n->type != NODE_OR) return false;
//    int other = oppositeOperator(n->type);
//    for (int side = 0; side < 2; side++) {
//        Node* x = n->left;
//        Node* y = n->right;
//        if (side == 1) { x = n->right; y = n->left; }
//        if (y->type == other && (sameTree(y->left, x) || sameTree(y->right, x))) {
//            n = keepOnly(n, x);
//            return true;
//        }
//    }
//    return false;
//}
//
//bool ruleDeMorgan(Node*& n) {
//    if (n->type == NODE_NOT && (n->left->type == NODE_AND || n->left->type == NODE_OR)) {
//        Node* inner = n->left;
//        Node* a = inner->left;
//        Node* b = inner->right;
//        Node* result = makeBin(oppositeOperator(inner->type), makeNot(a), makeNot(b));
//        inner->left = NULL;
//        inner->right = NULL;
//        delete inner;
//        n->left = NULL;
//        delete n;
//        n = result;
//        return true;
//    }
//    return false;
//}
//
//bool applyRule(int rule, Node*& n) {
//    if (rule == RULE_CONTRAPOSITIVE) return ruleContrapositive(n);
//    if (rule == RULE_BICONDITIONAL)  return ruleBiconditional(n);
//    if (rule == RULE_CONDITIONAL)    return ruleConditional(n);
//    if (rule == RULE_DOUBLE_NEG)     return ruleDoubleNegation(n);
//    if (rule == RULE_NEG_CONSTANT)   return ruleNegateConstant(n);
//    if (rule == RULE_IDENTITY)       return ruleIdentity(n);
//    if (rule == RULE_DOMINATION)     return ruleDomination(n);
//    if (rule == RULE_IDEMPOTENT)     return ruleIdempotent(n);
//    if (rule == RULE_COMPLEMENT)     return ruleComplement(n);
//    if (rule == RULE_ABSORPTION)     return ruleAbsorption(n);
//    return ruleDeMorgan(n);
//}
//
//bool applyAnywhere(Node*& n, int rule) {
//    if (applyRule(rule, n)) return true;
//    if (n->left != NULL && applyAnywhere(n->left, rule)) return true;
//    if (n->right != NULL && applyAnywhere(n->right, rule)) return true;
//    return false;
//}
//
//bool hasImplication(Node* n) {
//    if (n == NULL) return false;
//    if (n->type == NODE_IMP || n->type == NODE_IFF) return true;
//    return hasImplication(n->left) || hasImplication(n->right);
//}
//
//bool simplifyStep(Node*& root, int& rule) {
//    for (int r = RULE_DOUBLE_NEG; r <= RULE_DEMORGAN; r++) {
//        if (applyAnywhere(root, r)) {
//            rule = r;
//            return true;
//        }
//    }
//    return false;
//}
//
//string moveName(int kind) {
//    if (kind == 0) return "Commutative Law";
//    if (kind == 3) return "Distributive Law";
//    return "Associative Law";
//}
//
//bool doMove(Node*& n, int kind) {
//    if (n->type != NODE_AND && n->type != NODE_OR) return false;
//    int op = n->type;
//    int other = oppositeOperator(op);
//
//    if (kind == 0) {
//        Node* temp = n->left;
//        n->left = n->right;
//        n->right = temp;
//        return true;
//    }
//    if (kind == 1) {
//        if (n->left->type != op) return false;
//        Node* old = n->left;
//        Node* a = old->left;
//        Node* b = old->right;
//        Node* c = n->right;
//        Node* result = makeBin(op, a, makeBin(op, b, c));
//        old->left = NULL; old->right = NULL; delete old;
//        n->left = NULL;   n->right = NULL;   delete n;
//        n = result;
//        return true;
//    }
//    if (kind == 2) {
//        if (n->right->type != op) return false;
//        Node* old = n->right;
//        Node* a = n->left;
//        Node* b = old->left;
//        Node* c = old->right;
//        Node* result = makeBin(op, makeBin(op, a, b), c);
//        old->left = NULL; old->right = NULL; delete old;
//        n->left = NULL;   n->right = NULL;   delete n;
//        n = result;
//        return true;
//    }
//    if (kind == 3) {
//        if (n->right->type != other) return false;
//        Node* old = n->right;
//        Node* a = n->left;
//        Node* b = old->left;
//        Node* c = old->right;
//        Node* result = makeBin(other, makeBin(op, a, b), makeBin(op, cloneTree(a), c));
//        old->left = NULL; old->right = NULL; delete old;
//        n->left = NULL;   n->right = NULL;   delete n;
//        n = result;
//        return true;
//    }
//    return false;
//}
//
//bool moveAt(Node*& n, int target, int kind, int& counter, bool& reached) {
//    if (n == NULL) return false;
//    if (counter == target) {
//        reached = true;
//        return doMove(n, kind);
//    }
//    counter++;
//    if (moveAt(n->left, target, kind, counter, reached)) return true;
//    if (reached) return false;
//    return moveAt(n->right, target, kind, counter, reached);
//}
//
//int planNode[MAX_PLAN_LENGTH];
//int planKind[MAX_PLAN_LENGTH];
//int planLength = 0;
//int planTriesLeft = 0;
//
//bool searchPlan(Node* tree, int movesLeft, int level, int originalSize) {
//    int total = countNodes(tree);
//    for (int node = 0; node < total; node++) {
//        for (int kind = 0; kind < 4; kind++) {
//            Node* next = cloneTree(tree);
//            int counter = 0;
//            bool reached = false;
//            if (!moveAt(next, node, kind, counter, reached)) {
//                freeTree(next);
//                continue;
//            }
//            planTriesLeft--;
//            if (planTriesLeft < 0) { freeTree(next); return false; }
//            planNode[level] = node;
//            planKind[level] = kind;
//
//            if (movesLeft == 1) {
//                Node* test = cloneTree(next);
//                int rule;
//                if (simplifyStep(test, rule)) {
//                    int guard = 0;
//                    while (guard < MAX_STEPS && simplifyStep(test, rule)) guard++;
//                    if (countNodes(test) < originalSize) {
//                        planLength = level + 1;
//                        freeTree(test);
//                        freeTree(next);
//                        return true;
//                    }
//                }
//                freeTree(test);
//            }
//            else {
//                if (searchPlan(next, movesLeft - 1, level + 1, originalSize)) {
//                    freeTree(next);
//                    return true;
//                }
//            }
//            freeTree(next);
//        }
//    }
//    return false;
//}
//
//bool findPlan(Node* root) {
//    int size = countNodes(root);
//    if (size > MAX_PLAN_NODES) return false;
//    planTriesLeft = MAX_PLAN_TRIES;
//    for (int length = 1; length <= MAX_PLAN_LENGTH; length++) {
//        if (searchPlan(root, length, 0, size)) return true;
//        if (planTriesLeft < 0) break;
//    }
//    return false;
//}
//
//void printStep(int number, Node* root, const string& law) {
//    cout << "Step " << number << ": " << treeToString(root) << "\t\t[" << law << "]" << endl;
//}
//
//int simplifyExpression(Node*& root, bool& hitLimit) {
//    int steps = 0;
//    hitLimit = false;
//    while (true) {
//        if (steps >= MAX_STEPS) { hitLimit = true; break; }
//
//        if (hasImplication(root)) {
//            bool done = false;
//            for (int rule = RULE_CONTRAPOSITIVE; rule <= RULE_CONDITIONAL && !done; rule++) {
//                if (applyAnywhere(root, rule)) {
//                    steps++;
//                    printStep(steps, root, lawName(rule));
//                    done = true;
//                }
//            }
//            if (done) continue;
//            break;
//        }
//
//        int rule;
//        if (simplifyStep(root, rule)) {
//            steps++;
//            printStep(steps, root, lawName(rule));
//            continue;
//        }
//
//        if (findPlan(root)) {
//            for (int i = 0; i < planLength; i++) {
//                int counter = 0;
//                bool reached = false;
//                moveAt(root, planNode[i], planKind[i], counter, reached);
//                steps++;
//                printStep(steps, root, moveName(planKind[i]));
//            }
//            continue;
//        }
//        break;
//    }
//    return steps;
//}
//
//bool truthTableCheck(Node* original, Node* simplified) {
//    bool seen[256];
//    for (int i = 0; i < 256; i++) seen[i] = false;
//    markVariables(original, seen);
//
//    char vars[4];
//    int varCount = 0;
//    for (int c = 0; c < 256; c++) {
//        if (seen[c] && varCount < 4) { vars[varCount] = (char)c; varCount++; }
//    }
//
//    cout << "\nTruth table verification:" << endl << "  ";
//    for (int i = 0; i < varCount; i++) cout << vars[i] << " ";
//    cout << "| Original | Simplified | Match" << endl;
//    cout << "------------------------------------" << endl;
//
//    bool allEqual = true;
//    int rows = 1;
//    for (int i = 0; i < varCount; i++) rows = rows * 2;
//
//    for (int row = 0; row < rows; row++) {
//        bool values[256];
//        for (int i = 0; i < 256; i++) values[i] = false;
//        cout << "  ";
//        for (int i = 0; i < varCount; i++) {
//            int bitFromLeft = varCount - 1 - i;
//            bool isTrue = ((row >> bitFromLeft) & 1) == 0;
//            values[(unsigned char)vars[i]] = isTrue;
//            cout << (isTrue ? "T" : "F") << " ";
//        }
//        bool a = evaluate(original, values);
//        bool b = evaluate(simplified, values);
//        if (a != b) allEqual = false;
//        cout << "| " << (a ? "T" : "F") << "        | " << (b ? "T" : "F") << "          | "
//            << (a == b ? "yes" : "NO") << endl;
//    }
//    return allEqual;
//}
//
//int main() {
//#ifdef _WIN32
//    SetConsoleCP(65001);       // Set Windows console input to UTF-8
//    SetConsoleOutputCP(65001); // Set Windows console output to UTF-8
//#endif
//
//    cout << "==================================================" << endl;
//    cout << "       Propositional Logic Simplifier (Q1 Part a) " << endl;
//    cout << "==================================================" << endl;
//    cout << "Operators: " << NOT_SYM << " (NOT)  " << AND_SYM << " (AND)  " << OR_SYM << " (OR)  "
//        << IMP_SYM << " (IMPLIES)  " << IFF_SYM << " (BICONDITIONAL)   constants: T F" << endl;
//    cout << "Aliases allowed: ~ or !   &   |   ->   <->" << endl;
//    cout << "Constraints: max 4 variables, 10 to 20 tokens. Type 'quit' to exit.\n" << endl;
//
//    string line;
//    while (true) {
//        cout << "Enter equation: ";
//        if (!getline(cin, line)) break;
//        line = trimText(line);
//        if (line == "quit" || line == "exit") break;
//        if (line.empty()) continue;
//
//        string error = tokenize(line);
//        if (error == "") error = checkParentheses();
//        if (error == "") error = checkSyntax();
//        if (error == "" && countDistinctVariables() > 4) error = "Rejected: more than 4 distinct variables.";
//        if (error == "") {
//            int count = countNonBracketTokens();
//            if (count < 10) error = "Rejected: equation has " + to_string(count) + " tokens (minimum is 10).";
//            else if (count > 20) error = "Rejected: equation has " + to_string(count) + " tokens (maximum is 20).";
//        }
//        if (error != "") {
//            cout << error << "\n" << endl;
//            continue;
//        }
//
//        Node* original = parseTokens();
//        Node* current = cloneTree(original);
//
//        cout << "Original: " << line << endl;
//        cout << "Parsed as: " << treeToString(original) << endl << endl;
//
//        bool hitLimit = false;
//        int steps = simplifyExpression(current, hitLimit);
//        if (steps == 0) cout << "(no law is applicable - the equation is already simplified)" << endl;
//        if (hitLimit) cout << "(step limit reached - stopping here)" << endl;
//
//        cout << "\nFinal: " << treeToString(current) << endl;
//        bool equivalent = truthTableCheck(original, current);
//        cout << "\nTruth-table check: " << (equivalent ? "EQUIVALENT" : "NOT EQUIVALENT") << "\n" << endl;
//
//        freeTree(original);
//        freeTree(current);
//    }
//    system("pause");
//    return 0;
//}
