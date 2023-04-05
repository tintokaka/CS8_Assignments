#ifndef HEAP_H
#define HEAP_H

#include <cassert>    // Provides assert
#include <iomanip>    // Provides std::setw
#include <iostream>   // Provides std::cout

template <class Item>
class Heap
{
public:
  // TYPEDEF
  typedef Item value_type;

  // CONSTRUCTOR
  Heap(){
    _heap = new Item[CAPACITY];
    _size = 0;
  }

  Heap(Item* arr, int size){
    _heap = new Item[CAPACITY];
    _size = 0;
    for (int i = 0; i < size; i++)
    {
        insert(arr[i]);
    }
  }

  // BIG 3
  Heap(const Heap& x){
    _heap = new Item[CAPACITY];
    _size = 0;
    for (int i = 0; i < x._size; i++)
    {
        insert(x._heap[i]);
    }
  }                //Constructs a container with a copy of each of the elements in x, in the same order.

  ~Heap(){
    delete[] _heap;
  }                            //Destroys the container object.

  Heap& operator = (const Heap& x){
    if (this != &x){
        delete[] _heap;
        _heap = new Item[CAPACITY];
        _size = 0;
        for (int i = 0; i < x._size; i++)
        {
            insert(x._heap[i]);
        }
    }
    return *this;
  };   //Assigns new contents to the container, replacing its current contents, and modifying its size accordingly.

  // MODIFICATION MEMBER FUNCTIONS
  void heapify(int i){
    if (!is_leaf(i))
    {
        int left = left_child_index(i);
        int right = right_child_index(i);
        int largest = i;
        if (left < _size && _heap[left] > _heap[largest])
        {
            largest = left;
        }
        if (right < _size && _heap[right] > _heap[largest])
        {
            largest = right;
        }
        if (largest != i)
        {
            std::swap(_heap[i], _heap[largest]);
            heapify(largest);
        }
    }
  };                //heapify the new node following a Bottom-up approach

  void heapify(Item*& arr, int n, int i){
     if (!is_leaf(i))
    {
        int left = left_child_index(i);
        int right = right_child_index(i);
        int largest = i;
        if (left < n && arr[left] > arr[largest])
        {
            largest = left;
        }
        if (right < n && arr[right] > arr[largest])
        {
            largest = right;
        }
        if (largest != i)
        {
            std::swap(arr[i], arr[largest]);
            heapify(arr, n, largest);
        }
    }
  };//heapify the new node following a Bottom-up approach

  void insert(const Item& insert_me){
    if (_size == CAPACITY)
    {
        std::cout << "Heap overflow" << std::endl;
        return;
    }
    _heap[_size++] = insert_me;
    int i = _size - 1;
    while (!is_root(i) && _heap[i] > _heap[parent_index(i)])
    {
        swap_with_parent(i);
        i = parent_index(i);
    }
  }; //insert into the heap

  void remove(){
    if (_size == 0)
    {
        std::cout << "Heap underflow" << std::endl;
        return;
    }
    std::swap(_heap[0], _heap[--_size]);
    heapify(0);
  };                      //remove top value

  void remove_node(const Item& delete_me){
    // Search for the node to be deleted
    int i;
    for (i = 0; i < _size; i++) {
      if (_heap[i] == delete_me) {
        break;
      }
    }
    if (i == _size) {
      // The value was not found in the heap
      return;
    }

    // Replace the value of the node with the value of the last leaf node
    _heap[i] = _heap[_size - 1];

    // Remove the last leaf node
    _size--;

    // Restore the heap property by swapping with parent if necessary
    int parent = parent_index(i);
    while (i > 0 && _heap[i] < _heap[parent]) {
      swap_with_parent(i);
      i = parent;
      parent = parent_index(i);
    }
    heapify(i);
  }; 

  Item top() {
    if (_size == 0) {
    throw std::out_of_range("Heap is empty");
    }
    return _heap[0];
  }      //return the value at the top of heap

  // CONST MEMBER FUNCTIONS
  bool is_empty() const{ 
    return _size == 0;
  }

  int size() const{
    return _size;
  };

  int capacity() const{
    return CAPACITY;
  };

  bool verify() const{
    return verify(0);
  };                //true if tree is verified to be a heap

  Item *heap_array() const{
    Item* copy = new Item[_size];
    std::copy(_heap, _heap + _size, copy);
    return copy;
  };           //returns a copy of underlying array:
                                      //  don't forget to delete when done
  Item *heap_sort(){
    Item* copy = heap_array();
    for (int i = _size - 1; i > 0; i--) {
        std::swap(copy[0], copy[i]);
        int j = 0;
        int index;
        do {
            index = left_child_index(j);
            if (index < i - 1 && copy[index] < copy[index + 1]) {
                index++;
            }
            if (index < i && copy[j] < copy[index]) {
                std::swap(copy[j], copy[index]);
            }
            j = index;
        } while (index < i);
    }
    return copy;
  };                  //returns a copy of sorted array
  
  // OVERLOAD OPERATOR FUNCTIONS
  friend std::ostream& operator << (std::ostream& outs, const Heap<Item>& print_me) {
    print_me.print_tree();
    return outs;
  }
private:
  static const int CAPACITY = 1000;
  int _size;   // the space used
  Item* _heap; // dynamic array

  void print_tree(std::ostream& outs = std::cout) const{
    print_tree(0, 0, outs);
  };

  void print_tree(int root, int level = 0, std::ostream& outs = std::cout) const{
    if (root >= _size) return;

    print_tree(right_child_index(root), level + 1, outs);

    for (int i = 0; i < level; i++) {
        outs << "\t";
    }

    outs << _heap[root] << std::endl;

    print_tree(left_child_index(root), level + 1, outs);
  };

  bool verify(int root) const{
    if (root >= _size) {
        return true;
    }
    int left_child = left_child_index(root);
    int right_child = right_child_index(root);
    if (left_child < _size && _heap[root] < _heap[left_child]) {
        return false;
    }
    if (right_child < _size && _heap[root] < _heap[right_child]) {
        return false;
    }
    return verify(left_child) && verify(right_child);
  };

  bool is_root(int i) const{
     return (i == 0);
  };
  bool is_leaf(int i) const{
     return (left_child_index(i) >= _size);
  };
  int parent_index(int i) const{
     return (i - 1) / 2;
  };
  int left_child_index(int i) const{
    return 2 * i + 1;
  };
  int right_child_index(int i) const{
    return 2 * i + 2;
  };
  int first_nonleaf() const{
    return parent_index(_size - 1);
  };
  void swap_with_parent(int i){
    std::swap(_heap[i], _heap[parent_index(i)]);
  };
};


// Implementation

// TODO

#endif // HEAP_H
