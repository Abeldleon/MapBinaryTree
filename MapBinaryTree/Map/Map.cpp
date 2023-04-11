#include "Map.h"
#include <iostream>

////////////////////MAP FUNCTIONS////////////////////////

Map::Map()
{
    for (size_t i = 0; i < TABLESIZE; i++)
    {
        table[i] = nullptr;
    }
}
Map::Map(const Map& other)
{
    for (size_t i = 0; i < TABLESIZE; i++)
    {
        table[i] = nullptr;
    }

    KeyType key;
    ValueType value;

    for (int i = 0; i < other.size(); i++)
    {
        other.get(i, key, value);
        insert(key, value);
    }
}
Map& Map::operator=(const Map& other)
{
    if (this != &other) {
        KeyType key;
        ValueType value;

        for (int i = 0; i < TABLESIZE; i++)
        {
            deleteBinaryTree(table[i]);
        }
        for (int i = 0; i < other.size(); i++)
        {
            other.get(i, key, value);
            insert(key, value);
        }
    }
    return *this;
}
Map::~Map()
{
    //KeyType key;
    //ValueType value;
    for (int i = 0; i < TABLESIZE; i++)
    {
        deleteBinaryTree(table[i]);
    }
}
bool Map::empty() const
{
    bool empty = true;
    for (int i = 0; i < TABLESIZE; i++)
    {
        if (table[i])
        {
            empty = false;
            break;
        }
    }
    return empty;
}
bool Map::erase(const KeyType& key)
{
    return (binaryTreeErase(table[hashFunction(key)], key));
}
bool Map::contains(const KeyType& key) const
{
    return binaryTreeContains(table[hashFunction(key)], key);
}

bool Map::get(const KeyType& key, ValueType& value) const
{
    return binaryTreeGet(table[hashFunction(key)], key, value);
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
    int count = 0;
    for (int k = 0; k < TABLESIZE; k++)
    {
        if (table[k])
        {            
            if (binaryTreeGetWithIndex(table[k], i, count, key, value))
                return true;
        }
    }

    return false;
}

int Map::size() const
{
    int size = 0;
    for (int i = 0; i < TABLESIZE; i++)
    {
        size += numNodes(table[i]);
    }
    return size;

}

bool Map::insert(const KeyType& key, const ValueType& value)
{
    if (contains(key))
    {
        return false;
    }
    return binaryTreeInsert(table[hashFunction(key)], key, value);
    
}

bool Map::update(const KeyType& key, const ValueType& value)
{
    return binaryTreeUpdate(table[hashFunction(key)], key, value);
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
    if(!update(key, value)){
        insert(key, value);
    }
    return true;
}

void Map::swap(Map& other)
{
    Node* temp[TABLESIZE];
    for (int i = 0; i < TABLESIZE; i++)
    {
        temp[i] = table[i];
    }
    for (int i = 0; i < TABLESIZE; i++)
    {
        table[i] = other.table[i];
    }
    for (int i = 0; i < TABLESIZE; i++)
    {
        other.table[i] = temp[i];
    }

}

void Map::dump() const
{
    for (int i = 0; i < size(); i++)
    {
        KeyType key;
        ValueType value;
        get(i, key, value);
        std::cerr << key << " " << value << std::endl;
    }
}
////////////////////BINARY TREE FUNCTIONS////////////////////////
Map::Node::Node(KeyType k2, ValueType v2, Node* l, Node* r)
{
    k = k2;
    v = v2;
    left = l;
    right = r;
}

bool Map::binaryTreeInsert(Node*& node, const KeyType& key, const ValueType& value)
{

    if (node == nullptr)
    {
        Node* newNode = new Node(key,value,nullptr,nullptr);
        node = newNode;
    }
    else
    {
        if (key < node->k)
        {
            binaryTreeInsert(node->left, key, value);
        }
        else if (key > node->k)
        {
            binaryTreeInsert(node->right, key, value);
        }
    }
    return true;
}

bool Map::binaryTreeUpdate(Node*& node, const KeyType& key, const ValueType& value)
{
    if(node == nullptr)
        return false;

    if(node->k == key)
    {
        node->v = value;
        return true;
    }

    if(binaryTreeUpdate(node->left, key, value) || binaryTreeUpdate(node->right, key, value))
        return true;

    return false;
}

