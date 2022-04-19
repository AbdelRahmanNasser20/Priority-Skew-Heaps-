// CMSC 341 - Spring 2022 - Project 3
#include "squeue.h"
#include <random>
#include <vector>

// the followings are sample priority functions to be used by SQueue class
// users can define their own priority functions
// Priority functions compute an integer priority for a post.  Internal
// computations may be floating point, but must return an integer.
int priorityFn1(const Post &post);// works with a MAXHEAP
int priorityFn2(const Post &post);// works with a MINHEAP

enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else{ //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }

private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution

};

class Tester{
public:
    bool testInsertionMinHeap();
    bool testInsertionMaxHeap();

    bool testRemovalMinHeap();
    bool testRemovalMaxHeap();

    bool testCopyConsNormalCase();
    bool testCopyConsEdgeCase();

    bool testAssignmentOperNormalCase();
    bool testAssignmentOperEdgeCase();

    void testDequeueEmptyQueue();
    bool testMergeWithQueueNormalCase();

    bool testRepeatedRebuildOfAHeap();
    void testMergeDifferentTypesOfHeaps();

    bool preOrderCompareValuesMinHeap(Node * node);
    bool preOrderCompareValuesMaxHeap(Node * node);
    void retriveAllNodes(Node * node, vector <Node*> &vect);

};

void assertTest(bool flag, string stat);

int main(){

    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);
    SQueue queue1(priorityFn2, MINHEAP);
    for (int i=0;i<10;i++){
        Post aPost(idGen.getRandNum(),
                   likesGen.getRandNum(),
                   conLevelGen.getRandNum(),
                   timeGen.getRandNum(),
                   interestGen.getRandNum());
        queue1.insertPost(aPost);
    }
    cout << "\nDump of the queue with priorityFn2 (MINHEAP):\n";
    queue1.dump();
    queue1.setPriorityFn(priorityFn1, MAXHEAP);
    cout << "\nDump of the queue with priorityFn1 (MAXHEAP):\n";
    queue1.dump();
    cout << "\nPreorder traversal of the nodes in the queue with priorityFn1 (MAXHEAP):\n";
    queue1.printPostsQueue();

    Tester tester;

    {
        assertTest(tester.testInsertionMaxHeap(), " Insert 300 nodes in max heap\n");
    }

    {
        assertTest(tester.testInsertionMinHeap(), " Insert 300 nodes in min heap\n");
    }

    {
        assertTest(tester.testRemovalMinHeap(), " removing 150 of 300 nodes in min heap\n");
    }

    {
        assertTest(tester.testRemovalMaxHeap(), " removing 150 of 300 nodes in max heap\n");
    }

    {
        assertTest(tester.testRepeatedRebuildOfAHeap(), "rebuilding the same heap into different types of heaps\n");
    }

    {
        assertTest(tester.testMergeWithQueueNormalCase(), " MergeWithQueue Normal Case \n");
    }

    {
        tester.testDequeueEmptyQueue();
    }

    {
        tester.testMergeDifferentTypesOfHeaps();
    }

    {
        assertTest(tester.testCopyConsNormalCase(), " Deep Copy Using Copy Cons Normal Case \n");
    }

    {
        assertTest(tester.testAssignmentOperNormalCase(), " Deep Copy Using Assignment Oper Normal Case \n");
    }

    {
        assertTest(tester.testCopyConsEdgeCase(), " Copy Cons Edge Case \n");
    }

    {
        assertTest(tester.testAssignmentOperEdgeCase(), " Assignment Oper Edge Case\n");
    }


    return 0;
}

void assertTest(bool flag, string stat){
    if (flag){
        cout << "Test Passed " << stat << endl;
        return;
    }
    cout << "Test Failed " << stat << endl;
}

int priorityFn1(const Post &post) {
    //needs MAXHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [1-510]
    //the highest priority would be 500+10 = 510
    //the lowest priority would be 0+1 = 1
    //the larger value means the higher priority
    int priority = post.getNumLikes() + post.getInterestLevel();
    return priority;
}

int priorityFn2(const Post &post) {
    //needs MINHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [2-55]
    //the highest priority would be 1+1 = 2
    //the lowest priority would be 5+50 = 55
    //the smaller value means the higher priority
    int priority = post.getPostTime() + post.getConnectLevel();
    return priority;
}

bool Tester::testInsertionMinHeap() {
    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);
    SQueue queue1(priorityFn2, MINHEAP);
    for (int i=0;i<300;i++){
        Post aPost(idGen.getRandNum(),
                   likesGen.getRandNum(),
                   conLevelGen.getRandNum(),
                   timeGen.getRandNum(),
                   interestGen.getRandNum());
        queue1.insertPost(aPost);
    }

    if (!preOrderCompareValuesMinHeap(queue1.m_heap))
        return false;
    return true;
}

