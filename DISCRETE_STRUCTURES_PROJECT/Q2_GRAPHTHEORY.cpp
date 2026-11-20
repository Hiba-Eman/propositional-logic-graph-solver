// ============================================================================
//  Q2: Graph Theory - one menu-driven program with ONE graph that is kept in an
//  adjacency matrix and changed step by step.
//    1. BFS two-colouring (bipartite test) and a repair by subdividing an edge
//    2. Hamiltonian path / circuit by BACKTRACKING and a repair by adding an edge
//    3. Edge weights, joining components, Kruskal's algorithm with Union-Find
// ============================================================================
#include <iostream>
#include <string>
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;

const int MAX_V = 40;   // the graph can have at most 40 vertices (new vertices included)

// ---------------------------------------------------------------------------
//  The graph: vertex names, adjacency matrix (1 = edge, 0 = no edge) and the
//  weights of the edges (0 = no edge). The graph is undirected, so both
//  matrices are always symmetric.
// ---------------------------------------------------------------------------
struct Graph {
    int n;                       // number of vertices
    string labels[MAX_V];        // names of the vertices: A, B, C, ...
    int adj[MAX_V][MAX_V];
    int wt[MAX_V][MAX_V];
};

void clearGraph(Graph& g) {
    g.n = 0;
    for (int i = 0; i < MAX_V; i++) {
        for (int j = 0; j < MAX_V; j++) {
            g.adj[i][j] = 0;
            g.wt[i][j] = 0;
        }
    }
}

// Adds (present = true) or removes (present = false) the edge u-v in BOTH directions
void setEdge(Graph& g, int u, int v, bool present) {
    if (present) {
        g.adj[u][v] = 1;
        g.adj[v][u] = 1;
    }
    else {
        g.adj[u][v] = 0;
        g.adj[v][u] = 0;
        g.wt[u][v] = 0;
        g.wt[v][u] = 0;
    }
}

int degree(const Graph& g, int u) {
    int d = 0;
    for (int v = 0; v < g.n; v++) d = d + g.adj[u][v];
    return d;
}

int edgeCount(const Graph& g) {
    int count = 0;
    for (int u = 0; u < g.n; u++)
        for (int v = u + 1; v < g.n; v++)
            count = count + g.adj[u][v];
    return count;
}

// Adds a new vertex without edges. Returns its number, or -1 if the graph is full.
int addVertex(Graph& g, const string& label) {
    if (g.n >= MAX_V) return -1;
    g.labels[g.n] = label;
    g.n++;
    return g.n - 1;
}

// ---------------------------------------------------------------------------
//  Small text helpers
// ---------------------------------------------------------------------------
string toUpperText(string s) {
    for (int i = 0; i < (int)s.length(); i++) {
        if (s[i] >= 'a' && s[i] <= 'z') s[i] = (char)(s[i] - 'a' + 'A');
    }
    return s;
}

string trim(const string& s) {
    int start = 0;
    int end = (int)s.length() - 1;
    while (start <= end && (s[start] == ' ' || s[start] == '\t' || s[start] == '\r' || s[start] == '\n')) start++;
    while (end >= start && (s[end] == ' ' || s[end] == '\t' || s[end] == '\r' || s[end] == '\n')) end--;
    return s.substr(start, end - start + 1);
}

// Cuts a line into words (spaces, commas and '-' separate the words).
// The first 'maxWords' words are stored in 'words'. Returns the number of words found.
int splitWords(const string& line, string words[], int maxWords) {
    int count = 0;
    string current = "";
    for (int i = 0; i <= (int)line.length(); i++) {
        char c = (i < (int)line.length()) ? line[i] : ' ';
        if (c == ' ' || c == ',' || c == '-' || c == '\t' || c == '\r') {
            if (current != "") {
                if (count < maxWords) words[count] = current;
                count++;
                current = "";
            }
        }
        else {
            current = current + c;
        }
    }
    return count;
}

bool isWholeNumber(const string& s) {
    if (s.length() == 0 || s.length() > 9) return false;
    for (int i = 0; i < (int)s.length(); i++) {
        if (s[i] < '0' || s[i] > '9') return false;
    }
    return true;
}

