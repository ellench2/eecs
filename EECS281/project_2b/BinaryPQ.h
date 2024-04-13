// Project identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043

#ifndef BINARYPQ_H
#define BINARYPQ_H


#include <algorithm>
#include "Eecs281PQ.h"

// A specialized version of the priority queue ADT implemented as a binary
// heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class BinaryPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Description: Construct an empty PQ with an optional comparison functor.
    // Runtime: O(1)
    explicit BinaryPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
            static TYPE temp;
            data.insert(data.begin(), temp);
    } // BinaryPQ


    // Description: Construct a PQ out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    template<typename InputIterator>
    BinaryPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
        // TODO: Implement this function
            static TYPE temp;
            data.insert(data.begin(), temp);
            while (start != end) {
                data.push_back(*start);
                start++;
            }
            this->updatePriorities();
            
        // These lines are present only so that this provided file compiles.

    } // BinaryPQ


    // Description: Destructor doesn't need any code, the data vector will
    //              be destroyed automatically.
    virtual ~BinaryPQ() {
    } // ~BinaryPQ()


    // Description: Assumes that all elements inside the heap are out of
    //              order and 'rebuilds' the heap by fixing the heap
    //              invariant.
    // Runtime: O(n)
    virtual void updatePriorities() {
        //heapify: fixdown from bottom to top
        for (int i = static_cast<int>(data.size() - 1); i > 0; i--) {
            fixdown(i);
        }
        // TODO: Implement this function.
    } // updatePriorities()


    // Description: Add a new element to the PQ.
    // Runtime: O(log(n))
    virtual void push(const TYPE &val) {
        // TODO: Implement this function.
        data.push_back(val);
        if (data.size() > 1) {
            fixup(static_cast<int>(data.size() - 1));
        }
        else {
            data.push_back(val);
        }

    } // push()


    // Description: Remove the most extreme (defined by 'compare') element
    //              from the PQ.
    // Note: We will not run tests on your code that would require it to pop
    //       an element when the PQ is empty. Though you are welcome to if
    //       you are familiar with them, you do not need to use exceptions in
    //       this project.
    // Runtime: O(log(n))
    virtual void pop() {
        if (data.size() == 1) {
            return;
        }
        data[1] = data[data.size() - 1];
        data.pop_back();
        if (this->size() > 0) {
            fixdown(1);
        }
        //replace top with last element, remove last element, fix down
        // TODO: Implement this function.
    } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the PQ. This should be a reference for speed. It MUST
    //              be const because we cannot allow it to be modified, as
    //              that might make it no longer be the most extreme element.
    // Runtime: O(1)
    virtual const TYPE &top() const {
        // TODO: Implement this function.
        return data[1];
        // These lines are present only so that this provided file compiles.

    } // top()


    // Description: Get the number of elements in the PQ.
    // Runtime: O(1)
    virtual std::size_t size() const {
        // TODO: Implement this function. Might be very simple,
        // depending on your implementation.
        return data.size()-1;

    } // size()


    // Description: Return true if the PQ is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        // TODO: Implement this function. Might be very simple,
        // depending on your implementation.
        return data.size() == 1; // TODO: Delete or change this line
    } // empty()


private:
    // Note: This vector *must* be used for your PQ implementation.
    std::vector<TYPE> data;
    // NOTE: You are not allowed to add any member variables. You don't need
    //       a "heapSize", since you can call your own size() member
    //       function, or check data.size().
    void fixup(int k) {
        while (k > 1 && this->compare(data[k/2],data[k])) {
            TYPE temp = data[k];
            data[k] = data[k/2];
            data[k/2] = temp;
            k /= 2;
        }
    }
    void fixdown(int k) {
        while (2*k < static_cast<int>(data.size())) {
            int j = 2*k;
            if (j < static_cast<int>(data.size()) - 1 && this->compare(data[j],data[j+1])) ++j;
            if (this->compare(data[j],data[k]) ||
                (!(this->compare(data[j],data[k])) && !(this->compare(data[k],data[j])))) break;
            TYPE temp = data[j];
            data[j] = data[k];
            data[k] = temp;
            k = j;
        }
    }

}; // BinaryPQ


#endif // BINARYPQ_H
