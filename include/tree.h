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
        while (current->left != nullptr) {
            current = current->left;
        }

        return Iterator(current);
    }
    Iterator end() {
        BaseNode *current = pFirst;
        while (current->right != nullptr) {
            current = current->right;
        }

        return Iterator(current);
    }

    BaseTree() : pFirst(nullptr) {}

    BaseTree(T key, H value) : pFirst(new BaseNode(Pair{key, value}, nullptr, nullptr, nullptr)) {}

    BaseTree(const BaseTree<T, H, BaseNode> &root) {
        pFirst = copyNode(root.pFirst);
        sz = root.sz;
    }

    BaseTree(vector<Pair> elements) {
        logger("Base const vector<Pair>", 1);
        sort(elements.begin(), elements.end());
        int plug = 0;
        pFirst = nullptr;
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
        BaseNode *new_node = new Node<T, H>{node->data, parent, nullptr, nullptr};
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
        if (node->left == nullptr && node->right == nullptr) {  // удаление листа
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

        } else if (((node->left == nullptr) + (node->right == nullptr)) == 1) {  // один потомок
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
        } else {  // 2 потомка
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

    int count() { return sz; };

    vector<T> keys() {
        vector<T> data;
        RecTree(GetFirst(), data);
        sort(data.begin(), data.end());
        return data;
    };

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
    void Insert(T key, H value) {
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

    void InsertNode(Node<T, H> *current) {
        if (this->pFirst == nullptr) {
            this->pFirst = current;
            this->sz++;
        } else {
            Node<T, H> *tmp = this->pFirst;
            while (true) {
                if (tmp->data.key > current->data.key) {
                    if (tmp->left != nullptr) {
                        tmp = tmp->left;
                    } else {
                        tmp->left = current;
                        this->sz++;
                        break;
                    }
                } else if (tmp->data.key < current->data.key) {
                    if (tmp->right != nullptr) {
                        tmp = tmp->right;
                    } else {
                        tmp->right = current;
                        this->sz++;
                        break;
                    }
                } else {
                    throw invalid_argument("Incorrect insert");
                }
            }
        }
    }

    virtual void Delete(T key) {
        Node<T, H> *node = this->FindNode(key);
        if (!node) {
            throw runtime_error("Нет ключа");
        }
        if (node->left == nullptr && node->right == nullptr) {  // удаление листа
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

        } else if (((node->left == nullptr) + (node->right == nullptr)) == 1) {  // один потомок
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
        } else {  // 2 потомка
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

// template <typename T, typename H>
// class AVLTree : protected Tree<T, H> {
//    protected:
//     using typename Tree<T, H>::Pair;
//     using typename Tree<T, H>::Node;

//     struct AVLNode : public Node {
//         int advantage;

//         AVLNode(const Pair &d, Node *p = nullptr) : Node(d, nullptr, nullptr, p), advantage(0) {}
//     };

//     AVLNode *LL(AVLNode *node) {};
//     AVLNode *RR(AVLNode *node) {};
//     AVLNode *RL(AVLNode *node) {};
//     AVLNode *LR(AVLNode *node) {};

//     AVLNode *_Insert(AVLNode *root, const Pair &kv) {};

//     AVLNode *root;

//    public:
//     AVLTree() : root(nullptr) {}

//     void Delete(T key) {}
//     void Insert(T key, H value) {}

//     int Balance(AVLNode *node) { return 1; }

//     ~AVLTree() {}
// };

template <typename T, typename H>
class RedBlackTree : public BaseTree<T, H, RBNode<T, H>> {
   protected:
    using typename Map<T, H>::Pair;
    using RBNodePtr = RBNode<T, H> *;

   public:
    using BaseTree<T, H, RBNode<T, H>>::BaseTree;
    RedBlackTree(vector<T> elements) {
        logger("Base const vector<T>", 1);
        sort(elements.begin(), elements.end());
        int plug = 0;
        this->pFirst = nullptr;
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
        logger("Insert RBT", 1);
        BaseTree<T, H, RBNode<T, H>>::Insert(key, value);
        RBNodePtr needNode = this->FindNode(key);
        if (needNode != nullptr) {
            logger("Correct insert", 1);
            this->printTree(this->GetFirst());
            this->Balance(needNode);
            logger("Correct Balance", 1);
            this->printTree(this->GetFirst());
        } else {
            throw invalid_argument("Inccorrect insert");
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
                    logger("uncle right", 1);
                    uncle = childNode->parent->parent->right;
                } else {
                    logger("uncle left", 1);
                    uncle = childNode->parent->parent->left;
                }
                if (uncle != nullptr) {
                    logger(to_string(childNode->data.key) + " uncle:" + to_string(uncle->data.key), 1);
                }

                if (uncle == nullptr || uncle->color == 'b') {
                    if (parentNode == parentNode->parent->left && childNode == parentNode->left) {  // LL
                        logger("LL", 1);
                        this->LL(childNode);
                    } else if (parentNode == parentNode->parent->left &&
                               childNode == parentNode->right) {  // LR
                        logger("LR", 1);
                        this->LR(childNode);
                    } else if (parentNode == parentNode->parent->right &&
                               childNode == parentNode->right) {  // RR
                        logger("RR", 1);
                        this->RR(childNode);
                    } else {
                        logger("RL", 1);  // RL
                        this->RL(childNode);
                    }

                } else {
                    logger("No route", 1);
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
        RBNodePtr childRihgt = parentNode->right;
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
        grandpa->left = childRihgt;
        grandpa->color = 'r';
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
        this->printTree(this->GetFirst());
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
        logger("Delete RBT " + to_string(key), 1);
        RBNodePtr node = this->FindNode(key);
        if (!node) {
            throw runtime_error("Нет ключа");
        }
        if (node->left != nullptr && node->right != nullptr) {
            logger("Node two children", 1);
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
            logger("Red Node 0 child", 1);
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
            logger("Black one red child", 1);
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
            logger("Black Node", 1);
            if (node == this->pFirst) {
                delete node;
                this->pFirst = nullptr;
                this->sz = 0;
                return;
            }
            fixDoubleBlack(node);
            if (node->parent != nullptr) {
                if (node->parent->left == node) {
                    node->parent->left = nullptr;
                } else {
                    node->parent->right = nullptr;
                }
            }

            delete node;
            this->sz--;
            return;
        }
    }
    void fixDoubleBlack(RBNodePtr node) {
        if (node == nullptr || node == this->pFirst) return;
        RBNodePtr parent = node->parent;
        if (parent == nullptr) return;
        RBNodePtr brouther;
        if (parent->left == node) {
            brouther = parent->right;
        } else {
            brouther = parent->left;
        }
        if (brouther == nullptr) return;

        // 2A: Красный брат
        if (brouther->color == 'r') {
            logger("2A Red brouther", 1);
            parent->color = 'r';
            brouther->color = 'b';
            RBNodePtr grandpa = parent->parent;
            if (brouther == parent->right) {
                // Левый поворот
                if (grandpa) {
                    if (grandpa->left == parent) {
                        grandpa->left = brouther;
                    } else {
                        grandpa->right = brouther;
                    }
                } else {
                    this->pFirst = brouther;
                }
                brouther->parent = grandpa;
                parent->parent = brouther;
                RBNodePtr broutherLeft = brouther->left;
                brouther->left = parent;
                parent->right = broutherLeft;
                if (broutherLeft) broutherLeft->parent = parent;
            } else {
                // Правый поворот
                if (grandpa) {
                    if (grandpa->left == parent) {
                        grandpa->left = brouther;
                    } else {
                        grandpa->right = brouther;
                    }
                } else {
                    this->pFirst = brouther;
                }
                brouther->parent = grandpa;
                parent->parent = brouther;
                RBNodePtr broutherRight = brouther->right;
                brouther->right = parent;
                parent->left = broutherRight;
                if (broutherRight){
                    broutherRight->parent = parent;
                }
            }
            fixDoubleBlack(node);
            return;
        }

        // 2B: Черный брат, оба племянника черные
        bool leftNephewBlack = (brouther->left == nullptr || brouther->left->color == 'b');
        bool rightNephewBlack = (brouther->right == nullptr || brouther->right->color == 'b');

        if (leftNephewBlack && rightNephewBlack) {
            logger("2B: black brouther black nephews", 1);
            brouther->color = 'r';

            if (parent->color == 'r') {
                parent->color = 'b';
            } else {
                fixDoubleBlack(parent);
            }
            return;
        }

        // 2C/2D: Черный брат с красным племянником
        if (brouther == parent->right) {
            // 2D: Ближний племянник красный (левый ребенок брата)
            if (brouther->left && brouther->left->color == 'r' &&
                (brouther->right == nullptr || brouther->right->color == 'b')) {
                logger("2D: red nephew", 1);
                RBNodePtr nephew = brouther->left;
                // Правый поворот
                brouther->left = nephew->right;
                if (nephew->right) nephew->right->parent = brouther;
                nephew->parent = brouther->parent;
                if (brouther->parent) {
                    if (brouther->parent->right == brouther) {
                        brouther->parent->right = nephew;
                    } else {
                        brouther->parent->left = nephew;
                    }
                }
                nephew->right = brouther;
                brouther->parent = nephew;
                brouther->color = 'r';
                nephew->color = 'b';
                brouther = nephew;
            }

            // 2C: Дальний племянник красный (правый ребенок брата)
            if (brouther->right && brouther->right->color == 'r') {
                logger("2C: Far red nephew", 1);
                // Левый поворот
                brouther->color = parent->color;
                parent->color = 'b';
                brouther->right->color = 'b';
                RBNodePtr grandpa = parent->parent;
                if (grandpa) {
                    if (grandpa->left == parent) {
                        grandpa->left = brouther;
                    } else {
                        grandpa->right = brouther;
                    }
                } else {
                    this->pFirst = brouther;
                }
                brouther->parent = grandpa;
                parent->parent = brouther;
                RBNodePtr broutherLeft = brouther->left;
                brouther->left = parent;
                parent->right = broutherLeft;
                if (broutherLeft) {
                    broutherLeft->parent = parent;
                }
            }
        } else {
            // 2D: Ближний племянник красный (правый ребенок брата)
            if (brouther->right && brouther->right->color == 'r' &&
                (brouther->left == nullptr || brouther->left->color == 'b')) {
                logger("2D: Close red nephew", 1);

                RBNodePtr nephew = brouther->right;

                // Левый поворот
                brouther->right = nephew->left;
                if (nephew->left) nephew->left->parent = brouther;
                nephew->parent = brouther->parent;
                if (brouther->parent) {
                    if (brouther->parent->left == brouther) {
                        brouther->parent->left = nephew;
                    } else {
                        brouther->parent->right = nephew;
                    }
                }

                nephew->left = brouther;
                brouther->parent = nephew;
                brouther->color = 'r';
                nephew->color = 'b';
                brouther = nephew;
            }

            // 2C: Дальний племянник красный (левый ребенок брата)
            if (brouther->left && brouther->left->color == 'r') {
                logger("2C: Far red nephew", 1);
                // Правый поворот
                brouther->color = parent->color;
                parent->color = 'b';
                brouther->left->color = 'b';
                RBNodePtr grandpa = parent->parent;
                if (grandpa) {
                    if (grandpa->left == parent) {
                        grandpa->left = brouther;
                    } else {
                        grandpa->right = brouther;
                    }
                } else {
                    this->pFirst = brouther;
                }
                brouther->parent = grandpa;
                parent->parent = brouther;
                RBNodePtr broutherRight = brouther->right;
                brouther->right = parent;
                parent->left = broutherRight;
                if (broutherRight){
                    broutherRight->parent = parent;
                }
            }
        }
    }
};