int toNumber(const string& s) {
    int value = 0;
    for (int i = 0; i < (int)s.length(); i++) value = value * 10 + (s[i] - '0');
    return value;
}

// Shows the prompt and reads one line. Returns false when the input has ended.
bool readLine(const string& prompt, string& line) {
    cout << prompt;
    if (!getline(cin, line)) return false;
    return true;
}

// Asks for a whole number between low and high until the user gives one.
// Returns false only when the input has ended.
bool readNumber(const string& prompt, int low, int high, int& value) {
    string line;
    while (true) {
        if (!readLine(prompt, line)) return false;
        line = trim(line);
        if (isWholeNumber(line)) {
            int v = toNumber(line);
            if (v >= low && v <= high) {
                value = v;
                return true;
            }
        }
        cout << "  Invalid input: please enter a whole number from " << low << " to " << high << "." << endl;
    }
}

// Writes a number or text right-aligned in a field of 'width' characters
string alignRight(const string& s, int width) {
    string result = s;
    while ((int)result.length() < width) result = " " + result;
    return result;
}

// ---------------------------------------------------------------------------
//  Finding vertices and showing the graph
// ---------------------------------------------------------------------------
// Number of the vertex with this name (any letter case), or -1
int findVertex(const Graph& g, const string& label) {
    string wanted = toUpperText(label);
    for (int i = 0; i < g.n; i++) {
        if (toUpperText(g.labels[i]) == wanted) return i;
    }
    return -1;
}

string edgeName(const Graph& g, int u, int v) {
    return g.labels[u] + "-" + g.labels[v];
}

// Prints the adjacency matrix (or the weight matrix if weighted is true)
void printMatrix(const Graph& g, bool weighted) {
    int width = 1;
    for (int i = 0; i < g.n; i++) {
        if ((int)g.labels[i].length() > width) width = g.labels[i].length();
    }
    if (weighted) {
        for (int i = 0; i < g.n; i++)
            for (int j = 0; j < g.n; j++) {
                int digits = (int)to_string(g.wt[i][j]).length();
                if (digits > width) width = digits;
            }
    }
    width = width + 2;

    cout << alignRight(" ", width);
    for (int j = 0; j < g.n; j++) cout << alignRight(g.labels[j], width);
    cout << endl;
    for (int i = 0; i < g.n; i++) {
        cout << alignRight(g.labels[i], width);
        for (int j = 0; j < g.n; j++) {
            int value = weighted ? g.wt[i][j] : g.adj[i][j];
            cout << alignRight(to_string(value), width);
        }
        cout << endl;
    }
}

// Writes a list of vertices like {B, A, C}. If closeCircuit is true the first
// vertex is written again at the end.
string listToString(const Graph& g, const int list[], int length, bool closeCircuit) {
    string text = "{";
    for (int i = 0; i < length; i++) {
        if (i > 0) text = text + ", ";
        text = text + g.labels[list[i]];
    }
    if (closeCircuit && length > 0) text = text + ", " + g.labels[list[0]];
    return text + "}";
}

