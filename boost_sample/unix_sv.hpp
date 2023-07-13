#ifndef UNIX_SV_HPP
#define UNIX_SV_HPP

#include <boost/asio.hpp>
#include <iostream>
#include <boost/asio/io_context.hpp>
#include <boost/asio/local/datagram_protocol.hpp>
#include <boost/system/system_error.hpp>
#include <filesystem>
#include <string>

#define SOCK_UNIX_CLIENT "/tmp/unix_sender.sock"
#define SOCK_UNIX_SERVER "/tmp/unix_server.sock"
#define ASYNC_REQUEST_JSON_FILE "/tmp/async.json"

namespace fs = std::filesystem;

using boost::asio::basic_datagram_socket;
using boost::asio::generic::datagram_protocol;
using boost::asio::io_context;
using std::cout;
using std::endl;
using std::cerr;
using fs::path;
using std::string;

typedef boost::asio::local::datagram_protocol::socket boost_unix_socket;
typedef boost::system::system_error boost_err;
typedef boost::asio::local::datagram_protocol unix_protocol;
typedef boost::asio::local::datagram_protocol::endpoint unix_endpoint;

#endif
