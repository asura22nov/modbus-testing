# modbus-testing
#
#
#Reference : http://www.simplymodbus.ca/FAQ.htm 

What is Modbus?

•	Modbus is a serial communication protocol developed by Modicon published by Modicon® in 1979 for use with its programmable logic controllers (PLCs). 
•	In simple terms, it is a method used for transmitting information over serial lines between electronic devices. 
•	The device requesting the information is called the Modbus Master and the devices supplying information are Modbus Slaves. 
•	In a standard Modbus network, there is one Master and up to 247 Slaves, each with a unique Slave Address from 1 to 247. 
•	The Master can also write information to the Slaves.
•	The official Modbus specification can be found at www.modbus.org/specs.php .

What is it used for?

•	Modbus is an open protocol, meaning that it's free for manufacturers to build into their equipment without having to pay royalties. 
•	Modbus is typically used to transmit signals from instrumentation and control devices back to a main controller or data gathering system, 
•	for example a system that measures temperature and humidity and communicates the results to a computer. 
•	Modbus is often used to connect a supervisory computer with a remote terminal unit (RTU) in supervisory control and data acquisition (SCADA) systems. 
•	Versions of the Modbus protocol exist for serial lines (Modbus RTU and Modbus ASCII) and for Ethernet (Modbus TCP).

How does it work?

•	Modbus is transmitted over serial lines between devices. 
•	The simplest setup would be a single serial cable connecting the serial ports on two devices, a Master and a Slave.
•	The data is sent as series of ones and zeroes called bits. 
•	Each bit is sent as a voltage.
•	Zeroes are sent as positive voltages and a ones as negative. 
•	The bits are sent very quickly. 
•	A typical transmission speed is 9600 baud (bits per second).

What is hexadecimal?

•	When troubleshooting problems, it can be helpful to see the actual raw data being transmitted.
•	Long strings of ones and zeroes are difficult to read, so the bits are combined and shown in hexadecimal.
•	Each block of 4 bits is represented by one of the sixteen characters from 0 to F.

0000 = 0	0100 = 4	1000 = 8	1100 = C
0001 = 1	0101 = 5	1001 = 9	1101 = D
0010 = 2	0110 = 6	1010 = A	1110 = E
0011 = 3	0111 = 7	1011 = B	1111 = F

Each block of 8 bits (called a byte) is represented by one of the 256 character pairs from 00 to FF.

15	F	0000 1111 - 4
255	FF	1111 1111 - 8

How is data stored in Standard Modbus?

•	Information is stored in the Slave device in four different tables.
•	Two tables store on/off discrete values (coils) and two store numerical values (registers). 
•	The coils and registers each have a read-only table and read-write table.

Each table has 9999 values.
Each coil or contact is 1 bit and assigned a data address between 0000 and 270E.
Each register is 1 word = 16 bits = 2 bytes and also has data address between 0000 and 270E.

Coil/Register Numbers	Data Addresses	Type	Table Name

1-9999 or 
00001-09999	0000 to 270E	Read-Write	Discrete Output Coils

10001-19999	0000 to 270E	Read-Only	Discrete Input Contacts

30001-39999	0000 to 270E	Read-Only	Analog Input Registers

40001-49999	0000 to 270E	Read-Write	Analog Output Holding Registers

Coil/Register Numbers can be thought of as location names since they do not appear in the actual messages. The Data Addresses are used in the messages.
For example, the first Holding Register, number 40001, has the Data Address 0000.
The difference between these two values is the offset.
Each table has a different offset. 1, 10001, 30001 and 40001.

What is the Slave ID?

•	Each slave in a network is assigned a unique unit address from 1 to 247. 
•	When the master requests data, the first byte it sends is the Slave address. 
•	This way each slave knows after the first byte whether or not to ignore the message. 

What is a function code?

•	The second byte sent by the Master is the Function code. 
•	This number tells the slave which table to access and whether to read from or write to the table.

Function Code	Action	Table Name
01 (01 hex)	Read	Discrete Output Coils
05 (05 hex)	Write single	Discrete Output Coil
15 (0F hex)	Write multiple	Discrete Output Coils
02 (02 hex)	Read	Discrete Input Contacts
04 (04 hex)	Read	Analog Input Registers
03 (03 hex)	Read	Analog Output Holding Registers
06 (06 hex)	Write single	Analog Output Holding Register
16 (10 hex)	Write multiple	Analog Output Holding Registers

What are the formats of Modbus commands and responses?

Follow the links in this table to see examples of the requests and responses.
Data Addresses	Read	Write Single	Write Multiple
Discrete Output Coils 0xxxx	FC01
FC05
FC15

Discrete Input Contacts 1xxxx	FC02
NA	NA
Analog Input Registers 3xxxx	FC04
NA	NA
Analog Output Holding Registers 4xxxx	FC03
FC06
FC16


Exception Responses
Following a request, there are 4 possible outcomes from the slave.
  1.  The request is successfully processed by the slave and a valid response is sent.
  2.  The request is not received by the slave therefore no response is sent.
  3. The request is received by the slave with a parity, CRC or LRC error.
      The slave ignores the request and sends no response.
  4. The request is received without an error, but cannot be processed by the slave for another reason.  The slave replies with an exception response.

In a normal response, the slave echoes the function code.  The first sign of an exception response is that the function code is shown in the echo with its highest bit set. All function codes have 0 for their most significant bit.  Therefore, setting this bit to 1 is the signal that the slave cannot process the request.
Function Code in Request	Function Code in Exception Response

01 (01 hex) 0000 0001	129 (81 hex) 1000 0001
02 (02 hex) 0000 0010	130 (82 hex) 1000 0010
03 (03 hex) 0000 0011	131 (83 hex) 1000 0011
04 (04 hex) 0000 0100	132 (84 hex) 1000 0100
05 (05 hex) 0000 0101	133 (85 hex) 1000 0101
06 (06 hex) 0000 0110	134 (86 hex) 1000 0110
15 (0F hex) 0000 1111	143 (8F hex) 1000 1111
16 (10 hex) 0001 0000	144 (90 hex) 1001 0000

Here is an example of a request with an Exception Response:

Request

This command is requesting the ON/OFF status of discrete coil #1186
from the slave device with address 10.
0A 01 04A1 0001 AC63
0A: The Slave Address (0A hex = address10 )
01: The Function Code 1 (read Coil Status)
04A1: The Data Address of the first coil to read
             ( 04A1 hex = 1185 , + 1 offset = coil #1186 )
0001: The total number of coils requested. 
AC63: The CRC (cyclic redundancy check) for error checking.

Response

0A 81 02 B053
0A: The Slave Address (0A hex = address10 )
81: The Function Code 1 (read Coil Status - with the highest bit set)
02: The Exception Code
B053: The CRC (cyclic redundancy check).
Following the Function Code is the Exception Code.  
The exception code gives an indication of the nature of the problem. The possible codes are shown in the table below.
The exception code shown above 02 is an indication that coil #1186 is an illegal address in the slave.  This coil has not been defined in the slave's modbus map.