// ---------------------------------------------------------------------------
//  Reading the graph from the user (with checks)
// ---------------------------------------------------------------------------
bool readGraph(Graph& g) {
    clearGraph(g);
    int n = 0, m = 0;
    if (!readNumber("Enter number of vertices (1-26): ", 1, 26, n)) return false;

    while (true) {
        string line;
        if (!readLine("Enter " + to_string(n) + " vertex labels separated by spaces (or press Enter for A, B, C, ...): ", line))
            return false;
        string words[MAX_V];
        int wordCount = splitWords(trim(line), words, MAX_V);
        if (wordCount == 0) {
            for (int i = 0; i < n; i++) addVertex(g, string(1, (char)('A' + i)));
            break;
        }
        if (wordCount != n) {
            cout << "  Invalid: you must give exactly " << n << " labels." << endl;
            continue;
        }
        bool duplicate = false;
        for (int i = 0; i < n; i++)
            for (int j = i + 1; j < n; j++)
                if (toUpperText(words[i]) == toUpperText(words[j])) duplicate = true;
        if (duplicate) {
            cout << "  Invalid: labels must be different from each other." << endl;
            continue;
        }
        for (int i = 0; i < n; i++) addVertex(g, words[i]);
        break;
    }

    int maxEdges = n * (n - 1) / 2;
    if (!readNumber("Enter number of edges (0-" + to_string(maxEdges) + "): ", 0, maxEdges, m)) return false;

    for (int e = 1; e <= m; e++) {
        while (true) {
            string line;
            string example = g.labels[0];
            if (n > 1) example = example + " " + g.labels[1];
            if (!readLine("Edge " + to_string(e) + " (two vertex labels, e.g. " + example + "): ", line)) return false;

            string words[4];
            int wordCount = splitWords(line, words, 4);
            if (wordCount != 2) {
                cout << "  Invalid: enter exactly two vertex labels." << endl;
                continue;
            }
            int u = findVertex(g, words[0]);
            int v = findVertex(g, words[1]);
            if (u < 0 || v < 0) {
                cout << "  Invalid: unknown vertex \"" << (u < 0 ? words[0] : words[1]) << "\"." << endl;
                continue;
            }
            if (u == v) {
                cout << "  Invalid: a self-loop is not allowed." << endl;
                continue;
            }
            if (g.adj[u][v] == 1) {
                cout << "  Invalid: edge " << edgeName(g, u, v) << " was already entered." << endl;
                continue;
            }
            setEdge(g, u, v, true);
            break;
        }
    }
    return true;
}

// ---------------------------------------------------------------------------
//  Connected components (using BFS with a simple array as the queue)
// ---------------------------------------------------------------------------
int findComponents(const Graph& g, int component[]) {
    for (int i = 0; i < g.n; i++) component[i] = -1;
    int count = 0;
    for (int start = 0; start < g.n; start++) {
        if (component[start] != -1) continue;
        int queue[MAX_V];
        int head = 0, tail = 0;
        queue[tail++] = start;
        component[start] = count;
        while (head < tail) {
            int u = queue[head++];
            for (int v = 0; v < g.n; v++) {
                if (g.adj[u][v] == 1 && component[v] == -1) {
                    component[v] = count;
                    queue[tail++] = v;
                }
            }
        }
        count++;
    }
    return count;
}

bool isConnected(const Graph& g) {
    int component[MAX_V];
    return findComponents(g, component) <= 1;
}

string vertexSetToString(const Graph& g, const int numbers[], int value) {
    string text = "{";
    bool first = true;
    for (int i = 0; i < g.n; i++) {
        if (numbers[i] == value) {
            if (!first) text = text + ", ";
            text = text + g.labels[i];
            first = false;
        }
    }
    return text + "}";
}

// ---------------------------------------------------------------------------
//  BFS two-colouring: the bipartite test
// ---------------------------------------------------------------------------
struct ColourResult {
    bool bipartite;
    int colour[MAX_V];    // 0 or 1 (-1 = not coloured yet)
    int parent[MAX_V];    // the vertex from which BFS reached this vertex
    int conflictU;        // two neighbours with the same colour (if not bipartite)
    int conflictV;
};

void bfsTwoColour(const Graph& g, ColourResult& r) {
    r.bipartite = true;
    r.conflictU = -1;
    r.conflictV = -1;
    for (int i = 0; i < g.n; i++) {
        r.colour[i] = -1;
        r.parent[i] = -1;
    }

    for (int start = 0; start < g.n; start++) {
        if (r.colour[start] != -1) continue;
        r.colour[start] = 0;
        int queue[MAX_V];
        int head = 0, tail = 0;
        queue[tail++] = start;
        while (head < tail) {
            int u = queue[head++];
            for (int v = 0; v < g.n; v++) {
                if (g.adj[u][v] == 0) continue;
                if (r.colour[v] == -1) {
                    r.colour[v] = 1 - r.colour[u];
                    r.parent[v] = u;
                    queue[tail++] = v;
                }
                else if (r.colour[v] == r.colour[u]) {
                    r.bipartite = false;
                    r.conflictU = u;
                    r.conflictV = v;
                    return;
                }
            }
        }
    }
}

