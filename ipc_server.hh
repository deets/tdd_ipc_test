#ifndef IPC_SERVER_HH
#define IPC_SERVER_HH

#include <algorithm>
#include <boost/interprocess/ipc/message_queue.hpp>

#include <string>

using namespace std;
using namespace boost::interprocess;


template <class QueueClass>
class Server {
public:

  Server(const char *name, int capacity, int message_size)
  : _name(name)
  , _mq(create_only, name, capacity, message_size)
  {
  }

  const string& name() const { return _name; }


  void send(const string& message) {
    int offset = 0;
    int length = message.size();
    const char *data = message.data();
    const int max_message_size = _mq.get_max_msg_size();
    while(length) {
      int send_size = min(max_message_size, length);
      _mq.send(data, send_size, 0);
      length -= send_size;
      data += send_size;
    }
  }

private:
  string _name;

  QueueClass _mq;

};

#endif
