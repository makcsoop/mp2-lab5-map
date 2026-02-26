#pragma once
#include <algorithm>
#include <cstddef>   // For std::ptrdiff_t
#include <iterator>  // For std::forward_iterator_tag
#include <string>
#include <vector>

#include "log.h"
#include "map.h"

template <typename T, typename H>
class Tree : public Map<T, H> {
   protected:
    using typename Map<T, H>::Pair;
    struct Node {
        Pair data;
        Node *left, *right, *parent;

        Node(const Pair &d, Node *l = nullptr, Node *r = nullptr, Node *p = nullptr)
            : data(d), left(l), right(r), parent(p) {}
    };

    struct Iterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = Node;
        using pointer = Node *;    // or also value_type*
        using reference = Node &;  // or also value_type&

        Iterator(pointer ptr) : m_ptr(ptr) {}
        reference operator*() const { return *m_ptr; }
        pointer operator->() { return m_ptr; }

        Iterator &operator++() {
            Node *current = m_ptr;
            T original_key = m_ptr->data.key;
            if (m_ptr->right != nullptr) {
                current = m_ptr->right;
                while (current->left != nullptr) {
                    current = current->left;
                }

            } else {
                do {
                    current = current->parent;
                } while (current != nullptr && current->data.key < original_key);
            }
            m_ptr = current;
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        Iterator operator+(difference_type n) const {
            Iterator tmp = *this;
            return tmp += n;
        }

        Iterator &operator+=(difference_type n) {
            while (n > 0) {
                ++(*this);
                --n;
            }
            return *this;
        }

        Iterator &operator--() {
            Node *current = m_ptr;
            T original_key = m_ptr->data.key;
            if (m_ptr->left != nullptr) {
                current = m_ptr->left;
                while (current->right != nullptr) {
                    current = current->right;
                }

            } else {
                do {
                    current = current->parent;
                } while (current != nullptr && current->data.key > original_key);
            }
            m_ptr = current;
            return *this;
        }

        Iterator operator--(int) {
            Iterator tmp = *this;
            --(*this);
            return tmp;
        }

        Iterator operator-(difference_type n) const {
            Iterator tmp = *this;
            return tmp -= n;
        }

        Iterator &operator-=(difference_type n) {
            while (n > 0) {
                --(*this);
                --n;
            }
            return *this;
        }

        reference operator[](difference_type n) {
            Iterator tmp = *this;
            if (n < 0) {
                n *= (-1);
                tmp -= n;
            } else {
                tmp += n;
            }
            return *tmp;
        }

        friend bool operator==(const Iterator &a, const Iterator &b) { return a.m_ptr == b.m_ptr; };
        friend bool operator!=(const Iterator &a, const Iterator &b) { return a.m_ptr != b.m_ptr; };
        pointer m_ptr;
    };

    Node *pFirst;
    int sz = 0;

   public:
    Iterator begin() {
        Node *current = pFirst;
        while (current->left != nullptr) {
            current = current->left;
        }

        return Iterator(current);
    }
    Iterator end() {
        Node *current = pFirst;
        while (current->right != nullptr) {
            current = current->right;
        }

        return Iterator(current);
    }

    Tree() : pFirst(nullptr) {}

    Tree(T key, H value) : pFirst(new Node(Pair{key, value}, nullptr, nullptr, nullptr)) {}

    Tree(const Tree<T, H> &root) {
        pFirst = copyNode(root.pFirst);
        sz = root.sz;
    }

    Tree(vector<Pair> elements) {
        sort(elements.begin(), elements.end());
        int plug = 0;
        pFirst = nullptr;
        while (elements.size() != 0) {
            if (plug % 3 == 0) {
                Insert(elements[elements.size() / 2].key, elements[elements.size() / 2].value);
                elements.erase(elements.begin() + elements.size() / 2);
            } else if (plug % 2 == 0) {
                Insert(elements[elements.size() / 4].key, elements[elements.size() / 4].value);
                elements.erase(elements.begin() + elements.size() / 4);
            } else {
                Insert(elements[elements.size() / 4 * 3].key, elements[elements.size() / 4 * 3].value);
                elements.erase(elements.begin() + elements.size() / 4 * 3);
            }
            plug++;
        }
    }