bool isBipartite(const Graph& g) {
    ColourResult r;
    bfsTwoColour(g, r);
    return r.bipartite;
}

void plainColours(const Graph& g, int colour[]) {
    for (int i = 0; i < g.n; i++) colour[i] = -1;
    for (int start = 0; start < g.n; start++) {
        if (colour[start] != -1) continue;
        colour[start] = 0;
        int queue[MAX_V];
        int head = 0, tail = 0;
        queue[tail++] = start;
        while (head < tail) {
            int u = queue[head++];
            for (int v = 0; v < g.n; v++) {
                if (g.adj[u][v] == 1 && colour[v] == -1) {
                    colour[v] = 1 - colour[u];
                    queue[tail++] = v;
                }
            }
        }
    }
}

void printTwoSets(const Graph& g, const int colour[]) {
    cout << "  Set 1: " << vertexSetToString(g, colour, 0) << endl;
    cout << "  Set 2: " << vertexSetToString(g, colour, 1) << endl;
}

int findOddCycle(const ColourResult& r, int cycle[], int& subA, int& subB) {
    int pathU[MAX_V], pathV[MAX_V];
    int lengthU = 0, lengthV = 0;
    int a = r.conflictU;
    int b = r.conflictV;
    pathU[lengthU++] = a;
    pathV[lengthV++] = b;
    while (a != b && r.parent[a] != -1 && r.parent[b] != -1) {
        a = r.parent[a];
        b = r.parent[b];
        pathU[lengthU++] = a;
        pathV[lengthV++] = b;
    }
    int length = 0;
    for (int i = lengthU - 1; i >= 0; i--) cycle[length++] = pathU[i];
    for (int i = 0; i < lengthV; i++) cycle[length++] = pathV[i];
    subA = pathV[lengthV - 1];
    subB = pathV[lengthV - 2];
    return length;
}

string newLabel(const Graph& g) {
    for (char c = 'A'; c <= 'Z'; c++) {
        if (findVertex(g, string(1, c)) < 0) return string(1, c);
    }
    int k = 1;
    while (findVertex(g, "V" + to_string(k)) >= 0) k++;
    return "V" + to_string(k);
}

int subdivideEdge(Graph& g, int u, int v) {
    int h = addVertex(g, newLabel(g));
    if (h < 0) return -1;
    setEdge(g, u, v, false);
    setEdge(g, u, h, true);
    setEdge(g, h, v, true);
    return h;
}

// ---------------------------------------------------------------------------
//  PART (a): bipartite check and repair
// ---------------------------------------------------------------------------
void partA(Graph& g) {
    cout << endl << "=== Part (a): Bipartite check (BFS two-colouring) ===" << endl;
    cout << "Adjacency matrix:" << endl;
    printMatrix(g, false);

    int component[MAX_V];
    int componentCount = findComponents(g, component);
    cout << endl << "Connected components: " << componentCount << " (every component is coloured)" << endl;

    ColourResult result;
    bfsTwoColour(g, result);
    if (result.bipartite) {
        cout << "Result: the graph IS bipartite. Disjoint vertex sets:" << endl;
        printTwoSets(g, result.colour);
        return;
    }

    cout << "Result: the graph is NOT bipartite." << endl;
    int round = 0;
    while (!result.bipartite) {
        round++;
        cout << endl << "BFS conflict: " << g.labels[result.conflictU] << " and " << g.labels[result.conflictV]
            << " are adjacent but both received colour " << result.colour[result.conflictU] << "." << endl;

        int cycle[2 * MAX_V];
        int subA = 0, subB = 0;
        int length = findOddCycle(result, cycle, subA, subB);
        cout << "Odd cycle responsible (length " << (length - 1) << "): ";
        for (int i = 0; i < length; i++) {
            if (i > 0) cout << " - ";
            cout << g.labels[cycle[i]];
        }
        cout << endl;

        if (round <= 10) {
            int h = subdivideEdge(g, subA, subB);
            if (h < 0) { cout << "The graph is full (" << MAX_V << " vertices), cannot add more vertices." << endl; return; }
            cout << "Modification: subdivide edge " << g.labels[subA] << "-" << g.labels[subB] << " with new vertex " << g.labels[h]
                << " (remove " << g.labels[subA] << "-" << g.labels[subB]
                << ", add " << g.labels[subA] << "-" << g.labels[h]
                << ", add " << g.labels[h] << "-" << g.labels[subB] << ")." << endl;
        }
        else {
            int colour[MAX_V];
            plainColours(g, colour);
            int oldN = g.n;
            for (int u = 0; u < oldN; u++) {
                for (int v = u + 1; v < oldN; v++) {
                    if (g.adj[u][v] == 1 && colour[u] == colour[v]) {
                        int h = subdivideEdge(g, u, v);
                        if (h < 0) { cout << "The graph is full, cannot add more vertices." << endl; return; }
                        cout << "Modification: subdivide edge " << g.labels[u] << "-" << g.labels[v]
                            << " with new vertex " << g.labels[h] << "." << endl;
                    }
                }
            }
        }
        cout << "Updated adjacency matrix:" << endl;
        printMatrix(g, false);
        cout << endl << "Running BFS two-colouring again..." << endl;
        bfsTwoColour(g, result);
    }
    cout << "Result: the graph is now bipartite. Disjoint vertex sets:" << endl;
    printTwoSets(g, result.colour);
}

