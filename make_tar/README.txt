a) Name: Gautam Bhatnagar
b) USC ID: 9236948851
c) In this assignment I have a emulated a healthcenter environment using socket programming. I have created
   a healthcenter server which provides available time slots of a doctor to a requesting patient (authenticating the client first).
   On receiving the preferred selection, the server reserves that slot for that patient and send the doctor information including 
   doctor name and port on which doctor server is running. Patient then sends the insurance details to the doctor server
   doctor server computes the cost of attendance and sends it to the patient. Patient prints the cost as received from the doctor
   The project is implemented in C++ language.
   
d) Code files are:
   d.1) healthcenterserver.cpp : This code creates a TCP socket using port number 21851 (851 last 3 digit of USC ID) and accepts client connection
        in sequential order. First it authenticates the client and then sends list of available appointment details. Client then selects one of the
        appointment and sends it to the server. Server reserves that time slot for that patient and sends the doctor details. All this interaction is
         TCP socket. Client gets a dynamic socket number while server binds to 21851. List of appointment, patient credentials are read from files 
		 availablities.txt and users.txt when server starts up.
   d.2) doctor1.cpp: This code emulates doctor 1 server and sends the cost of attendance upon receiving insurance details from a patient. All this
         interaction is done over bidirectional UDP socket. Doctor1 is bind to port 41851. Insurance info is read from doc1.txt file when server starts.
   d.3)	doctor2.cpp: This code emulates doctor 2 server and sends the cost of attendance upon receiving insurance details from a patient. All this
         interaction is done over bidirectional UDP socket. Doctor2 is bind to port 42851. Insurance info is read from doc1.txt file when server starts.
   d.4) patient1.cpp: this code emulates patient 1. First it sends logs into healthcenterserver by sending credentials read from patient1.txt file over TCP
        socket. Upon successful authentication, it receives a list of available time slots and selects one. It then sends the selection to healthcenterserver
         over same TCP socket and receives doctor information. It then closes TCP connection and sends insurance details to doctor server over UDP socket.
         Upon receiving cost of attendance , its prints the information  and quits.
   d.5) patient2.cpp: this code emulates patient 2. First it sends logs into healthcenterserver by sending credentials read from patient2.txt file over TCP
        socket. Upon successful authentication, it receives a list of available time slots and selects one. It then sends the selection to healthcenterserver
         over same TCP socket and receives doctor information. It then closes TCP connection and sends insurance details to doctor server over UDP socket.
         Upon receiving cost of attendance , its prints the information  and quits.
		 
e) To run the program just run the makefile(type make), it will compile all the code. Then start the ./server->./doctor1->./doctor2->./patient1->./patient2
   Please note that code is made to run in sequential order. Please note that I am adding last 3 digits of my USC ID after receiving doctor info from
   server(in patient code). Please use 41000,42000 as UDP port number in availabilities.txt. Sometimes last part of patient message is not printed on console because
   patient exits immediately after printing it. Note that this is note an issue with the code.
   
f) All the messages exchanges are char arrays in c++. The content is similar as given in project description.

g) From my testing, I could not see any scenarios in which the code fails. However, the list of available slots shrinks by 1 every time a patient is invoked
   the list will be empty after 6 invocation. If so please restart the server (kill and execute). I am also assuming that user will press ctrl-c to suspend the server
   process. Patient process will terminate automatically after phase 3.
   
h) I have reused code from beej to get IP address and port number. I have also reused code for bidirectional UDP from  https://www.cs.rutgers.edu/~pxk/417/notes/sockets/udp.html 