    Tree(vector<T> elements) {
        sort(elements.begin(), elements.end());
        int plug = 0;
        pFirst = nullptr;
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

    virtual ~Tree() { deleteNode(pFirst); }

    Node *copyNode(Node *node, Node *parent = nullptr) {
        if (node == nullptr) {
            return nullptr;
        }
        Node *new_node = new Node{node->data, parent, nullptr, nullptr};
        new_node->left = copyNode(node->left, new_node);
        new_node->right = copyNode(node->right, new_node);
        return new_node;
    }

    void deleteNode(Node *current) {
        if (current == nullptr) return;
        deleteNode(current->left);
        deleteNode(current->right);
        delete current;
    }

    Node *GetFirst() { return pFirst; }

    virtual void Insert(T key, H value) {
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

    void printTreeWithKey(Node *root, std::string indent = "", bool isLeft = true) {
        if (root == nullptr) return;
        if (root->right) {
            printTreeWithKey(root->right, indent + (isLeft ? "│   " : "    "), false);
        }
        cout << indent << (isLeft ? "└── " : "┌── ") << "(" << root->data.key << ',' << root->data.value
             << ")" << endl;
        if (root->left) {
            printTreeWithKey(root->left, indent + (isLeft ? "    " : "│   "), true);
        }
    }

    virtual void Delete(T key) {
        Node *node = FindNode(key);
        if (!node) {
            throw runtime_error("Нет ключа");
        }
        if (node->left == nullptr && node->right == nullptr) {  // удаление листа
            if (node == pFirst) {
                delete pFirst;
                pFirst = nullptr;
                sz = 0;
                return;
            }
            Node *parent = node->parent;
            if (parent->left == node) {
                parent->left = nullptr;
            } else {
                parent->right = nullptr;
            }
            delete node;
            sz--;

        } else if (((node->left == nullptr) + (node->right == nullptr)) == 1) {  // один потомок
            Node *child = (node->left != nullptr) ? node->left : node->right;
            if (node == pFirst) {
                child->parent = nullptr;
                delete pFirst;
                pFirst = child;
                sz--;
                return;
            }
            Node *parent = node->parent;
            if (parent->left == node) {
                parent->left = child;
            } else {
                parent->right = child;
            }
            child->parent = parent;
            delete node;
            sz--;
        } else {  // 2 потомка
            if (node == pFirst) {
                Node *minNode = node->right;
                while (minNode->left != nullptr) {
                    minNode = minNode->left;
                }
                T minKey = minNode->data.key;
                H minValue = minNode->data.value;
                Delete(minKey);
                pFirst = new Node{Pair{minKey, minValue}, node->left, node->right, nullptr};
                if (pFirst->left) {
                    pFirst->left->parent = pFirst;
                }
                if (pFirst->right) {
                    pFirst->right->parent = pFirst;
                }
                delete node;
            } else {
                Node *minNode = node->right;
                while (minNode->left != nullptr) {
                    minNode = minNode->left;
                }
                node->data.key = minNode->data.key;
                node->data.value = minNode->data.value;
                Node *minParent = minNode->parent;
                if (minParent->left == minNode) {
                    minParent->left = minNode->right;
                } else {
                    minParent->right = minNode->right;
                }

                if (minNode->right) {
                    minNode->right->parent = minParent;
                }

                delete minNode;
                sz--;
            }
        }
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

    virtual H *Find(T key) {
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

    H &operator[](T key) {
        H *tmp = this->Find(key);
        if (tmp == nullptr) {
            this->Insert(key, H{});
            return *this->Find(key);
        }
        return *tmp;
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

    bool isTrueSort() { return RecSearch(pFirst, nullptr, nullptr); }

    bool RecSearch(Node *node, Node *minNode, Node *maxNode) {
        if (node == nullptr) return true;
        if (minNode != nullptr && node->data.key <= minNode->data.key) return false;
        if (maxNode != nullptr && node->data.key >= maxNode->data.key) return false;
        return RecSearch(node->left, minNode, node) && RecSearch(node->right, node, maxNode);
    }
};

template <typename T, typename H>
class AVLTree : protected Tree<T, H> {
   protected:
    using typename Tree<T, H>::Pair;
    using typename Tree<T, H>::Node;

    struct AVLNode : public Node {
        int advantage;

        AVLNode(const Pair &d, Node *p = nullptr) : Node(d, nullptr, nullptr, p), advantage(0) {}
    };

    AVLNode *LL(AVLNode *node) {};
    AVLNode *RR(AVLNode *node) {};
    AVLNode *RL(AVLNode *node) {};
    AVLNode *LR(AVLNode *node) {};

    AVLNode *_Insert(AVLNode *root, const Pair &kv) {};

    AVLNode *root;

   public:
    AVLTree() : root(nullptr) {}

    void Delete(T key) {}
    void Insert(T key, H value) {}

    int Balance(AVLNode *node) { return 1; }

    ~AVLTree() {}
};