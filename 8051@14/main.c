/*#include<reg52.h>

sbit SDA = P3^0;
sbit SCL = P3^1;
void delay_us(unsigned int us){
	int i,j;
	for(i = 0; i<us ; i++){
		for(j= 0 ; j<1 ; j++){
			//
		}
	}
}

void i2c_start(){
	SDA = 1;
	SCL = 1;
	delay_us(5);
	SDA = 0;
	delay_us(5);
	SCL = 0;
}
void i2c_stop(){
	SDA = 0;
	SCL = 1;
	delay_us(5);
	SDA = 1;
}

void i2c_write(unsigned char byte){
	unsigned char i;
	for (i = 0; i<8 ; i++){
		if(byte & 0x80)
		   SDA = 1;
		else
		    SDA = 0;
		SCL = 1;
		delay_us(5);
		SCL = 0;
		delay_us(5);
		byte <<= 1;
	}
	SDA = 1; //ACK
	SCL = 1;
	delay_us(5);
	SCL = 0;
	delay_us(5);
	
}

unsigned char i2c_read(bit ack){
	unsigned char i;
	unsigned char byte = 0;
	SDA = 1;
	
	for (i = 0 ; i<8 ; i++){
		SCL = 1;
		delay_us(5);
	    byte = (byte << 1) | SDA ;
		SCL = 0;
		delay_us(5);	
	}
	
	if(ack == 1)  //ACK OR NACK
	  SDA = 1;
	else
	  SDA = 0;
	  
	  SCL = 1;
	  delay_us(5);
	  SCL = 0;
	  
	  SDA = 1;
	return byte;
}

void UART_Init(){
	SCON = 0x50;
	TMOD = 0x20;
	TH1 = 0xFD;
	TR1 = 1;
}
void UART_Tx(char c){
	SBUF = c;
	while(!TI);
	TI = 0;
}
char UART_Rx(){
	while(!RI);
	RI = 0;
	return SBUF;
}
void UART_TxString(char str[]){
	unsigned char i;
	while(str[i] != '\0'){
		UART_Tx(str[i]);
		i++;
	}
}

void send_hex(unsigned char byte){
	char hex[] = "0123456789ABCDEF" ;
	UART_Tx(hex[byte >> 4]);
	UART_Tx(hex[byte & 0x0F]);
}

#define EEPROM_W 0xA0
#define EEPROM_R 0xA1

void eeprom_write(unsigned int addr,unsigned char data){
	i2c_start();
	i2c_write(EEPROM_W);
	i2c_write(addr >> 8);
	i2c_write(addr & 0xFF);
	i2c_write(data);
	i2c_stop();
}

unsigned char eeprom_read(unsigned int addr){
	unsigned char data;
	i2c_start();
	i2c_write(EEPROM_W);
	i2c_write(addr >> 8);
	i2c_write(addr & 0xFF);
	i2c_start();
	i2c_write(EEPROM_R);
	data  = i2c_read(0);
	i2c_stop();
	return data;
}

void main(){
	unsigned char val;
	SDA = 1;
	SCL = 1;
	UART_Init();
	val = eeprom_read(0x0010);
	if(val != 0x55){
		eeprom_write(0x0010 , 0x55);
	}
	while(1){
	eeprom_write(0x0010 , 0x55);
	delay_us(1000);
    val = eeprom_read(0x0010);
	send_hex(val);
	UART_Tx('\n');
	}
	
}*/


#include<reg52.h>

sbit SDA = P2^0;
sbit SCL = P2^1;
sbit DHT11 = P2^2;
void delay_us(unsigned int us){
	int i,j;
	for(i = 0; i<us ; i++){
		for(j= 0 ; j<1 ; j++){
			//
		}
	}
}
void delay_ms(unsigned int ms){
	int i,j;
	for (i = 0 ; i < ms; i++){
		for(j= 0 ; j<1275 ; j++){
			//
		}
	}
}
void i2c_start(){
	SDA = 1;
	SCL = 1;
	delay_us(5);
	SDA = 0;
	delay_us(5);
	SCL = 0;
}
void i2c_stop(){
	SDA = 0;
	SCL = 1;
	delay_us(5);
	SDA = 1;
}