bool Tester::testRemovalMinHeap() {
    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);
    SQueue queue1(priorityFn2, MINHEAP);
    for (int i=0;i<300;i++){
        Post aPost(idGen.getRandNum(),
                   likesGen.getRandNum(),
                   conLevelGen.getRandNum(),
                   timeGen.getRandNum(),
                   interestGen.getRandNum());
        queue1.insertPost(aPost);
    }
    int tempId = 0;
    for (int i=0;i<150;i++){
        tempId = priorityFn2(queue1.m_heap->getPost());

        if (priorityFn2(queue1.getNextPost()) != tempId){
            return false;
        }
    }

    if (!preOrderCompareValuesMinHeap(queue1.m_heap))
        return false;
    return true;
}

bool Tester::testRemovalMaxHeap() {
    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);
    SQueue queue1(priorityFn1, MAXHEAP);
    for (int i=0;i<300;i++){
        Post aPost(idGen.getRandNum(),
                   likesGen.getRandNum(),
                   conLevelGen.getRandNum(),
                   timeGen.getRandNum(),
                   interestGen.getRandNum());
        queue1.insertPost(aPost);
    }
    int tempId = 0;
    for (int i=0;i<150;i++){
        tempId = priorityFn1(queue1.m_heap->getPost());

        if (priorityFn1(queue1.getNextPost()) != tempId){
            return false;
        }
    }

    if (!preOrderCompareValuesMaxHeap(queue1.m_heap))
        return false;
    return true;
}

bool Tester::testInsertionMaxHeap() {
    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);
    SQueue queue1(priorityFn1, MAXHEAP);
    for (int i=0;i<300;i++){
        Post aPost(idGen.getRandNum(),
                   likesGen.getRandNum(),
                   conLevelGen.getRandNum(),
                   timeGen.getRandNum(),
                   interestGen.getRandNum());
        queue1.insertPost(aPost);
    }

    if (!preOrderCompareValuesMaxHeap(queue1.m_heap))
        return false;
    return true;
}

bool Tester::testRepeatedRebuildOfAHeap(){
    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);
    SQueue queue1(priorityFn2, MINHEAP);
    for (int i=0;i<100;i++){
        Post aPost(idGen.getRandNum(),
                   likesGen.getRandNum(),
                   conLevelGen.getRandNum(),
                   timeGen.getRandNum(),
                   interestGen.getRandNum());
        queue1.insertPost(aPost);
    }

    queue1.setPriorityFn(priorityFn1, MAXHEAP);

    if (!preOrderCompareValuesMaxHeap(queue1.m_heap))
        return false;

    queue1.setPriorityFn(priorityFn2, MINHEAP);

    if (!preOrderCompareValuesMinHeap(queue1.m_heap))
        return false;

    return true;


}

bool Tester::testMergeWithQueueNormalCase(){

    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);
    SQueue queue1(priorityFn2, MINHEAP);
    for (int i=0;i<150;i++){
        Post aPost(idGen.getRandNum(),
                   likesGen.getRandNum(),
                   conLevelGen.getRandNum(),
                   timeGen.getRandNum(),
                   interestGen.getRandNum());
        queue1.insertPost(aPost);
    }
    SQueue queue2(priorityFn2, MINHEAP);

    for (int i=0;i<150;i++) {
        Post aPost(idGen.getRandNum(),
                   likesGen.getRandNum(),
                   conLevelGen.getRandNum(),
                   timeGen.getRandNum(),
                   interestGen.getRandNum());
        queue1.insertPost(aPost);
    }
    queue1.mergeWithQueue(queue2);
    if (queue1.m_size != 300)
        return false;

    if (!preOrderCompareValuesMinHeap(queue1.m_heap))
        return false;
    return true;

}

void Tester::testDequeueEmptyQueue() {
    SQueue queue1(priorityFn2, MINHEAP);

    try{
        queue1.getNextPost();
        assertTest(false, "dequeue an empty queue throws an out_of_range exception.\n");

    }
    catch(...){assertTest(true, "dequeue an empty queue throws an out_of_range exception.\n");}

}

void Tester::testMergeDifferentTypesOfHeaps() {
    SQueue queue1(priorityFn2, MINHEAP);
    SQueue queue2(priorityFn1, MAXHEAP);

    try{
        queue1.mergeWithQueue(queue2);
        assertTest(false, "merge queues with different priority functions throws a domain_error exception.\n");
    }
    catch(...){assertTest(true, "merge queues with different priority functions throws a domain_error exception.\n");}

}

