// CMSC 341 - Spring 2022 - Project 3
#include "squeue.h"

SQueue::SQueue(prifn_t priFn, HEAPTYPE heapType)
{
    m_heap = nullptr;
    m_heapType = heapType;
    m_priorFunc = priFn;
    m_size = 0;
}

SQueue::~SQueue()
{
    clear();
}

void SQueue::clear() {
    clear(m_heap);
    m_heap = nullptr;
    m_size = 0;

}

Node* SQueue::clear(Node * node)
{

    if (node != nullptr) {
        // reucrsive call to destroy the right and left children
        clear(node->m_right);
        clear(node->m_left);
        delete node;
        node = nullptr;
    }

}

SQueue::SQueue(const SQueue& rhs)
{
    // protects against self copy
    if (&rhs != this) {
        m_heap = nullptr;
        m_heapType = rhs.m_heapType;
        m_priorFunc = rhs.m_priorFunc;
        m_size = rhs.m_size;
        m_heap = clone(rhs.m_heap);
    }
}

SQueue& SQueue::operator=(const SQueue& rhs)
{
    // protects against self assignment
    if (&rhs != this) {
        m_heap = nullptr;
        m_heapType = rhs.m_heapType;
        m_priorFunc = rhs.m_priorFunc;
        m_size = rhs.m_size;
        m_heap = clone(rhs.m_heap); // clones each node
    }
    return *this;
}

Node* SQueue::clone(Node* node)
{
    if (node == nullptr)
        return nullptr;

    // allocate mem for the new post
    Node * cloneNode = new Node(node->getPost());
    cloneNode->m_right = clone(node->m_right); // clone the right node
    cloneNode->m_left = clone(node->m_left); // clone the left node

    return cloneNode;
}

void SQueue::insertPost(const Post& post) {

    m_heap = merge(new Node(post), m_heap);
    m_size++;
}

Node *SQueue::merge(Node *node1, Node *heap2) {
    if (node1 == nullptr)
        return heap2;
    if (heap2 == nullptr)
        return node1;

    // if this is a min heap ensure node 1 has higher priority
    if (getHeapType() == MINHEAP) {
        if (m_priorFunc(node1->getPost()) > m_priorFunc(heap2->getPost()))
            swap(&node1, &heap2);
    }

    // if this is a max heap ensure node 1 has higher priority
    if (getHeapType() == MAXHEAP) {
        if (m_priorFunc(node1->getPost()) < m_priorFunc(heap2->getPost()))
            swap(&node1, &heap2);
    }

    // swap the children address
    swap(&node1->m_left, &node1->m_right);

    // merge node ones left with the heap
    node1->m_left = merge(heap2, node1->m_left);
    return node1;
}

Post SQueue::getNextPost()
{
    // if theres no next post
    if (m_size == 0)
        throw out_of_range ("Cant Dequeue empty queue");

    Node * rootToDelete = m_heap; // temp store the node to delete
    Post temp = m_heap->getPost(); // store the post to return
    m_heap = merge(m_heap->m_left,m_heap->m_right); // merge the left and the right children of the root
    m_size--; // decrease the size by one
    delete rootToDelete; // free the node
    return temp; // return the post
}

void SQueue::mergeWithQueue(SQueue& rhs) {

    // protects against merging different heaps
    if (m_heapType != rhs.m_heapType)
        throw domain_error("Cant Merge different types of Heaps");

    // protects against self merging
    if (this != &rhs) {
        // mearges the two heaps
        m_heap = merge(m_heap, rhs.m_heap);
        m_size += rhs.m_size;

        //rhs heap pointer and size will no longer exist
        rhs.m_heap = nullptr;
        rhs.m_size = 0;

    }


}

int SQueue::numPosts() const{ return m_size;}

void SQueue::printPostsQueue() const {preOrderTraversal(m_heap);}

void SQueue::preOrderTraversal(Node * node) const{
    if (node == nullptr)
        return;

    cout << node->getPost() << endl;
    preOrderTraversal(node->m_left);
    preOrderTraversal(node->m_right);

}

prifn_t SQueue::getPriorityFn() const {return m_priorFunc;}

void SQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {
    SQueue temp(*this);

    clear(); // clear all the nodes and change the private member variables to set the priorty

    m_heapType = heapType;
    m_priorFunc = priFn;

    while (temp.m_heap != nullptr) {
        insertPost(temp.getNextPost());
    }


}

void SQueue::dump() const
{
    if (m_size == 0) {
        cout << "Empty skew heap.\n" ;
    } else {
        dump(m_heap);
        cout << endl;
    }
}

void SQueue::dump(Node *pos) const {
    if ( pos != nullptr ) {
        cout << "(";
        dump(pos->m_left);
        cout << m_priorFunc(pos->m_post) << ":" << pos->m_post.getPostID();
        dump(pos->m_right);
        cout << ")";
    }
}

HEAPTYPE SQueue::getHeapType() const {return m_heapType;}

void SQueue::swap(Node ** h1, Node** h2) {
    Node * temp = *h1;
    *h1 = *h2;
    *h2 = temp;
}


ostream& operator<<(ostream& sout, const Post& post) {
    sout << "Post ID: " << post.getPostID()
         << ", number of likes: " << post.getNumLikes()
         << ", interest level: " << post.getInterestLevel()
         << ", connection level: " << post.getConnectLevel()
         << ", post time: " << post.getPostTime();
    return sout;
}

ostream& operator<<(ostream& sout, const Node& node) {
    sout << node.getPost();
    return sout;
}