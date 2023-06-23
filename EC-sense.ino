#include <SoftwareSerial.h> 
SoftwareSerial mySerial ( 12 ,  13 ) ;  //Uno Rx Tx (13 12) = mySerial 
byte Active_mode [ 9 ]  =  { 0xFF ,  0x01 ,  0x78 ,  0x40 ,  0x00 ,  0x00 ,  0x00 ,  0x00 ,  0x47 } ;   
byte Gas_value_request [ 9 ]  =  { 0xFF ,  0x01 ,  0x86 ,  0x00 ,  0x00,  0x00 ,  0x00 ,  0x00 ,  0x79 } ;  // request gas value 
void setup ( )  { 
  Serial. begin ( 115200 ) ;  //Serial communication initialization 
  delay ( 1000 ) ;  
  while ( ! mySerial ) { }  // Wait until the serial communication port is connected 
  mySerial. begin ( 115200 ) ;   
  mySerial. write ( Active_mode ,  9 ) ; // mode set 
  delay ( 100 ) ; 
} 
void loop ( )  
{ 
  unsigned  char receive_data [ 9 ]  =  {  0x00 ,  } ;  //Save all numbers as positive numbers (0x00~0xFF) 
  int packetIndex =  0 ;  // Initialize packetIndex to 0 
  while ( mySerial. available ( ) > 0 ) {  // If the received data exceeds 0, i.e. data exists, execute the code 
    int ch = mySerial. read () ;  //Save serial data to integer ch 
    receive_data [ packetIndex ]  = ch ; 
    //Serial. println ( ch , HEX ) ;  //Output the data received from the serial monitor 
    //Serial. println ( ' ' ) ; 
    packetIndex +=  1 ; 
  } 
  // After receiving all packets, check data validity using checksum 
  // Response ex) FF 86 2C 88 3 E8 27 10 A4, 0x2710 = 10000   
  /* - Sensor TB600C-CO-1000 used in the example */ 
  if (  ( packetIndex ==  9 )  && ( 1 +  ( 0xFF  ^  ( byte ) ( receive_data [ 1 ]  + receive_data [ 2 ]  + receive_data [ 3 ]  
  + receive_data [ 4 ]  + receive_data [ 5 ]  + receive_data [ 6 ]  + receive_data [ 7 ] ) ) ) == receive_data [ 8 ] )  
  //checksum = create 8-bit data by adding data from 1 to 7 digits, invert each bit and add 1 to the end 
    { 
       //Serial. println ( " " ) ;  
       //Serial. print ( receive_data [ 6 ] , HEX ) ;  
      // Serial. println ( " <--- CO High byte " ) ; 
       //Serial. print ( receive_data [ 7 ] , HEX ) ;  
       //Serial. println ( " <--- CO Low byte " ) ; 
      float CO_value=  ( ( receive_data [ 6 ]  *  256 )  + receive_data [ 7 ] ) ;  
      CO_value = CO_value / 10 ; 
      //CO_value = CO_value * 100 ; 

      // /10 may vary depending on the type of sensor or measurement range, so it is recommended to check the number of digits in reading module information   
      Serial. print ( "NO2 : " ) ; 
      Serial. print ( CO_value ) ; 
      //Serial. println ( "ppm" ) ;  // The unit can be checked with the Read Module Information (0xD7) command 
      Serial. println ( "ppb" ) ;
     Serial.println ( "----------------------------------------------- -" ) ; 
      delay ( 500 ) ;  
    } 
}
