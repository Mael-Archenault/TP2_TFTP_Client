# TP2_TFTP_Client

**ARCHENAULT Maël**

**CARVALHO FONTES Hugo**


## Question 1
When executing a function, the different arguments (separated by spaces) are stored in argv as strings. argc is the variable that stores the number of arguments.
So we just need to get argv to gather all arguments of the executed function

## Question 2
This is exactly like what we've done in the TD
We use getaddrinfo to get the IP address of the server.

## Question 3

This question is just about creating a socket with the parameters we got calling the functions getaddrinfo and getnameinfo.

## Question 4
This question has many parts :
- writing the reading request according to the RFC documentation
The request is 010[filename]0octet. This way, the TFTP server will transmit data inside the file named [filename]

- receiving the data packet
The servers sends a frame, with a data packet
Each data packet is : 03[block number][data]

-resend an acknowledgment

With the client, we need to resend an acknoledgment, to confirm the data packet was successfully transmitted
The acknoledgment packet is : 04[number of the received block]

With this method, we are able to transmit a data packet (max 512 bytes)

We will test it with this file in the server

![alt text](https://github.com/Mael-Archenault/TP2_TFTP_Client/blob/main/img/Q4_testFile.png)

On wireshark, we get :

![alt text](https://github.com/Mael-Archenault/TP2_TFTP_Client/blob/main/img/Q4_wireshark.png)

We can see the data has been successfully transmitted
![alt text](https://github.com/Mael-Archenault/TP2_TFTP_Client/blob/main/img/Q4_dataResponse.png)

To transmit higher sizes of data, we need to check for the size of the transmitted data.
  - if it is equal to 512 bytes, there is still data to get
  - else it is the last packet

![alt text](https://github.com/Mael-Archenault/TP2_TFTP_Client/blob/main/img/Q4_multiple_wireshark.png)

## Question 5

For this question, there is approximately the same code as for Question 4. We need to change the request frame
For writing : 020[filename]0octet
WARNING : we did not find how to create a file on the server. Is there a special request for that ? The solution we found was to already have a file in the server and overwrite it.

When the writing request is sent, the server respond with an acknowledgment with block number zero. Once the client receives this acknowledgement, we can send the data (opened with a file descriptor) by packets of 512 bytes.

We did not have time to finish this part with several packets, however, the single-frame mode works.
![alt text](https://github.com/Mael-Archenault/TP2_TFTP_Client/blob/main/img/Q5_wireshark.png)




