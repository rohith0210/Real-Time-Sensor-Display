/*#include <REGX51.H>
#include <stdio.h>
#include <intrins.h>

// Pin connections
sbit RS = P1^0;
sbit E  = P1^1;
sbit D4 = P1^4;
sbit D5 = P1^5;
sbit D6 = P1^6;
sbit D7 = P1^7;

sbit DHT = P2^0; // DHT11 data pin

// Simple delay functions
void delay_us(unsigned int us) {
    while(us--) {_nop_(); _nop_();}
}

void delay_ms(unsigned int ms) {
    while(ms--) delay_us(1000);
}

// UART functions
void uart_init() {
    TMOD |= 0x20;      // Timer1 in Mode2
    TH1 = 0xFD;        // Baud rate 9600
    SCON = 0x50;       // 8-bit UART mode
    TR1 = 1;           // Start Timer1
}

void uart_send(char ch) {
    SBUF = ch;
    while (TI == 0);
    TI = 0;
}

void uart_send_string(char* str) {
    while(*str) {
        uart_send(*str++);
    }
}

// LCD functions
void lcd_enable() {
    E = 1; delay_us(5); E = 0;
}

void lcd_send_nibble(unsigned char nib) {
    D4 = (nib >> 0) & 1;
    D5 = (nib >> 1) & 1;
    D6 = (nib >> 2) & 1;
    D7 = (nib >> 3) & 1;
    lcd_enable();
}

void lcd_send_byte(unsigned char byte, bit is_cmd) {
    RS = is_cmd ? 0 : 1;
    lcd_send_nibble(byte >> 4);
    lcd_send_nibble(byte & 0x0F);
    delay_ms(2);
}

void lcd_cmd(unsigned char cmd) {
    lcd_send_byte(cmd, 1);
}

void lcd_data(unsigned char dat) {
    lcd_send_byte(dat, 0);
}

void lcd_init() {
    delay_ms(20);
    lcd_send_nibble(0x03); delay_ms(5);
    lcd_send_nibble(0x03); delay_ms(1);
    lcd_send_nibble(0x03); delay_ms(1);
    lcd_send_nibble(0x02);  // 4-bit mode

    lcd_cmd(0x28); // 2-line
    lcd_cmd(0x0C); // display on
    lcd_cmd(0x06); // move right
    lcd_cmd(0x01); // clear screen
}

void lcd_print(char* str) {
    while(*str) lcd_data(*str++);
}

// DHT11 functions
bit dht_start() {
    DHT = 0;
    delay_ms(18);
    DHT = 1;
    delay_us(20);

    if(DHT == 0) {
        while(DHT == 0); // wait for response
        while(DHT == 1);
        return 1;
    }
    return 0;
}

unsigned char dht_read_byte() {
    unsigned char i, result = 0;
    for(i = 0; i < 8; i++) {
        while(DHT == 0);
        delay_us(30);
        result <<= 1;
        if(DHT == 1)
            result |= 1;
        while(DHT == 1);
    }
    return result;
}

bit dht_read(unsigned char *temp, unsigned char *hum) {
    unsigned char h_int, h_dec, t_int, t_dec, checksum;
    if (!dht_start()) return 0;

    h_int = dht_read_byte();
    h_dec = dht_read_byte();
    t_int = dht_read_byte();
    t_dec = dht_read_byte();
    checksum = dht_read_byte();

    if ((h_int + h_dec + t_int + t_dec) != checksum)
        return 0;

    *temp = t_int;
    *hum  = h_int;
    return 1;
}

// MAIN
void main() {
    unsigned char temperature, humidity;
    char buf[32];

    uart_init();
    lcd_init();

    while(1) {
        lcd_cmd(0x80);
        if (dht_read(&temperature, &humidity)) {
            lcd_print("Temp: ");
            lcd_data(temperature / 10 + '0');
            lcd_data(temperature % 10 + '0');
            lcd_data('C');

            lcd_cmd(0xC0);
            lcd_print("Hum: ");
            lcd_data(humidity / 10 + '0');
            lcd_data(humidity % 10 + '0');
            lcd_data('%');

            sprintf(buf, "Temp:%dC Hum:%d%%\r\n", temperature, humidity);
            uart_send_string(buf);
        } else {
            lcd_print("Sensor Error");
            uart_send_string("Sensor Error\r\n");
        }

        delay_ms(2000);
    }
}
*/
#include<REG52.H>
#include<string.h>
sbit RS   = P2^0;
sbit RW   = P2^1;
sbit E    = P2^2;
#define LCD_PORT P3    
sbit DHT11 = P1^0;       
unsigned char rx[16];
unsigned char rx_idx = 0;
void delay_ms(unsigned int ms){
	int i,j;
	for(i = 0;i<ms;i++){
		for(j = 0;j<1275;j++){
			
		}
	}
}
void delay_us(unsigned int us) {
  unsigned int i, j;
  for (i = 0; i < us; i++) {
    for (j = 0; j < 1; j++){
    	
	}
  }
}
void lcd_enable(){
	E = 1;
	delay_us(2);
	E = 0;
}
void lcd_cmd(unsigned char cmd){
	RS = 0;
	RW = 0;
	//LCD_DATA = (cmd & 0xF0);
	P3 = (P3 & 0x0F) | (cmd & 0xF0);
	lcd_enable();
//	LCD_DATA = ((cmd<<4) & 0xF0);
P3 = (P3 & 0x0F) | ((cmd<<4) & 0xF0);
	lcd_enable();
}
void lcd_data(unsigned char dat){
	RS = 1;
	RW = 0;
	//LCD_DATA = (dat & 0xF0);
	P3 = (P3 & 0x0F) | (dat & 0xF0);
	lcd_enable();
	//LCD_DATA = ((dat<<4) & 0xF0);
	P3 = (P3 & 0x0F) | ((dat<<4) & 0xF0);
	lcd_enable();
}
void lcd_string(char *str){
  while(*str){
	  lcd_data(*str++);
	}
}
void lcd_Init(){
	lcd_cmd(0x28);
	lcd_cmd(0x0C);
	lcd_cmd(0x06);
	lcd_cmd(0x01);
}
void UART_Init(){
	TMOD = 0x20;
	SCON = 0x50;
	TH1 = 0xFD;
	TR1 = 1;
}
void UART_Tx(char c){
	SBUF = c;
	while(!TI);
		TI = 0;
}
void UART_TxString(char s[]){
   int i = 0;
   while(s[i] != '\0'){
   	 UART_Tx(s[i]);
   	 i++;
   }	
}

