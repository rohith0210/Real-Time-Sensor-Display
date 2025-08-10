/*  8051 + DHT11 + 16×2 LCD (4-bit)  |  Crystal: 12 MHz
 *  Displays Temperature (°C) and Humidity (%) every 2 s
 *  Tested in Keil µVision 5 (C51) + Proteus
 */
#include <REGX51.H>
#include <intrins.h>          // _nop_()

/* ---------------- Pin Assignments ---------------- */
sbit LCD_RS  = P1^0;
sbit LCD_E   = P1^1;
sbit LCD_D4  = P1^4;
sbit LCD_D5  = P1^5;
sbit LCD_D6  = P1^6;
sbit LCD_D7  = P1^7;

sbit DHT11   = P2^0;          // DATA line

/* ---------------- Delay Helpers ------------------ */
void delay_us(unsigned int us)        // ˜1 µs per loop @12 MHz
{
    while(us--) { _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); }
}

void delay_ms(unsigned int ms)
{
    while(ms--) delay_us(1000);
}

/* ---------------- LCD Low-level ------------------ */
void lcd_pulse_e(void)
{
    LCD_E = 1; _nop_(); _nop_(); LCD_E = 0;
}

void lcd_send_nibble(unsigned char nib)
{
    LCD_D4 = (nib >> 0) & 1;
    LCD_D5 = (nib >> 1) & 1;
    LCD_D6 = (nib >> 2) & 1;
    LCD_D7 = (nib >> 3) & 1;
    lcd_pulse_e();
}

void lcd_send_byte(unsigned char value, bit is_cmd)
{
    LCD_RS = (is_cmd) ? 0 : 1;
    lcd_send_nibble(value >> 4);
    lcd_send_nibble(value & 0x0F);
    delay_us(50);
}

void lcd_cmd (unsigned char value) { lcd_send_byte(value, 1); }
void lcd_data(unsigned char value) { lcd_send_byte(value, 0); }

void lcd_init(void)
{
    delay_ms(20);                       // LCD power-up
    lcd_send_nibble(0x03); delay_ms(5);
    lcd_send_nibble(0x03); delay_us(100);
    lcd_send_nibble(0x03);
    lcd_send_nibble(0x02);              // 4-bit mode
    lcd_cmd(0x28);                      // 2 lines, 5×8 font
    lcd_cmd(0x0C);                      // display ON, cursor OFF
    lcd_cmd(0x06);                      // entry mode
    lcd_cmd(0x01); delay_ms(2);         // clear
}

void lcd_print(const char *s)
{
    while(*s) lcd_data(*s++);
}

void lcd_print_uint(unsigned int num)   // up to 3 digits
{
    char buf[4]; buf[3] = '\0';
    buf[2] = '0' + (num % 10); num /= 10;
    buf[1] = '0' + (num % 10); num /= 10;
    buf[0] = '0' + (num % 10);
    lcd_print(buf + (buf[0] == '0' ? (buf[1] == '0' ? 2 : 1) : 0));
}

/* ---------------- DHT11 Routines ----------------- */
bit dht11_start(void)
{
    DHT11 = 0;                           // Host LOW
    delay_ms(20);                        // =18 ms
    DHT11 = 1;                           // Release line
    delay_us(30);                        // 20–40 µs

    if(DHT11 == 0) {                     // DHT pulls LOW (80 µs)
        while(DHT11 == 0);               // wait LOW end
        while(DHT11 == 1);               // wait HIGH end
        return 1;                        // response OK
    }
    return 0;                            // no response
}

unsigned char dht11_read_byte(void)
{
    unsigned char i, byte_val = 0;
    for(i = 0; i < 8; i++) {
        while(DHT11 == 0);               // 50 µs LOW
        delay_us(35);                    // sample mid-HIGH
        byte_val <<= 1;
        if(DHT11) byte_val |= 1;         // bit-1 if still HIGH
        while(DHT11);                    // wait end of HIGH
    }
    return byte_val;
}

bit dht11_read(unsigned char *temp, unsigned char *hum)
{
    unsigned char h_int, h_dec, t_int, t_dec, sum;
    if(!dht11_start()) return 0;

    h_int = dht11_read_byte();
    h_dec = dht11_read_byte();
    t_int = dht11_read_byte();
    t_dec = dht11_read_byte();
    sum   = dht11_read_byte();

    if((h_int + h_dec + t_int + t_dec) != sum) return 0;

    *temp = t_int;
    *hum  = h_int;
    return 1;
}

/* ---------------- Main --------------------------- */
void main(void)
{
    unsigned char temp, hum;

    lcd_init();
lcd_cmd(0x80);
    lcd_print("LCD OK");
    while(1)
    {
        if(dht11_read(&temp, &hum)) {
            lcd_cmd(0x80);               // 1st line
            lcd_print("Temp:"); lcd_print_uint(temp); lcd_data('C');
            lcd_cmd(0xC0);               // 2nd line
            lcd_print("Hum :");  lcd_print_uint(hum);  lcd_data('%');
        } else {
            lcd_cmd(0x80); lcd_print("  Sensor Error  ");
            lcd_cmd(0xC0); lcd_print("Check wiring!  ");
        }

        delay_ms(2000);                  // =1 s between reads
    }
}
