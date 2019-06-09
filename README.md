# LogMeInAssignment
This is an interview assignment for LogMeIn

## Description
The TCP server read a SIP registrations file and store it in memory. Then the server listen to any address (IPV4 and IPV6) on the port 8888. The server can handle multiple socket connections. 

Then a client can connect and request a SIP registration by sending an address of records.

## Getting Started

The project is build on top of the Qt library. To build the project you will have to download the latest [Qt Open source](https://www.qt.io/download). After that, you can just use Qt Creator to build/run and test the project. Qt Creator use qmake to build the project.

### Installing

* Todo

### Exemple of output

TCPServer
```txt
Reading and parsing SIPs file...
Starting TCP server...
Server started. Listening on port 8888
New client connected:  "::ffff:127.0.0.1"
Client  "::ffff:127.0.0.1" has just been disconnected
```

TCPClient
```txt
Server replied with:  "Connected to host!"

Enter the address of record that you're looking for:
0128841e7193fea0272e000100620001

Querying server with AOR:  "0128841e7193fea0272e000100620001"

Server replied with:  "{\"addressOfRecord\":\"0128841e7193fea0272e000100620001\",\"tenantId\":\"0127d974-f9f3-0704-2dee-000100420001\",\"uri\":\"sip:0128841e7193fea0272e000100620001@39.172.243.72\",\"contact\":\"<sip:0128841e7193fea0272e000100620001@68.5.238.218>;methods=\\\"INVITE, ACK, BYE, CANCEL, OPTIONS, INFO, MESSAGE, SUBSCRIBE, NOTIFY, PRACK, UPDATE, REFER\\\"\",\"path\":[\"<sip:Mi0xOTkuODcuMTIwLjEyNi0xMDM1@155.37.243.188:5060;lr>\"],\"source\":\"83.90.153.13:1035\",\"target\":\"12.21.50.41:5061\",\"userAgent\":\"polycom.vvx.600\",\"rawUserAgent\":\"PolycomVVX-VVX_600-UA/3.118.142.226\",\"created\":\"2017-01-05T14:39:03.240Z\",\"lineId\":\"0128841e-7199-fea4-272e-000100620001\"}"
```
Have fun playing with it!
