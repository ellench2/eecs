// Project identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043

#ifndef PAIRINGPQ_H
#define PAIRINGPQ_H

#include "Eecs281PQ.h"
#include <deque>
#include <utility>

// A specialized version of the priority queue ADT implemented as a pairing
// heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class PairingPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Each node within the pairing heap
    class Node {
        public:
            explicit Node(const TYPE &val)
            : elt{ val }, child{ nullptr }, sibling{ nullptr }, previous{ nullptr }
            {}

            // Description: Allows access to the element at that Node's
            //              position. There are two versions, getElt() and a
            //              dereference operator, use whichever one seems
            //              more natural to you.
            // Runtime: O(1) - this has been provided for you.
            const TYPE &getElt() const { return elt; }
            const TYPE &operator*() const { return elt; }

            // The following line allows you to access any private data
            // members of this Node class from within the PairingPQ class.
            // (ie: myNode.elt is a legal statement in PairingPQ's add_node()
            // function).
            friend PairingPQ;

        private:
            TYPE elt;
            Node *child;
            Node *sibling;
            Node *previous;
    }; // Node


    // Description: Construct an empty pairing heap with an optional
    //              comparison functor.
    // Runtime: O(1)
    explicit PairingPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
            root = nullptr;
            count = 0;
    } // PairingPQ()


    // Description: Construct a pairing heap out of an iterator range with an
    //              optional comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    template<typename InputIterator>
    PairingPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
            root = nullptr;
            count = 0;
            while (start != end) {
                push(*start);
                start++;
            }
    } // PairingPQ()


    // Description: Copy constructor.
    // Runtime: O(n)
    PairingPQ(const PairingPQ &other) :
        BaseClass{ other.compare } {
            root = nullptr;
            count = 0;
            std::deque<Node*> dq;
            dq.push_back(other.root);
            while (!dq.empty()) {
                if (dq.front()) {
                    if (dq.front()->child) {
                        dq.push_back(dq.front()->child);
                    }
                    if (dq.front()->sibling) {
                        dq.push_back(dq.front()->sibling);
                    }
                    push(dq.front()->elt);
                }
                dq.pop_front();
            }
    } // PairingPQ()


    // Description: Copy assignment operator.
    // Runtime: O(n)
    PairingPQ &operator=(const PairingPQ &rhs) {
        PairingPQ temp(rhs);
        Node* temp_root = this->root;
        this->root = temp.root;
        temp.root = temp_root;
        uint32_t temp_count = this->count;
        count = temp.count;
        temp.count = temp_count;
        return *this;
    } // operator=()


    // Description: Destructor
    // Runtime: O(n)
    ~PairingPQ() {
        std::deque<Node*> dq;
        dq.push_back(root);
        while (!dq.empty()) {
            if (dq.front()) {
                if (dq.front()->child) {
                    dq.push_back(dq.front()->child);
                }
                if (dq.front()->sibling) {
                    dq.push_back(dq.front()->sibling);
                }
            }
            delete dq.front();
            dq.front() = nullptr;
            dq.pop_front();
        }
    } // ~PairingPQ()


    // Description: Assumes that all elements inside the pairing heap are out
    //              of order and 'rebuilds' the pairing heap by fixing the
    //              pairing heap invariant. You CANNOT delete 'old' nodes
    //              and create new ones!
    // Runtime: O(n)
    virtual void updatePriorities() {
        std::deque<Node*> dq;
        dq.push_back(root);
        root = nullptr;
        while (!dq.empty()) {
            if (dq.front()->child) {
                dq.push_back(dq.front()->child);
                dq.front()->child = nullptr;
            }
            if (dq.front()->sibling) {
                dq.push_back(dq.front()->sibling);
                dq.front()->sibling = nullptr;
            }
            if (dq.front()->previous) {
                dq.front()->previous = nullptr;
            }
            if (root!=nullptr) {
                Node* temp_root = root;
                root = meld(dq.front(), temp_root);
            }
            else {
                root = dq.front();
            }
            dq.pop_front();
            //do something
        }
    } // updatePriorities()


    // Description: Add a new element to the pairing heap. This is already
    //              done. You should implement push functionality entirely in
    //              the addNode() function, and this function calls
    //              addNode().
    // Runtime: O(1)
    virtual void push(const TYPE &val) {
        addNode(val);
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element
    //              from the pairing heap.
    // Note: We will not run tests on your code that would require it to pop
    //       an element when the pairing heap is empty. Though you are
    //       welcome to if you are familiar with them, you do not need to use
    //       exceptions in this project.
    // Runtime: Amortized O(log(n))
    virtual void pop() {
        count--;
        if (count == 0) {
            delete root;
            root = nullptr;
            return;
        }
        Node* temp = nullptr;
        if (root->child) {
            temp = (root->child);
        }
        std::deque<Node*> dq;
        while(temp) {
            dq.push_back(temp);
            temp = temp->sibling;
        }
        while(dq.size() > 1) {
            dq[0]->previous = nullptr;
            if (dq[0]->sibling) {
                dq[0]->sibling = nullptr;
            }
            if (dq[1]->sibling) {
                dq[1]->sibling = nullptr;
            }
            dq[1]->previous = nullptr;
            dq.push_back(meld(dq[0], dq[1]));
            dq.pop_front();
            dq.pop_front();
        }
        delete root;
        root = dq.front();
//        root->previous = nullptr;
    } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the pairing heap. This should be a reference for speed.
    //              It MUST be const because we cannot allow it to be
    //              modified, as that might make it no longer be the most
    //              extreme element.
    // Runtime: O(1)
    virtual const TYPE &top() const {
        return root->elt;
    } // top()


    // Description: Get the number of elements in the pairing heap.
    // Runtime: O(1)
    virtual std::size_t size() const {
        return count;
    } // size()

    // Description: Return true if the pairing heap is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        return count == 0;
    } // empty()


    // Description: Updates the priority of an element already in the pairing
    //              heap by replacing the element refered to by the Node with
    //              new_value. Must maintain pairing heap invariants.
    //
    // PRECONDITION: The new priority, given by 'new_value' must be more
    //               extreme (as defined by comp) than the old priority.
    //
    // Runtime: As discussed in reading material.
    void updateElt(Node* node, const TYPE &new_value) {
        node->elt = new_value;
        if (node->previous) {
            if (node->previous->child == node) {
                if (node->sibling) {
                    node->previous->child = node->sibling;
                    node->sibling->previous = node->previous;
                }
                else {
                    node->previous->child = nullptr;
                }
            }
            else {
                if (node->sibling) {
                    node->previous->sibling = node->sibling;
                    node->sibling->previous = node->previous;
                }
                else {
                    node->previous->sibling = nullptr;
                }
            }
            node->previous = nullptr;
            if (node->sibling) {
                node->sibling = nullptr;
            }
            Node* temp_root = root;
            root = meld(node, temp_root);
        }
    } // updateElt()


    // Description: Add a new element to the pairing heap. Returns a Node*
    //              corresponding to the newly added element.
    // NOTE: Whenever you create a node, and thus return a Node *, you must
    //       be sure to never move or copy/delete that node in the future,
    //       until it is eliminated by the user calling pop(). Remember this
    //       when you implement updateElt() and updatePriorities().
    // Runtime: O(1)
    Node* addNode(const TYPE &val) {
        Node* newval = new Node(val);
        if (root!=nullptr) {
            count++;
            Node* temp_root = root;
            root = meld(temp_root, newval);
            return newval;
        }
        else {
            count++;
            root = newval;
            return newval;
        }
        // This line is present only so that this provided file compiles.
    } // addNode()


private:
    Node* root;
    uint32_t count;
    Node* meld(Node* a, Node* b) {
        if (this->compare(a->elt, b->elt)) {
            if (!b->child) b->child = a;
            else {
                Node* temp = (b->child);
                b->child = a;
                temp->previous = a;
                a->sibling = temp;
            }
            a->previous = b;
            return b;
        }
        else {
            if (!(a->child)) {
                a->child = b;
            }
            else {
                Node* temp = (a->child);
                a->child = b;
                temp->previous = b;
                b->sibling = temp;
            }
            b->previous = a;
            return a;
        }
    }
    
    // NOTE: For member variables, you are only allowed to add a "root
    //       pointer" and a "count" of the number of nodes. Anything else
    //       (such as a deque) should be declared inside of member functions
    //       as needed.
};


#endif // PAIRINGPQ_H