bit i2c_write(unsigned char byte){
	unsigned char i;
	bit ack;
	for (i = 0; i<8 ; i++){
		if(byte & 0x80)
		   SDA = 1;
		else
		    SDA = 0;
		SCL = 1;
		delay_us(5);
		SCL = 0;
		delay_us(5);
		byte <<= 1;
	}
	SDA = 1; //ACK
	SCL = 1;
	delay_us(5);
	ack = !SDA;
	SCL = 0;
	delay_us(5);
	return ack;
	
}

unsigned char i2c_read(bit ack){
	unsigned char i;
	unsigned char byte = 0;
	SDA = 1;
	
	for (i = 0 ; i<8 ; i++){
		SCL = 1;
		delay_us(5);
	    byte = (byte << 1) | SDA ;
		SCL = 0;
		delay_us(5);	
	}
	
	if(ack)  //ACK OR NACK
	  SDA = 0;
	else
	  SDA = 1;
	  
	  SCL = 1;
	  delay_us(5);
	  SCL = 0;
	  
	  SDA = 1;
	  return byte;
}

void UART_Init(){
	SCON = 0x50;
	TMOD = 0x20;
	TH1 = 0xFD;
	TR1 = 1;
}
void UART_Tx(char c){
	SBUF = c;
	while(!TI);
	TI = 0;
}
/*char UART_Rx(){
	while(!RI);
	RI = 0;
	return SBUF;
}*/

  void UART_TxString(char str[]){
	unsigned char i;
	while(str[i] != '\0'){
		UART_Tx(str[i]);
		i++;
	}
}

void send_hex(unsigned char byte){
	char hex[16] = "0123456789ABCDEF" ;
	UART_Tx(hex[(byte >> 4) & 0x0F]);
	UART_Tx(hex[byte & 0x0F]);
}

#define EEPROM_W 0xA0
#define EEPROM_R 0xA1

void eeprom_write(unsigned int addr,unsigned char value){
	i2c_start();
	i2c_write(EEPROM_W);
	i2c_write(addr >> 8);
	i2c_write(addr & 0xFF);
	i2c_write(value);
	i2c_stop();
	
	do {
     i2c_start();           
     } while (!i2c_write(EEPROM_W));    
     i2c_stop();
	
}

unsigned char eeprom_read(unsigned int addr){
	unsigned char value;
	i2c_start();
	i2c_write(EEPROM_W);
	i2c_write(addr >> 8);
	i2c_write(addr & 0xFF);
	i2c_start();
	i2c_write(EEPROM_R);
	value  = i2c_read(0);
	i2c_stop();
	return value;
}

bit dht11_start(){
	DHT11 = 0 ;
	delay_ms(20);
	DHT11 = 1;
	delay_us(30);
	
	if(DHT11 == 0){
		while(DHT11 == 0);
		while(DHT11 == 1);
		return 1;
	}
	return 0;
}

bit dht11_response(){
	delay_us(40);
	if (DHT11 == 0){
		delay_us(80);
		delay_us(80);
	  if(DHT11 == 1){
	  	delay_us(80);
	    return 1;
	}
	 
	  }
  return 0; //this
}
bit read_bit(){
	while(!DHT11);
	delay_us(35);
	return DHT11;
	
}
unsigned char read_byte(){
	unsigned char i;
	unsigned char value = 0;
	for(i  = 0;i < 8 ;i++){
		value <<= 1;
		if(read_bit())
		   value |= 1;
		while(DHT11);
	}
	return value;
}

