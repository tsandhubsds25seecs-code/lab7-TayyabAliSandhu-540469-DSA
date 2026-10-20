#include <iostream>
#include <string>
#include <stack>
using namespace std;

// ================================================================
// NODE DEFINITION
// ================================================================
struct TreeNode {
    char data;
    TreeNode* left;
    TreeNode* right;

    TreeNode(char c) {
        data = c;
        left = nullptr;
        right = nullptr;
    }
};
// ================================================================
//Generate a Binary Tree from the Given String
// String format: a(b(d,e),c(,f(g,)))
//   '('  -> left child begins
//   ','  -> left ends, right begins
//   ')'  -> both children end
// ================================================================
TreeNode* buildTree(const string& s, int& i) {
    if (i >= (int)s.size()) return nullptr;

    //  Read the current node's character
    char value = s[i];
    i++;  // move to next character
    TreeNode* root = new TreeNode(value);

    // : If '(' is found, children exist
    if (i < (int)s.size() && s[i] == '(') {
        i++;  // skip '('

        // Left child exists only if next char is NOT ',' or ')'
        if (i < (int)s.size() && s[i] != ',' && s[i] != ')') {
            root->left = buildTree(s, i);
        }

        // If ',' is found, right child follows
        if (i < (int)s.size() && s[i] == ',') {
            i++;  // skip ','
            if (i < (int)s.size() && s[i] != ')') {
                root->right = buildTree(s, i);
            }
        }

        // Skip the closing ')'
        if (i < (int)s.size() && s[i] == ')') {
            i++;
        }
    }

    return root;
}

TreeNode* generateTree(const string& s) {
    int i = 0;
    return buildTree(s, i);
}

// ================================================================
// TASK 2: Preorder Traversal (Recursive)
// Order: Root -> Left -> Right
// ================================================================
void preorder(TreeNode* root) {
    if (root == nullptr) return;

    cout << root->data << " ";   // 1. Print root
    preorder(root->left);         // 2. Visit left
    preorder(root->right);        // 3. Visit right
}
// ================================================================
// TASK 3: Inorder Traversal (Iterative using Stack)
// Order: Left -> Root -> Right
// Stack = LIFO (last in, first out)
// ================================================================
void inorderIterative(TreeNode* root) {
    stack<TreeNode*> st;
    TreeNode* current = root;

    while (current != nullptr || !st.empty()) {
        // Step 1: Push all left descendants onto the stack
        while (current != nullptr) {
            st.push(current);
            current = current->left;
        }

        // Step 2: Pop one node, print it
        current = st.top();
        st.pop();
        cout << current->data << " ";

        // Step 3: Move to the right subtree
        current = current->right;
    }
}

// ================================================================
// TASK 4: Postorder Traversal (Iterative using 2 Stacks)
// Order: Left -> Right -> Root
// Trick: Stack 1 produces Root->Right->Left
//        Reversing it gives Left->Right->Root
// ================================================================
void postorderIterative(TreeNode* root) {
    if (root == nullptr) return;

    stack<TreeNode*> stack1, stack2;
    stack1.push(root);

    // Push nodes into stack2 in Root->Right->Left order
    while (!stack1.empty()) {
        TreeNode* node = stack1.top();
        stack1.pop();
        stack2.push(node);

        if (node->left)  stack1.push(node->left);
        if (node->right) stack1.push(node->right);
    }

    //  Pop from stack2 gives Left->Right->Root
    while (!stack2.empty()) {
        cout << stack2.top()->data << " ";
        stack2.pop();
    }
}

// ================================================================
// TASK 5: Validate Tree String WITHOUT Building the Tree
// Checks:
//   - Parentheses are balanced
//   - No empty parentheses "()"
//   - Letters placed correctly (not two letters in a row)
//   - Comma only appears inside parentheses
// ================================================================
bool validateTreeString(const string& s) {
    if (s.empty()) return false;

    int i = 0;
    int n = s.size();
    int depth = 0;                // parenthesis nesting level
    bool expectNode = true;       // true when a letter is expected

    while (i < n) {
        char ch = s[i];

        if (ch == '(') {
            // '(' must come after a node
            if (expectNode) return false;
            i++;
            // Empty parentheses "()" is invalid
            if (i < n && s[i] == ')') return false;
            depth++;
            expectNode = true;
        }
        else if (ch == ',') {
            // ',' must come after a node (left child finished)
            if (expectNode) return false;
            // ',' must be inside parentheses
            if (depth == 0) return false;
            i++;
            expectNode = false;   // right child is optional
        }
        else if (ch == ')') {
            // Cannot close more than opened
            if (depth == 0) return false;
            i++;
            depth--;
            expectNode = false;
        }
        else if (isalpha(ch)) {
            // Two letters in a row is invalid
            if (!expectNode) return false;
            i++;
            expectNode = false;
        }
        else {
            return false;  // invalid character
        }
    }

    return depth == 0 && !expectNode;
}

// ================================================================
// TASK 6: Height and Depth (Recursive)
// ================================================================
int treeHeight(TreeNode* root) {
    if (root == nullptr) return -1;   // empty tree = -1, leaf = 0
    int leftHeight  = treeHeight(root->left);
    int rightHeight = treeHeight(root->right);
    return 1 + max(leftHeight, rightHeight);
}

void nodeDepths(TreeNode* root, int depth = 0) {
    if (root == nullptr) return;
    cout << "Node " << root->data << " -> depth " << depth << "\n";
    nodeDepths(root->left,  depth + 1);
    nodeDepths(root->right, depth + 1);
}

// ================================================================
// MAIN FUNCTION
// ================================================================
int main() {
    string input = "a(b(d,e),c(,f(g,)))";

    cout << "Tree string: " << input << "\n";
    cout << "Validation: " << (validateTreeString(input) ? "VALID" : "INVALID") << "\n\n";

    TreeNode* root = generateTree(input);

    cout << "Preorder  (recursive): ";
    preorder(root);
    cout << "\n";

    cout << "Inorder   (iterative): ";
    inorderIterative(root);
    cout << "\n";

    cout << "Postorder (iterative): ";
    postorderIterative(root);
    cout << "\n\n";

    cout << "Tree height: " << treeHeight(root) << "\n";
    cout << "Depth of each node:\n";
    nodeDepths(root);

    return 0;
}
