#include "LCD4884.h"
#include "Arduino.h"
#include "Fonts.h"


LCD4884::LCD4884()
{
    for(short int i = 0; i < NUM_KEYS; i++)
    {
        button_count[i] = 0;
        button_status[i] = 0;
        button_flag[i] = 0;
    }

    longPress = false;
};


LCD4884::~LCD4884()
{}



LCD4884 lcd;



void LCD4884::turnBacklightOn(bool switchOn)
{
    if(switchOn == true)
    {
        digitalWrite(LCD_BACKLIGHT, HIGH);
    }

    else
    {
        digitalWrite(LCD_BACKLIGHT, LOW);
    }
}


void LCD4884::init(void)
{
    unsigned int pins[6] = {2, 3, 4, 5, 6, 7};

    for(short int i = 0; i < 6; i++)
    {
        pinMode(pins[i], OUTPUT);
        digitalWrite(pins[i], LOW);
    }

    digitalWrite(LCD_RST, LOW);
    delayMicroseconds(1);
    digitalWrite(LCD_RST, HIGH);

    digitalWrite(SPI_CS, LOW);
    delayMicroseconds(1);
    digitalWrite(SPI_CS, HIGH);
    delayMicroseconds(1);
    digitalWrite(LCD_BACKLIGHT, HIGH);

    writeByte(0x21, 0);
    writeByte(0xc0, 0);
    writeByte(0x06, 0);
    writeByte(0x13, 0);
    writeByte(0x20, 0);
    clear();
    writeByte(0x0c, 0);

    digitalWrite(SPI_CS, LOW);
}

void LCD4884::writeByte(unsigned char data, unsigned char dataType)
{
    digitalWrite(SPI_CS, LOW);

    if(dataType == 0)
    {
        digitalWrite(LCD_DC, LOW);
    }

    else
    {
        digitalWrite(LCD_DC, HIGH);
    }

    for(short int i = 0; i < 8; i++)
    {
        if(data & 0x80)
        {
            digitalWrite(SPI_MOSI, HIGH);
        }
        
        else
        {
            digitalWrite(SPI_MOSI, LOW);
        }

        digitalWrite(SPI_SCK, LOW);
        data = data << 1;
        digitalWrite(SPI_SCK, HIGH);
    }
 
    digitalWrite(SPI_CS, HIGH);
}


void LCD4884::writeCharBig(unsigned char x, unsigned char y, unsigned char ch, char mode)
{
    unsigned char i, j;
    unsigned char *pFont;
    unsigned char ch_dat;

    pFont = (unsigned char*)big_number;

    if(ch == '.')
    {
        ch = 10;
    }

    else if(ch == '+')
    {
        ch = 11;
    }

    else if(ch == '-')
    {
        ch = 12;
    }

    else
    {
        ch = ch & 0x0F;
    }

    for(i = 0; i < 3; i++)
    {
        setCursorPosition(x, y + i);

        for(j = 0; j < 16; j++)
        {
            ch_dat = pgm_read_byte(pFont + ch * 48 + i * 16 + j);
            writeByte((mode == MENU_NORMAL) ? ch_dat : (ch_dat ^ 0xFF), 1);
        }
    }
}


void LCD4884::writeChar(unsigned char c, char mode)
{
    unsigned char line;
    unsigned char *pFont;
    byte ch;

    pFont = (unsigned char*)font6_8;
    c -= 32;

    for(line = 0; line < 6; line++)
    {
        ch = pgm_read_byte(pFont + c * 6 + line);
        writeByte((mode == MENU_NORMAL) ? ch : (ch ^ 0xff), 1);
    }
}


void LCD4884::setCursorPosition(unsigned char x, unsigned char y)
{
    writeByte(0x40 | y, 0);
    writeByte(0x80 | x, 0);
}


void LCD4884::clear(void)
{
    writeByte(0x0C, 0);
    writeByte(0x80, 0);

    for(short int i = 0; i < 504; i++)
    {
        writeByte(0, 1);
    }
}


void LCD4884::writeString(unsigned char X, unsigned char Y, const char *s, char mode)
{
    setCursorPosition(X, Y);

    while(*s)
    {
        writeChar(*s, mode);
        s++;
    }
}


