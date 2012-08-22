#include <string>
#include "gtest/gtest.h"
#include <map>
#include <list>

#include <string.h>

#include <boost/interprocess/ipc/message_queue.hpp>

#include "ipc_server.hh"

using namespace std;


// this is just to ensure that the server compiles
// with a real boost queue implementation.

void real_queue_instantiation() {
  Server<message_queue> RealServer("foobar", 5, 100);
  RealServer.send("foobar");
}


struct Message {

  int priority;
  int length;
  char *data;

  Message(const char* data, int length, int priority)
    : priority(priority)
    , length(length)
  {
    this->data = (char*)malloc(length);
    memcpy(this->data, data, length);
  }


  Message(const Message& other) {
    length = other.length;
    priority = other.priority;
    data = (char*)malloc(length);
    memcpy(data, other.data, length);
  }

  virtual ~Message() {
    free(data);
  }

};

class MockQueueData {

public:

  MockQueueData() {}

  MockQueueData(int max_size)
  : _max_size(max_size)
  {}


  MockQueueData(const MockQueueData &other)
  : _messages(other._messages)
  , _max_size(other._max_size)
  {}


  MockQueueData& operator=( const MockQueueData& rhs ) {
    _messages = rhs._messages;
    _max_size = rhs._max_size;
    return *this;
  }


  int get_max_msg_size() const {
    return _max_size;
  }


  int count() {
    return _messages.size();
  }


  void send(const char *data, int length, int priority) {
    _messages.push_back(Message(data, length, priority));
  }


  void get_message(char *buf, int buflen, int &recvd_size) {
    const Message &msg = _messages.front();
    recvd_size = msg.length;
    memcpy(buf, msg.data, msg.length);
    _messages.pop_front();
  }


private:
  list<Message> _messages;
  int _max_size;

};


class MockQueue {

public:

  MockQueue(const boost::interprocess::create_only_t&, const string &name, int capacity, int max_size)
  : _name(name)
  {
    _name2message_data[name] = MockQueueData(max_size);
  }


  void send(const char *data, int length, int priority) {
    _name2message_data[_name].send(data, length, priority);
  }

  int get_max_msg_size() const {
    return _name2message_data[_name].get_max_msg_size();
  }


  static int message_count(const string &name) {
    return _name2message_data[name].count();
  }


  static void get_message(const string &name, char *buf, int buflen, int &recvd_size) {
    _name2message_data[name].get_message(buf, buflen, recvd_size);
  }

private:
  const string _name;

  static map<string, MockQueueData> _name2message_data;
};


map<string, MockQueueData> MockQueue::_name2message_data;

#define MAX_MESSAGES 5
#define MAX_MESSAGE_SIZE 20

namespace {

// The fixture for testing class Server.
class ServerTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  ServerTest() : pipe_name("test_server_pipe_name")
  {
    // You can do set-up work for each test here.
    _server = new Server<MockQueue>(pipe_name.c_str(), MAX_MESSAGES, MAX_MESSAGE_SIZE);
  }

  virtual ~ServerTest() {
    delete _server;
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }

  int message_count() {
    return MockQueue::message_count(_server->name());
  }


  void get_message(char *buf, int buflen, int &recvd_size) {
    MockQueue::get_message(_server->name(), buf, buflen, recvd_size);
  }

  // objects declared here can be used by all tests in the test case for
  // Server.

  const string pipe_name;

  Server<MockQueue> *_server;
};

// Tests that the Server::Bar() method does Abc.
TEST_F(ServerTest, ConstructionWithPipeName) {
  EXPECT_EQ(pipe_name, _server->name());
}

// Tests that sending string messages works
TEST_F(ServerTest, SendMessage) {
  _server->send("foobar");
  EXPECT_EQ(1, message_count());
  _server->send("pillepalle");
  EXPECT_EQ(2, message_count());

  char buf[200];
  int recvd_size;

  get_message(buf, sizeof(buf), recvd_size);
  EXPECT_EQ(recvd_size, 6);
  buf[recvd_size] = '\0';
  ASSERT_TRUE(!strcmp("foobar", buf));
  EXPECT_EQ(1, message_count());

  get_message(buf, sizeof(buf), recvd_size);
  EXPECT_EQ(recvd_size, 10);
  buf[recvd_size] = '\0';
  ASSERT_TRUE(!strcmp("pillepalle", buf));
  EXPECT_EQ(0, message_count());

}


  // Tests that sending oversized messages results in
  // the message being split up in several
  TEST_F(ServerTest, SendOversizedMessage) {
    string message("a message longer as max message size");
    ASSERT_LT(MAX_MESSAGE_SIZE, message.size());
    ASSERT_GE(MAX_MESSAGE_SIZE * 2, message.size());
    _server->send(message);
    EXPECT_EQ(2, message_count());
  }
}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
