//Carter Mooring
//CPSC 223
//Dr. Bowers
//hash_table_collection.h
//This file helps run the base program and everything I implemented

#ifndef HASH_TABLE_COLLECTION_H
#define HASH_TABLE_COLLECTION_H

#include <vector>
#include <algorithm>
#include <functional>
#include "collection.h"

using namespace std;

template <typename K, typename V>
class HashTableCollection : public Collection <K,V>{
  public:

    // create an empty linked
    HashTableCollection();

    // copy a linked list
    HashTableCollection(const HashTableCollection <K,V>& rhs);

    // assign a linked list
   HashTableCollection<K,V>& operator=(const HashTableCollection <K,V>& rhs);

   // delete a linked list
   ~HashTableCollection();

   // insert a key-value pair into the collection
   void insert(const K& key, const V& val);

   // remove a key-value pair from the collection
   void remove(const K& key);

   // find the value associated with the key
   bool find(const K& key, V& val) const;

   // find the keys associated with the range
   void find(const K& k1, const K& k2, std::vector<K>& keys) const;

   // return all keys in the collection
   void keys(std::vector <K>& keys) const;

   // return collection keys in sorted order
   void sort(std::vector <K>& keys) const;

   // return the number of keys in collection
   int size() const;

   private:
     // helper to empty entire hash table
     void make_empty();

     // resize and rehash the hash table
     void resize_and_rehash();

     // linked list node structure
     struct Node {
       K key;
       V value;
       Node* next;
     };

     // number of k-v pairs in the collection
     int collection_size;

     // number of hash table buckets (default is 16)
     int table_capacity;

     // hash table array load factor (set at 75% for resizing)
     const double load_factor_threshold;

     // hash table array
     Node** hash_table;
   };

// create an empty linked
template <typename K, typename V>
HashTableCollection <K,V>::HashTableCollection() : collection_size(0), table_capacity(16), load_factor_threshold(0.75) {
  // dynamically allocate the hash table array
   hash_table = new Node*[table_capacity];

   // initialize the hash table chains
   for (int i = 0; i < table_capacity; ++i)
     hash_table[i] = nullptr;
}

// copy a linked list
template <typename K, typename V>
HashTableCollection<K,V>::HashTableCollection(const HashTableCollection<K,V>& rhs) : hash_table(nullptr){
  *this = rhs;
}

// assign a linked list
template <typename K, typename V>
HashTableCollection <K,V>& HashTableCollection <K,V>::operator=(const HashTableCollection<K,V>& rhs){
  // check if rhs is current object and return current object
  if (this == &rhs)
    return *this;

  // delete current object
  make_empty ();
  // initialize current object
  collection_size = 0;
  load_factor_threshold = 0.75;
  // create the hash table
  hash_table = new Node*[rhs.table_capacity];
  // do the copy
  Node* ptr;
  for(int i = 0; i < rhs.table_capacity; i++){
    ptr = rhs.hash_table[i];
    while(ptr != nullptr){
      insert(ptr->key, ptr->value);
      ptr = ptr->next;
    }
  }
  return *this;
}

// delete a linked list
template <typename K, typename V>
HashTableCollection <K,V>::~HashTableCollection(){
  make_empty();
}

// insert a key-value pair into the collection
template <typename K, typename V>
void HashTableCollection<K,V>::insert(const K& key, const V& val){
  // check current load factor versus load factor threshold,
  std::hash<K> hash_fun; // K- based hash function object
  size_t value = hash_fun(key); // get int - based value for key // hash the key
  size_t index = value % table_capacity; // calculate the index

  Node* ptr = new Node;
  ptr->key = key;
  ptr->value = val;

  // and resize and copy if necessary by calling resize_and_rehash()
  if(double(collection_size / table_capacity) > load_factor_threshold){
    resize_and_rehash();
  }

  if(hash_table[index] == nullptr){ //is that bucket empty?
    ptr->next = nullptr;  //yes, so this will be the first node, set next to null since nothing after
    hash_table[index] = ptr;  //set the bucket pointer to the head of new node
  } else if (hash_table[index] != nullptr){ //this bucket has existing nodes
    ptr->next = hash_table[index];  //point new node to the original first node in bucket
    hash_table[index] = ptr; //bucket now points to head of new node
  }

  collection_size++; // update the size
}

