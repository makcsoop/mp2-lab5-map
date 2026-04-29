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
    using pointer = TypeNode *;
    using reference = TypeNode &;

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

template <typename T, typename H, typename Derived>
struct BaseNode {
    using Pair = typename Map<T, H>::Pair;
    Pair data;
    Derived *left;
    Derived *right;
    Derived *parent;
    BaseNode(const Pair &d, Derived *l = nullptr, Derived *r = nullptr, Derived *p = nullptr)
        : data(d), left(l), right(r), parent(p) {}
};

template <typename T, typename H>
struct Node : public BaseNode<T, H, Node<T, H>> {
    using Base = BaseNode<T, H, Node<T, H>>;
    using typename Base::Pair;

    Node(const Pair &d, Node *l = nullptr, Node *r = nullptr, Node *p = nullptr) : Base(d, l, r, p) {}
};

template <typename T, typename H>
struct AVLNode : public BaseNode<T, H, AVLNode<T, H>> {
    using Base = BaseNode<T, H, AVLNode<T, H>>;
    using typename Base::Pair;
    int _balance;

    AVLNode(const Pair &d, AVLNode *l = nullptr, AVLNode *r = nullptr, AVLNode *p = nullptr, int _b = 0)
        : Base(d, l, r, p), _balance(_b) {}

    AVLNode() : Base(Pair{T{}, H{}}, nullptr, nullptr, nullptr), _balance(0) {}

    AVLNode(const AVLNode &other, AVLNode *parent = nullptr)
        : Base(other.data, nullptr, nullptr, parent), _balance(other._balance) {
        if (other.left != nullptr) {
            this->left = new AVLNode(*static_cast<const AVLNode *>(other.left), this);
        }
        if (other.right != nullptr) {
            this->right = new AVLNode(*static_cast<const AVLNode *>(other.right), this);
        }
    }
};

template <typename T, typename H>
struct RBNode : public BaseNode<T, H, RBNode<T, H>> {
    using Base = BaseNode<T, H, RBNode<T, H>>;
    using typename Base::Pair;
    char color;
    RBNode(const Pair &d, RBNode *l = nullptr, RBNode *r = nullptr, RBNode *p = nullptr, char c = 'r')
        : Base(d, l, r, p), color(c) {}
};

template <typename T, typename H, typename BaseNode>
class BaseTree : public Map<T, H> {
   protected:
    using typename Map<T, H>::Pair;
    BaseNode *pFirst;
    int sz = 0;

   public:
    using Iterator = TreeIterator<BaseNode>;

    Iterator begin() {
        BaseNode *current = pFirst;
        if (!current) return Iterator(nullptr);
        while (current->left != nullptr) {
            current = current->left;
        }
        return Iterator(current);
    }

    Iterator end() {
        BaseNode *current = pFirst;
        if (!current) return Iterator(nullptr);
        while (current->right != nullptr) {
            current = current->right;
        }
        return Iterator(current);
    }

    BaseTree() : pFirst(nullptr), sz(0) {}

    BaseTree(T key, H value) : pFirst(new BaseNode(Pair{key, value}, nullptr, nullptr, nullptr)), sz(1) {}

    BaseTree(const BaseTree<T, H, BaseNode> &other) : pFirst(nullptr), sz(0) {
        pFirst = copyNode(other.pFirst);
        sz = other.sz;
    }

    // ОПЕРАТОР ПРИСВАИВАНИЯ - ИСПРАВЛЕНО
    BaseTree &operator=(const BaseTree<T, H, BaseNode> &other) {
        if (this != &other) {
            // Очищаем текущее дерево
            deleteNode(pFirst);
            // Копируем новое
            pFirst = copyNode(other.pFirst);
            sz = other.sz;
        }
        return *this;
    }

    BaseTree(vector<Pair> elements) {
        logger("Base const vector<Pair>", 1);
        sort(elements.begin(), elements.end());
        int plug = 0;
        pFirst = nullptr;
        sz = 0;
        while (elements.size() != 0) {
            if (plug % 3 == 0) {
                this->Insert(elements[elements.size() / 2].key, elements[elements.size() / 2].value);
                elements.erase(elements.begin() + elements.size() / 2);
            } else if (plug % 2 == 0) {
                this->Insert(elements[elements.size() / 4].key, elements[elements.size() / 4].value);
                elements.erase(elements.begin() + elements.size() / 4);
            } else {
                this->Insert(elements[elements.size() / 4 * 3].key, elements[elements.size() / 4 * 3].value);
                elements.erase(elements.begin() + elements.size() / 4 * 3);
            }
            plug++;
        }
    }

