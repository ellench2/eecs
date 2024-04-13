#ifndef LIST_H
#define LIST_H
/* List.h
 *
 * doubly-linked, double-ended list with Iterator interface
 * Project UID c1f28c309e55405daf00c565d57ff9ad
 * EECS 280 Project 4
 */

#include <iostream>
#include <cassert> //assert
#include <cstddef> //NULL


template <typename T>

class List {
  //OVERVIEW: a doubly-linked, double-ended list with Iterator interface
public:

  //EFFECTS:  returns true if the list is empty
  bool empty() const;

  //EFFECTS: returns the number of elements in this List
  //HINT:    Traversing a list is really slow.  Instead, keep track of the size
  //         with a private member variable.  That's how std::list does it.
  int size() const;

  //REQUIRES: list is not empty
  //EFFECTS: Returns the first element in the list by reference
  T & front();

  //REQUIRES: list is not empty
  //EFFECTS: Returns the last element in the list by reference
  T & back();

  //EFFECTS:  inserts datum into the front of the list
  void push_front(const T &datum);

  //EFFECTS:  inserts datum into the back of the list
  void push_back(const T &datum);

  //REQUIRES: list is not empty
  //MODIFIES: may invalidate list iterators
  //EFFECTS:  removes the item at the front of the list
  void pop_front();

  //REQUIRES: list is not empty
  //MODIFIES: may invalidate list iterators
  //EFFECTS:  removes the item at the back of the list
  void pop_back();

  //MODIFIES: may invalidate list iterators
  //EFFECTS:  removes all items from the list
  void clear();
    
  List();
    
  List(const List &other);
    
  ~List();
    
  List & operator=(const List &other);

  // You should add in a default constructor, destructor, copy constructor,
  // and overloaded assignment operator, if appropriate. If these operations
  // will work correctly without defining these, you can omit them. A user
  // of the class must be able to create, copy, assign, and destroy Lists

private:
  //a private type
  struct Node {
    Node *next;
    Node *prev;
    T datum;
  };

  //REQUIRES: list is empty
  //EFFECTS:  copies all nodes from other to this
  void copy_all(const List<T> &other);

  Node *first;   // points to first Node in list, or nullptr if list is empty
  Node *last;    // points to last Node in list, or nullptr if list is empty
    int size_;

public:
    ////////////////////////////////////////
    class Iterator {
    //OVERVIEW: Iterator interface to List
  public:
      friend class List;
    // You should add in a default constructor, destructor, copy constructor,
    // and overloaded assignment operator, if appropriate. If these operations
    // will work correctly without defining these, you can omit them. A user
    // of the class must be able to create, copy, assign, and destroy Iterators.

    // Your iterator should implement the following public operators: *,
    // ++ (prefix), default constructor, == and !=.
      T & operator*() const {
          return node_ptr->datum;
      }
      
      bool operator==(Iterator rhs) const {
          return (node_ptr == rhs.node_ptr);
      }
      Iterator& operator++();
      bool operator!=(Iterator rhs) const {
          return (node_ptr != rhs.node_ptr);
      }

 
    // This operator will be used to test your code. Do not modify it.
    // Requires that the current element is dereferenceable.
    Iterator& operator--() {
      assert(node_ptr);
      node_ptr = node_ptr->prev;
      return *this;
    }

  private:
    Node *node_ptr; //current Iterator position is a List node
    // add any additional necessary member variables here
    friend class List;
    // add any friend declarations here

    // construct an Iterator at a specific position
      Iterator(Node *p) {
          node_ptr = p;
      }

  };//List::Iterator
  ////////////////////////////////////////

  // return an Iterator pointing to the first element
  Iterator begin() const {
    return Iterator(first);
  }

  // return an Iterator pointing to "past the end"
  Iterator end() const;

  //REQUIRES: i is a valid, dereferenceable iterator associated with this list
  //MODIFIES: may invalidate other list iterators
  //EFFECTS: Removes a single element from the list container
  void erase(Iterator i);

  //REQUIRES: i is a valid iterator associated with this list
  //EFFECTS: inserts datum before the element at the specified position.
  void insert(Iterator i, const T &datum);
  
    
};//List


////////////////////////////////////////////////////////////////////////////////
// Add your member function implementations below or in the class above
// (your choice). Do not change the public interface of List, although you
// may add the Big Three if needed.  Do add the public member functions for
// Iterator.

