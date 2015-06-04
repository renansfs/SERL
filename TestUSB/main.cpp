/* 
 * File:   main.cpp
 * Author: station1
 *
 * Created on June 2, 2015, 3:53 PM
 */

#include <stdio.h> // standard input / output functions
#include <string.h> // string function definitions
#include <unistd.h> // UNIX standard function definitions
#include <fcntl.h> // File control definitions
#include <errno.h> // Error number definitions
#include <termios.h> // POSIX terminal control definitionss
#include <time.h>   // time calls
#include <iostream>
#include <fstream>



using namespace std;

/*
 * 
 */

int open_port(void);
int query_modem(int);
int configure_port(int);
int execute();
int readT();

int main(int argc, char** argv) {
  //  readT();
    int fd = open_port();
        configure_port(fd);
	query_modem(fd);
    return 0;
}

int open_port(void)
{
	int fd; // file description for the serial port
                                                              
	fd = open("/dev/ttyUSB0", O_RDONLY);
        
	if(fd == -1) // if open is unsucessful
            
	{
		printf("\n pen_port: Unable to open /dev/ttyS0. \n");
                return -1;
	}
	else
	{
		fcntl(fd, F_SETFL, 0);
		printf("port is open.\n");
	}
        
	
	return(fd);
                                                      
    

}




int configure_port(int fd)      // configure the port
{
	struct termios port_settings;      // structure to store the port settings in

	cfsetispeed(&port_settings, B57600);    // set baud rates
	cfsetospeed(&port_settings, B57600);

	port_settings.c_cflag &= ~PARENB;    // set no parity, stop bits, data bits
	port_settings.c_cflag &= ~CSTOPB;
	port_settings.c_cflag &= ~CSIZE;
	port_settings.c_cflag |= CS8;
	
	tcsetattr(fd, TCSANOW, &port_settings);    // apply the settings to the port
	return(fd);

} //configure_port

int query_modem(int fd)   // query modem with an AT command
{
	char n;
	fd_set rdfs;
	struct timeval timeout;
	
	// initialise the timeout structure
	timeout.tv_sec = 10; // ten second timeout
	timeout.tv_usec = 0;
	
	//Create byte array
//	unsigned char send_bytes[283];
	unsigned char send_bytes[283];
        
        int result;
	
        
        while(true){
            result = read(fd, &send_bytes, 283);  //read data
            if(result < 0) printf("Failed reading \n");
            else printf("Sucessful reading %d \n", send_bytes);
            sleep(1);
        

	n = 0;
	// do the select
	//n = select(fd + 1, &rdfs, NULL, NULL, &timeout);
	
	// check if an error has occured
	if(n < 0)
	{
	 perror("select failed\n");
	}
	else if (n == 0)
	{
	 puts("Timeout!");
	}
	else
	{
	 printf("\nBytes detected on the port!\n");
	}
        }

	return 0;
	
} //query_modem


int readT(){
     std::ifstream is ("COM4", std::ifstream::binary);
  
     while (true) {
    // get length of file:
    is.seekg (0, is.end);
    int length = is.tellg();
    is.seekg (0, is.beg);

    char * buffer = new char [length];

    std::cout << "Reading " << length << " characters... ";
    // read data as a block:
    is.read (buffer,length);

    if (is)
      std::cout << "all characters read successfully \n.";
    else
      std::cout << "error: only " << is.gcount() << " could be read";
    //is.close();
    sleep(1);
    // ...buffer contains the entire file...

    delete[] buffer;
  }
  return 0;


}


int execute () {
  streampos size;
  char * memblock;

  ifstream file ("/sys/bus/usb/devices/4-2:1.0/ttyUSB0", ios::in|ios::binary|ios::ate);
  if (file.is_open())
  {
    size = file.tellg();
    memblock = new char [size];
    file.seekg (0, ios::beg);
    file.read (memblock, size);
    file.close();

    cout << "the entire file content is in memory";

    delete[] memblock;
  }
  else cout << "Unable to open file";
  return 0;
}