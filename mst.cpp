#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

struct node{

};

int int main(int argc, char const *argv[]) {
  string iPath = "input.txt";
  ifstream iFile(iPath);
  /*Read Data*/
  iFile.close();
  
  /*Execute Data*/

  oPath = "output.txt";
  ofstream of(oPath);
  /*Write Data*/
  //of << to_string();
  of.close();
  return 0;
}