// ---------------------------------------------------------------------------
//  PART (b): Hamiltonian path and circuit by BACKTRACKING
// ---------------------------------------------------------------------------
int hamPath[MAX_V];
bool hamUsed[MAX_V];
int hamLength = 0;
long hamCalls = 0;
bool hamLimitHit = false;
bool hamWantCircuit = false;
const long HAM_CALL_LIMIT = 5000000;

bool hamExtend(const Graph& g) {
    hamCalls++;
    if (hamCalls > HAM_CALL_LIMIT) { hamLimitHit = true; return false; }

    int last = hamPath[hamLength - 1];
    if (hamLength == g.n) {
        if (hamWantCircuit) return g.adj[last][hamPath[0]] == 1;
        return true;
    }
    for (int v = g.n - 1; v >= 0; v--) {
        if (g.adj[last][v] == 0 || hamUsed[v]) continue;
        hamPath[hamLength] = v;
        hamLength++;
        hamUsed[v] = true;
        if (hamExtend(g)) return true;
        hamUsed[v] = false;
        hamLength--;
        if (hamLimitHit) return false;
    }
    return false;
}

bool findHamiltonianPath(const Graph& g, int result[], long& calls, bool& limitHit) {
    calls = 0;
    limitHit = false;
    for (int start = 0; start < g.n; start++) {
        for (int i = 0; i < g.n; i++) hamUsed[i] = false;
        hamPath[0] = start;
        hamUsed[start] = true;
        hamLength = 1;
        hamCalls = 0;
        hamLimitHit = false;
        hamWantCircuit = false;
        bool found = hamExtend(g);
        calls = calls + hamCalls;
        if (hamLimitHit) limitHit = true;
        if (found) {
            for (int i = 0; i < g.n; i++) result[i] = hamPath[i];
            return true;
        }
        if (hamLimitHit) return false;
    }
    return false;
}

bool findHamiltonianCircuit(const Graph& g, int result[], long& calls, bool& limitHit) {
    calls = 0;
    limitHit = false;
    if (g.n < 3) return false;
    for (int i = 0; i < g.n; i++) hamUsed[i] = false;
    hamPath[0] = 0;
    hamUsed[0] = true;
    hamLength = 1;
    hamCalls = 0;
    hamLimitHit = false;
    hamWantCircuit = true;
    bool found = hamExtend(g);
    calls = hamCalls;
    limitHit = hamLimitHit;
    if (!found) return false;

    for (int i = 0; i < g.n; i++) result[i] = hamPath[i];
    if (result[1] > result[g.n - 1]) {
        int left = 1, right = g.n - 1;
        while (left < right) {
            int temp = result[left];
            result[left] = result[right];
            result[right] = temp;
            left++;
            right--;
        }
    }
    return true;
}