void main(){
		
	/*SDA = 1;
	SCL = 1;
	UART_Init();
	val = eeprom_read(0x0010);
	if(val != 0x55){
		eeprom_write(0x0010 , 0x55);
	}
	while(1);
	eeprom_write(0x0010 , 0x55);
	delay_ms(1000);
    val = eeprom_read(0x0010);
	send_hex(val);
	UART_Tx('\n');
*/
	unsigned int eep_addr = 0;
	//unsigned char temp , hum,checksum;
	unsigned char hum_int;
	unsigned char hum_dec;
	unsigned char temp_int;
	unsigned char temp_dec;
	unsigned char checksum;
	unsigned char val1,val2;
	SDA = 1;
	SCL = 1;
	UART_Init();
	UART_TxString("\r\n DHT11 logging to EEPROM \r\n");
	delay_ms(1200); 
	if(!dht11_start()){
		UART_TxString("\r\n Start fail\r\n"); 
		delay_ms(1000);
 
	}
	while(1){
		dht11_start();
		if(!dht11_response()){
			UART_TxString("\r\n DHT11 Error! \r\n");
			delay_ms(2000);
			continue;
		}
		 hum_int = read_byte();
     	hum_dec = read_byte();
     	temp_int = read_byte();
     temp_dec = read_byte();
	    checksum = read_byte();
	if(((hum_int +hum_dec + temp_int + temp_dec) & 0xFF) != checksum){
		UART_TxString("\r\n checksum Fail!! \r\n");
		delay_ms(2000);
		continue;
	}
	//	eeprom_write(eep_addr++,temp_int);
	//eeprom_write(eep_addr++,hum_int);

	eeprom_write(eep_addr,temp_int);
	UART_TxString("Temp = 0x");
	 val1 = eeprom_read(eep_addr);
   send_hex(val1);  
	//UART_TxString("Temp = 0x");
	//send_hex(temp);
	  UART_TxString(" (" );
    UART_Tx(temp_int / 10 + '0');
    UART_Tx(temp_int % 10 + '0');
    UART_TxString(" C)\r\n");
	++eep_addr;
	
	 
	eeprom_write(eep_addr,hum_int);
	UART_TxString("RH  = ");
	  val2 = eeprom_read(eep_addr);
	 send_hex(val2);
    UART_Tx(hum_int / 10 + '0');
    UART_Tx(hum_int % 10 + '0');
	  UART_TxString("% ");
	++eep_addr;
	
	if(eep_addr >= 0x8000){
		eep_addr = 0;
	}
}
	/*UART_TxString("Temp = 0x");
	 val1 = eeprom_read(eep_addr);
   send_hex(val1);  
	//UART_TxString("Temp = 0x");
	//send_hex(temp);
	  UART_TxString(" (" );
    UART_Tx(temp_int / 10 + '0');
    UART_Tx(temp_int % 10 + '0');
    UART_TxString(" C)\r\n");
	 
     UART_TxString("RH  = ");
	  val2 = eeprom_read(eep_addr++);
	 send_hex(val2);
    UART_Tx(hum_int / 10 + '0');
    UART_Tx(hum_int % 10 + '0');
	  UART_TxString("% ");
  }
	
	
	   OPTIONAL FROM CHATGPT!!!
	    unsigned int addr = eep_addr;          // remember starting address

     eeprom_write(addr,     temp_int);      // TEMP
        eeprom_write(addr + 1, hum_int);       // HUM

 
    val1 = eeprom_read(addr);
    val2 = eeprom_read(addr + 1);


   addr += 2;
   if (addr >= 0x8000) addr = 0;
   eep_addr = addr;


    UART_TxString("Temp = 0x"); send_hex(temp_int);
   UART_TxString(" (");
    UART_Tx(temp_int/10 + '0'); UART_Tx(temp_int%10 + '0');
   UART_TxString(" C)   RH = 0x"); send_hex(hum_int);
   UART_TxString(" (");
    UART_Tx(hum_int/10 + '0');  UART_Tx(hum_int%10 + '0');
    UART_TxString("%)\r\n");
		
		
   */