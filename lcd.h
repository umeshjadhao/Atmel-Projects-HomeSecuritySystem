#ifndef lcd_H
#define lcd_H


#define line1 0x80
#define line2 0xC0
#define EN 3
//#define RW 6
#define RS 2

//#define RW_INIT()	{PORTD|=(1<<RW);PORTD&=~(1<<RS);DDRC&=PORTC&=~(1<<7);}

void lcd_config(void);
void lcd_init(void);
void lcd_en_tgl (void);
void lcd_cmd(unsigned char letter);
void lcd_char(unsigned char letter);
void lcd_nibble(unsigned char nib,unsigned char rs);
void lcd_clr_line(unsigned char line);
void lcd_string(unsigned char *senpoint, unsigned char line);
int lcd_position_char(unsigned char *senpoint,unsigned char line,unsigned char position);
void lcd_scroll_1(unsigned char *s);
void lcd_write_int(int val,unsigned int field_length);
#endif
