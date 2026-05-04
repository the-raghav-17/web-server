/*
 * copyright: Raghav Sharma
 * github:    github.com/the-raghav-17
 * License:   GNU GPLv3
 */

/*
 * server.cpp
 *
 * Definitions of server class
 */


// Overview of everything the server does, happens here
void Server::start()
{
    // SEE: Kya ye saara kaam constructor me hona chahiye?

    const auto IP_TYPE    { AF_UNSPEC };
    const auto SOCK_TYPE  { SOCK_STREAM };
    const auto PORT_NO    { 3490 };

    // TODO: Handle all exceptions the socket throws

    Socket socket{ SOCK_TYPE, IP_TYPE, PORT_NO };
    socket.bind();

    const auto QUEUE_SIZE { 5 };  // Max. no. of connections that can wait to be connected
    socket.listen( QUEUE_SIZE );
}
