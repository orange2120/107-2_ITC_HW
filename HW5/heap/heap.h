#ifndef _HEAP_H_
#define _HEAP_H_

#include <iostream>

using namespace std;

#define HEAPSIZE 500000

template <class K, class E>
class Node {
    public:
        K key;
        E element;
};

template <class K, class E>
class AbsHeap {

    public:
        AbsHeap() {
            ary = new Node<K,E> [HEAPSIZE]; // ary is array of Node<K,E> type
            _size = 0;
        }

        ~AbsHeap() {
            delete[] ary;
        }

        // Remember to ++size
        virtual void push(K _key, E _element) = 0;

        // return the element with the minimum key, remember to --size
        virtual E pop() = 0;

        void printArray() const {
            cout << "(Index, Key, Element)" << endl;
            for (int i = 0; i < _size; i++)
                cout << "(" << i << "," << ary[i].key << "," << ary[i].element << ")" << endl;
        }

        void printByPopping() {
            while (!isEmpty())
                cout << pop();
            cout << endl;
        }

        K getTopKey() const {
            return ary[0].key;
        }

        bool isEmpty() const {
            return (_size == 0);
        }

        int size() const {
            return _size;
        }

        // This function is for your convenience
        void swap(int i, int j) {
            Node<K,E> temp = ary[i];
            ary[i] = ary[j];
            ary[j] = temp;
        }

    protected:
        Node<K,E> *ary;
        int _size;

};


template <class K, class E>
class Heap: public AbsHeap<K,E> {

    // You need these three lines for template inheritance.
    // Otherwise, you'll have to use this->ary to access ary
    using AbsHeap<K,E>::ary;
    using AbsHeap<K,E>::_size;
    using AbsHeap<K,E>::swap;

    /**************************
     * Complete your code here.
     * You need to implement allprintArray
     * pure virtual functions (push & pop).
     * ************************/
  public:
    void push(K _key, E _element)
    {
        int t = _size + 1;
        ary[_size].key = _key;
        ary[_size].element = _element;
        _size++;
        while (t > 1)
        {
            int p = t / 2; // get parent
            if(!(ary[t - 1].key < ary[p - 1].key))
                break;
            swap(t - 1, p - 1); // swap with its parents
            t = p;
        }
    }

    E pop()
    {
        E top = ary[0].element;
        int root = 0, l = 0, r = 0; // root , child index
        int smallest = 0;
        
        _size--; // pop back

        // heapfying : bubble down
        swap(0, _size);
        while (root < _size)
        {
            l = 2 * root + 1;
            r = l + 1;

            // if left child is smaller
            if (l < _size && (ary[l].key < ary[smallest].key))
                smallest = l;

            // if right child is smaller
            if (r < _size && (ary[r].key < ary[smallest].key))
                smallest = r;

            //cout << "r[" << r - 1 << "]->" << ary[r - 1].key << " L[" << ch - 1 << "]->" << ary[ch - 1].key << " R[" << ch << "]->" << ary[ch].key << endl;
            //cout << "S[" << smallest - 1 << "]->" << ary[smallest - 1].key << endl;

            if (smallest == root) // no need to exchange
                break;

            swap(root, smallest);
            root = smallest;
        }
        return top;
    }
};

#endif
