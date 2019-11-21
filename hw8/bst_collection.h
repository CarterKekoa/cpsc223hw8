#ifndef BST_COLLECTION_H
#define BST_COLLECTION_H
#include <vector>
#include "collection.h"

using namespace std;

template<typename K, typename V>
class BSTCollection : public Collection<K, V>{
public:

  // create an empty BST
  BSTCollection();

  // copy a BST
  BSTCollection(const BSTCollection<K, V>&);

  // assigne a BST
  BSTCollection<K, V>& operator=(const BSTCollection<K,V>& rhs);

  // delete a BST
  ~BSTCollection();

  // insert a KV-pair from the Collection
  void insert(const K& key, const V& val);

  // remove a KV-pair from the Collection
  void remove(const K& key);

  // find the value associate with the key
  bool find(const K& key, V& val) const;

  // find the keys associated with the range
  void find(const K& k1, const K& k2, std::vector<K>& keys) const;

  // return all keys in the collection;
  void keys(std::vector<K>& keys) const;

  // return collection keys in sorted order
  void sort(std::vector<K>& keys) const;

  // return the number of keys in collection
  int size() const;

  // return the height of the tree
  int height() const;

private:

  // binary search tree node structures
  struct Node {
    K key;
    V value;
    Node* left;
    Node* right;
  };

  // root node of the search tree
  Node* root;

  // number of k-v pairs in the collection
  int collection_size;

  // helper to recursively empty search tree
  void make_empty(Node* subtree_root);

  // helper to recursively build sorted list of keys
  void inorder(const Node* subtree, std::vector<K>& keys) const;

  // helper to recursively build sorted list of keys
  void preorder(const Node* subtree, std::vector<K>& keys) const;

  // helper to recursively find range of keys
  void range_search(const Node* subtree, const K& k1,
                    const K& k2, std::vector<K>& keys) const;

  // return the height of the tree rooted at subtree_root
  int height(const Node* subtree_root) const;
};

template<typename K, typename V>
BSTCollection<K,V>::BSTCollection() : collection_size(0), root(nullptr){}

template<typename K, typename V>
BSTCollection<K,V>::BSTCollection(const BSTCollection<K,V>& rhs)
  : collection_size(0), root(nullptr)
{
  *this = rhs;
}

template<typename K, typename V>
BSTCollection<K,V>& BSTCollection<K,V>::operator=(const BSTCollection<K,V>& rhs){
  if (this == &rhs) {
    return *this;
  }
  // delete current
  make_empty(root);
  // build tree
  std::vector<K> ks;
  preorder(rhs.root, ks);
  int val = 0;
  for(int i = 0; i < rhs.collection_size; i++){
    rhs.find(ks[i], val);
    insert(ks[i], val);
  }
  return *this;
}

template<typename K, typename V>
BSTCollection<K,V>::~BSTCollection()
{
  make_empty(root);
}

template<typename K, typename V>
void BSTCollection<K,V>::insert(const K& key, const V& val)
{
  Node* tree = new Node;
  tree->key = key;
  tree->value = val;
  tree->left = nullptr;
  tree->right = nullptr;

  if(root == nullptr){
    root = tree;
    tree->left = nullptr;
    tree->right = nullptr;
  }else{
    Node* cur = new Node;
    cur = root;
    while (cur != nullptr)
     if(tree->key < cur->key){
        if(cur->left == nullptr){
          cur->left = tree;
          cur = nullptr;
        }else{
          cur = cur->left;
        }
     }else{
        if(cur->right == nullptr){
          cur->right = tree;
          cur = nullptr;
        }else{
          cur = cur->right;
          tree->left = nullptr;
          tree->right = nullptr;
      }
    }
 }
 collection_size++;
}

template<typename K, typename V>
void BSTCollection<K,V>::remove(const K& key)
{
  // leave empty for HW9
}

template<typename K, typename V>
bool BSTCollection<K,V>::find(const K& key, V& val) const{
  Node* tree = new Node;
  Node* cur = root;
  tree->key = key;
  tree->value = val;
  tree->left = nullptr;
  tree->right = nullptr;
  bool found = false;
  int size = collection_size;

  while(!found && size !=0){
    if(tree->key == cur->key){
      val = cur->value;
      return true;
    }else if(tree->key > cur->key){
      cur = cur->right;
    }else if(tree->key < cur->key){
      cur = cur->left;
    }
    size--;
  }
  return false;
}

template<typename K, typename V>
void BSTCollection<K,V>::find(const K& k1, const K& k2,std::vector<K>& ks) const{
  // defer to the range search (recursive) helper function
  range_search(root, k1, k2, ks);
}

template<typename K, typename V>
void BSTCollection<K,V>::range_search(const Node* subtree, const K& k1, const K& k2, std::vector<K>& ks)
const {
  // use as recursive helper function
  if(subtree == nullptr){
    return;
  }
  range_search(subtree->left, k1, k2, ks);

  if(k1 <= subtree->key && k2 >= subtree->key){
    ks.push_back(subtree->key);
  }
  range_search(subtree->right, k1, k2, ks);
}

template<typename K, typename V>
void BSTCollection<K,V>::keys(std::vector<K>& ks) const
{
  // defer to the inorder (recursive) helper function
  inorder(root, ks);
}

template<typename K, typename V>
void BSTCollection<K,V>::sort(std::vector<K>& ks) const
{
  // defer to the inorder (recursive) helper function
  inorder(root, ks);
}

template<typename K, typename V>
int BSTCollection<K,V>::size() const
{
  return collection_size;
}

template<typename K, typename V>
int BSTCollection<K,V>::height(const Node* subtree_root) const{
  // recursive helper
  if(subtree_root == nullptr){
    return 0;
  }

  int left = height(subtree_root->left);
  int right = height(subtree_root->right);

  if(left > right){
    return(left+1);
  }else{
    return(right+1);
  }
}

template<typename K, typename V>
void BSTCollection<K,V>::make_empty(Node* subtree_root) {
  if(subtree_root == nullptr){
    return;
  }

  make_empty(subtree_root->left);
  make_empty(subtree_root->right);
  subtree_root == nullptr;

}

template<typename K, typename V>
void BSTCollection<K,V>::inorder(const Node* subtree, std::vector<K>& ks) const
{
  // recursive helper function
  if(subtree == nullptr){
    return;
  }
  inorder(subtree->left, ks);
  ks.push_back(subtree->key);
  inorder(subtree->right, ks);
}

template<typename K, typename V>
void BSTCollection<K,V>::preorder(const Node* subtree, std::vector<K>& ks) const
{
  // recursive helper function
  if(subtree == nullptr){
    return;
  }
  ks.push_back(subtree->key);
  inorder(subtree->left, ks);
  inorder(subtree->right, ks);
}

template<typename K, typename V>
int BSTCollection<K,V>::height() const
{
  // defer to the height (recursive) helper function
  return height(root);
}

#endif
