#include <avr/io.h>
#define F_CPU 11059200UL
#include <avr/interrupt.h>
#include <util/delay.h>
#define F_CPU 11059200UL
#define RS 7 // выбор регистра
#define E 6  // строб передачи
uint8_t sec = 0;
uint8_t  i = 0, j = 0;

void timer_ini(void);
void LCD_cmd(uint8_t cmd);
void LCD_init(void);
uint8_t Code(uint8_t symb);
void LCD_data(char data);


char color[7][6]={"Red   ","Orange","Yellow","Green ","Blue  ","Violet"};

uint8_t TabCon[] = {0x41, 0xA0, 0x42, 0xA1, 0xE0, 0x45, 0xA3, 0xA4,
0xA5, 0xA6, 0x4B, 0xA7, 0x4D, 0x48, 0x4F, 0xA8, 0x50, 0x43, 0x54, 0xA9,
0xAA, 0x58, 0xE1, 0xAB, 0xAC, 0xE2, 0xAD, 0xAE, 0x62, 0xAF, 0xB0, 0xB1,
0x61, 0xB2, 0xB3, 0xB4, 0xE3, 0x65, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB,
0xBC, 0xBD, 0x6F, 0xBE, 0x70, 0x63, 0xBF, 0x79, 0x5C, 0x78, 0xE5, 0xC0,
0xC1, 0xE6, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7};


void timer_ini(void) //нам больше не нужен делей
{	
	//таймер для смены картинки
	TIMSK = (1<<TOIE1); //разрешение использования счетчика для прерываний
	TCCR1B = (1<<CS12); //используем делитель частоты, итог 43200
    // один тик примерно 0.02315 мс
    // 1 секунда это 43 196 тика
	TCNT1 = 65535 - 43196; //initial value for timer to start counting 
    // max 65535

	
}

ISR (TIMER1_OVF_vect) //смена рисуемого столбцаи и вывод картинки
{
	TCNT1 = 65535 - 43196;
    sec ++;
    if (sec>=3)
        sec = 0;
        i = (i+1)%6;
        LCD_cmd((1<<7) |0);
        for (j=0;j<=5;j++){
            LCD_data(Code(color[i][j]));
        }
	//действие по таймеру 1
}

ISR (INT2_vect) //смена светодиода
{
    
}
    
    


void LCD_cmd(uint8_t cmd)
{
//[]--------------------------------------------------[]
//| Назначение: запись кодов в регистр команд ЖКИ |
//| Входные параметры: cmd - код команды |
//[]--------------------------------------------------[]
DDRC = 0xFF;                    // все разряды PORTC на выход
DDRD |= ((1 << E) | (1 << RS)); // разряды PORTD на выход
PORTD &= ~(1 << RS);            // выбор регистра команд RS=0
PORTC = cmd;                    // записать команду в порт PORTC
PORTD |= (1 << E);              // \ сформировать на
_delay_us(5);                   // | выводе E строб 1-0
PORTD &= ~(1 << E);             // / передачи команды
_delay_ms(5);                   // задержка для завершения записи
}

void LCD_init(void)
{
//[]--------------------------------------------------[]
//| Назначение: инициализация ЖКИ |
//[]--------------------------------------------------[]
DDRC = 0xFF;                    // все разряды PORTC на выход
DDRD |= ((1 << E) | (1 << RS)); // разряды PORTD на выход
_delay_ms(100);                 // задержка для установления напряжения питания
LCD_cmd(0x30);                  // \ вывод
LCD_cmd(0x30);                  // | трех
LCD_cmd(0x30);                  // / команд 0x30
LCD_cmd(0x38);                  // 8 разр.шина, 2 строки, 5 ? 7 точек
LCD_cmd(0x0E);                  // включить ЖКИ и курсор, без мерцания
LCD_cmd(0x06);                  // инкремент курсора, без сдвига экрана
LCD_cmd(0x01);                  // очистить экран, курсор в начало
}


uint8_t Code(uint8_t symb)
{
//[]------------------------------------------------[]
//| Назначение: перекодировка символов кириллицы |
//| Входные параметры: symb – символ ASCII |
//| Функция возвращает код отображения символа |
//[]------------------------------------------------[]
uint8_t a = symb >= 192 ? TabCon[symb - 192] : symb;
return a;
}

void LCD_data(char data)
{
DDRC = 0xFF;                    // все разряды PORTC на выход
DDRD |= ((1 << E) | (1 << RS)); // разряды PORTD на выход
PORTD |= (1 << RS);             // выбор регистра data RS=1
PORTC = data;                   // записать команду в порт PORTC
PORTD |= (1 << E);              // \ сформировать на
_delay_us(5);                   // | выводе E строб 1-0
PORTD &= ~(1 << E);             // / передачи команды
_delay_ms(1);                   // задержка для завершения записи
}

int main(void){
    LCD_init();
    timer_ini();
    sei();
    
    DDRE = (1<<4)|(1<<5);
    PORTE = (1<<4)|(1<<5);
    while (1)
    {   
    
    }

}