// remove a key-value pair from the collection
template <typename K, typename V>
void HashTableCollection<K,V>::remove(const K& key){
  std::hash<K> hash_fun; // K- based hash function object
  size_t value = hash_fun(key); // get int - based value for key // hash the key
  size_t index = value % table_capacity; // calculate the index
  Node* ptr;
  Node* temp;

  if(hash_table != nullptr){  //make sure hash table has values
    ptr = hash_table[index];  //point ptr to first node in bucket
    if(hash_table[index] != nullptr){
      if(ptr->key == key){    //if the first node has the key
        if(ptr->next == nullptr){ //if there is only one node
          delete ptr;
          hash_table[index] = nullptr;
          collection_size--;
        } else {            //else there are other nodes so need to point different
          hash_table[index] = ptr->next;
          ptr->next = nullptr;
          delete ptr;
          collection_size--;
        }
      }else{ //else the key is not in the first node of the bucket
        temp = ptr;     //require temp node to hold previous value
        if(ptr != nullptr){
          ptr = ptr -> next;
          while(ptr != nullptr){  //while not at the last node of the bucket
            if(ptr->key == key){  //if current node (not first) has the key
              if(ptr->next = nullptr){  //if it is the last node in bucket
                temp->next = nullptr;
                delete ptr;
                collection_size--;
              } else{     //else its a node between two other nodes at least
                temp->next = ptr->next;
                ptr->next = nullptr;
                delete ptr;
                collection_size--;
              }
            }
            temp = ptr;
            ptr = ptr->next;
          }
        }
      }
    }
  }
}

// find the value associated with the key
template <typename K, typename V>
bool HashTableCollection<K,V>::find(const K& key, V& val) const{
  Node* temp;
  std::hash<K> hash_fun; // K- based hash function object
  size_t value = hash_fun(key); // get int - based value for key
  size_t index = value % table_capacity; // calculate the index

  if(hash_table != nullptr){
    if(hash_table[index] == nullptr){
      return false;
    } else {
      temp = hash_table[index];
      while(temp != nullptr){
        if(temp->key == key){
          val = temp -> value;
          return true;
        } else{
          temp = temp->next;
        }
      }
    }
  }
  return false;
}

// find the keys associated with the range
template <typename K, typename V>
void HashTableCollection<K,V>::find(const K& k1, const K& k2, std::vector<K>& keys) const{
  Node* ptr;
    for(int i = 0; i < table_capacity; i++){
      ptr = hash_table[i];
      while(ptr != nullptr){
        if(ptr->key >= k1 && ptr->key <= k2){
          keys.push_back(ptr->key);
        }
        ptr = ptr->next;
      }
    }
}

// return all keys in the collection
template <typename K, typename V>
void HashTableCollection<K,V>::keys(std::vector <K>& keys) const{
  Node* ptr;
    for(int i = 0; i < table_capacity; i++){
      ptr = hash_table[i];
      while(ptr != nullptr){
    		keys.push_back(ptr->key);
    		ptr = ptr->next;
      }
    }
}

// return collection keys in sorted order
template<typename K,typename V>
void HashTableCollection<K,V>::sort(std::vector<K>& ks) const{
  keys(ks);
  std::sort(ks.begin(), ks.end());
}

// return the number of keys in collection
template <typename K, typename V>
int HashTableCollection <K,V>::size() const{
  return collection_size;
}

// helper to empty entire hash table
template <typename K, typename V>
void HashTableCollection <K,V>::make_empty(){
  Node* ptr; //acting head pointer
  Node* next; //points to next

  if(hash_table != nullptr){ // make sure hash table exists
    for(int i = 0; i < collection_size; i++){   //remove each key until end of hash table
      ptr = hash_table[i]; //point to head of first spot in hash_table
      while(hash_table[i] != nullptr){  //iterate through entire linked list in spot of hash table_capacity
        next = ptr->next; //set next pointer to the next node (one after first node)
        delete ptr; //since ptr is pointing to head and we have next spot, delete first Node
        ptr = next; //set ptr pointer to the next spot which will now be the new first spot
        hash_table[i] = ptr; //set first spot in hash_table equal to the new head node
      }
    }
  }
 delete hash_table; // remove the hash table
}

template <typename K, typename V>
void HashTableCollection<K,V>::resize_and_rehash(){
  // setup new table
  int new_capacity = table_capacity * 2;
  // ... similarly with collection size ...
  // dynamically allocate the new table
  Node** new_table = new Node*[new_capacity];
  // initialize new table
  for(int i = 0; i < new_capacity; i++){
    new_table[i] = nullptr;
  }
  // insert key values
  std::vector <K> ks;
  keys(ks);
  size_t index;
  V inVal;

  for(K key:ks){
    // hash the key
    std::hash<K> hash_fun; // K- based hash function object
    size_t value = hash_fun(key); // get int - based value for key
    size_t index = value % table_capacity; // calculate the index
    // create a new node in new table
    Node* newNode = new Node;
    bool temp = find(key, inVal);
    newNode->key = key;
    newNode->value = inVal;
    if(new_table[index] == nullptr){
      hash_table[index] = newNode;
      newNode->next = nullptr;
    } else {
      newNode->next = new_table[index];
      new_table[index] = newNode;
    }
  }
  // clear the current data
  make_empty();
  // update to the new settings
  hash_table = new_table;
  // ... update remaining vars ...
  table_capacity = new_capacity;
}

#endif
