#include <string>
using KeyType = std::string; //string
using ValueType = double; //double

class Map
{
public:
    Map();              // Create an empty map (i.e., one with no key/value pairs)
    Map(const Map & m);
    Map & operator=(const Map& other);
    ~Map();
    bool empty() const; // Return true if the map is empty, otherwise false.

    int size() const;   // Return the number of key/value pairs in the map.

    bool insert(const KeyType& key, const ValueType& value);
    // If key is not equal to any key currently in the map, and if the
    // key/value pair can be added to the map, then do so and return true.
    // Otherwise, make no change to the map and return false (indicating
    // that either the key is already in the map).

    bool update(const KeyType& key, const ValueType& value);
    // If key is equal to a key currently in the map, then make that key no
    // longer map to the value it currently maps to, but instead map to
    // the value of the second parameter; return true in this case.
    // Otherwise, make no change to the map and return false.

    bool insertOrUpdate(const KeyType& key, const ValueType& value);
    // If key is equal to a key currently in the map, then make that key no
    // longer map to the value it currently maps to, but instead map to
    // the value of the second parameter; return true in this case.
    // If key is not equal to any key currently in the map then add it and 
    // return true. In fact this function always returns true.

    bool erase(const KeyType& key);
    // If key is equal to a key currently in the map, remove the key/value
    // pair with that key from the map and return true.  Otherwise, make
    // no change to the map and return false.

    bool contains(const KeyType& key) const;
    // Return true if key is equal to a key currently in the map, otherwise
    // false.

    bool get(const KeyType& key, ValueType& value) const;
    // If key is equal to a key currently in the map, set value to the
    // value in the map that that key maps to, and return true.  Otherwise,
    // make no change to the value parameter of this function and return
    // false.

    bool get(int i, KeyType& key, ValueType& value) const;
    // If 0 <= i < size(), copy into the key and value parameters the
    // key and value of one of the key/value pairs in the map and return
    // true.  Otherwise, leave the key and value parameters unchanged and
    // return false.  (See below for details about this function.)

    void swap(Map& other);
    // Exchange the contents of this map with the other one.

    //Dump
    void dump() const;

private:
    struct Node
    {
        KeyType k;
        ValueType v;
        Node* left;
        Node* right;
        Node(KeyType k2, ValueType v2, Node* l, Node* r);
    };

 

    

    bool binaryTreeInsert(Node*& node, const KeyType& key, const ValueType& value);

    bool binaryTreeUpdate(Node*& node, const KeyType& key, const ValueType& value);

    bool binaryTreeErase(Node*& node, const KeyType& key);

    bool binaryTreeContains(Node* node, const KeyType& key) const;

    bool binaryTreeGet(Node* node, const KeyType& key, ValueType& value) const;

    bool binaryTreeGetWithIndex(Node* node, int i, int& count, KeyType& key, ValueType& value) const;

    void deleteBinaryTree(Node*& node);

    int numNodes(Node* node) const;

    /////////////////HASH///////////////////
    const static int TABLESIZE = 7;
    Node* table[TABLESIZE];
    //Node** table = new Node* [TABLESIZE];
    int hashFunction(const KeyType& key) const;

};


bool combine(const Map& m1, const Map& m2, Map& result);
void subtract(const Map& m1, const Map& m2, Map& result);