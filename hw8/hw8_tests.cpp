#include <iostream>
#include <string>
#include <algorithm>
#include <gtest/gtest.h>
#include "bst_collection.h"

using namespace std;

// test 1
TEST(BasicCollectionTest, CorrectSize) {
  BSTCollection<string,double> c;
  ASSERT_EQ(0, c.size());
  c.insert("a", 10.0);
  ASSERT_EQ(1, c.size());
  c.insert("b", 20.0);
  ASSERT_EQ(2, c.size());
}

// test 2
TEST(BasicCollectionTest, InsertAndFind) {
  BSTCollection<string,double> c;
  double v;
  ASSERT_EQ(false, c.find("a", v));
  c.insert("a", 10.0);
  ASSERT_EQ(true, c.find("a", v));
  ASSERT_EQ(v, 10.0);
  ASSERT_EQ(false, c.find("b",v));
  c.insert("b", 20.0);
  ASSERT_EQ(true, c.find("b",v));
  ASSERT_EQ(20.0, v);
}

// test 3 -- should fail for hw8
TEST(BasicCollectionTest, RemoveElems) {
  BSTCollection<string,double> c;
  c.insert("a", 10.0);
  c.insert("b", 20.0);
  c.insert("c", 30.0);
  double v;
  //c.remove("a");
  //ASSERT_EQ(false, c.find("a", v));
  ASSERT_EQ(true, c.find("b", v));
  ASSERT_EQ(true, c.find("c", v));
  //c.remove("b");
  //ASSERT_EQ(false, c.find("b", v));
  //ASSERT_EQ(true, c.find("c", v));
  //c.remove("c");
  //ASSERT_EQ(false, c.find("c", v));
  //ASSERT_EQ(0, c.size());
}

// test 4
TEST(BasicCollectionTest, GetKeys) {
  BSTCollection<string,double> c;
  c.insert("a", 10.0);
  c.insert("b", 20.0);
  c.insert("c", 30.0);
  vector<string> ks;
  c.keys(ks);
  vector<string>::iterator iter;
  iter = find(ks.begin(), ks.end(), "a");
  ASSERT_NE(ks.end(), iter);
  iter = find(ks.begin(), ks.end(), "b");
  ASSERT_NE(ks.end(), iter);
  iter = find(ks.begin(), ks.end(), "c");
  ASSERT_NE(ks.end(), iter);
  iter = find(ks.begin(), ks.end(), "d");
  ASSERT_EQ(ks.end(), iter);
}

// test 5
TEST(BasicCollectionTest, GetKeyRange) {
  BSTCollection<string,double> c;
  c.insert("a", 10.0);
  c.insert("b", 20.0);
  c.insert("c", 30.0);
  c.insert("d", 40.0);
  c.insert("e", 50.0);
  vector<string> ks;
  c.find("b", "d", ks);
  vector<string>::iterator iter;
  iter = find(ks.begin(), ks.end(), "b");
  ASSERT_NE(ks.end(), iter);
  iter = find(ks.begin(), ks.end(), "c");
  ASSERT_NE(ks.end(), iter);
  iter = find(ks.begin(), ks.end(), "d");
  ASSERT_NE(ks.end(), iter);
  iter = find(ks.begin(), ks.end(), "a");
  ASSERT_EQ(ks.end(), iter);
  iter = find(ks.begin(), ks.end(), "e");
  ASSERT_EQ(ks.end(), iter);
}

// test 6
TEST(BasicCollectionTest, KeySort) {
  BSTCollection<string,double> c;
  c.insert("a", 10.0);
  c.insert("b", 20.0);
  c.insert("c", 30.0);
  c.insert("d", 40.0);
  c.insert("e", 50.0);
  vector<string> sorted_ks;
  c.sort(sorted_ks);
  ASSERT_EQ(c.size(), sorted_ks.size());
  for (int i = 0; i < int(sorted_ks.size()) - 1; ++i) {
    ASSERT_LE(sorted_ks[i], sorted_ks[i+1]);
  }
}

// test 7
TEST(BasicCollectionTest, AssignOpTest) {
  BSTCollection<string,int> c1;
  c1.insert("c", 10);
  c1.insert("b", 15);
  c1.insert("d", 20);
  c1.insert("a", 20);
  BSTCollection<string, int> c2;
  c2 = c1;
  ASSERT_EQ(c1.size(), c2.size());
  ASSERT_EQ(c1.height(), c2.height());
}

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
