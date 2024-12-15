#include <iostream>
#include <queue>
#include <string>
#include <cstring>
#include <iomanip>
using namespace std;

// This program implements Huffman coding a data compression algorithm.
// 1. The program takes input from the user.

// 2. It then Calculates the frequency of each character in the string, including spaces.

// 3. It builds a Huffman tree based on the frequencies of characters.
//    - A priority queue is used to build the Huffman tree. The tree construction follows the following steps:
//      - Create a leaf node for each unique character and insert it into the priority queue.
//      - Extract two nodes with the lowest frequencies, create a new internal node with these two nodes as children, and push this new node back into the priority queue.
//      - Repeat this process until only one node (the root of the tree) remains.

// 4. Then it generates a binary code for each character. The binary code for each character is determined by traversing the tree.

// 5. A recursive function (`traverseTree`) is used to traverse the Huffman tree and generate the codes. The tree is traversed recursively, with the function appending '0' for left branches and '1' for right branches.

// 6. The input string is then encoded using the generated Huffman code. Each character in the input string is replaced by its corresponding Huffman code, forming the encoded string.

// 7. The program then also decodes the encoded string.

struct TreeNode {
    char character;
    int frequency;
    TreeNode* left;
    TreeNode* right;
};

class HuffmanTree {
public:
    TreeNode* root;

    HuffmanTree() : root(nullptr) {}

    TreeNode* createNewNode(char character, int frequency, TreeNode* left = nullptr, TreeNode* right = nullptr);
    void buildTree(const string& input);
    void displayFrequencyTable(const string& input);
    void generateHuffmanCodes(string huffmanCodes[256]);
    void encodeString(const string& input, string huffmanCodes[256]);
    void decodeString(const string& encodedString);
    void buildTreeAndGenerateCodes(const string& input, string huffmanCodes[256]);
    void printCodes(string huffmanCodes[256]);

private:
    void traverseTreeForCodes(TreeNode* node, const string& str, string huffmanCodes[256]);
};

TreeNode* HuffmanTree::createNewNode(char character, int frequency, TreeNode* left, TreeNode* right) {
    TreeNode* newNode = new TreeNode;
    newNode->character = character;
    newNode->frequency = frequency;
    newNode->left = left;
    newNode->right = right;
    return newNode;
}

void HuffmanTree::buildTree(const string& input) {
    int frequencyTable[256] = { 0 };

    for (char character : input) {
        frequencyTable[character]++;
    }

    cout << "\nCharacter Frequency Table:\n";
    cout << left << setw(20) << "Character" << "Frequency" << endl;
    for (int i = 0; i < 256; i++) {
        if (frequencyTable[i] > 0) {
            cout << left << setw(20) << (char)i << frequencyTable[i] << endl;
        }
    }

    priority_queue<TreeNode*, vector<TreeNode*>> priorityQueue;

    for (int i = 0; i < 256; i++) {
        if (frequencyTable[i] > 0) {
            priorityQueue.push(createNewNode((char)i, frequencyTable[i]));
        }
    }

    while (priorityQueue.size() > 1) {
        TreeNode* left = priorityQueue.top();
        priorityQueue.pop();
        TreeNode* right = priorityQueue.top();
        priorityQueue.pop();
        priorityQueue.push(createNewNode('\0', left->frequency + right->frequency, left, right));
    }

    root = priorityQueue.top();
}

void HuffmanTree::displayFrequencyTable(const string& input) {
    int frequencyTable[256] = { 0 };
    for (char character : input) {
        frequencyTable[character]++;
    }

    cout << "\nCharacter Frequency Table:\n";
    cout << left << setw(20) << "Character" << "Frequency" << endl;
    for (int i = 0; i < 256; i++) {
        if (frequencyTable[i] > 0) {
            cout << left << setw(20) << (char)i << frequencyTable[i] << endl;
        }
    }
}

void HuffmanTree::generateHuffmanCodes(string huffmanCodes[256]) {
    traverseTreeForCodes(root, "", huffmanCodes);
}

void HuffmanTree::encodeString(const string& input, string huffmanCodes[256]) {
    cout << "\nOriginal string: " << input << "\n";

    string encoded = "";
    for (char character : input) {
        if (!huffmanCodes[character].empty()) {
            encoded += huffmanCodes[character];
        }
        else {
            cout << "Error: Code for character '" << character << "' not found!" << endl;
            return;
        }
    }
    cout << "\nEncoded string: " << encoded << "\n";

    cout << "\nCompression analysis:\n";
    cout << "Original string size (in bits): " << input.size() * 8 << " bits\n";
    cout << "Encoded string size (in bits): " << encoded.size() << " bits\n";

    decodeString(encoded);
}

void HuffmanTree::decodeString(const string& encodedString) {
    cout << "\nDecoded string:\n";
    TreeNode* currentNode = root;
    for (char bit : encodedString) {
        currentNode = (bit == '0') ? currentNode->left : currentNode->right;

        if (!currentNode->left && !currentNode->right) {
            cout << currentNode->character;
            currentNode = root;
        }
    }
    cout << "\n";
}

void HuffmanTree::traverseTreeForCodes(TreeNode* node, const string& str, string huffmanCodes[256]) {
    if (!node) return;

    if (!node->left && !node->right) {
        huffmanCodes[node->character] = str;
    }

    traverseTreeForCodes(node->left, str + "0", huffmanCodes);
    traverseTreeForCodes(node->right, str + "1", huffmanCodes);
}

void HuffmanTree::buildTreeAndGenerateCodes(const string& input, string huffmanCodes[256]) {
    buildTree(input);  
    generateHuffmanCodes(huffmanCodes);  
}

void HuffmanTree::printCodes(string huffmanCodes[256]) {
    cout << "\nHuffman Codes:\n";
    cout << left << setw(20) << "Character" << "Huffman Code\n";
    for (int i = 0; i < 256; i++) {
        if (!huffmanCodes[i].empty()) {
            cout << left << setw(20) << (char)i << huffmanCodes[i] << endl;
        }
    }
}

int main() {
    HuffmanTree tree;
    string text;
    string encodedInput;  
    string codes[256];  
    char continueChoice;  
    cout << "Huffman Coding Program\n";

    while (true) {
        cout << "\nEnter a string: ";
        getline(cin, text);

        tree.buildTreeAndGenerateCodes(text, codes);
        tree.printCodes(codes);
        tree.encodeString(text, codes);

        cout << "\nWould you like to enter another string? ('y' to continue. Press any other key to exit program.) ";
        cin >> continueChoice;
        cin.ignore();  

        if (continueChoice != 'y' && continueChoice != 'Y') {
            break;  
        }
    }

    cout << "Exiting program.\n";
    return 0;
}
