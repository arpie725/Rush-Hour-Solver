#ifndef HEAP_H
#define HEAP_H
#include <functional>
#include <stdexcept>
#include <vector>

template <typename T, typename PComparator = std::less<T> >
class Heap
{
public:
  /**
   * @brief Construct a new Heap object
   * 
   * @param m ary-ness of heap tree (default to 2)
   * @param c binary predicate function/functor that takes two items
   *          as an argument and returns a bool if the first argument has
   *          priority over the second.
   */
  Heap(int m=2, PComparator c = PComparator());

  /**
  * @brief Destroy the Heap object
  * 
  */
  ~Heap();

  /**
   * @brief Push an item to the heap
   * 
   * @param item item to heap
   */
  void push(const T& item);

  /**
   * @brief Returns the top (priority) item
   * 
   * @return T const& top priority item
   * @throw std::underflow_error if the heap is empty
   */
  T const & top() const;

  /**
   * @brief Remove the top priority item
   * 
   * @throw std::underflow_error if the heap is empty
   */
  void pop();

  /// returns true if the heap is empty

  /**
   * @brief Returns true if the heap is empty
   * 
   */
  bool empty() const;

private:
  /// Add whatever helper functions and data members you need below
  void heapify(int idx);
  std::vector<T> items_;
  int m_;
  PComparator c_;
};

// Add implementation of member functions here

template <typename T, typename PComparator>
Heap<T, PComparator>::Heap(int m, PComparator c) 
  : m_(m), c_(c)
{
}

template <typename T, typename PComparator>
Heap<T, PComparator>::~Heap()
{
}

template <typename T, typename PComparator>
void Heap<T, PComparator>::push(const T& item) {
  items_.push_back(item);
  if(items_.size() == 1) {
    return;
  }
  int loc = items_.size() - 1;
  int parent = (loc - 1) / m_;
  while(loc >= 1 && c_(items_[loc], items_[parent])) {
    std::swap(items_[loc], items_[parent]);
    loc = parent;
    parent = (loc - 1) / m_;
  }
}

// We will start top() for you to handle the case of 
// calling top on an empty heap
template <typename T, typename PComparator>
T const & Heap<T,PComparator>::top() const
{
  // Here we use exceptions to handle the case of trying
  // to access the top element of an empty heap
  if(empty()) {
    // ================================
    // throw the appropriate exception
    // ================================
    throw std::underflow_error("heap is empty, cannot get top!");
  }
  // If we get here we know the heap has at least 1 item
  // Add code to return the top element
  return items_[0];
}


// We will start pop() for you to handle the case of 
// calling top on an empty heap
template <typename T, typename PComparator>
void Heap<T,PComparator>::pop()
{
  if(empty()){
    // ================================
    // throw the appropriate exception
    // ================================
    throw std::underflow_error("heap is empty, cannot pop!");
  }
  items_[0] = items_.back(); // make a copy of the last item 
  items_.pop_back(); // remove the last item
  heapify(0);
}

template <typename T, typename PComparator>
void Heap<T, PComparator>::heapify(int idx) {
  if((idx * m_) + 1 >= (int) items_.size()) { // idx is a leaf node
    return;
  }
  int parentIDX = idx;
  int bestChildIDX = (idx * m_) + 1; // starting with left
  int changingNode = bestChildIDX;
  // don't know how many children the parent has so have to use a while loop
  while(changingNode++ <= (idx * m_) + m_ && changingNode <= (int) items_.size() - 1) {
    if(c_(items_[changingNode], items_[bestChildIDX])) {
      // if the current node is better than the bestChilIDX
      bestChildIDX = changingNode;
    }
  }
  // at this point, the bestChildIDX has been found and now have to check if it is better than the parent
  if(c_(items_[bestChildIDX], items_[parentIDX])) {
    // if bestChildIDX is better, swap it's pos with parent's
    T temp = items_[bestChildIDX];
    items_[bestChildIDX] = items_[parentIDX];
    items_[parentIDX] = temp;
    // recursively call heapify to move vertically down the tree
    heapify(bestChildIDX);
  }
}

template <typename T, typename PComparator>
bool Heap<T, PComparator>::empty() const {
  return items_.empty();
}

#endif