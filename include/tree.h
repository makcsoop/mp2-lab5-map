#include <map.h>

template <typename T, typename H>
class Tree : public Map<T, H> {
    using typename Map<T, H>::Pair;
    struct Node {
        Pair data;
        Node *left, *right, *parent;
    };
    Node *pFirst;

   public:
    Tree() : pFirst(nullptr){};
    Node *Get_pFirst() { return pFirst; }
    void Insert(T key, H value) {
        if (pFirst == nullptr) {
            pFirst = new Node{Pair{key, value}, nullptr, nullptr, nullptr};
        } else {
            Node *tmp = pFirst;
            while (true) {
                if (tmp->data.key < key) {
                    if (tmp->left != nullptr) {
                        tmp = tmp->left;
                    } else {
                        Node *new_node = new Node{Pair{key, value}, nullptr, nullptr, tmp};
                        tmp->left = new_node;
                        break;
                    }
                } else {
                    if (tmp->right != nullptr) {
                        tmp = tmp->right;
                    } else {
                        Node *new_node = new Node{Pair{key, value}, nullptr, nullptr, tmp};
                        tmp->right = new_node;
                        break;
                    }
                }
            }
        }
    }

    void Print() {}

    void printTree(Node *root, std::string indent = "", bool isLeft = true) {
        if (root == nullptr) return;

        if (root->right) {
            printTree(root->right, indent + (isLeft ? "│   " : "    "), false);
        }

        std::cout << indent << (isLeft ? "└── " : "┌── ") << root->data.key << std::endl;

        if (root->left) {
            printTree(root->left, indent + (isLeft ? "    " : "│   "), true);
        }
    }

    void Delete(T key) { key = 0; };
    int count() { return 0; };
    vector<T> keys() { return vector<T>({9}); };
    H *Find(T key) {
        key = 0;
        return pFirst->data.value;
    }
    ~Tree() = default;
};