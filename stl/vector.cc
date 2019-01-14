
#include <iostream>
#include <vector>
#include <string>
#include <cstring>

using std::string;
using std::vector;
using std::cout;
using std::endl;

const int NAME = 5;
int main() {
  vector<string> vecName;
  const char* pName = "emv_config.xml";
  const char* arrName[NAME] = {"castlspay.xml", 
                               "castlespaycon.xml", 
                               "castlespaycloud.xml",
                               "emvcl_config.xml",
                               "emv_config.xml"};
//  vecName.push_back(pName);
  for (int i = 0; i < NAME; ++i) {
    vecName.push_back(arrName[i]);
  }

  for (int i = 0; i < NAME; ++i) {
    if (!memcmp(vecName[i].c_str(), pName, strlen(pName)))
      cout << vecName[i] << endl;
  }

  return 0;

}