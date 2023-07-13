#include "unix_sv.hpp"

int main(int argc, char *argv[]) {
    unix_protocol uprot;
    io_context baseIO;
    unix_endpoint unixClEpoint(SOCK_UNIX_CLIENT);
    boost_unix_socket sock(baseIO.get_executor());
    path unixSockClientPath(SOCK_UNIX_CLIENT), asyncJsonPath(ASYNC_REQUEST_JSON_FILE),
        unixSockServerPath(SOCK_UNIX_SERVER);§
    
    if (argc < 3 ) {
        cout << "Usage: unix_sv <unix_server_socket_path> <arg_1> <arg_2> <arg_3> ..." << endl;
        return EXIT_FAILURE;
    }
    try {
        if (fs::exists(unixSockClientPath))
            fs::remove(unixSockServerPath);
        sock.open(uprot);
        sock.bind(unixClEpoint);
        unix_endpoint unixSvEpoint(argv[1]);
        for (int i = 2; i < argc; ++i) {
            string recvData;
            sock.send_to(boost::asio::buffer(argv[i], std::strlen(argv[i])), unixSvEpoint);
            sock.receive_from(boost::asio::buffer(recvData, 1024), unixSvEpoint);
            cout << "received " << recvData.size() << " from server ("
                << argv[1] << "): " << recvData << endl;
            sock.close();
        }
    } catch (const boost::system::system_error &e) {
        cerr << "socket I/O error: " << e.std::exception::what() << endl;
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}
