#include <map.h>

#include <algorithm>
#include <vector>

template <typename T, typename H>
class Tree : public Map<T, H> {
    using typename Map<T, H>::Pair;
    struct Node {
        Pair data;
        Node *left, *right, *parent;

        Node(const Pair &d, Node *l = nullptr, Node *r = nullptr, Node *p = nullptr)
            : data(d), left(l), right(r), parent(p) {}
    };
    Node *pFirst;
    int sz = 0;

   public:
    Tree() : pFirst(nullptr) {}

    Tree(T key, H value) : pFirst(Pair{key, value}, nullptr, nullptr, nullptr) {}

    Tree(vector<int> elements) {
        sort(elements.begin(), elements.end());
        int plug = 0;
        while (elements.size() != 0) {
            if (plug % 3 == 0) {
                Insert(elements[elements.size() / 2], H{});
                elements.erase(elements.begin() + elements.size() / 2);
            } else if (plug % 2 == 0) {
                Insert(elements[elements.size() / 4], H{});
                elements.erase(elements.begin() + elements.size() / 4);
            } else {
                Insert(elements[elements.size() / 4 * 3], H{});
                elements.erase(elements.begin() + elements.size() / 4 * 3);
            }
            plug++;
        }
    }
    ~Tree() { deleteNode(pFirst); }
    void deleteNode(Node *current) {
        if (current == nullptr) {
            return;
        } else {
            deleteNode(current->left);
            deleteNode(current->right);
            delete current;
        }
    }
    Node *GetFirst() { return pFirst; }
    void Insert(T key, H value) {
        if (pFirst == nullptr) {
            pFirst = new Node{Pair{key, value}, nullptr, nullptr, nullptr};
            sz++;
        } else {
            Node *tmp = pFirst;
            while (true) {
                if (tmp->data.key > key) {
                    if (tmp->left != nullptr) {
                        tmp = tmp->left;
                    } else {
                        Node *new_node = new Node{Pair{key, value}, nullptr, nullptr, tmp};
                        tmp->left = new_node;
                        sz++;
                        break;
                    }
                } else if (tmp->data.key < key) {
                    if (tmp->right != nullptr) {
                        tmp = tmp->right;
                    } else {
                        Node *new_node = new Node{Pair{key, value}, nullptr, nullptr, tmp};
                        tmp->right = new_node;
                        sz++;
                        break;
                    }
                } else {
                    throw invalid_argument("Incorrect insert");
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

    Node *GetNext(Node *current) {
        if (current != pFirst) {
            if (current->parent->right->data.key == current->data.key) {
                return pFirst;  //(обработка максимального звена) если бросить исключение то эта дура начинает
                                // сбрасывать память, как заглушка пока пусть так
            }
        }
        if (current->right != nullptr) {
            current = current->right;
            while (current->left != nullptr) {
                current = current->left;
            }
            return current;
        } else {
            if (current->parent->data.key > current->data.key) {
                current = current->parent;
                return current;
            } else {
                T remember = current->data.key;
                do {
                    current = current->parent;
                } while (current->parent->data.key < remember);
                return current;
            }
        }
    }

    void Delete(T key) { key++; }

    int count() { return sz; };

    vector<T> keys() { return vector<T>({9}); };

    H *Find(T key) {
        Node *current = pFirst;
        bool ___coco_jambo = 0;
        while (!___coco_jambo) {
            if (current->data.key == key) {
                return &current->data.value;
            }
            while (current->data.key > key) {
                current = current->left;
            }
            while (current->data.key < key) {
                current = current->right;
            }
            if (current == nullptr) {
                ___coco_jambo = 1;
            }
        }
        return nullptr;
    }

    // тестил файнд, он просто мусор выкидывает в случае H*, у нас и так здесь ключей нема. поэтому решил
    // сделать еще такое

    Node *FindNode(T key) {
        Node *current = pFirst;
        while (current != nullptr) {
            if (current->data.key == key) {
                return current;
            }
            if (current->data.key > key) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return nullptr;
    }
};