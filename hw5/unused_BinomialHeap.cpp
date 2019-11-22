#include "BinomialHeap.h"

/* some definition for convinience */
//template<class T>
//using BT = typename BinomialHeap<T>::BinomialTree;
//template<class T>
//using BH = BinomialHeap<T>;
//template<class T>
//using CarrySum = std::pair<typename BinomialHeap<T>::BinomialTree*, typename BinomialHeap<T>::BinomialTree*>;
//template<class T>
//using MaxRemainder = std::pair<T, BH<T>>;

void printTab(const int& n){
    for(int i = 0; i<n; ++i){
        std::cout << "\t";
    }
}

template<class T>
void BinomialHeap<T>::BinomialTree::printBT(const int& n){
    printTab(n);
    std::cout << element << std::endl;
    for(typename std::list<BinomialTree*>::iterator it = children.begin(); it != children.end(); ++it){
        if((*it)->size() != 0)
            (*it)->children->printBT(n+1);
    }
}

template<class T>
typename BinomialHeap<T>::CarrySum BinomialHeap<T>::merge_tree(BT const*a, BT const*b, BT const*c) {
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
        if(a->element >= b->element){
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
typename BinomialHeap<T>::MaxRemainder BinomialHeap<T>::pop_max(BT const *a) {
    // write your code here.
    T max = a->element;
    BH bh;
    for(typename std::list<BT*>::iterator it = a->children.begin(); it != a->children.end(); ++it){
        unsigned int i = (*it)->size();
        int log_i = 0;
        while(i > 1){
            i >>= 1;
            ++log_i;
        }
        bh.trees[log_i] = *it;
    }
    return std::make_pair(max, bh);
}

/* merge all elements in the binomial heap b into *this, and clear the binomial heap b.
 *
 * INPUT:   b: a reference of BinomialHeap
 */
template<class T> 
void BinomialHeap<T>::merge(BH &b) {
    // write your code here.
    size += b.size;
    BT *carrier = nullptr;
    for(int i = 0; i<32; ++i){
        CarrySum sum = merge_tree(trees[i], b.trees[i], carrier);
        carrier = sum.first;
        trees[i] = sum.second;
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

        size -= trees[max_tree]._size;
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
