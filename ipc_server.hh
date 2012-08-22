#ifndef IPC_SERVER_HH
#define IPC_SERVER_HH

#include <boost/interprocess/ipc/message_queue.hpp>

#include <string>

using namespace std;
using namespace boost::interprocess;


template <class QueueClass>
class Server {
public:

  Server(const char *name)
  : _name(name)
  , _mq(create_only, name, 5, 100)
  {
  }

  const string& name() const { return _name; }


  void send(const string& message) {
    _mq.send(message.data(), message.size(), 0);
  }

private:
  string _name;

  QueueClass _mq;

};

#endif
