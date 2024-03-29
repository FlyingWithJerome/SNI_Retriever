## SNI (Server Name Indication) Retriever 🐶

#### - How it works 🐶

Server Name Indication, abbr. SNI allows a server to serve with multiple hostnames. When setting up TLS connections with servers, clients that support SNI append TLS Extension in their Client Hello packet, showing which hostnames they would like to talk with.

SNI Extension includes the following fields:

- Extension Type (SNI = 0)
- Extension Length
- Server Name List Length
- Server Name Type
- Server Name Length
- Server Name

SNI Retriever filters the Client Hello packets and looks for the Server Name in SNI Extension (if it exists).

In this little demo, a server listens in port 443 (HTTPS). If anyone tries to setup a TLS session with the server,  the demo will read SNI (which should be the hostname of the server itself) from the Client Hello and shut down the session (because we do not have a certificate…).

See RFC 3546 Sec. 3.1 for technical standards.



#### - Dependencies 🐶

- Runs on Linux and macOS

- Boost.asio

- gtest