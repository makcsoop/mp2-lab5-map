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

        AVLNode() : Node(Pair{T{}, H{}}, nullptr, nullptr, nullptr), _balance(0) {}

        // AVLNode&operator=(AVLNode* other){

        // }
        AVLNode(const AVLNode &other, Node *parent = nullptr)
            : Node(other.data, nullptr, nullptr, parent), _balance(other._balance) {
            if (other.left != nullptr) {
                this->left = new AVLNode(*static_cast<const AVLNode *>(other.left), this);
            }
            if (other.right != nullptr) {
                this->right = new AVLNode(*static_cast<const AVLNode *>(other.right), this);
            }
        }
    };
    AVLNode *_pFirst;
    int _sz;

   public:
    typename Tree<T, H>::Iterator begin() {
        AVLNode *current = _pFirst;
        while (current->left != nullptr) {
            current = static_cast<AVLNode *>(current->left);  // Исправлено
        }
        return Iterator(current);
    }

    typename Tree<T, H>::Iterator end() {
        AVLNode *current = _pFirst;
        while (current->right != nullptr) {
            current = static_cast<AVLNode *>(current->right);  // Исправлено
        }
        return Iterator(current);
    }

    AVLTree() : _pFirst(nullptr), _sz(0) {}

    int count() { return _sz; }

    H *Find(T key) {
        AVLNode *current = _pFirst;
        while (current != nullptr) {
            if (current->data.key == key) {
                return &current->data.value;
            }
            if (current->data.key > key) {
                current = static_cast<AVLNode *>(current->left);
            } else {
                current = static_cast<AVLNode *>(current->right);
            }
        }
        return nullptr;
    }

    AVLNode *FindNode(T key) {
        AVLNode *current = _pFirst;
        while (current != nullptr) {
            if (current->data.key == key) {
                return current;
            }
            if (current->data.key > key) {
                current = static_cast<AVLNode *>(current->left);
            } else {
                current = static_cast<AVLNode *>(current->right);
            }
        }
        return nullptr;
    }

    void Delete(T key) {
        AVLNode *node = FindNode(key);
        if (node == nullptr) return;

        AVLNode *parent = static_cast<AVLNode *>(node->parent);
        bool isRoot = (node == _pFirst);

        // лист
        if (node->left == nullptr && node->right == nullptr) {
            if (isRoot) {
                delete _pFirst;
                _pFirst = nullptr;
                _sz = 0;
                return;
            }

            if (parent->left == node)
                parent->left = nullptr;
            else
                parent->right = nullptr;

            delete node;
            _sz--;

            AVLNode *current = parent;
            while (current != nullptr) {
                AVLNode *next = static_cast<AVLNode *>(current->parent);
                Rotate(current);
                current = next;
            }
            return;
        }

        // один потомок
        if (node->left == nullptr || node->right == nullptr) {
            AVLNode *child = static_cast<AVLNode *>((node->left != nullptr) ? node->left : node->right);

            if (isRoot) {
                child->parent = nullptr;
                delete _pFirst;
                _pFirst = child;
                _sz--;

                AVLNode *current = _pFirst;
                while (current != nullptr) {
                    AVLNode *next = static_cast<AVLNode *>(current->parent);
                    Rotate(current);
                    current = next;
                }
                return;
            }

            if (parent->left == node)
                parent->left = child;
            else
                parent->right = child;
            child->parent = parent;

            delete node;
            _sz--;

            AVLNode *current = parent;
            while (current != nullptr) {
                AVLNode *next = static_cast<AVLNode *>(current->parent);
                Rotate(current);
                current = next;
            }
            return;
        }

        // два потомка
        AVLNode *min = static_cast<AVLNode *>(node->right);
        while (min->left != nullptr) min = static_cast<AVLNode *>(min->left);

        swap_data(node, min);

        AVLNode *minParent = static_cast<AVLNode *>(min->parent);

        if (minParent->left == min)
            minParent->left = min->right;
        else
            minParent->right = min->right;

        if (min->right != nullptr) min->right->parent = minParent;

        delete min;
        _sz--;

        AVLNode *current = minParent;
        while (current != nullptr) {
            AVLNode *next = static_cast<AVLNode *>(current->parent);
            Rotate(current);
            current = next;
        }
    }
    void Insert(T key, H value) override {
        _pFirst = _Insert(_pFirst, key, value, nullptr);
        _sz++;
    }

    H &operator[](T key) {
        H *tmp = this->Find(key);
        if (tmp == nullptr) {
            this->Insert(key, H{});
            return *this->Find(key);
        }
        return *tmp;
    }

    int Balance(AVLNode *node) {
        if (node == nullptr) return 0;

        int leftHeight = GetHeight(static_cast<AVLNode *>(node->left));
        int rightHeight = GetHeight(static_cast<AVLNode *>(node->right));

        int balance = rightHeight - leftHeight;

        node->_balance = balance;
        return balance;
    }

    void Rotate(AVLNode *node) {
        if (Balance(node) == 2) {
            AVLNode *tmp = static_cast<AVLNode *>(node->right);
            if (tmp != nullptr) {
                if (Balance(tmp) == 1) {
                    node = RR(node);
                } else if (Balance(tmp) == -1) {
                    node = RL(node);
                }

                return;
            }
        } else if (Balance(node) == -2) {
            AVLNode *tmp = static_cast<AVLNode *>(node->left);
            if (tmp != nullptr) {
                if (Balance(tmp) == 1) {
                    node = LR(node);
                } else if (Balance(tmp) == -1) {
                    node = LL(node);
                }
                return;
            }
        }

        return;
    }

    void _printTreeWithKey(AVLNode *root, std::string indent = "", bool isLeft = true) {
        if (root == nullptr) return;
        if (static_cast<AVLNode *>(root->right)) {
            _printTreeWithKey(static_cast<AVLNode *>(root->right), indent + (isLeft ? "│   " : "    "),
                              false);
        }
        cout << indent << (isLeft ? "└── " : "┌── ") << "(" << root->data.key /*<< ',' << root->data.value*/
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

    AVLNode *LL(AVLNode *node) {
        if (node == nullptr || node->left == nullptr) return node;

        AVLNode *left_child = static_cast<AVLNode *>(node->left);
        AVLNode *parent = static_cast<AVLNode *>(node->parent);
        AVLNode *left_child_right = static_cast<AVLNode *>(left_child->right);

        if (left_child == nullptr) return node;

        node->left = left_child_right;
        if (left_child_right != nullptr) {
            left_child_right->parent = node;
        }

        left_child->right = node;
        node->parent = left_child;

        left_child->parent = parent;
        if (parent != nullptr) {
            if (parent->left == node) {
                parent->left = left_child;
            } else {
                parent->right = left_child;
            }
        }
        if (node == _pFirst) {
            _pFirst = left_child;
        }
        Balance(node);
        Balance(left_child);

        return left_child;
    }

    AVLNode *RR(AVLNode *node) {
        if (node == nullptr || node->right == nullptr) return node;

        AVLNode *right_child = static_cast<AVLNode *>(node->right);
        AVLNode *parent = static_cast<AVLNode *>(node->parent);

        node->right = right_child->left;
        if (right_child->left != nullptr) {
            right_child->left->parent = node;
        }

        right_child->left = node;
        right_child->parent = parent;
        node->parent = right_child;

        if (parent != nullptr) {
            if (parent->left == node) {
                parent->left = right_child;
            } else {
                parent->right = right_child;
            }
        }

        if (node == _pFirst) {
            _pFirst = right_child;
        }

        Balance(node);
        Balance(right_child);

        return right_child;
    }

    AVLNode *RL(AVLNode *node) {
        if (node == nullptr || node->right == nullptr) return node;

        AVLNode *right_child = static_cast<AVLNode *>(node->right);
        AVLNode *rc_left_child = static_cast<AVLNode *>(node->right->left);

        AVLNode *root = static_cast<AVLNode *>(node->parent);

        AVLNode *rclc_left = static_cast<AVLNode *>(rc_left_child->left);

        if (rc_left_child->right != nullptr) {
            rc_left_child->right->parent = right_child;
        }
        right_child->left = rc_left_child->right;

        rc_left_child->parent = root;
        rc_left_child->right = right_child;

        if (root != nullptr) {
            if (static_cast<AVLNode *>(root->left) == node) {
                root->left = rc_left_child;
            } else {
                root->right = rc_left_child;
            }
        } else if (node == _pFirst) {
            _pFirst = rc_left_child;
        }

        if (rclc_left != nullptr) {
            rclc_left->parent = node;
        }
        right_child->parent = rc_left_child;

        rc_left_child->left = node;
        node->parent = rc_left_child;

        Balance(node);
        Balance(right_child);
        Balance(rc_left_child);

        return rc_left_child;
    }
    AVLNode *LR(AVLNode *node) {
        if (node == nullptr || node->left == nullptr) return node;

        AVLNode *left_child = static_cast<AVLNode *>(node->left);
        AVLNode *lc_right_child = static_cast<AVLNode *>(node->left->right);

        if (lc_right_child == nullptr) return LL(node);

        AVLNode *root = static_cast<AVLNode *>(node->parent);

        AVLNode *lcrc_right = static_cast<AVLNode *>(lc_right_child->right);

        if (lc_right_child->left != nullptr) {
            lc_right_child->left->parent = left_child;
        }
        left_child->right = lc_right_child->left;

        lc_right_child->parent = root;
        lc_right_child->left = left_child;

        if (root != nullptr) {
            if (static_cast<AVLNode *>(root->left) == node) {
                root->left = lc_right_child;
            } else {
                root->right = lc_right_child;
            }
        } else if (node == _pFirst) {
            _pFirst = lc_right_child;
        }

        if (lcrc_right != nullptr) {
            lcrc_right->parent = node;
        }
        left_child->parent = lc_right_child;

        lc_right_child->right = node;
        node->parent = lc_right_child;

        Balance(node);
        Balance(left_child);
        Balance(lc_right_child);

        return lc_right_child;
    }

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
                node = RR(node);
            } else {
                node = RL(node);
            }
        }
        if (Balance(node) == -2) {
            if (Balance(static_cast<AVLNode *>(node->left)) == 1) {
                node = LR(node);
            } else {
                node = LL(node);
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