bool Tester::testCopyConsNormalCase() {
    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);
    SQueue queue1(priorityFn2, MINHEAP);

    for (int i=0;i<10;i++){
        Post aPost(idGen.getRandNum(),
                   likesGen.getRandNum(),
                   conLevelGen.getRandNum(),
                   timeGen.getRandNum(),
                   interestGen.getRandNum());
        queue1.insertPost(aPost);
    }
    SQueue queue2(queue1);

    vector <Node*> queue1Nodes;
    vector <Node*> queue2Nodes;
    // retriving the nodes of both heaps
    retriveAllNodes(queue1.m_heap,queue1Nodes);
    retriveAllNodes(queue2.m_heap,queue2Nodes);

    // comparing the priority of all posts and the memory address of all nodes
    for (int i = 0; i < queue1.m_size; i++){

        // checks if the priority number is the same for all posts
        if (priorityFn2(queue1Nodes[i]->getPost()) != priorityFn2(queue2Nodes[i]->getPost()))
            return false;

        // checks the memeroy location of each node to see if there are differnet
        if (queue1Nodes[i] == queue2Nodes[i])
            return false;
    }
    return true;
}

bool Tester::testAssignmentOperNormalCase() {
    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);
    SQueue queue1(priorityFn2, MINHEAP);

    for (int i=0;i<10;i++){
        Post aPost(idGen.getRandNum(),
                   likesGen.getRandNum(),
                   conLevelGen.getRandNum(),
                   timeGen.getRandNum(),
                   interestGen.getRandNum());
        queue1.insertPost(aPost);
    }
    SQueue queue2 = (queue1);

    vector <Node*> queue1Nodes;
    vector <Node*> queue2Nodes;
    // retriving the nodes of both heaps
    retriveAllNodes(queue1.m_heap,queue1Nodes);
    retriveAllNodes(queue2.m_heap,queue2Nodes);

    // comparing the priority of all posts and the memory address of all nodes
    for (int i = 0; i < queue1.m_size; i++){

        // checks if the priority number is the same for all posts
        if (priorityFn2(queue1Nodes[i]->getPost()) != priorityFn2(queue2Nodes[i]->getPost()))
            return false;

        // checks the memeroy location of each node to see if there are differnet
        if (queue1Nodes[i] == queue2Nodes[i])
            return false;

    }
    return true;
}

void Tester::retriveAllNodes(Node * node, vector <Node*> &vect){
    if (node == nullptr)
        return;

    vect.push_back(node);
    retriveAllNodes(node->m_left,vect);
    retriveAllNodes(node->m_right,vect);
}

bool Tester::testCopyConsEdgeCase() {
    SQueue queue1(priorityFn2, MINHEAP);
    SQueue queue2(queue1);

    if (queue1.m_heap != nullptr and queue2.m_heap != nullptr)
        return false;
    if (queue1.m_size != 0 and queue2.m_size != 0)
        return false;
    return true;

}

bool Tester::testAssignmentOperEdgeCase() {
    SQueue queue1(priorityFn2, MINHEAP);
    SQueue queue2 = (queue1);

    if (queue1.m_heap != nullptr and queue2.m_heap != nullptr)
        return false;
    if (queue1.m_size != 0 and queue2.m_size != 0)
        return false;
    return true;

}

bool Tester::preOrderCompareValuesMaxHeap(Node *node){

    if (node == nullptr) {
        return true;
    }

    else{

        if (node->m_right != nullptr and priorityFn1(node->m_right->getPost()) > priorityFn1(node->getPost())) {
            return false;
        }

        if (node->m_left != nullptr and priorityFn1(node->m_left->getPost()) > priorityFn1(node->getPost())) {
            return false;
        }
    }

    if (!preOrderCompareValuesMaxHeap(node->m_left))
        return false;

    else if (!preOrderCompareValuesMaxHeap(node->m_right))
        return false;

    else
        return true;

}

bool Tester::preOrderCompareValuesMinHeap(Node *node){

    if (node == nullptr) {
        return true;
    }

    else{

        if (node->m_right != nullptr and priorityFn2(node->m_right->getPost()) < priorityFn2(node->getPost())) {
            return false;
        }

        if (node->m_left != nullptr and priorityFn2(node->m_left->getPost()) < priorityFn2(node->getPost())) {
            return false;
        }
    }

    if (!preOrderCompareValuesMinHeap(node->m_left))
        return false;

    else if (!preOrderCompareValuesMinHeap(node->m_right))
        return false;

    else
        return true;

}