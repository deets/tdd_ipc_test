#ifndef IPC_SERVER_HH
#define IPC_SERVER_HH

#include <string>

using namespace std;

class Server {
public:

  Server(const string& name) : _name(name) {}

  const string& name() const { return _name; }

private:
  string _name;
};

#endif