    BaseTree(vector<T> elements) {
        logger("Base const vector<T>", 1);
        sort(elements.begin(), elements.end());
        int plug = 0;
        pFirst = nullptr;
        sz = 0;
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

    virtual ~BaseTree() { deleteNode(pFirst); }

    BaseNode *copyNode(BaseNode *node, BaseNode *parent = nullptr) {
        if (node == nullptr) {
            return nullptr;
        }
        BaseNode *new_node = new BaseNode(node->data, nullptr, nullptr, parent);
        new_node->left = copyNode(node->left, new_node);
        new_node->right = copyNode(node->right, new_node);
        return new_node;
    }

    void deleteNode(BaseNode *current) {
        if (current == nullptr) return;
        deleteNode(current->left);
        deleteNode(current->right);
        delete current;
    }

    BaseNode *GetFirst() { return pFirst; }

    int count(T key) {
        int res = 0;
        for (auto x : keys()) {
            if (x == key) {
                res++;
            }
        }
        return res;
    }
    int size() { return sz; };

    void Insert(T key, H value) {
        if (pFirst == nullptr) {
            pFirst = new BaseNode{Pair{key, value}, nullptr, nullptr, nullptr};
            sz++;
        } else {
            BaseNode *tmp = pFirst;
            while (true) {
                if (tmp->data.key > key) {
                    if (tmp->left != nullptr) {
                        tmp = tmp->left;
                    } else {
                        BaseNode *new_node = new BaseNode{Pair{key, value}, nullptr, nullptr, tmp};
                        tmp->left = new_node;
                        sz++;
                        break;
                    }
                } else if (tmp->data.key < key) {
                    if (tmp->right != nullptr) {
                        tmp = tmp->right;
                    } else {
                        BaseNode *new_node = new BaseNode{Pair{key, value}, nullptr, nullptr, tmp};
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

    void InsertNode(BaseNode *current) {
        if (pFirst == nullptr) {
            pFirst = current;
            sz++;
        } else {
            BaseNode *tmp = pFirst;
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

    void printTree(BaseNode *root, std::string indent = "", bool isLeft = true) {
        if (root == nullptr) return;
        if (root->right) {
            printTree(root->right, indent + (isLeft ? "│   " : "    "), false);
        }
        cout << indent << (isLeft ? "└── " : "┌── ") << root->data.key << endl;
        if (root->left) {
            printTree(root->left, indent + (isLeft ? "    " : "│   "), true);
        }
    }

    void printTreeWithKey(BaseNode *root, std::string indent = "", bool isLeft = true) {
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
        BaseNode *node = FindNode(key);
        if (!node) {
            throw runtime_error("Нет ключа");
        }
        if (node->left == nullptr && node->right == nullptr) {
            if (node == pFirst) {
                delete pFirst;
                pFirst = nullptr;
                sz = 0;
                return;
            }
            BaseNode *parent = node->parent;
            if (parent->left == node) {
                parent->left = nullptr;
            } else {
                parent->right = nullptr;
            }
            delete node;
            sz--;
        } else if (((node->left == nullptr) + (node->right == nullptr)) == 1) {
            BaseNode *child = (node->left != nullptr) ? node->left : node->right;
            if (node == pFirst) {
                child->parent = nullptr;
                delete pFirst;
                pFirst = child;
                sz--;
                return;
            }
            BaseNode *parent = node->parent;
            if (parent->left == node) {
                parent->left = child;
            } else {
                parent->right = child;
            }
            child->parent = parent;
            delete node;
            sz--;
        } else {
            if (node == pFirst) {
                BaseNode *minNode = node->right;
                while (minNode->left != nullptr) {
                    minNode = minNode->left;
                }
                T minKey = minNode->data.key;
                H minValue = minNode->data.value;
                Delete(minKey);
                pFirst = new BaseNode{Pair{minKey, minValue}, node->left, node->right, nullptr};
                if (pFirst->left) {
                    pFirst->left->parent = pFirst;
                }
                if (pFirst->right) {
                    pFirst->right->parent = pFirst;
                }
                delete node;
            } else {
                BaseNode *minNode = node->right;
                while (minNode->left != nullptr) {
                    minNode = minNode->left;
                }
                node->data.key = minNode->data.key;
                node->data.value = minNode->data.value;
                BaseNode *minParent = minNode->parent;
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

    int count() { return sz; }

    vector<T> keys() {
        vector<T> data;
        RecTree(GetFirst(), data);
        sort(data.begin(), data.end());
        return data;
    }

    void RecTree(const BaseNode *node, vector<T> &keys) {
        if (node == nullptr) return;
        keys.push_back(node->data.key);
        RecTree(node->left, keys);
        RecTree(node->right, keys);
    }

    virtual H *Find(T key) {
        BaseNode *current = pFirst;
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

    BaseNode *FindNode(T key) {
        BaseNode *current = pFirst;
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

    bool RecSearch(BaseNode *node, BaseNode *minNode, BaseNode *maxNode) {
        if (node == nullptr) return true;
        if (minNode != nullptr && node->data.key <= minNode->data.key) return false;
        if (maxNode != nullptr && node->data.key >= maxNode->data.key) return false;
        return RecSearch(node->left, minNode, node) && RecSearch(node->right, node, maxNode);
    }
};

template <typename T, typename H>
class Tree : public BaseTree<T, H, Node<T, H>> {
    using typename Map<T, H>::Pair;

   public:
    using BaseTree<T, H, Node<T, H>>::BaseTree;

    void Insert(T key, H value) override {
        if (this->pFirst == nullptr) {
            this->pFirst = new Node<T, H>{Pair{key, value}, nullptr, nullptr, nullptr};
            this->sz++;
        } else {
            Node<T, H> *tmp = this->pFirst;
            while (true) {
                if (tmp->data.key > key) {
                    if (tmp->left != nullptr) {
                        tmp = tmp->left;
                    } else {
                        Node<T, H> *new_node = new Node<T, H>{Pair{key, value}, nullptr, nullptr, tmp};
                        tmp->left = new_node;
                        this->sz++;
                        break;
                    }
                } else if (tmp->data.key < key) {
                    if (tmp->right != nullptr) {
                        tmp = tmp->right;
                    } else {
                        Node<T, H> *new_node = new Node<T, H>{Pair{key, value}, nullptr, nullptr, tmp};
                        tmp->right = new_node;
                        this->sz++;
                        break;
                    }
                } else {
                    throw invalid_argument("Incorrect insert");
                }
            }
        }
    }

    void Delete(T key) override {
        Node<T, H> *node = this->FindNode(key);
        if (!node) {
            throw runtime_error("Нет ключа");
        }
        if (node->left == nullptr && node->right == nullptr) {
            if (node == this->pFirst) {
                delete this->pFirst;
                this->pFirst = nullptr;
                this->sz = 0;
                return;
            }
            Node<T, H> *parent = node->parent;
            if (parent->left == node) {
                parent->left = nullptr;
            } else {
                parent->right = nullptr;
            }
            delete node;
            this->sz--;
        } else if (((node->left == nullptr) + (node->right == nullptr)) == 1) {
            Node<T, H> *child = (node->left != nullptr) ? node->left : node->right;
            if (node == this->pFirst) {
                child->parent = nullptr;
                delete this->pFirst;
                this->pFirst = child;
                this->sz--;
                return;
            }
            Node<T, H> *parent = node->parent;
            if (parent->left == node) {
                parent->left = child;
            } else {
                parent->right = child;
            }
            child->parent = parent;
            delete node;
            this->sz--;
        } else {
            if (node == this->pFirst) {
                Node<T, H> *minNode = node->right;
                while (minNode->left != nullptr) {
                    minNode = minNode->left;
                }
                T minKey = minNode->data.key;
                H minValue = minNode->data.value;
                Delete(minKey);
                this->pFirst = new Node<T, H>{Pair{minKey, minValue}, node->left, node->right, nullptr};
                if (this->pFirst->left) {
                    this->pFirst->left->parent = this->pFirst;
                }
                if (this->pFirst->right) {
                    this->pFirst->right->parent = this->pFirst;
                }
                delete node;
            } else {
                Node<T, H> *minNode = node->right;
                while (minNode->left != nullptr) {
                    minNode = minNode->left;
                }
                node->data.key = minNode->data.key;
                node->data.value = minNode->data.value;
                Node<T, H> *minParent = minNode->parent;
                if (minParent->left == minNode) {
                    minParent->left = minNode->right;
                } else {
                    minParent->right = minNode->right;
                }
                if (minNode->right) {
                    minNode->right->parent = minParent;
                }
                delete minNode;
                this->sz--;
            }
        }
    }
};

template <typename T, typename H>
class AVLTree : public BaseTree<T, H, AVLNode<T, H>> {
   protected:
    using typename BaseTree<T, H, AVLNode<T, H>>::Pair;
    using Node = AVLNode<T, H>;
    using Iterator = TreeIterator<Node>;

    Node *_pFirst;
    int _sz;

   public:
    Iterator begin() {
        Node *current = _pFirst;
        if (!current) return Iterator(nullptr);
        while (current->left != nullptr) {
            current = static_cast<Node *>(current->left);
        }
        return Iterator(current);
    }

    Iterator end() {
        Node *current = _pFirst;
        if (!current) return Iterator(nullptr);
        while (current->right != nullptr) {
            current = static_cast<Node *>(current->right);
        }
        return Iterator(current);
    }

    AVLTree() : _pFirst(nullptr), _sz(0) {}

    AVLTree(T key, H value) : _pFirst(new Node(Pair{key, value})), _sz(1) {}

    AVLTree(const AVLTree &other) {
        _pFirst = copyNode(other._pFirst);
        _sz = other._sz;
    }

    AVLTree &operator=(const AVLTree &other) {
        if (this != &other) {
            deleteNode(_pFirst);
            _pFirst = copyNode(other._pFirst);
            _sz = other._sz;
        }
        return *this;
    }

    AVLTree(vector<Pair> elements) {
        sort(elements.begin(), elements.end());
        _pFirst = nullptr;
        _sz = 0;
        for (const auto &elem : elements) {
            Insert(elem.key, elem.value);
        }
    }

    AVLTree(vector<T> elements) {
        sort(elements.begin(), elements.end());
        _pFirst = nullptr;
        _sz = 0;
        for (const auto &key : elements) {
            Insert(key, H{});
        }
    }

    int count(T key) override {
        int res = 0;
        for (auto x : keys()) {
            if (x == key) {
                res++;
            }
        }
        return res;
    }
    int size() override { return _sz; };

    int count() override { return _sz; }

    vector<T> keys() override {
        vector<T> data;
        this->RecTree(this->GetFirst(), data);
        sort(data.begin(), data.end());
        return data;
    }

    H *Find(T key) override {
        Node *current = _pFirst;
        while (current != nullptr) {
            if (current->data.key == key) {
                return &current->data.value;
            }
            if (current->data.key > key) {
                current = static_cast<Node *>(current->left);
            } else {
                current = static_cast<Node *>(current->right);
            }
        }
        return nullptr;
    }

    Node *FindNode(T key) {
        Node *current = _pFirst;
        while (current != nullptr) {
            if (current->data.key == key) {
                return current;
            }
            if (current->data.key > key) {
                current = static_cast<Node *>(current->left);
            } else {
                current = static_cast<Node *>(current->right);
            }
        }
        return nullptr;
    }

    void Delete(T key) override {
        Node *node = FindNode(key);
        if (node == nullptr) return;

        Node *parent = static_cast<Node *>(node->parent);
        bool isRoot = (node == _pFirst);

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
            Node *current = parent;
            while (current != nullptr) {
                Node *next = static_cast<Node *>(current->parent);
                Rotate(current);
                current = next;
            }
            return;
        }

        if (node->left == nullptr || node->right == nullptr) {
            Node *child = static_cast<Node *>((node->left != nullptr) ? node->left : node->right);
            if (isRoot) {
                child->parent = nullptr;
                delete _pFirst;
                _pFirst = child;
                _sz--;
                Node *current = _pFirst;
                while (current != nullptr) {
                    Node *next = static_cast<Node *>(current->parent);
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
            Node *current = parent;
            while (current != nullptr) {
                Node *next = static_cast<Node *>(current->parent);
                Rotate(current);
                current = next;
            }
            return;
        }

        Node *min = static_cast<Node *>(node->right);
        while (min->left != nullptr) min = static_cast<Node *>(min->left);
        swap_data(node, min);
        Node *minParent = static_cast<Node *>(min->parent);
        if (minParent->left == min)
            minParent->left = min->right;
        else
            minParent->right = min->right;
        if (min->right != nullptr) static_cast<Node *>(min->right)->parent = minParent;
        delete min;
        _sz--;
        Node *current = minParent;
        while (current != nullptr) {
            Node *next = static_cast<Node *>(current->parent);
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

    int Balance(Node *node) {
        if (node == nullptr) return 0;
        int leftHeight = GetHeight(static_cast<Node *>(node->left));
        int rightHeight = GetHeight(static_cast<Node *>(node->right));
        int balance = rightHeight - leftHeight;
        node->_balance = balance;
        return balance;
    }

    void Rotate(Node *node) {
        if (node == nullptr) return;
        if (Balance(node) == 2) {
            Node *tmp = static_cast<Node *>(node->right);
            if (tmp != nullptr) {
                if (Balance(tmp) == 1) {
                    node = RR(node);
                } else if (Balance(tmp) == -1) {
                    node = RL(node);
                }
            }
        } else if (Balance(node) == -2) {
            Node *tmp = static_cast<Node *>(node->left);
            if (tmp != nullptr) {
                if (Balance(tmp) == 1) {
                    node = LR(node);
                } else if (Balance(tmp) == -1) {
                    node = LL(node);
                }
            }
        }
    }

    void printTreeWithKey(Node *root = nullptr, std::string indent = "", bool isLeft = true) {
        if (root == nullptr) root = _pFirst;
        if (root == nullptr) return;
        if (static_cast<Node *>(root->right)) {
            printTreeWithKey(static_cast<Node *>(root->right), indent + (isLeft ? "│   " : "    "), false);
        }
        cout << indent << (isLeft ? "└── " : "┌── ") << "(" << root->data.key << "," << root->data.value
             << "," << root->_balance << ")" << endl;
        if (static_cast<Node *>(root->left)) {
            printTreeWithKey(static_cast<Node *>(root->left), indent + (isLeft ? "    " : "│   "), true);
        }
    }

    Node *GetFirst() { return _pFirst; }

    ~AVLTree() { deleteNode(_pFirst); }

    bool isTrueSort() { return RecSearch(_pFirst, nullptr, nullptr); }

   private:
    void deleteNode(Node *current) {
        if (current == nullptr) return;
        deleteNode(static_cast<Node *>(current->left));
        deleteNode(static_cast<Node *>(current->right));
        delete current;
    }

    Node *copyNode(Node *node, Node *parent = nullptr) {
        if (node == nullptr) return nullptr;
        Node *new_node = new Node(node->data, nullptr, nullptr, parent, node->_balance);
        new_node->left = copyNode(static_cast<Node *>(node->left), new_node);
        new_node->right = copyNode(static_cast<Node *>(node->right), new_node);
        return new_node;
    }

    bool RecSearch(Node *node, Node *minNode, Node *maxNode) {
        if (node == nullptr) return true;
        if (minNode != nullptr && node->data.key <= minNode->data.key) return false;
        if (maxNode != nullptr && node->data.key >= maxNode->data.key) return false;
        return RecSearch(static_cast<Node *>(node->left), minNode, node) &&
               RecSearch(static_cast<Node *>(node->right), node, maxNode);
    }

    void swap_data(Node *first, Node *second) {
        Pair tmp = first->data;
        first->data = second->data;
        second->data = tmp;
    }

    Node *LL(Node *node) {
        if (node == nullptr || node->left == nullptr) return node;
        Node *left_child = static_cast<Node *>(node->left);
        Node *parent = static_cast<Node *>(node->parent);
        Node *left_child_right = static_cast<Node *>(left_child->right);
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

    Node *RR(Node *node) {
        if (node == nullptr || node->right == nullptr) return node;
        Node *right_child = static_cast<Node *>(node->right);
        Node *parent = static_cast<Node *>(node->parent);
        node->right = right_child->left;
        if (right_child->left != nullptr) {
            static_cast<Node *>(right_child->left)->parent = node;
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

    Node *RL(Node *node) {
        if (node == nullptr || node->right == nullptr) return node;
        Node *right_child = static_cast<Node *>(node->right);
        Node *rc_left_child = static_cast<Node *>(node->right->left);
        Node *parent = static_cast<Node *>(node->parent);
        if (rc_left_child == nullptr) return RR(node);
        Node *rclc_left = static_cast<Node *>(rc_left_child->left);
        Node *rclc_right = static_cast<Node *>(rc_left_child->right);
        rc_left_child->left = node;
        rc_left_child->right = right_child;
        rc_left_child->parent = parent;
        node->parent = rc_left_child;
        node->right = rclc_left;
        if (rclc_left) rclc_left->parent = node;
        right_child->parent = rc_left_child;
        right_child->left = rclc_right;
        if (rclc_right) rclc_right->parent = right_child;
        if (parent != nullptr) {
            if (parent->left == node) {
                parent->left = rc_left_child;
            } else {
                parent->right = rc_left_child;
            }
        }
        if (node == _pFirst) {
            _pFirst = rc_left_child;
        }
        Balance(node);
        Balance(right_child);
        Balance(rc_left_child);
        return rc_left_child;
    }

    Node *LR(Node *node) {
        if (node == nullptr || node->left == nullptr) return node;
        Node *left_child = static_cast<Node *>(node->left);
        Node *lc_right_child = static_cast<Node *>(node->left->right);
        Node *parent = static_cast<Node *>(node->parent);
        if (lc_right_child == nullptr) return LL(node);
        Node *lcrc_left = static_cast<Node *>(lc_right_child->left);
        Node *lcrc_right = static_cast<Node *>(lc_right_child->right);
        lc_right_child->right = node;
        lc_right_child->left = left_child;
        lc_right_child->parent = parent;
        node->parent = lc_right_child;
        node->left = lcrc_right;
        if (lcrc_right) lcrc_right->parent = node;
        left_child->parent = lc_right_child;
        left_child->right = lcrc_left;
        if (lcrc_left) lcrc_left->parent = left_child;
        if (parent != nullptr) {
            if (parent->left == node) {
                parent->left = lc_right_child;
            } else {
                parent->right = lc_right_child;
            }
        }
        if (node == _pFirst) {
            _pFirst = lc_right_child;
        }
        Balance(node);
        Balance(left_child);
        Balance(lc_right_child);
        return lc_right_child;
    }

    Node *_Insert(Node *node, const T key, const H value, Node *parent) {
        if (node == nullptr) {
            return new Node(Pair{key, value}, nullptr, nullptr, parent, 0);
        }
        if (key < node->data.key) {
            node->left = _Insert(static_cast<Node *>(node->left), key, value, node);
        } else if (key > node->data.key) {
            node->right = _Insert(static_cast<Node *>(node->right), key, value, node);
        } else {
            throw std::invalid_argument("Key already exists");
        }
        Balance(node);
        if (Balance(node) == 2) {
            if (Balance(static_cast<Node *>(node->right)) == 1) {
                node = RR(node);
            } else {
                node = RL(node);
            }
        } else if (Balance(node) == -2) {
            if (Balance(static_cast<Node *>(node->left)) == 1) {
                node = LR(node);
            } else {
                node = LL(node);
            }
        }
        return node;
    }

    int GetHeight(Node *node) {
        if (node == nullptr) return 0;
        int leftHeight = GetHeight(static_cast<Node *>(node->left));
        int rightHeight = GetHeight(static_cast<Node *>(node->right));
        return 1 + std::max(leftHeight, rightHeight);
    }
};

// ПОЛНОСТЬЮ ИСПРАВЛЕННЫЙ REDBLACKTREE
template <typename T, typename H>
class RedBlackTree : public BaseTree<T, H, RBNode<T, H>> {
   protected:
    using typename Map<T, H>::Pair;
    using RBNodePtr = RBNode<T, H> *;

   public:
    using BaseTree<T, H, RBNode<T, H>>::BaseTree;

    // КОНСТРУКТОР ПО УМОЛЧАНИЮ
    RedBlackTree() : BaseTree<T, H, RBNode<T, H>>() {}

    // КОНСТРУКТОР КОПИРОВАНИЯ
    RedBlackTree(const RedBlackTree &other) : BaseTree<T, H, RBNode<T, H>>(other) {}

    // ОПЕРАТОР ПРИСВАИВАНИЯ
    RedBlackTree &operator=(const RedBlackTree &other) {
        if (this != &other) {
            BaseTree<T, H, RBNode<T, H>>::operator=(other);
        }
        return *this;
    }

    // Конструктор от вектора
    RedBlackTree(vector<T> elements) {
        logger("RedBlackTree const vector<T>", 1);
        sort(elements.begin(), elements.end());
        int plug = 0;
        this->pFirst = nullptr;
        this->sz = 0;
        while (elements.size() != 0) {
            if (plug % 3 == 0) {
                this->Insert(elements[elements.size() / 2], H{});
                elements.erase(elements.begin() + elements.size() / 2);
            } else if (plug % 2 == 0) {
                this->Insert(elements[elements.size() / 4], H{});
                elements.erase(elements.begin() + elements.size() / 4);
            } else {
                this->Insert(elements[elements.size() / 4 * 3], H{});
                elements.erase(elements.begin() + elements.size() / 4 * 3);
            }
            plug++;
        }
    }

    void Insert(T key, H value) override {
        BaseTree<T, H, RBNode<T, H>>::Insert(key, value);
        RBNodePtr needNode = this->FindNode(key);
        if (needNode != nullptr) {
            this->Balance(needNode);
        } else {
            throw invalid_argument("Incorrect insert");
        }
    }

    void Balance(RBNodePtr childNode) {
        if (childNode->parent != nullptr) {
            RBNodePtr parentNode = childNode->parent;
            if (parentNode->color == 'r') {
                if (parentNode->parent == nullptr) {
                    parentNode->color = 'b';
                    return;
                }
                RBNodePtr uncle;
                if (parentNode == parentNode->parent->left) {
                    uncle = childNode->parent->parent->right;
                } else {
                    uncle = childNode->parent->parent->left;
                }
                if (uncle == nullptr || uncle->color == 'b') {
                    if (parentNode == parentNode->parent->left && childNode == parentNode->left) {
                        this->LL(childNode);
                    } else if (parentNode == parentNode->parent->left && childNode == parentNode->right) {
                        this->LR(childNode);
                    } else if (parentNode == parentNode->parent->right && childNode == parentNode->right) {
                        this->RR(childNode);
                    } else {
                        this->RL(childNode);
                    }
                } else {
                    uncle->color = 'b';
                    parentNode->color = 'b';
                    parentNode->parent->color = 'r';
                    Balance(parentNode->parent);
                }
            }
        } else {
            childNode->color = 'b';
        }
    }

    void LL(RBNodePtr childNode) {
        RBNodePtr parentNode = childNode->parent;
        RBNodePtr childRight = parentNode->right;
        RBNodePtr grandpa = parentNode->parent;
        RBNodePtr root = grandpa->parent;
        if (root == nullptr) {
            this->pFirst = parentNode;
            parentNode->parent = nullptr;
        } else {
            parentNode->parent = root;
            if (root->left == grandpa) {
                root->left = parentNode;
            } else {
                root->right = parentNode;
            }
        }
        parentNode->color = 'b';
        parentNode->right = grandpa;
        grandpa->parent = parentNode;
        grandpa->left = childRight;
        grandpa->color = 'r';
        if (childRight) childRight->parent = grandpa;
        Balance(parentNode);
    }

    void LR(RBNodePtr childNode) {
        RBNodePtr parentNode = childNode->parent;
        RBNodePtr grandpa = parentNode->parent;
        grandpa->left = childNode;
        childNode->parent = grandpa;
        childNode->left = parentNode;
        parentNode->right = nullptr;
        parentNode->parent = childNode;
        this->LL(parentNode);
    }

    void RR(RBNodePtr childNode) {
        RBNodePtr parentNode = childNode->parent;
        RBNodePtr childLeft = parentNode->left;
        RBNodePtr grandpa = parentNode->parent;
        RBNodePtr root = grandpa->parent;
        if (root == nullptr) {
            this->pFirst = parentNode;
            parentNode->parent = nullptr;
        } else {
            parentNode->parent = root;
            if (root->right == grandpa) {
                root->right = parentNode;
            } else {
                root->left = parentNode;
            }
        }
        parentNode->color = 'b';
        parentNode->left = grandpa;
        grandpa->parent = parentNode;
        grandpa->right = childLeft;
        grandpa->color = 'r';
        if (childLeft) childLeft->parent = grandpa;
        Balance(parentNode);
    }

    void RL(RBNodePtr childNode) {
        RBNodePtr parentNode = childNode->parent;
        RBNodePtr grandpa = parentNode->parent;
        grandpa->right = childNode;
        childNode->parent = grandpa;
        childNode->right = parentNode;
        parentNode->left = nullptr;
        parentNode->parent = childNode;
        this->RR(parentNode);
    }

    void printTree(RBNodePtr root, std::string indent = "", bool isLeft = true) {
        if (root == nullptr) return;
        if (root->right) {
            printTree(root->right, indent + (isLeft ? "│   " : "    "), false);
        }
        cout << indent << (isLeft ? "└── " : "┌── ") << "( " << root->data.key << "," << root->color << " )"
             << endl;
        if (root->left) {
            printTree(root->left, indent + (isLeft ? "    " : "│   "), true);
        }
    }

    void Delete(T key) override {
        RBNodePtr node = this->FindNode(key);
        if (!node) {
            throw runtime_error("Нет ключа");
        }

        if (node->left != nullptr && node->right != nullptr) {
            RBNodePtr successor = node->right;
            while (successor->left != nullptr) {
                successor = successor->left;
            }
            T successorKey = successor->data.key;
            H successorValue = successor->data.value;
            Delete(successorKey);
            node->data.key = successorKey;
            node->data.value = successorValue;
            return;
        }

        if (node->color == 'r' && node->left == nullptr && node->right == nullptr) {
            RBNodePtr parent = node->parent;
            if (parent != nullptr) {
                if (parent->left == node) {
                    parent->left = nullptr;
                } else {
                    parent->right = nullptr;
                }
            }
            delete node;
            this->sz--;
            return;
        }

        if (node->color == 'b' && ((node->left != nullptr) ^ (node->right != nullptr)) == 1) {
            RBNodePtr child = (node->left != nullptr) ? node->left : node->right;
            if (node->parent == nullptr) {
                this->pFirst = child;
                child->parent = nullptr;
            } else {
                if (node->parent->left == node) {
                    node->parent->left = child;
                } else {
                    node->parent->right = child;
                }
                child->parent = node->parent;
            }
            child->color = 'b';
            delete node;
            this->sz--;
            return;
        }

        if (node->color == 'b' && node->left == nullptr && node->right == nullptr) {
            if (node == this->pFirst) {
                delete node;
                this->pFirst = nullptr;
                this->sz = 0;
                return;
            }

            RBNodePtr parent = node->parent;
            bool isLeft = (parent->left == node);

            fixDoubleBlack(node);

            if (isLeft) {
                parent->left = nullptr;
            } else {
                parent->right = nullptr;
            }
            delete node;
            this->sz--;
            return;
        }
    }

    void fixDoubleBlack(RBNodePtr node) {
        if (node == nullptr || node == this->pFirst) return;
        if (node->parent == nullptr) return;

        RBNodePtr parent = node->parent;
        RBNodePtr brother;
        if (parent->left == node) {
            brother = parent->right;
        } else {
            brother = parent->left;
        }
        if (brother == nullptr) return;

        if (brother->color == 'r') {
            parent->color = 'r';
            brother->color = 'b';
            RBNodePtr grandpa = parent->parent;
            if (brother == parent->right) {
                if (grandpa) {
                    if (grandpa->left == parent) {
                        grandpa->left = brother;
                    } else {
                        grandpa->right = brother;
                    }
                } else {
                    this->pFirst = brother;
                }
                brother->parent = grandpa;
                parent->parent = brother;
                RBNodePtr brotherLeft = brother->left;
                brother->left = parent;
                parent->right = brotherLeft;
                if (brotherLeft) brotherLeft->parent = parent;
            } else {
                if (grandpa) {
                    if (grandpa->left == parent) {
                        grandpa->left = brother;
                    } else {
                        grandpa->right = brother;
                    }
                } else {
                    this->pFirst = brother;
                }
                brother->parent = grandpa;
                parent->parent = brother;
                RBNodePtr brotherRight = brother->right;
                brother->right = parent;
                parent->left = brotherRight;
                if (brotherRight) {
                    brotherRight->parent = parent;
                }
            }
            fixDoubleBlack(node);
            return;
        }

        bool leftNephewBlack = (brother->left == nullptr || brother->left->color == 'b');
        bool rightNephewBlack = (brother->right == nullptr || brother->right->color == 'b');

        if (leftNephewBlack && rightNephewBlack) {
            brother->color = 'r';
            if (parent->color == 'r') {
                parent->color = 'b';
            } else {
                fixDoubleBlack(parent);
            }
            return;
        }

        if (brother == parent->right) {
            if (brother->left && brother->left->color == 'r' &&
                (brother->right == nullptr || brother->right->color == 'b')) {
                RBNodePtr nephew = brother->left;
                brother->left = nephew->right;
                if (nephew->right) nephew->right->parent = brother;
                nephew->parent = brother->parent;
                if (brother->parent) {
                    if (brother->parent->right == brother) {
                        brother->parent->right = nephew;
                    } else {
                        brother->parent->left = nephew;
                    }
                }
                nephew->right = brother;
                brother->parent = nephew;
                brother->color = 'r';
                nephew->color = 'b';
                brother = nephew;
            }
            if (brother->right && brother->right->color == 'r') {
                brother->color = parent->color;
                parent->color = 'b';
                brother->right->color = 'b';
                RBNodePtr grandpa = parent->parent;
                if (grandpa) {
                    if (grandpa->left == parent) {
                        grandpa->left = brother;
                    } else {
                        grandpa->right = brother;
                    }
                } else {
                    this->pFirst = brother;
                }
                brother->parent = grandpa;
                parent->parent = brother;
                RBNodePtr brotherLeft = brother->left;
                brother->left = parent;
                parent->right = brotherLeft;
                if (brotherLeft) {
                    brotherLeft->parent = parent;
                }
            }
        } else {
            if (brother->right && brother->right->color == 'r' &&
                (brother->left == nullptr || brother->left->color == 'b')) {
                RBNodePtr nephew = brother->right;
                brother->right = nephew->left;
                if (nephew->left) nephew->left->parent = brother;
                nephew->parent = brother->parent;
                if (brother->parent) {
                    if (brother->parent->left == brother) {
                        brother->parent->left = nephew;
                    } else {
                        brother->parent->right = nephew;
                    }
                }
                nephew->left = brother;
                brother->parent = nephew;
                brother->color = 'r';
                nephew->color = 'b';
                brother = nephew;
            }
            if (brother->left && brother->left->color == 'r') {
                brother->color = parent->color;
                parent->color = 'b';
                brother->left->color = 'b';
                RBNodePtr grandpa = parent->parent;
                if (grandpa) {
                    if (grandpa->left == parent) {
                        grandpa->left = brother;
                    } else {
                        grandpa->right = brother;
                    }
                } else {
                    this->pFirst = brother;
                }
                brother->parent = grandpa;
                parent->parent = brother;
                RBNodePtr brotherRight = brother->right;
                brother->right = parent;
                parent->left = brotherRight;
                if (brotherRight) {
                    brotherRight->parent = parent;
                }
            }
        }
    }
};