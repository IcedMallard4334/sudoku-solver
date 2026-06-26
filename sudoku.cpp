#include <iostream>
#include <vector>
#include <cstring>
#include <chrono>
#include <memory>

using namespace std;
using namespace std::chrono;

int N, BOX_SIZE, N2, MAX_NODES;

struct Node {
    Node *left, *right, *up, *down;
    Node *column;
    int rowID;
    int size;
};

class DLXSudoku {
private:
    unique_ptr<Node[]> nodePool;
    int nodeCount;
    Node *header;
    vector<Node*> columns;
    vector<Node*> solution;
    int solutionSize;
    vector<vector<int>> grid;
    bool solved;

    int getBox(int r, int c) { 
        return (r / BOX_SIZE) * BOX_SIZE + (c / BOX_SIZE); 
    }

    Node* newNode() {
        if (nodeCount >= MAX_NODES) exit(1);
        Node* n = &nodePool[nodeCount++];
        n->left = n->right = n->up = n->down = n;
        n->column = nullptr;
        n->rowID = -1;
        n->size = 0;
        return n;
    }

    Node* newColumn() {
        Node* col = newNode();
        col->column = col;
        col->size = 0;
        col->left = header->left;
        col->right = header;
        header->left->right = col;
        header->left = col;
        columns.push_back(col);
        return col;
    }

    void addRow(int rowID, const vector<int>& cols) {
        if (cols.empty()) return;
        Node* first = nullptr;
        for (int colIdx : cols) {
            if (colIdx >= (int)columns.size()) continue;
            Node* col = columns[colIdx];
            Node* newNode_ = newNode();
            newNode_->rowID = rowID;
            newNode_->column = col;
            newNode_->up = col->up;
            newNode_->down = col;
            col->up->down = newNode_;
            col->up = newNode_;
            col->size++;
            if (!first) first = newNode_;
            else {
                newNode_->left = first->left;
                newNode_->right = first;
                first->left->right = newNode_;
                first->left = newNode_;
            }
        }
    }

    void cover(Node* col) {
        col->right->left = col->left;
        col->left->right = col->right;
        for (Node* row = col->down; row != col; row = row->down)
            for (Node* node = row->right; node != row; node = node->right) {
                node->down->up = node->up;
                node->up->down = node->down;
                node->column->size--;
            }
    }

    void uncover(Node* col) {
        for (Node* row = col->up; row != col; row = row->up)
            for (Node* node = row->left; node != row; node = node->left) {
                node->column->size++;
                node->down->up = node;
                node->up->down = node;
            }
        col->right->left = col;
        col->left->right = col;
    }

    Node* chooseColumn() {
        Node* minCol = nullptr;
        int minSize = MAX_NODES;
        for (Node* col = header->right; col != header; col = col->right)
            if (col->size < minSize) { minSize = col->size; minCol = col; }
        return minCol;
    }

    bool search(int depth) {
        if (header->right == header) { solutionSize = depth; solved = true; return true; }
        Node* col = chooseColumn();
        if (!col || col->size == 0) return false;
        cover(col);
        for (Node* row = col->down; row != col; row = row->down) {
            solution[depth] = row;
            for (Node* node = row->right; node != row; node = node->right) cover(node->column);
            if (search(depth + 1)) return true;
            for (Node* node = row->left; node != row; node = node->left) uncover(node->column);
        }
        uncover(col);
        return false;
    }

    void getConstraints(int r, int c, int digit, vector<int>& cols) {
        cols.clear();
        int pos = r * N + c;
        cols.push_back(pos);
        cols.push_back(N2 + r * N + digit);
        cols.push_back(2 * N2 + c * N + digit);
        cols.push_back(3 * N2 + getBox(r, c) * N + digit);
    }

public:
    DLXSudoku() : nodeCount(0), solutionSize(0), solved(false) {
        MAX_NODES = (4 * N2 * N) + (4 * N2) + 10000;
        nodePool = make_unique<Node[]>(MAX_NODES);
        solution.resize(N2);
        grid.assign(N, vector<int>(N, 0));
    }

    void readInput() {
        cout << "Enter puzzle row by row (" << N << " values per row)." << endl;
        cout << "Use 0 for empty cells";
        if (N > 9) cout << ", and values 10-" << N << " for digits above 9";
        cout << ":" << endl;

        for (int r = 0; r < N; r++) {
            cout << "Row " << (r + 1) << ": ";
            for (int c = 0; c < N; c++) cin >> grid[r][c];
        }
    }

    void buildMatrix() {
        columns.clear();
        nodeCount = 0;
        solved = false;
        header = newNode();
        for (int i = 0; i < 4 * N2; i++) newColumn();
        for (int r = 0; r < N; r++) {
            for (int c = 0; c < N; c++) {
                if (grid[r][c] != 0) {
                    int digit = grid[r][c] - 1;
                    vector<int> cols;
                    getConstraints(r, c, digit, cols);
                    addRow((r * N + c) * N + digit, cols);
                } else {
                    for (int digit = 0; digit < N; digit++) {
                        vector<int> cols;
                        getConstraints(r, c, digit, cols);
                        addRow((r * N + c) * N + digit, cols);
                    }
                }
            }
        }
    }

    bool solve() {
        bool result = search(0);
        if (result) {
            for (int i = 0; i < solutionSize; i++) {
                int rowID = solution[i]->rowID;
                int digit = rowID % N;
                rowID /= N;
                int c = rowID % N;
                int r = rowID / N;
                grid[r][c] = digit + 1;
            }
        }
        return result;
    }

    void printGrid() {
        for (int r = 0; r < N; r++) {
            if (r > 0 && r % BOX_SIZE == 0) {
                for (int i = 0; i < N * 2 + BOX_SIZE - 1; i++) cout << "-";
                cout << endl;
            }
            for (int c = 0; c < N; c++) {
                if (c > 0 && c % BOX_SIZE == 0) cout << "| ";
                if (grid[r][c] == 0) cout << ". ";
                else if (grid[r][c] < 10) cout << grid[r][c] << " ";
                else cout << char('A' + grid[r][c] - 10) << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
};

int main() {
    cout << "Select puzzle size:" << endl;
    cout << "1. 9x9" << endl;
    cout << "2. 16x16" << endl;
    cout << "3. 25x25" << endl;
    cout << "Choice: ";

    int choice;
    cin >> choice;

    switch (choice) {
        case 1: N = 9;  BOX_SIZE = 3; break;
        case 2: N = 16; BOX_SIZE = 4; break;
        case 3: N = 25; BOX_SIZE = 5; break;
        default:
            cout << "Invalid choice." << endl;
            return 1;
    }
    N2 = N * N;

    DLXSudoku solver;
    solver.readInput();
    solver.buildMatrix();

    if (solver.solve()) solver.printGrid();
    else cout << "No solution found." << endl;

    return 0;
}