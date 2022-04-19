# Priority-Skew-Heaps
Prioritizing Social Media Posts 


# Introduction
I implemented a priority queue ADT in the form of a skew heap data structure for organizing the posts based on priorities. The application automatically prioritizes the posts. The prioritization is based on some specific criteria. The algorithm that uses the criteria to calculate the priorities can change. Therefore, the application should have the possibility of changing the post priorities. An algorithm that can determine the priority will be implemented in a function. Such an architecture a

# Expiereince I gained 
- Practice constructing and using heap data structure as a priority queue ADT
- Practice writing merge operation in skew heap data structure
- Gain additional experience constructing and using binary trees
- Practice using recursion in programs
- Learn to use function pointers


# Specifications
There are three classes in this project. The class SQueue has a memeber variable of type Node, and the class Node has a member variable of type Post.


# Class SQueue

The class SQueue constructs a min-skew-heap or a max-skew-heap data structure. This class has a member variable called m_heap. The member variable m_heap presents the root node of the heap data structure and it is of the type Node. The following table presents the list of member functions that need implementation.

# Class Post

- m_postID stores a unique ID for every post.

- m_likes stores the number likes for the post.

- m_connectLevel stores a number presenting the distance between the poster and the user who will see the post. The value 1 indicates that the poster and the viewer arr directly connected. The value 2 indicates that there is third person between the poster and the viewer. There can be up to 5 connection levels.

- m_postTime stores the time that a message was posted. This variable can take a value between 1 and 50. It indicates the placement of post. Using this value we can see what message was posted earlier comparing to other messages. For example, the value 50 indicates that the message was the last one posted.

- m_interestLevel stores the level that a post may be in the interest of the viewer. The value of 1 presents the least and the value of 10 presents the most.

# Class Node
This class constructs a node in the heap data structure. It has a member variable of the type Post, i.e. m_post. The member varaible is initialized throught the Node alternative constructor. The class SQueue is a friend of Node class, it means it has access to private members of Node class. You are not allowed to modify this class.



