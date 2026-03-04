#pragma once
#include <algorithm>
#include <cstddef>   // For std::ptrdiff_t
#include <iterator>  // For std::forward_iterator_tag
#include <string>
#include <vector>

#include "log.h"
#include "map.h"

template <typename TypeNode>
struct TreeIterator {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = TypeNode;
    using pointer = TypeNode *;    // or also value_type*
    using reference = TypeNode &;  // or also value_type&

    TreeIterator(pointer ptr) : m_ptr(ptr) {}
    reference operator*() const { return *m_ptr; }
    pointer operator->() { return m_ptr; }

    TreeIterator &operator++() {
        TypeNode *current = m_ptr;
        auto original_key = m_ptr->data.key;
        if (m_ptr->right != nullptr) {
            current = static_cast<TypeNode *>(m_ptr->right);
            while (current->left != nullptr) {
                current = static_cast<TypeNode *>(current->left);
            }

        } else {
            do {
                current = static_cast<TypeNode *>(current->parent);
            } while (current != nullptr && current->data.key < original_key);
        }
        m_ptr = current;
        return *this;
    }

    TreeIterator operator++(int) {
        TreeIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    TreeIterator operator+(difference_type n) const {
        TreeIterator tmp = *this;
        return tmp += n;
    }

    TreeIterator &operator+=(difference_type n) {
        while (n > 0) {
            ++(*this);
            --n;
        }
        return *this;
    }

    TreeIterator &operator--() {
        TypeNode *current = m_ptr;
        auto original_key = m_ptr->data.key;
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

    TreeIterator operator--(int) {
        TreeIterator tmp = *this;
        --(*this);
        return tmp;
    }

    TreeIterator operator-(difference_type n) const {
        TreeIterator tmp = *this;
        return tmp -= n;
    }

    TreeIterator &operator-=(difference_type n) {
        while (n > 0) {
            --(*this);
            --n;
        }
        return *this;
    }

    reference operator[](difference_type n) {
        TreeIterator tmp = *this;
        if (n < 0) {
            n *= (-1);
            tmp -= n;
        } else {
            tmp += n;
        }
        return *tmp;
    }

    friend bool operator==(const TreeIterator &a, const TreeIterator &b) { return a.m_ptr == b.m_ptr; };
    friend bool operator!=(const TreeIterator &a, const TreeIterator &b) { return a.m_ptr != b.m_ptr; };
    pointer m_ptr;
};

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

    Node *pFirst;
    int sz = 0;

   public:
    using Iterator = TreeIterator<Node>;
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
    using typename Tree<T, H>::Iterator;

    struct AVLNode : public Node {
        int _balance;

        AVLNode(const Pair &d, Node *p = nullptr) : Node(d, nullptr, nullptr, p), _balance(0) {}

        // AVLNode&operator=(AVLNode* other){

        // }
    };
    AVLNode *_pFirst;
    int _sz;

   public:
    typename Tree<T, H>::Iterator begin() {
        AVLNode *current = _pFirst;

        while (current->left != nullptr) {
            current = current->left;
        }

        return Iterator(current);
    }
    typename Tree<T, H>::Iterator end() {
        AVLNode *current = _pFirst;
        while (current->right != nullptr) {
            current = current->right;
        }

        return Iterator(current);
    }

    AVLTree() : _pFirst(nullptr), _sz(0) {}

    void Delete(T key) { key++; }
    void Insert(T key, H value) override {
        _pFirst = _Insert(_pFirst, key, value, nullptr);
        _sz++;
    }

    int Balance(AVLNode *node) {
        if (node == nullptr) return 0;

        int leftHeight = GetHeight(static_cast<AVLNode *>(node->left));
        int rightHeight = GetHeight(static_cast<AVLNode *>(node->right));

        int balance = rightHeight - leftHeight;

        node->_balance = balance;
        return balance;
    }

    void _printTreeWithKey(AVLNode *root, std::string indent = "", bool isLeft = true) {
        if (root == nullptr) return;
        if (static_cast<AVLNode *>(root->right)) {
            _printTreeWithKey(static_cast<AVLNode *>(root->right), indent + (isLeft ? "│   " : "    "),
                              false);
        }
        cout << indent << (isLeft ? "└── " : "┌── ") << "(" << root->data.key << ',' << root->data.value
             << ")" << endl;
        if (static_cast<AVLNode *>(root->left)) {
            _printTreeWithKey(static_cast<AVLNode *>(root->left), indent + (isLeft ? "    " : "│   "), true);
        }
    }

    AVLNode *GetFirst() { return _pFirst; }

    ~AVLTree() {}

   private:
    void swap_data(AVLNode *first, AVLNode *second) {
        Pair tmp = first->data;
        first->data = second->data;
        second->data = tmp;
    };

    void swap_pointer(AVLNode *node) {
        AVLNode *tmp = static_cast<AVLNode *>(node->left);
        node->left = static_cast<AVLNode *>(node->right);
        node->right = tmp;
    };

    void swap_data_and_childs(AVLNode* first,AVLNode* second){

    };

    AVLNode *LL(AVLNode *node) {
        swap_data(node, static_cast<AVLNode *>(node->left));
        swap_pointer(node);
        swap_pointer(static_cast<AVLNode *>(node->right));

        AVLNode*tmp=static_cast<AVLNode *>(node->right->right);
        AVLNode*tmp1=static_cast<AVLNode *>(node->left);
        tmp->parent=node;



        tmp1->parent=node->right;
        // node->right->right->parent=node;

        Balance(node);
        Balance(static_cast<AVLNode *>(node->right));

        return node;
    };

    AVLNode *RR(AVLNode *node) {
        swap_data(node,static_cast<AVLNode*>(node->right));
        swap_pointer(node);
        swap_pointer(static_cast<AVLNode*>(node->left));

        AVLNode *tmp = static_cast<AVLNode *>(node->left->left);

        AVLNode *tmp1 = static_cast<AVLNode *>(node->right);
        tmp->parent = node;

        tmp1->parent = static_cast<AVLNode*>(node->left);

        Balance(node);
        Balance(static_cast<AVLNode *>(node->left));

        return node;
    };

    AVLNode *RL(AVLNode *node) {
        swap_data(node,static_cast<AVLNode*>(node->right->left));
        AVLNode* n1=static_cast<AVLNode*>(node->left);
        AVLNode* n2=static_cast<AVLNode*>(node->right->left);
        n1->parent=static_cast<AVLNode*>(node->right);
        n2->parent=node;
        swap_pointer(n1);

        AVLNode* n3=static_cast<AVLNode*>(node->left->left);

        n3->parent=static_cast<AVLNode*>(node->right);



        return node;
    };
    AVLNode *LR(AVLNode *node) {
        node->data.key++;
        node->data.key--;
        return nullptr;
    };

    AVLNode *_Insert(AVLNode *node, const T key, const H value, AVLNode *parent) {
        if (node == nullptr) {
            return new AVLNode(Pair{key, value}, parent);
        }
        if (key < node->data.key) {
            node->left = _Insert(static_cast<AVLNode *>(node->left), key, value, node);

        } else if (key > node->data.key) {
            node->right = _Insert(static_cast<AVLNode *>(node->right), key, value, node);
        } else {
            throw invalid_argument("Error");
        }

        if (Balance(node) == 2) {
            if (Balance(static_cast<AVLNode *>(node->right)) == 1) {
                RR(node);
            } else {
                RL(node);
            }
        }
        if (Balance(node) == -1) {
            if (Balance(static_cast<AVLNode *>(node->left)) == 1) {
                LR(node);
            } else {
                LL(node);
            }
        }

        return node;
    };

    int GetHeight(AVLNode *node) {
        if (node == nullptr) return 0;

        int leftHeight = GetHeight(static_cast<AVLNode *>(node->left));
        int rightHeight = GetHeight(static_cast<AVLNode *>(node->right));

        return 1 + max(leftHeight, rightHeight);
    };
};

template <typename T, typename H>
class RedBlackTree : protected Tree<T, H> {
   protected:
    using typename Tree<T, H>::Pair;
    using typename Tree<T, H>::Node;

    struct RedBlackNode : public Node {
        char color;

        RedBlackNode(const Pair &d, Node *p = nullptr) : Node(d, nullptr, nullptr, p), color('b') {}
    };

    RedBlackNode *pFirst;
    using Iterator = TreeIterator<RedBlackNode>;

   public:
    Iterator begin() {
        RedBlackNode *current = pFirst;
        while (current->left != nullptr) {
            current = static_cast<RedBlackNode *>(current->left);
        }

        return Iterator(current);
    }
    Iterator end() {
        RedBlackNode *current = pFirst;
        while (current->right != nullptr) {
            current = static_cast<RedBlackNode *>(current->right);
        }

        return Iterator(current);
    }
    RedBlackTree() : pFirst(nullptr) {}

    // void Delete(T key) {}
    // void Insert(T key, H value) {}

    // int Balance(AVLNode *node) { return 1; }

    ~RedBlackTree() {}
};
