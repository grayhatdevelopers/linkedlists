//
// Created by Hamza Maqsood on 9/22/2019.
//

#ifndef SINGLYLINKEDLIST_SINGLYLINKEDLIST_H
#define SINGLYLINKEDLIST_SINGLYLINKEDLIST_H

#include <iostream>

template<typename T>
class Node {
public:
    T data;
    Node<T> *next;

    explicit Node(T data) : data(data), next(nullptr) {}

    Node() = delete;

    ~Node() = default;
};

template<typename T>
class SinglyLinkedList {
private:
    int size;
    Node<T> *head;
    Node<T> *tail;
public:
    SinglyLinkedList() : size(0), head(nullptr), tail(nullptr) {}
    ~SinglyLinkedList(){
        clear();
    }

    /*
     *Insert at the beginning; this coming element will become the head
     */
    void insertAtBeginning(T data) {
        auto node = new Node<T>(data);
        if (head == nullptr) {//list is empty, this is the first element
            head = tail = node;
            head->next = tail->next = nullptr;
        } else {
            Node<T> *temp = head;
            head = node;
            head->next = temp;
        }
        size++;
    }

    /*
     *Insert at the end; this coming element will become the tail
     */
    void insertAtEnd(T data) {
        if (head == nullptr)//this is the first element to be inserted
            insertAtBeginning(data);
        else {
            auto node = new Node<T>(data);
            tail->next = node;
            tail = node;
            size++;
        }
    }

    /*
     * inserts an element at the specified position
     * throws an exception if the specified position is out of bounds
     */
    void insertAtPosition(T data, unsigned position) {
        if (position > size) throw std::out_of_range("Specified index is out of bounds");
        else if (!position) insertAtBeginning(data);
        else if (position == size) insertAtEnd(data);
        else {
            auto *node = new Node<T>(data);
            Node<T> *temp = getAtPosition(position - 1);//get a pointer to the previous element
            node->next = temp->next;
            temp->next = node;
            size++;
        }
    }

    /*
     *Returns true if the linked is empty, false otherwise
     */
    bool isEmpty() {
        return !size;
    }

    /*
     * Returns the size of the list
     */
    unsigned getSize() {
        return size;
    }

    /*
     *Inserts an element after the given element
     *Inserts at the end if no such element is found
     */
    void insertAfter(T element, T data) {
        Node<T> *temp = getElement(element);
        auto *node = new Node<T>(data);
        node->next = temp->next;
        temp->next = node;
        size++;
    }

    /*
     *Removes an element after the given element
     *throws an exception if element not found
     */
    void eraseAfter(T element) {
        Node<T> *temp = getElement(element);
        auto toDelete = temp->next;
        temp->next = temp->next->next;
        delete toDelete;
        size--;
    }

    /*
     * Returns a pointer to the element at the specified position
     * throws an exception if specified position is out of bounds
     */
    Node<T> *getAtPosition(unsigned position) {
        if (position > size) throw std::out_of_range("Specified index is out of bounds");
        else {
            Node<T> *n;
            int i;
            for (i = 0, n = head; i < position; ++i, n = n->next);
            return n;
        }
    }
    
    /*
     * return the address node of the specified data if exists
     *  throws an exception otherwise
     */
    Node<T> *getElement(T data) {
        if (!size) throw std::out_of_range("List is empty!");
        for (Node<T> *n = head; n != tail; n = n->next)
            if (n->data == data) return n;
        throw std::out_of_range("Specified element is not in the list");
    }

    /*
     *Returns the first element from the linked list and removes it
     */
    T popFront() {
        T data = head->data;
        Node<T> *temp = head;
        head = head->next;
        size--;
        delete temp;
        return data;
    }

    /*
     *Returns the last element from the linked list and then removes it
     */
    T popEnd() {
        T data = tail->data;
        Node<T> *temp = tail;
        tail = getAtPosition(--size - 1);
        tail->next = nullptr;
        delete temp;
        return data;
    }

    /*
     *Merges the incoming linked list with it's own
     * Merging starts from the end
     */
    SinglyLinkedList merge(SinglyLinkedList &other) {
        tail->next = other.head;
        tail = other.tail;
        size += other.size;
        return *this;
    }

    /*
     * Utility function for reversing elements
     */
private:
    void reverseUtil(Node<T> *e) {
        if (e == tail)
            return;
        reverseUtil(e->next);
        head->next = e;
        head = head->next;
    }

    /*
     *Reverses all the elements
     */
public:
    void reverse() {
        Node<T> *temp = head;
        head = tail;
        reverseUtil(temp);
        head = tail;
        tail = temp;
        tail->next = nullptr;
    }

    /*
     *Insert at the beginning; this coming element will become the head
     */
    void removeDuplicates() {
        if (isEmpty())
            return;
        sort();
        for (Node<T> *n = head; n != tail; n = n->next)
            if (n->data == n->next->data)
                eraseAfter(n->data);
    }

    /*
     * Returns the position of the specified element if found, -1 otherwise
     * searches the specified element in a linear fashion
     */
    int searchElement(const T &data) {
        int index = 0;
        for (Node<T> *n = head; n != nullptr; n = n->next, ++index)
            if (n->data == data)
                return index;
        return -1;
    }

    /*
     * Returns true if the specified element exists in the data set, false otherwise
     */
    bool contains(const T &data) {
        return searchElement(data) != -1;
    }

    void swapElements(Node<T> *lhs, Node<T> *rhs) {
        auto temp = lhs->data;
        lhs->data = rhs->data;
        rhs->data = temp;
    }

    /*
     *Sorts the linked list using Shell Sort algorithm with the complexity of O(n*logn) or in the worst case O(n*log^2 n)
     */
    void sort() {
        for (int gap = size / 2; gap > 0; gap /= 2) // reduce the gap value by half on each iteration
            for (int i = gap; i < size; ++i) {
                Node<T> newElement = *getAtPosition(
                        i); //we need to store this element because 'j' can change the value on this index
                int j = i;
                while (j >= gap and newElement.data < getAtPosition(j - gap)->data) {
                    swapElements(getAtPosition(j), getAtPosition(j - gap));
                    j -= gap;
                }
                swapElements(getAtPosition(j), &newElement);
            }
    }

private:
    void clearUtil(Node<T> *e) {
        if (e == tail)
            return;
        clearUtil(e->next);
        delete e;
    }

    /*
     *Clears All the elements in the linked list
     */
public:
    void clear() {
        clearUtil(head);
        head = tail = nullptr;
        size = 0;
    }

    /*
     *This function prints all the elements in the data set
     */
    void print() {
        for (Node<T> *n = head; n != nullptr; n = n->next)
            std::cout << "->" << n->data << std::endl;
    }

    /*
     *overloaded plus operator will merge two lists
     *merging starts at the end of existing list
     */
    SinglyLinkedList operator+(SinglyLinkedList const &rhs) {
        merge(rhs);
    }

    /*
     * overloaded subscript operator will return the element at specified index
     * throws an exception if the index is out of bound
     */
    T operator[](unsigned index) {
        return *(getAtPosition(index));
    }

};

#endif //SINGLYLINKEDLIST_SINGLYLINKEDLIST_H
