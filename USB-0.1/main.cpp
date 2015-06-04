
/* 
 * File:   Server.cpp
 * Author: Renan
 *
 * Created on May 26, 2015, 2:19 PM
 */

#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "/home/station1/Downloads/c_library-master/ualberta/mavlink.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> // File control definitions
#include <unistd.h> // UNIX standard function definitions
#include <termios.h> // POSIX terminal control definitionss

#define BUFSIZE 284
#define serverPort 14550

using namespace std;

int open_port(void);
uint8_t* query_modem(int);
int configure_port(int);
int count(int);

int main(int argc, char** argv) {
 
    ssize_t receive; /*# bytes received*/
    uint8_t buf[BUFSIZE]; /* receive buffer */
    uint64_t payload = 0;
    unsigned int temp = 0;
    
   
    cout << "Reading" << endl; 
  
   /*Waiting for messages*/ 
     int fd = open_port();
        configure_port(fd);

        
        char n;
	fd_set rdfs;

	//Create byte array
        
       
      
    while(true){
            memset(buf, 0, BUFSIZE);
            receive = read(fd, &buf, 283);  //read data
           
        if (receive > 0)
      	{
			// Something received - print out all bytes and parse packet
			mavlink_message_t msg;
			mavlink_status_t status;
			
			cout << "\nBytes Received: " << (int)receive << "\nDatagram:\n";
			for (int i = 0; i < receive; ++i)
			{
				temp = buf[i];
				printf("%02x ", (unsigned char)temp);
				if (mavlink_parse_char(MAVLINK_COMM_1, buf[i], &msg, &status))
				{
					// Packet received
					printf("\nReceived packet: SYS: %u, COMP: %u, LEN: %u, MSG ID: %u", msg.sysid, msg.compid, msg.len, msg.msgid);
                                        cout << " Payload: " << msg.payload64 << " \n\n";
                                }    
			}
                sleep(1); // Sleep one second
		
        }
        
    }
    
    return 0;  
}

int open_port(void){
	int fd; // file description for the serial port
        fd = open("/dev/ttyUSB3", O_RDWR);
        struct flock fl;
        
	if(fd == -1){ // if open is unsucessfulgt
		printf("\n pen_port: Unable to open /dev/ttyUSB3. \n");
                return -1;
	}else{
                fl.l_type = F_RDLCK;
                fl.l_whence = SEEK_SET;
                fl.l_start = 0;
                fl.l_len = 283;
                
		if (fcntl(fd, F_SETLK, &fl) == -1) printf("Error");
                      
		printf("port is open USB 2.\n");
	}	
	return(fd);
}
int configure_port(int fd){      // configure the port
	struct termios port_settings;      // structure to store the port settings in
	cfsetispeed(&port_settings, B57600);    // set baud rates
	cfsetospeed(&port_settings, B57600);
	port_settings.c_cflag &= ~CSTOPB;
	port_settings.c_cflag &= ~CSIZE;
        port_settings.c_cflag &= ~CREAD;
	port_settings.c_cflag &= CS8;	
	tcsetattr(fd, TCSANOW, &port_settings);    // apply the settings to the port
	return(fd);
} //configure_port



/*
 uint8_t* query_modem(int fd)   // query modem with an AT command
{
	char n;
	fd_set rdfs;
	struct timeval timeout;

	//Create byte array
	 uint8_t send_bytes[BUFSIZE];
        
        int result;
        
        while(true){
            result = read(fd, &send_bytes, 283);  //read data
            if(result < 0) printf("Failed reading \n");
            else printf("Sucessful reading \n");
            sleep(1);
        }
	return send_bytes;
	
} //query_modem


*/


