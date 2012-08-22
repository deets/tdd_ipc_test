#include <string>
#include "gtest/gtest.h"
#include <map>

#include <boost/interprocess/ipc/message_queue.hpp>

#include "ipc_server.hh"

using namespace std;



class MockQueue {

public:

  MockQueue(const string &name)
  : _name(name)
  {
    _name2message_count[name] = 0;
  }


  void send(const string&) {
    _name2message_count[_name]++;
  }

  static int message_count(const string &name) {
    return _name2message_count[name];
  }

private:
  const string _name;

  static map<string, int> _name2message_count;
};


map<string, int> MockQueue::_name2message_count;

namespace {

// The fixture for testing class Server.
class ServerTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  ServerTest() : pipe_name("test_server_pipe_name")
  {
    // You can do set-up work for each test here.
    _server = new Server<MockQueue>(pipe_name);
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

  // Objects declared here can be used by all tests in the test case for
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
  _server->send("foobar");
  EXPECT_EQ(2, message_count());
}

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
