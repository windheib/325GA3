#include <iostream>
#include <limits>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <iomanip>

using namespace std;

#define DISPLAY

struct edge{
  int start;
  int end;
  int weight;
  edge& operator=(const edge& a){
    start = a.start;
    end = a.end;
    weight = a.weight;
    return *this;
  }
  bool operator>(edge& a){
    return weight > a.weight;
  }
  bool operator<(edge& a){
    return weight < a.weight;
  }
};


class DisjointSet{
  struct Node{
    int data;
    int rank;
    Node *parent;
  };
  int size;
  Node *sets;
public:
  DisjointSet(){}
  DisjointSet(int n){
    size = 0;
    sets = new Node[n];
  }
  void make(int data){
    Node *n = new Node();
    n->data = data;
    n->rank = 0;
    n->parent = n;
    sets[size++] = *n;
  }

  bool join( int x, int y ){
    if (x > size || y > size){
      return false;
    }
    Node a = sets[x];
    Node b = sets[y];

    Node *p1 = findN(&a);
    Node *p2 = findN(&b);
    if (p1->parent == p2->parent){ //is cycle
      return false; //couldnt join
    }
    if(p1->rank >= p2->rank){
      p1->parent->rank = (p1->parent->rank == p2->parent->rank) ? p1->parent->rank+1 : p1->parent->rank;
      p2->parent->parent = p1->parent;
    } else {
      p1->parent->parent = p2->parent;
    }
    return true; //could join
  }/**/

  int find( int data ){
    Node *n = &sets[data];
    return findN(n)->data;
  }
  Node *findN(Node *n){
    Node *p = n->parent;
    if (n->parent == p->parent){
      return n;
    }
    n->parent = findN(n->parent);
    return n->parent;
  }/**/
  void print(){ //prints parent
    for (int i = 0; i < size; i++){
      cout << findN(&sets[i])->parent->data << " ";
    }
  }
  void free(){
    delete sets;
  }
};

//code for MinHeap adapted from geeksforgeeks
//http://www.geeksforgeeks.org/binary-heap/
class MinHeap{
  struct edge *arr;
  int MaxSize;
  int curSize;
public:
  MinHeap(int cap){
    MaxSize = cap;
    curSize = 0;
    arr = new edge[cap];
  }
  void MinHeapify(int i){
    int l = left(i);
    int r = right(i);
    int smallest = i;
    if (l < curSize && arr[l].weight < arr[i].weight)
        smallest = l;
    if (r < MaxSize && arr[r] < arr[smallest])
        smallest = r;
    if (smallest != i)
    {
        swap(&arr[i], &arr[smallest]);
        MinHeapify(smallest);
    }
  }
  int parent(int i) { return (i-1)/2; }
  int left(int i) { return (2*i + 1); }
  int right(int i) { return (2*i + 2); }
  edge extractMin(){
    if (curSize <= 0){
        edge e;
        e.start = NULL;
        e.end = NULL;
        e.weight = NULL;
        return e;
      }
    if (curSize == 1) {
        curSize--;
        return arr[0];
    }
    // Store the minimum value, and remove it from heap
    edge root = arr[0];
    arr[0] = arr[curSize-1];
    curSize--;
    MinHeapify(0);
    return root;
  }
  void decreaseKey(int i, edge new_val){
    arr[i] = new_val;
    while (i != 0 && arr[parent(i)] > arr[i]) {
       swap(&arr[i], &arr[parent(i)]);
       i = parent(i);
    }
  }
  edge getMin() { return arr[0]; }
  void deleteKey(int i){
    edge e;
    e.start = NULL;
    e.end = NULL;
    e.weight = NULL;
    decreaseKey(i, e);
    extractMin();
  }
  void insertKey(edge k){
    if (curSize == MaxSize) {
        cout << "\nOverflow: Could not insertKey\n";
        return;
    }
    // First insert the new key at the end
    curSize++;
    int i = curSize - 1;
    arr[i] = k;
    // Fix the min heap property if it is violated
    while (i != 0 && arr[parent(i)] > arr[i]) {
       swap(&arr[i], &arr[parent(i)]);
       i = parent(i);
    }
  }
  void swap(edge *x, edge *y){
    edge temp = *x;
    *x = *y;
    *y = temp;
  }
  void print(){
    cout << "Num Edges: " << curSize << "\n";
    int elems = 1;
    int count = 0;
    for (int i = 0; i < curSize; i++){
      cout << arr[i].weight;
      count++;
      if(count == elems){
        count = 0;
        elems *= 2;
        cout << "\n";
      } else if (count%2 == 0){
        cout << "|";
      } else {
        cout << ",";
      }
    }
  }
};


int main(int argc, char const *argv[]) {

  string iPath = "input.txt";
  ifstream ff(iPath);
  string line = "";
  ff >> line;
  int n = stoi(line);
  MinHeap edges = MinHeap(n*n);
  ff.seekg(line.length());
  string temp;

  //Reads directly to heap
  for (int i = 0; i < n; i++){
    ff>>temp;
    stringstream ss(temp);
    string garbage = "";
    for (int k = i; k > 0; k--){
      getline(ss, garbage, ',');
    }
    for (int j = i; j < n; j++){
      string tmp;
      getline(ss,tmp,',');
      int w = 0;
      if ((w=stoi(tmp)) > 0){
        edge e;
        e.start = i;
        e.end = j;
        e.weight = w;
        edges.insertKey(e);
      }
    }
  }/**/
  ff.close();

//  edges.print();
/******************************************************************************/
  int edgeIn = 0; //count of edges used in MST 1
  int edgeOut = 0; //count of edges not used in MST 1
  edge *used = new edge[n];
  edge *unused = new edge[n*n];
  int MST_Weight = 0;
/***********************************/
  DisjointSet sets = DisjointSet(n);
  for (int i = 0; i < n; i++){ //initialize DisjointSets
      sets.make(i);
  }
  /*******************************/
  while(edgeIn < n-1){
    edge e;
    e = edges.extractMin();
    if(sets.join(e.start,e.end)){
      MST_Weight += e.weight;
      used[edgeIn] = e;
      edgeIn++;
    } else {
      cout << "cycle\n";
      unused[edgeOut] = e;
      edgeOut++;
    }
  }/**/
  cout << MST_Weight;

/*
  cout << "\nedges used\n";
  for (int i = 0; i < edgeIn; i++){
    cout << "[" << used[i].start << ","<< used[i].end << ","<< used[i].weight << "]\n";
  }
  if(edgeOut == 0){
    edge e = edges.extractMin();
    unused[edgeOut++] = e;
  }
  cout << "\nedges checked but not used\n";
  for (int i = 0; i < edgeOut; i++){
    cout << "[" << unused[i].start << ","<< unused[i].end << ","<< unused[i].weight << "]\n";
  }/**/
/*
  int nmIndex = 0;
  edge *newMST = new edge[2];
  //newMST[nmIndex++] = unused[0];
  for (int i = 0; i < edgeIn; i++){
    if( used[i].start == unused[0].start ||  used[i].end == unused[0].end ){
      newMST[nmIndex++] = used[i];
    }
  }
  int lrg = 0;
  if (newMST[0].weight >= newMST[1].weight){
    lrg = newMST[0].weight;
  } else {
    lrg = newMST[1].weight;
  }
  cout << lrg << " " << unused[0].weight;
  MST_Weight = MST_Weight - lrg + unused[0].weight;
  cout << MST_Weight;
/**/
/******************************************************************************/

  string oPath = "output.txt";
  ofstream of(oPath);
  /*Write Data*/
  //of << to_string() << "\n";
  of.close();
  return 0;
}