void serial_ISR(void) interrupt 4{
		char ch;
	

	
	if (RI){
		ch = SBUF;
		RI = 0;
		if(ch == '\r'){
			rx[rx_idx] = '\0';
			if(strcmp(rx,"tem") == 0){
				lcd_cmd(0x01);
				lcd_cmd(0x80);
				lcd_string("Temperature is..");
				delay_ms(1000);
				UART_TxString("OK\r\n");
			}
			else if(strcmp(rx,"hum") == 0){
				lcd_cmd(0x01);
				lcd_cmd(0x80);
				lcd_string("Humidity is..");
				delay_ms(1000);
				UART_TxString("OK\r\n");
			}
			else if(strcmp(rx,"tem/hum") == 0){
				lcd_cmd(0x01);
				lcd_cmd(0x80);
				lcd_string("Tem/Hum is..");
				delay_ms(1000);
				UART_TxString("OK\r\n");
				
			}
			else{
				lcd_cmd(0x01);
				lcd_cmd(0x80);
				lcd_string("Error Cmd..");
				delay_ms(1000);
				UART_TxString("OK\r\n");
			}
			rx_idx = 0;
			
		}
		else {
    rx[rx_idx++] = ch;
    if (rx_idx >= 15) 
			rx_idx = 0;  // prevent buffer overflow
     }
		
	}
}
bit dht11_start(void){
	DHT11 = 0;
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

bit dht11_checkresponse(){
	delay_us(40);
	if(DHT11 == 0){
		delay_us(80);
		if(DHT11 == 1){
			delay_us(80);
			return 1;
	
		}
	}
	return 0;
}
bit dht_readbit(){
	while(!DHT11){
		delay_us(35);
	}
	return DHT11;
}
unsigned char dht_readByte(){
	unsigned char i ;
	unsigned char value = 0 ;
	for(i=0 ;i<8;i++){
		value <<=1 ;
		if(dht_readbit()){
			value = value | 1;
		}
		while(DHT11);
	}
	return value;
}
void main(){
	unsigned char h , t;
	UART_Init();
	EA = 1;
	ES = 1;
	lcd_Init();
	while(1){
		if(dht11_start() && dht11_checkresponse()){
			h  = dht_readByte();
			dht_readByte();
			t = dht_readByte();
			dht_readByte();
			dht_readByte();
			
			      lcd_cmd(0x01);
			      lcd_cmd(0x80); 
            delay_ms(1000);			
            lcd_string("T:");
            lcd_data((t/10)+'0');
            lcd_data((t%10)+'0');
            lcd_data(0xDF);     
            lcd_data('C');
            
            //lcd_cmd(0x01);
			//lcd_cmd(0x80);
			delay_ms(1000);
           lcd_cmd(0xC0);			
            lcd_string("H:");
            lcd_data((h/10)+'0');
            lcd_data((h%10)+'0');     
            lcd_data('%');
            
            
            delay_ms(1000);
		}
		else{
			lcd_cmd(0x01);
			lcd_cmd(0x80);
			lcd_string("DHT fail!");
		}
		delay_ms(2000);
	}
	
}