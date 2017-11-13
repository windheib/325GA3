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

struct node{
  int data;
  int rank;
  node *parent;
};

class DisjointSets{
  int size;
  node *sets;
public:
  DisjointSets(){}
  DisjointSets(int n){
    size = n;
    sets = new node[n];
  }

  void make(int i){
    //cout << i << " ";
    node n;
    n.data = i;
    n.rank = 0;
    n.parent = &n;
    sets[i] = n;
    //cout << sets[0].data << " ";
  }
  void print(){
    for (int i = 0; i < size; i ++){
      cout << sets[i].data << " ";
    }
  }

  void join( int x, int y ){
    cout << "Attempting join...\n";
    if (x > size || y > size){
      cout << "Outside of bounds\n";
      return;
    }
    node a = sets[x];
    node b = sets[y];

    node p1 = findN(a);
    cout << "found P1\n";
    node p2 = findN(b);
    cout << "found P2\n";

    if (p1.data == p2.data){
      cout << "Equal..\n";
      return;
    }
    if(p1.rank >= p2.rank){
      cout << "Greater\n";
      p1.rank = (p1.rank == p2.rank) ? p1.rank+1 : p1.rank;
      *p2.parent = p1;
      cout << "Assigning p2p\n";
    } else {
      cout << "Less\n";
      *p1.parent = p2;
    }/**/
  }

  int find( int v ){
    return findN(sets[v]).data;
  }
  private:
  node findN(node & n){
    cout << "Attempting Find...\n";
    node *p = n.parent;
    if (n.data == p->data){
          cout << "Equal...\n";
      return n;
    }
    //*n.parent = findN(*n.parent);
    return *n.parent;
  }/**/
};

//code for MinHeap gotten from geeksforgeeks
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
//  edge adjacencyMatrix[30][30];
  string iPath = "input.txt";
  ifstream ff(iPath);
  string line = "";
  ff >> line;
  int n = stoi(line);
  MinHeap edges = MinHeap(n*n);
  ff.seekg(line.length());
  string temp;
  /*/Reads to adjacencyMatrix
  for (int i = 0; i < n; i++){
    ff>>temp;
    stringstream ss(temp);
    for (int j = 0; j < n; j++){
      string tmp;
      getline(ss,tmp,',');
      edge e;
      e.start = i;
      e.end = j;
      e.weight = stoi(tmp);
      adjacencyMatrix[i][j] = e;
    }
  }/**/
//  readFile2D(&ff, line.length(), n, & adjacencyMatrix);
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

  int vertexCount = 0;
  int MST_Weight = 0;
  edge *used = new edge[n];
/***********************************/
  DisjointSets sets = DisjointSets(n);
  for (int i = 0; i < n; i++){
      sets.make(i);
  }
  sets.join(1,2);
  sets.print();

  /*******************************/
  while(vertexCount < n-1){
    edge e;
    e = edges.extractMin();
    bool loop = false;

  //  cout << e.start << "," << e.end << ",w: "<< e.weight<< "\n";
    used[vertexCount] = e;
    MST_Weight += e.weight;
    vertexCount++;
  }
//  cout << MST_Weight;
/******************************************************************************/


  #ifdef DISPLAY
  /*/Displays adjacencyMatrix
  for (int i = 0; i < n; i++){
    for (int j = 0; j < n; j++ ){
      cout <<setw(10) << setfill(' ')<< " [ " << adjacencyMatrix[i][j].start << "," << adjacencyMatrix[i][j].end << "," << adjacencyMatrix[i][j].weight << " ],";
    }
    cout << "\n";
  }/**/
  #endif DISPLAY

  string oPath = "output.txt";
  ofstream of(oPath);
  /*Write Data*/
  //of << to_string();
  of.close();
  return 0;
}