template<typename T>
bool List<T>::empty() const {
    return(first == nullptr);
}
template<typename T>
int List<T>::size() const {
    return size_;
}
template<typename T>
T & List<T>::front() {
    assert(!empty());
    return first->datum;
}
template<typename T>
T & List<T>::back() {
    assert(!empty());
    return last->datum;
}
template<typename T>
void List<T>::push_front(const T &datum) {
    Node *new_first = new Node{first, nullptr, datum};
    if (size_ >= 1) {
    first->prev = new_first;
    }
    first = new_first;
    if (size_ == 0) {
        last = first;
    }
    size_++;
}
template<typename T>
void List<T>::push_back(const T &datum) {
    Node *new_last = new Node{nullptr, last, datum};
    if (size_ >= 1){
    last->next = new_last;
    }
    last = new_last;
    if (size_ == 0) {
        first = last;
    }
    size_++;
}
template<typename T>
void List<T>::pop_front() {
    if (size_ > 1) {
    assert(!empty());
    Node *temp = new Node;
    temp->next = first->next;
    temp->prev = nullptr;
    temp->datum = 0;
    delete first;
    first = temp->next;
    first->next = temp->next->next;
    first->prev = nullptr;
    delete temp;
    }
    else {
        delete first;
        first = nullptr;
        last = nullptr;
    }
    size_--;
}
template<typename T>
void List<T>::pop_back() {
 //   assert(!empty());
    if (size_ > 1) {
    Node *temp = new Node;
    temp->next = nullptr;
    temp->prev = last->prev;
    temp->datum = 0;
    delete last;
    last = temp->prev;
    last->next = nullptr;
    last->prev = temp->prev->prev;
    delete temp;
    }
    else {
        delete last;
        first = nullptr;
        last = nullptr;
    }
    size_--;
}
template<typename T>
void List<T>::clear() {
    const int temp = size_;
    for (int i = 0; i < temp; i++) {
        pop_back();
    }
    assert(size_ == 0);
}
template<typename T>
List<T>::List() {
    first = nullptr;
    last = nullptr;
    size_ = 0;
}
template<typename T>
List<T>::List(const List<T> &other) : List() {
    if (this != &other) {
        *this = other;
    }
}
template<typename T>
List<T>::~List() {
    if (size_ > 1) {
        for (List<T>::Iterator it = *new Iterator(first); it.node_ptr != nullptr; ++it) {
            delete it.node_ptr;
        }
    }
}
template<typename T>
typename List<T>::Iterator & List<T>::Iterator::operator++() {
    assert(node_ptr);
    node_ptr = node_ptr->next;
    return *this;
}
template<typename T>
List<T> & List<T>::operator=(const List &other) {
    if (!empty()) {
        for (int i = 0; i < size_; i++) {
            pop_front();
        }
    }
        for (List<T>::Iterator it = *new Iterator(other.first);
             it.node_ptr != nullptr; ++it) {
            this->push_front(it.node_ptr->datum);
        }
        size_ = other.size_;
    
    return *this;
}
template<typename T>
void List<T>::copy_all(const List<T> &other) {
    this = other;
}

template<typename T>
typename List<T>::Iterator List<T>::end() const {
    return Iterator(nullptr);
}
template<typename T>
void List<T>::erase(Iterator i) {
    if (i.node_ptr->next) {
        if (i.node_ptr->prev) {
        i.node_ptr->next->prev = i.node_ptr->prev;
        i.node_ptr->prev->next = i.node_ptr->next;
        }
        else {
            i.node_ptr->next->prev = nullptr;
        }
    }
    else {
        if (i.node_ptr->prev) {
            i.node_ptr->prev->next = nullptr;
        }
    }
    if (i.node_ptr == first) {
        first = first->next;
    }
    if (i.node_ptr == last) {
        last = last->prev;
    }
    delete i.node_ptr;
    size_--;
}
template<typename T>
void List<T>::insert(Iterator i, const T &datum) {
    if (!i.node_ptr) {
        Node *insert = new Node;
        insert->datum = datum;
        insert->next = nullptr;
        insert->prev = nullptr;
        last = insert;
    }
    else {
    Node *insert = new Node;
    insert->next = i.node_ptr;
    insert->prev = i.node_ptr->prev;
    insert->datum = datum;
        if (i.node_ptr->prev) {
    i.node_ptr->prev->next = insert;
        }
    i.node_ptr->prev = insert;
        if (first == i.node_ptr) {
            first = insert;
        }
    }
    size_++;
}

#endif // Do not remove this. Write all your code above this line.