bool connectedWithoutEdge(Graph& g, int u, int v) {
    g.adj[u][v] = 0;
    g.adj[v][u] = 0;
    bool result = isConnected(g);
    g.adj[u][v] = 1;
    g.adj[v][u] = 1;
    return result;
}

void explainNoCircuit(Graph& g) {
    int n = g.n;
    bool explained = false;
    if (n < 3) {
        cout << "  - a circuit needs at least 3 vertices." << endl;
        return;
    }
    bool connected = isConnected(g);
    if (!connected) {
        cout << "  - the graph is disconnected, so no single circuit can visit every vertex." << endl;
        explained = true;
    }
    for (int u = 0; u < n; u++) {
        if (degree(g, u) < 2) {
            cout << "  - vertex " << g.labels[u] << " has degree " << degree(g, u)
                << " (a circuit needs degree 2 or more at every vertex)." << endl;
            explained = true;
        }
    }
    if (connected) {
        for (int u = 0; u < n; u++) {
            for (int v = u + 1; v < n; v++) {
                if (g.adj[u][v] == 1 && !connectedWithoutEdge(g, u, v)) {
                    cout << "  - edge " << edgeName(g, u, v) << " is a bridge (removing it disconnects the graph)." << endl;
                    explained = true;
                }
            }
        }
    }
    ColourResult cr;
    bfsTwoColour(g, cr);
    if (cr.bipartite && connected) {
        int zeros = 0;
        for (int i = 0; i < n; i++) if (cr.colour[i] == 0) zeros++;
        if (zeros * 2 != n) {
            cout << "  - the two bipartite sets have different sizes (" << zeros << " and " << (n - zeros)
                << "); a circuit alternates between the sets, so they must be equal." << endl;
            explained = true;
        }
    }
    if (!explained)
        cout << "  - the exhaustive backtracking search tried every possibility and none closes into a circuit." << endl;
}

void reportPathAndCircuit(const Graph& g, bool& hasCircuit, int circuit[]) {
    int path[MAX_V];
    long calls = 0;
    bool limitHit = false;

    bool hasPath = findHamiltonianPath(g, path, calls, limitHit);
    cout << "Hamiltonian PATH   (backtracking, " << calls << " calls): ";
    if (hasPath) cout << "EXISTS  " << listToString(g, path, g.n, false) << endl;
    else if (limitHit) cout << "not found (search limit reached)" << endl;
    else cout << "does NOT exist" << endl;

    hasCircuit = findHamiltonianCircuit(g, circuit, calls, limitHit);
    cout << "Hamiltonian CIRCUIT (backtracking, " << calls << " calls): ";
    if (hasCircuit) cout << "EXISTS  " << listToString(g, circuit, g.n, true) << endl;
    else if (limitHit) cout << "not found (search limit reached)" << endl;
    else cout << "does NOT exist" << endl;
}

void partB(Graph& g) {
    cout << endl << "=== Part (b): Make the bipartite graph Hamiltonian ===" << endl;
    ColourResult colours;
    bfsTwoColour(g, colours);
    if (!colours.bipartite) {
        cout << "The current graph is not bipartite yet. Please run option 1 first (it makes the graph bipartite)." << endl;
        return;
    }
    cout << "Starting graph: " << g.n << " vertices, " << edgeCount(g) << " edges (bipartite)." << endl;
    printMatrix(g, false);
    cout << endl;

    bool hasCircuit = false;
    int circuit[MAX_V];
    reportPathAndCircuit(g, hasCircuit, circuit);

    if (hasCircuit) {
        cout << endl << "The graph already has a Hamiltonian circuit, so no modification is needed." << endl;
        return;
    }

    cout << endl << "Why is there no Hamiltonian circuit?" << endl;
    explainNoCircuit(g);

    int n = g.n;
    bool found = false;
    int bestScore = 0, bestU = -1, bestV = -1;
    for (int u = 0; u < n; u++) {
        for (int v = u + 1; v < n; v++) {
            if (g.adj[u][v] == 1 || colours.colour[u] == colours.colour[v]) continue;
            setEdge(g, u, v, true);
            int tryCircuit[MAX_V];
            long calls;
            bool limit;
            bool works = findHamiltonianCircuit(g, tryCircuit, calls, limit);
            int score = (degree(g, u) + degree(g, v)) * 10000 + v * 100 + (99 - u);
            setEdge(g, u, v, false);
            if (works && (!found || score < bestScore)) {
                found = true;
                bestScore = score;
                bestU = u;
                bestV = v;
            }
        }
    }

    cout << endl;
    if (!found) {
        cout << "No single new edge between the two sets creates a Hamiltonian circuit, so the graph is left unchanged." << endl;
        return;
    }
    setEdge(g, bestU, bestV, true);
    cout << "Modification: add edge " << g.labels[bestU] << "-" << g.labels[bestV] << " ("
        << g.labels[bestU] << " and " << g.labels[bestV]
        << " lie in different sets, so the graph stays bipartite)." << endl;
    cout << "Updated adjacency matrix:" << endl;
    printMatrix(g, false);
    cout << endl;

    reportPathAndCircuit(g, hasCircuit, circuit);

    ColourResult after;
    bfsTwoColour(g, after);
    cout << endl << "Is the modified graph still bipartite? " << (after.bipartite ? "YES" : "NO") << endl;
    if (after.bipartite) printTwoSets(g, after.colour);
}

