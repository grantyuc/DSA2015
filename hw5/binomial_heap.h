#ifndef __BINOMIAL_HEAP_H__
#define __BINOMIAL_HEAP_H__

#include <iostream>
#include <utility>
#include <list>

struct EmptyHeap: public std::exception {};
struct HeapOverflow: public std::exception {};

class Task{
    private:
        int id;
        double p;       //priority

    public:
        Task(const int& i, const double& d) : id(i), p(d){}

        int getId() const {return id;}
        double getP() const {return p;}

        bool operator==(const Task& task){return p == task.p;}
        bool operator>(const Task& task){
            if(p == task.p) 
                return id < task.id;
            else return p > task.p;
        }
        friend std::ostream& operator<<(std::ostream& out, const Task& task){
            std::cout << "Task " << task.id << " with priority " << task.p;
            return out;
        }
};

template<class T>
class BinomialHeap {
    private:
        /* inner class: binomial tree */
        struct BinomialTree {
            int _size;
            T element;
            std::list<BinomialTree*> children;

            BinomialTree(T _ele): _size(1), element(_ele) {children.clear();}
            ~BinomialTree() {children.clear();}

            /* return the size of BinomialTree
             * #note that nullptr->size() == 0
             * (nullptr is a null pointer (like NULL in C))
             */
            int size() {return (this!=nullptr)?this->_size:0;}

            void printBT(const int& = 0);
            friend std::ostream& operator<<(std::ostream& out, BinomialTree& t){
                t.printBT();
                return out;
            }
        };
        /* some definition for convinience */
        typedef BinomialTree BT;
        typedef BinomialHeap<T> BH;
        typedef std::pair<BT*, BT*> CarrySum;
        typedef std::pair<T, BH> MaxRemainder;

        /* Merge three binomial trees which have the same sizes
         *
         * INPUT:   a: operand a, b: operand b, c: carry in
         *          (each of them are either nullptr or a pointer of BinomialTree with size 2^n)
         *
         * OUTPUT:  a pair consist of two variables.
         *          first: carry out (nullptr or a pointer of BinomialTree with size 2^(n+1))
         *          second: sum (nullptr or a pointer of BinomialTree with size 2^n)
         *
         * (just as a full adder: http://en.wikipedia.org/wiki/Adder_%28electronics%29#Full_adder )
         */
        CarrySum merge_tree(BT *, BT *, BT *);

        /* Pop the maximum element of a binomial tree and make other elements a binomial heap.
         *
         * INPUT:   a: a pointer of BinomialTree
         *
         * OUTPUT:  a pair consist of two variables.
         *          first: the maximum element
         *          second: a binomial heap consists of other elements
         */
        MaxRemainder pop_max(BT *);

        int size;
        BT* trees[32]; //binomial trees of the binomial heap, where trees[i] is a tree with size 2^i.

    public:
        BinomialHeap(): size(0) {for(int i=0; i<32; ++i) trees[i] = nullptr;}
        BinomialHeap(T element): size(1) {
            for(int i=0; i<32; ++i) trees[i] = nullptr;
            trees[0] = new BT(element);
        }
        //BinomialHeap<T>& operator=(BinomialHeap<T>&& h){
        //    if(this != &h){
        //        size = h.size;
        //        for(int i=0; i<32; ++i){
        //            trees[i] = h.trees[i];
        //            h.trees[i] = nullptr;
        //        }
        //    }
        //    return *this;
        //}

        int getSize() const {return size;}

        /* merge all elements in the binomial heap b into *this, and clear the binomial heap b.
         *
         * INPUT:   b: a reference of BinomialHeap
         */
        void merge(BH &b);

        void insert(const T &element) {
            BH tmp = BH(element);
            merge(tmp);
        }
        T pop();
        T top();

        friend std::ostream& operator<<(std::ostream& out, BinomialHeap& h){
            for(int i = 0; i<32; ++i){
                if(h.trees[i]->size() != 0)
                    std::cout << "trees[" << i << "]: \n" << *h.trees[i] << std::endl;
            }
            std::cout << "There are " << h.size << " tasks on this computer.";
            return out;
        }
};