void LCD4884::writeStringBig(unsigned char x, unsigned char y, const char *string, char mode)
{
    while(*string)
    {
        writeCharBig(x, y, *string, mode);

        if(*string++ == '.')
        {
            x += 5;
        }

        else
        {
            x += 12;
        }
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


void LCD4884::waitOKkey(void)
{
    short int key = 0xFF, i;

    updateButtonStatus();

    while(key != CENTER_KEY)
    {
        for(i = 0; i < NUM_KEYS; i++)
        {
            if(button_flag[i] != 0)
            {
                button_flag[i] = 0;

                if(i == CENTER_KEY)
                {
                    key = CENTER_KEY;
                }
            }

            updateButtonStatus();
        }
    }
}


char LCD4884::getKey(unsigned int input)
{
    short int k;
    static short int adc_key_val[NUM_KEYS] = {50, 200, 400, 600, 800};

    for(k = 0; k < NUM_KEYS; k++)
    {
        if(input < adc_key_val[k])
        {
            return k;
        }
    }

    return -1;
}


void LCD4884::updateButtonStatus(void)
{
    char key_in = getKey(analogRead(0));

    for(short int i = 0; i < NUM_KEYS; i++)
    {
        if(key_in == i)
        {
            if(button_count[i] < DEBOUNCE_MAX)
            {
                button_count[i]++;

                if(button_count[i] > DEBOUNCE_ON)
                {
                    if(button_status[i] == 0)
                    {
                        button_flag[i] = 1;
                        button_status[i] = 1;
                    }
                }
            }
        }

        else
        {
            if(button_count[i] > 0)
            { 
                button_flag[i] = 0;
                button_count[i]--;

                if(button_count[i] < DEBOUNCE_OFF)
                {
                    button_status[i] = 0;
                }
            }
        }
    }
}


bool LCD4884::getLongPress(void)
{
    return longPress;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


void LCD4884::initClass(const char *name, int maxElements, int minElements)
{
    lcd.init();
    lcd.clear();
    lcd.turnBacklightOn(false);

    max_menu_item = maxElements - 1;
    min_menu_item = minElements;
    current_menu_item = 0;
    
    nameSize = strlen(name);
    strcpy(menuName, name);
}


void LCD4884::showMenu(const char menuList[][NBCHAR_X])
{
    clear();
    writeString(CENTER(menuName), 0, menuName, MENU_NORMAL);

    for(short int i = 0; i <= max_menu_item - min_menu_item ; i++)
    {
        writeString(CENTER(menuList[i]), OFFSET_Y + i, menuList[i], MENU_NORMAL);
    }

    writeString(CENTER(menuList[current_menu_item]), OFFSET_Y + current_menu_item, menuList[current_menu_item], MENU_HIGHLIGHT);
}


void LCD4884::browseMenu(const char menuList[][NBCHAR_X], FONCTION *menuFunction)
{
    static unsigned long int timer = 0;

    longPress = false;

    updateButtonStatus();

    if(millis() - timer > TIMEBACKLIGHT)
    {
        turnBacklightOn(false);
        timer = 0;
    }

    for(short int i = 0; i < NUM_KEYS; i++)
    {
        if(button_flag[i] != 0)
        {
            button_flag[i] = 0;

            turnBacklightOn(true);
            timer = millis();

            switch(i)
            {
                case UP_KEY:
                    writeString(CENTER(menuList[current_menu_item]), OFFSET_Y + current_menu_item, menuList[current_menu_item], MENU_NORMAL);

                    current_menu_item--;

                    if(current_menu_item < min_menu_item)
                    {
                        current_menu_item = max_menu_item;
                    }

                    writeString(CENTER(menuList[current_menu_item]), OFFSET_Y + current_menu_item, menuList[current_menu_item], MENU_HIGHLIGHT);
                    break;

                case LEFT_KEY:
                    writeString(CENTER(menuList[current_menu_item]), OFFSET_Y + current_menu_item, menuList[current_menu_item], MENU_NORMAL);

                    current_menu_item = min_menu_item;

                    writeString(CENTER(menuList[current_menu_item]), OFFSET_Y + current_menu_item, menuList[current_menu_item], MENU_HIGHLIGHT);
                    break;

                case RIGHT_KEY:
                    writeString(CENTER(menuList[current_menu_item]), OFFSET_Y + current_menu_item, menuList[current_menu_item], MENU_NORMAL);

                    current_menu_item = max_menu_item;

                    writeString(CENTER(menuList[current_menu_item]), OFFSET_Y + current_menu_item, menuList[current_menu_item], MENU_HIGHLIGHT);
                    break;

                case DOWN_KEY:
                    writeString(CENTER(menuList[current_menu_item]), OFFSET_Y + current_menu_item, menuList[current_menu_item], MENU_NORMAL);

                    current_menu_item++;

                    if(current_menu_item > max_menu_item)
                    {
                        current_menu_item = min_menu_item;
                    }

                    writeString(CENTER(menuList[current_menu_item]), OFFSET_Y + current_menu_item, menuList[current_menu_item], MENU_HIGHLIGHT);
                    break;

                case CENTER_KEY:
                    do
                    {
                        updateButtonStatus();

                    } while(button_status[CENTER_KEY] != 0 && millis() - timer < TIMELONGPRESS);

                    if(millis() - timer < TIMELONGPRESS)
                    {
                        clear();
                        lcd.writeString(CENTER("OK"), 5, "OK", MENU_HIGHLIGHT);

                        (*menuFunction[current_menu_item])();

                        timer = millis();
                        button_status[CENTER_KEY] = 0;

                        do
                        {
                            updateButtonStatus();

                        } while(button_status[CENTER_KEY] == 0 && millis() - timer < TIMEINFCT);

                        if(button_status[CENTER_KEY] != 0)
                        {
                            timer = millis();
                        }
                        
                        showMenu(menuList);
                        i = NUM_KEYS;
                    }

                    else
                    {
                        longPress = true;
                        timer = millis();
                    }
                    break;

                default:
                    break;
            }
        }
    }
}
