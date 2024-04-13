// Project UID db1f506d06d84ab787baf250c265e24e

#include "BinarySearchTree.h"
#include "unit_test_framework.h"
#include <sstream>

TEST(test_basic) {
    BinarySearchTree<int> tree;
    
    tree.insert(1);
    tree.insert(5);
    tree.insert(0);
    tree.insert(2);
    
    
    ASSERT_TRUE(!tree.empty());
    ASSERT_TRUE(tree.size() == 4);
    ASSERT_TRUE(tree.height() == 3);
    
    BinarySearchTree<int> tree1 = tree;
    ASSERT_TRUE(*tree1.find(0) == 0);
    ASSERT_TRUE(*tree1.find(1) == 1);
    ASSERT_TRUE(*tree1.find(2) == 2);
    ASSERT_TRUE(*tree1.find(5) == 5);
    
    ASSERT_TRUE(*tree.min_element() == 0);
    ASSERT_TRUE(*tree.max_element() == 5);
    ASSERT_TRUE(*tree.min_greater_than(4) == 5);
    
    ASSERT_TRUE(tree.check_sorting_invariant());
    
    std::ostringstream oss_preorder;
    tree.traverse_preorder(oss_preorder);
    ASSERT_TRUE(oss_preorder.str() == "1 0 5 2 ");
    std::ostringstream oss_inorder;
    tree.traverse_inorder(oss_inorder);
    ASSERT_TRUE(oss_inorder.str() == "0 1 2 5 ");
    
    tree.insert(3);
    ASSERT_TRUE(tree.find(3) != tree.end());
    ASSERT_TRUE(*(++tree.find(3)) == 5);
    ASSERT_TRUE(++++tree.find(3) == tree.end());
    
    std::ostringstream oss_inorder_1;
    tree.traverse_inorder(oss_inorder_1);
    ASSERT_TRUE(oss_inorder_1.str() == "0 1 2 3 5 ");
    std::ostringstream oss_preorder_1;
    tree.traverse_preorder(oss_preorder_1);
    ASSERT_TRUE(oss_preorder_1.str() == "1 0 5 2 3 ");
    
}

TEST(test_empty) {
    BinarySearchTree<int> tree;
    
    ASSERT_TRUE(tree.begin() == tree.end());
    ASSERT_TRUE(tree.empty());
    ASSERT_TRUE(tree.size() == 0);
    ASSERT_TRUE(tree.height() == 0);
    
    BinarySearchTree<int> tree1 = tree;
    ASSERT_TRUE(tree1.find(0) == tree.end());
    ASSERT_TRUE(tree.find(0) == tree.end());
    
    ASSERT_TRUE(tree.min_element() == tree.max_element());
    ASSERT_TRUE(tree.min_greater_than(-1) == tree.end());
    ASSERT_TRUE(tree.check_sorting_invariant());
    
    std::ostringstream oss_preorder;
    tree.traverse_preorder(oss_preorder);
    ASSERT_TRUE(oss_preorder.str() == "");
    std::ostringstream oss_inorder;
    tree.traverse_inorder(oss_inorder);
    ASSERT_TRUE(oss_inorder.str() == "");
    
}

TEST(test_line) {
    BinarySearchTree<int> tree;
    
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(4);
    tree.insert(5);
    
    ASSERT_TRUE(!tree.empty());
    ASSERT_TRUE(tree.size() == tree.height());
    
    BinarySearchTree<int> tree1 = tree;
    ASSERT_TRUE(tree1.find(1) == tree1.begin());
    ASSERT_TRUE(tree.find(1) == tree.begin());
    
    ASSERT_TRUE(*tree.min_element() == 1);
    ASSERT_TRUE(*tree.max_element() == 5);
    ASSERT_TRUE(*tree.min_greater_than(4) == 5);
    ASSERT_TRUE(tree.min_greater_than(0) == tree.begin());
    
    std::ostringstream oss_preorder;
    tree.traverse_preorder(oss_preorder);
    std::ostringstream oss_inorder;
    tree.traverse_inorder(oss_inorder);
    ASSERT_TRUE(oss_inorder.str() == oss_preorder.str());
    
}

TEST(test_zigzag) {
    
    class Bug {
    public:
        Bug(): legs(4) {}
        Bug(int legs_in) : legs(legs_in) {}
        int getLegs() const {return legs; }
        
    private:
        int legs;
    };
    
    class Bug_less {
    public:
        bool operator() (const Bug &bug1, const Bug &bug2) {
            return bug1.getLegs() < bug2.getLegs();
        }
    };
    
    BinarySearchTree<Bug, Bug_less> bugtree;
    
    bugtree.insert(Bug());
    bugtree.insert(Bug(100));
    bugtree.insert(Bug(8));
    bugtree.insert(Bug(98));
    bugtree.insert(Bug(10));
    bugtree.insert(Bug(50));
    
    BinarySearchTree<Bug, Bug_less>::Iterator bugiter(bugtree.find(Bug()));
    
    *bugiter = Bug(1000);
    
    ASSERT_FALSE(bugtree.check_sorting_invariant());
    
    *bugiter = Bug();
    
    ASSERT_TRUE(bugtree.size() == bugtree.height());
    ASSERT_TRUE(bugtree.find(Bug(15)) == bugtree.end());
    bugtree.insert(Bug(5));
    ASSERT_TRUE(bugtree.size() - 1 == bugtree.height());
    const Bug centipede = Bug(100);
    ASSERT_TRUE((*bugtree.max_element()).getLegs() == centipede.getLegs());
    ASSERT_TRUE((*++bugtree.min_element()).getLegs() == 5);
    
   
    
}
TEST_MAIN()
