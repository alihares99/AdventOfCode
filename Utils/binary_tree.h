#pragma once

#include <stack>
#include <utility>
#include <functional>

template <class T>
struct BinaryTree {
    struct Node {
        T item;
        Node* l = nullptr;
        Node* r = nullptr;

        template<class FT>
        Node(FT&& item_) : item(std::forward<FT>(item_)) {}

        template<class FT>
        Node* insert_left(FT&& t) {
            if (l == nullptr)
                l = new Node(std::forward<FT>(t));
            return l;
        }

        template<class FT>
        Node* insert_right(FT&& t) {
            if (r == nullptr)
                r = new Node(std::forward<FT>(t));
            return r;
        }
    };

    Node* head = nullptr;

    BinaryTree() {}
    BinaryTree(const BinaryTree& other) = delete;
    BinaryTree& operator=(const BinaryTree& other) = delete;
    ~BinaryTree() {
        std::stack<Node*> to_delete;
        to_delete.push(head);
        while (!to_delete.empty()) {
            auto p = to_delete.top();
            to_delete.pop();
            while (p != nullptr) {
                if (p->l != nullptr) 
                    to_delete.push(p->l);
                auto next = p->r;
                delete p;
                p = next;
            }
        }
    }

    template<class FT>
    Node* insert_head(FT&& t) {
        if (!head) 
            head = new Node(std::forward<FT>(t));
        return head;
    }

    struct iterator_base {
        iterator_base() {}
        iterator_base(const iterator_base& other) = delete;
        iterator_base(iterator_base&& other) : path(move(other.path)) {}
        iterator_base& operator=(const iterator_base& other) = delete;
        iterator_base& operator=(iterator_base&& other) { path = move(other.path); }
        
        std::stack<Node*> path;

        bool is_valid() {
            return !path.empty();
        }

        T& operator*() {
            return path.top()->item;
        }

        Node* left() {
            return path.top()->l;
        }

        Node* right() {
            return path.top()->r;
        }
    };

    struct iterator_post_order : iterator_base {
        using iterator_base::iterator_base;
        using iterator_base::operator=;
        using iterator_base::path;

        void next() {
            Node* cur = path.top();
            path.pop();
            if (path.empty())
                return;
            Node* father = path.top();
            if (cur == father->r) {
                Node* p = father->l;
                while (p) {
                    path.push(p);
                    if (p->r)
                        p = p->r;
                    else
                        p = p->l;
                }
            }
        }
    };

    iterator_post_order begin_post_order() {
        iterator_post_order it;
        Node* p = head;
        while (p) {
            it.path.push(p);
            if (p->r)
                p = p->r;
            else
                p = p->l;
        }
        return std::move(it);
    }

    struct iterator_in_order : iterator_base {
        using iterator_base::iterator_base;
        using iterator_base::operator=;
        using iterator_base::path;

        void next() {
            Node* cur = path.top();
            if (cur->l) {
                auto p = cur->l;
                while (p) {
                    path.push(p);
                    p = p->r;
                }
            }
            else {
                path.pop();
                while (!path.empty() && cur == path.top()->l) {
                    cur = path.top();
                    path.pop();
                }
            }
        }
    };

    iterator_in_order begin_in_order() {
        iterator_in_order it;
        Node* p = head;
        while (p) {
            it.path.push(p);
            p = p->r;
        }
        return std::move(it);
    }

    struct iterator_pre_order : iterator_base {
        using iterator_base::iterator_base;
        using iterator_base::operator=;
        using iterator_base::path;

        void next() {
            Node* cur = path.top();
            if (cur->r) {
                path.push(cur->r);
            }
            else if (cur->l) {
                path.push(cur->l);
            }
            else {
                path.pop();
                while (!path.empty() && (cur == path.top()->l || path.top()->l == nullptr)) {
                    cur = path.top();
                    path.pop();
                }
                if (!path.empty()) {
                    path.push(path.top()->l);
                }
            }
        }
    };

    iterator_pre_order begin_pre_order() {
        iterator_pre_order it;
        it.path.push(head);
        return it;
    }

    struct Printer {
        BinaryTree<T>::Node* head;
        std::function<std::string(T)> to_string;

        Printer(BinaryTree<T>::Node* head, std::function<std::string(T)> to_string) :
            head(head), to_string(to_string) {}

        void print_internal(const std::string& prefix, const Node* node, bool is_right) {
            std::cout << prefix;
            std::cout << (is_right ? "|-" : "'-" );
            std::cout << (node ? to_string(node->item) : "---") << std::endl;
            if (node) {
                print_internal( prefix + (is_right ? "|  " : "   "), node->r, true);
                print_internal( prefix + (is_right ? "|  " : "   "), node->l, false);
            }
        }

        void print() {
            print_internal("", head, false);
        }
    };

    Printer get_printer(std::function<std::string(T)> to_string) {
        return Printer{head, to_string};
    }
};