// ---------------------------------------------------------------------------
//  PART (c): Kruskal's algorithm with Union-Find
// ---------------------------------------------------------------------------
struct UnionFind {
    int parent[MAX_V];
    int height[MAX_V];

    void start(int n) {
        for (int i = 0; i < n; i++) {
            parent[i] = i;
            height[i] = 0;
        }
    }
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    bool join(int a, int b) {
        int leaderA = find(a);
        int leaderB = find(b);
        if (leaderA == leaderB) return false;
        if (height[leaderA] < height[leaderB]) {
            int temp = leaderA; leaderA = leaderB; leaderB = temp;
        }
        parent[leaderB] = leaderA;
        if (height[leaderA] == height[leaderB]) height[leaderA]++;
        return true;
    }
};

struct Edge {
    int u, v, w;
};

void partC(Graph& g) {
    cout << endl << "=== Part (c): Kruskal's algorithm and Minimum Spanning Tree ===" << endl;
    int n = g.n;

    cout << "Assign a positive whole-number weight to every edge:" << endl;
    for (int u = 0; u < n; u++) {
        for (int v = u + 1; v < n; v++) {
            if (g.adj[u][v] == 0) continue;
            int w;
            if (!readNumber("  Weight of edge " + edgeName(g, u, v) + ": ", 1, 1000000, w)) return;
            g.wt[u][v] = w;
            g.wt[v][u] = w;
        }
    }
    cout << endl << "Weighted adjacency matrix:" << endl;
    printMatrix(g, true);

    cout << endl;
    int component[MAX_V];
    int componentCount = findComponents(g, component);
    if (componentCount == 1) {
        cout << "Connectivity check: the graph is connected." << endl;
    }
    else {
        cout << "Connectivity check: the graph is NOT connected (" << componentCount
            << " components), so no spanning tree exists yet." << endl;
        for (int c = 0; c < componentCount; c++)
            cout << "  Component " << (c + 1) << ": " << vertexSetToString(g, component, c) << endl;

        while (componentCount > 1) {
            ColourResult colours;
            bfsTwoColour(g, colours);

            int bestScore = -1, bestU = -1, bestV = -1;
            for (int u = 0; u < n; u++) {
                for (int v = u + 1; v < n; v++) {
                    if (component[u] == component[v]) continue;
                    int sameSet = (colours.bipartite && colours.colour[u] == colours.colour[v]) ? 1 : 0;
                    int score = sameSet * 1000000 + (degree(g, u) + degree(g, v)) * 10000 + v * 100 + (99 - u);
                    if (bestScore == -1 || score < bestScore) {
                        bestScore = score;
                        bestU = u;
                        bestV = v;
                    }
                }
            }
            cout << endl << "Modification: connect two components with edge " << edgeName(g, bestU, bestV) << "." << endl;
            int w;
            if (!readNumber("  Weight of the new edge " + edgeName(g, bestU, bestV) + ": ", 1, 1000000, w)) return;
            setEdge(g, bestU, bestV, true);
            g.wt[bestU][bestV] = w;
            g.wt[bestV][bestU] = w;
            componentCount = findComponents(g, component);
        }
        cout << endl << "Updated weighted adjacency matrix:" << endl;
        printMatrix(g, true);
        cout << "The graph is now connected. Still bipartite? " << (isBipartite(g) ? "YES" : "NO") << endl;
    }

    Edge edges[MAX_V * (MAX_V - 1) / 2];
    int edgeTotal = 0;
    for (int u = 0; u < n; u++) {
        for (int v = u + 1; v < n; v++) {
            if (g.adj[u][v] == 1) {
                edges[edgeTotal].u = u;
                edges[edgeTotal].v = v;
                edges[edgeTotal].w = g.wt[u][v];
                edgeTotal++;
            }
        }
    }
    for (int i = 1; i < edgeTotal; i++) {
        Edge current = edges[i];
        int j = i - 1;
        while (j >= 0 && edges[j].w > current.w) {
            edges[j + 1] = edges[j];
            j--;
        }
        edges[j + 1] = current;
    }

    cout << endl << "Edges sorted by weight (equal weights stay in matrix order):" << endl << "  ";
    for (int i = 0; i < edgeTotal; i++) {
        cout << edgeName(g, edges[i].u, edges[i].v) << "(" << edges[i].w << ")";
        if (i + 1 < edgeTotal) cout << ", ";
    }
    cout << endl;

    cout << endl << "Kruskal's algorithm - every edge is considered in order:" << endl;
    UnionFind groups;
    groups.start(n);
    Edge tree[MAX_V];
    int treeSize = 0;
    long totalWeight = 0;
    for (int i = 0; i < edgeTotal; i++) {
        int u = edges[i].u;
        int v = edges[i].v;
        int leaderU = groups.find(u);
        int leaderV = groups.find(v);
        cout << "  " << alignRight(to_string(i + 1), 2) << ". " << edgeName(g, u, v) << " (" << edges[i].w
            << "): find(" << g.labels[u] << ")=" << g.labels[leaderU]
            << ", find(" << g.labels[v] << ")=" << g.labels[leaderV] << " -> ";
        if (groups.join(u, v)) {
            cout << "ACCEPTED (different sets, union done)" << endl;
            tree[treeSize] = edges[i];
            treeSize++;
            totalWeight = totalWeight + edges[i].w;
        }
        else {
            cout << "REJECTED (same set, would form a cycle)" << endl;
        }
    }

    cout << endl << "Accepted MST edges:" << endl;
    for (int i = 0; i < treeSize; i++)
        cout << "  " << edgeName(g, tree[i].u, tree[i].v) << " (" << tree[i].w << ")" << endl;
    cout << "Number of MST edges: " << treeSize << " (a spanning tree of " << n << " vertices needs " << (n - 1) << ")" << endl;
    cout << "Total MST weight = " << totalWeight << endl;
}

// ---------------------------------------------------------------------------
//  main: the menu
// ---------------------------------------------------------------------------
int main() {
#ifdef _WIN32
    SetConsoleCP(65001);       // Set Windows console input to UTF-8
    SetConsoleOutputCP(65001); // Set Windows console output to UTF-8
#endif

    cout << "=== Graph Theory Project (Q2) ===" << endl;
    Graph g;
    if (!readGraph(g)) return 0;
    cout << endl << "Adjacency matrix of the entered graph (" << g.n << " vertices, " << edgeCount(g) << " edges):" << endl;
    printMatrix(g, false);

    int choice = 0;
    while (true) {
        cout << endl << "Main menu:" << endl;
        cout << "  1. Check whether the graph is bipartite and fix it if necessary" << endl;
        cout << "  2. Make the bipartite graph Hamiltonian" << endl;
        cout << "  3. Make the graph Kruskal-ready and calculate its MST" << endl;
        cout << "  4. Exit" << endl;
        if (!readNumber("Choice: ", 1, 4, choice)) break;
        if (choice == 1) partA(g);
        else if (choice == 2) partB(g);
        else if (choice == 3) partC(g);
        else break;
    }
    system("pause");
    return 0;
}