void printTab(const int& n){
    for(int i = 0; i<n; ++i){
        std::cout << "    ";
    }
}

template<class T>
void BinomialHeap<T>::BinomialTree::printBT(const int& n){
    printTab(n);
    std::cout << element << std::endl;
    for(typename std::list<BinomialTree*>::iterator it = children.begin(); it != children.end(); ++it){
        if((*it)->size() != 0)
            (*it)->printBT(n+1);
    }
}

template<class T>
typename BinomialHeap<T>::CarrySum BinomialHeap<T>::merge_tree(BT *a, BT *b, BT *c) {
    // write your code here.
    if(c == nullptr){
        if(b == nullptr){
            return std::make_pair(nullptr, a);
        }
        if(a == nullptr){
            return std::make_pair(nullptr, b);
        }
        BT *lar;
        BT *sma;
        if(a->element > b->element){
            lar = a;
            sma = b;
        }
        else{
            lar = b;
            sma = a;
        }
        lar->children.push_back(sma);
        lar->_size = lar->size() << 1;
        return std::make_pair(lar, nullptr);
    }
    else{
        if(b == nullptr){
            return merge_tree(a, c, b);
        }
        if(a == nullptr){
            return merge_tree(c, b, a);
        }
        CarrySum tmp = merge_tree(a, b, nullptr);
        CarrySum tmp2 = merge_tree(tmp.second, c, nullptr);
        return std::make_pair(tmp.first, tmp2.second);
    }
};

/* Pop the maximum element of a binomial tree and make other elements a binomial heap.
 *
 * INPUT:   a: a pointer of BinomialTree
 *
 * OUTPUT:  a pair consist of two variables.
 *          first: the maximum element
 *          second: a binomial heap consists of other elements
 */
template<class T>
typename BinomialHeap<T>::MaxRemainder BinomialHeap<T>::pop_max(BT* const a) {
    // write your code here.
    const T& max = a->element;
    BH rmd;
    for(typename std::list<BT*>::iterator it = a->children.begin(); it != a->children.end(); ++it){
        unsigned int i = (*it)->size();
        int log_i = 0;
        while(i > 1){
            i >>= 1;
            ++log_i;
        }
        rmd.trees[log_i] = *it;
        rmd.size += (*it)->_size;
    }
    return std::make_pair(max, rmd);
}

/* merge all elements in the binomial heap b into *this, and clear the binomial heap b.
 *
 * INPUT:   b: a reference of BinomialHeap
 */
template<class T> 
void BinomialHeap<T>::merge(BH &b) {
    // write your code here.
    size += b.size;
    b.size = 0;
    BT *carrier = nullptr;
    for(int i = 0; i<32; ++i){
        CarrySum sum = merge_tree(trees[i], b.trees[i], carrier);
        carrier = sum.first;
        trees[i] = sum.second;
        b.trees[i] = nullptr;
    }
    if(carrier != nullptr) throw HeapOverflow();
}

template<class T>
T BinomialHeap<T>::pop() {
    if(size==0) throw EmptyHeap();
    else {
        //find the tree contains maximum element
        int max_tree = -1;
        for(int i=0; i<32; ++i)
            if(trees[i]->size() > 0 && (max_tree == -1 || trees[i]->element > trees[max_tree]->element))
                max_tree = i;

        MaxRemainder m_r = pop_max(trees[max_tree]);
        T &max_element = m_r.first;
        BH &remainder = m_r.second;

        size -= trees[max_tree]->_size;
        delete trees[max_tree];
        trees[max_tree] = nullptr;
        merge(remainder);
        return max_element;
    }
}

template<class T>
T BinomialHeap<T>::top() {
    if(size==0) throw EmptyHeap();
    else {
        //find the tree contains maximum element
        int max_tree = -1;
        for(int i=0; i<32; ++i)
            if(trees[i]->size() > 0 && (max_tree == -1 || trees[i]->element > trees[max_tree]->element))
                max_tree = i;

        MaxRemainder m_r = pop_max(trees[max_tree]);
        T &max_element = m_r.first;

        return max_element;
    }
}
#endif
