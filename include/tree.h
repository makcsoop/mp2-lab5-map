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

    Tree(T key, H value) : pFirst(new Node(Pair{key, value}, nullptr, nullptr, nullptr)) {}

    Tree(vector<T> elements) {  // ?????????
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

    void InsertNode(Node *current) {
        if (pFirst == nullptr) {
            pFirst = current;
            sz++;
        } else {
            Node *tmp = pFirst;
            while (true) {
                if (tmp->data.key > current->data.key) {
                    if (tmp->left != nullptr) {
                        tmp = tmp->left;
                    } else {
                        tmp->left = current;
                        sz++;
                        break;
                    }
                } else if (tmp->data.key < current->data.key) {
                    if (tmp->right != nullptr) {
                        tmp = tmp->right;
                    } else {
                        tmp->right = current;
                        sz++;
                        break;
                    }
                } else {
                    throw invalid_argument("Incorrect insert");
                }
            }
        }
    }

    void printTree(Node *root, std::string indent = "", bool isLeft = true) {
        if (root == nullptr) return;
        if (root->right) {
            printTree(root->right, indent + (isLeft ? "│   " : "    "), false);
        }
        cout << indent << (isLeft ? "└── " : "┌── ") << root->data.key << endl;
        if (root->left) {
            printTree(root->left, indent + (isLeft ? "    " : "│   "), true);
        }
    }

    Node *GetNext(Node *current) {
        if (current != pFirst) {
            if (current->parent->right->data.key == current->data.key) {
                return pFirst;  //(обработка максимального звена) если бросить исключение то эта начинает
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

    void Delete(T key) {
        Node *node = FindNode(key);
        if (node->left == nullptr && node->right == nullptr) {  // удаление листа
            Node *parent = node->parent;
            if (parent->left == node) {
                parent->left = nullptr;
            } else {
                parent->right = nullptr;
            }

        } else if (((node->left == nullptr) + (node->right == nullptr)) == 1) {  // один потомок
            Node *parent = node->parent;
            Node *next = (node->left == nullptr) ? node->right : node->left;
            if (parent->left != nullptr) {
                parent->left = next;
            } else {
                parent->right = next;
            }
        } else {  // 2 потомка
            Node *parent = node->parent;
            Node *right = node->right;
            Node *left = node->left;
            if (parent->left == node) {
                parent->left = left;
                InsertNode(right);

            } else {
                parent->right = right;
                InsertNode(left);
            }
        }
        delete node;
        sz--;
    }

    int count() { return sz; };

    vector<T> keys() {
        vector<T> data;
        RecTree(GetFirst(), data);
        sort(data.begin(), data.end());
        return data;
    };

    void RecTree(const Node *node, vector<T> &keys) {
        if (node == nullptr) return;
        keys.push_back(node->data.key);
        RecTree(node->left, keys);
        RecTree(node->right, keys);
    }

    H *Find(T key) {
        Node *current = pFirst;
        while (current != nullptr) {
            if (current->data.key == key) {
                return &current->data.value;
            }
            if (current->data.key > key) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return nullptr;
    }

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