bool Map::binaryTreeErase(Node*& node, const KeyType& key)
{
    if (node == nullptr) 
        return false;
    if (node->k == key)
    {
        if (node->left != nullptr && node->right != nullptr)
        {
            Node* temp = node->right;
            while (temp->left != nullptr)
            {
                temp = temp->left;
            }
            node->k = temp->k;
            node->v = temp->v;
            delete temp;
            temp = nullptr;
            return true;
        }
        else if (node->right == nullptr && node->left == nullptr)
        {
            delete node;
            node = nullptr;
            return true;
        }
        else if(node->left == nullptr)
        {
            Node* temp = node->right;
            delete node;
            node = temp;
            temp = nullptr;
            return true;
        }
        else if (node->right == nullptr)
        {
            Node* temp = node->left;
            delete node;
            node = temp;
            temp = nullptr;
            return true;
        } 
    }
    return (key > node->k) ? binaryTreeErase(node->right, key) : binaryTreeErase(node->left, key);
}

bool Map::binaryTreeContains(Node* node, const KeyType& key) const
{
    if (node == nullptr)
    {
        return false;
    }
    if (key == node->k)
    {
        return true;
    }
    return (key > node->k)? binaryTreeContains(node->right, key): binaryTreeContains(node->left, key);
}

bool Map::binaryTreeGet(Node* node, const KeyType& key, ValueType& value) const
{
    if(node == nullptr)
        return false;

    if(node->k == key)
    {
        value = node->v;
        return true;
    }

    if(binaryTreeGet(node->left, key, value) || binaryTreeGet(node->right, key, value))
        return true;

    return false;
}

bool Map::binaryTreeGetWithIndex(Node* node, int i, int& count, KeyType& key, ValueType& value) const
{
    // node: root of the binary tree / current node;
    // i: the i'th elementh that we are looking for in the tree
    // count: keeps track of existing traversed nodes
    // key: the key we return 
    // value: the value we return
    // Returns: true if found, false not found

    if (node == nullptr)    //if the current node is empty 
        return false;      
    
    if (count == i)
    {
        key = node->k;
        value = node->v;
        return true;
    }
    count++;
    return (binaryTreeGetWithIndex(node->left, i, count, key, value)) ? true : binaryTreeGetWithIndex(node->right, i, count, key, value);
}

void Map::deleteBinaryTree(Node*& node)
{
    if (node == nullptr)
        return;
    
    deleteBinaryTree(node->left);
    deleteBinaryTree(node->right);
    delete node;
    node = nullptr;
}

int Map::numNodes(Node* node) const
{
    if (node == nullptr)
        return 0;
    int count1 = numNodes(node->left);
    int count2 = numNodes(node->right);
   
    return 1 + count1 + count2;
}

bool combine(const Map& m1, const Map& m2, Map& result) {

    KeyType key;
    ValueType value;
    ValueType tempValue;
    bool combined = false;
    while(result.size())
    {
        result.get(0, key, tempValue);
        result.erase(key);
    }
    for (int i = 0; i <  m1.size(); i++)
    {
        m1.get(i, key, value);
        if (!m2.contains(key))
        {   
            result.insert(key, value);
            combined = true;
        }
        else
        {
            m2.get(key, tempValue);

            if(value == tempValue)
            {
                result.insert(key, value);
                combined = true;
            }
            else
            {
                combined = false;
            }
        }
    }
    for (int i = 0; i < m2.size(); i++)
    {
        m2.get(i, key, value);  
        if (!m1.contains(key))        
            result.insert(key, value);
    }

    return combined;
}

void subtract(const Map& m1, const Map& m2, Map& result)
{
    KeyType key;
    ValueType value;
    ValueType tempValue;
    
    while(result.size())
    {
        result.get(0, key, tempValue);
        result.erase(key);
    }
    for (int i = 0; i < m1.size(); i++)
    {
        m1.get(i, key, value);
        if (!m2.contains(key))
        { 
            result.insert(key, value);
        }
    }
}

/////////////////////HASH//////////////////////////
int Convert_Key(std::string key)
{
    int hash = 0;

    for (auto& c : key)
    {
        hash = ((hash * 31) + (int)c)%100000;
    }
    return hash;

}

int Convert_Key(double key)
{
    double hash = 0;
    hash= (key * 31);
    
    return hash;
}

int Map::hashFunction(const KeyType& key) const
{
    return Convert_Key(key) % TABLESIZE;

}

