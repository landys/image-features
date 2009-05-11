#include <string>
#include <fstream>
#include "basefeature.h"

using namespace std;

bool BaseFeature::load(const ::std::string &filename) {
  /*//DBG(30) << "Loading from filename '" << filename << "'." << endl;
  igzstream is; // if igzstream is constructed with the file to be
                // opened, the state is wrong! Thus we have
                // construction and opening in two different lines
  is.open(filename.c_str());
  if(!is.good()) {
    ::std::cerr << "Canot open '" << filename << " for reading." << endl;
    return false;
  } else {
    if(not this->read(is)) {
      ::std::cerr << "Problem when reading '" << filename << "'." << endl;
      return false;
    }
  }
  //DBG(40) << "Loaded from filename '" << filename << "'." << endl;
  is.close();*/
  return true;
}

void BaseFeature::save(const ::std::string &filename) {
  //DBG(30) << "Writing to '" << filename << "'." << endl;
  ofstream os; // if ogzstream is constructed with the file to be
                // opened, the state is wrong! Thus we have
                // construction and opening in two different lines
  os.open(filename.c_str());
  if(!os.good()) {
    ::std::cerr << "Cannot open '" << filename << "' for writing." << endl;
  } else {
    this->write(os);
  }
  //DBG(40) << "Saved to '" << filename << "'." << endl;
}
