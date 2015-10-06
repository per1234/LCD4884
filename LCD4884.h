#ifndef LCD4884_H
#define LCD4884_H


#define SPI_SCK        2
#define SPI_MOSI       3
#define LCD_DC         4
#define SPI_CS         5
#define LCD_RST        6
#define LCD_BACKLIGHT  7

#define MENU_NORMAL    0
#define MENU_HIGHLIGHT 1
#define OFF            0
#define ON             1

#define DEBOUNCE_MAX  15
#define DEBOUNCE_ON   10
#define DEBOUNCE_OFF   3
#define NUM_KEYS       5

#define LEFT_KEY       0
#define CENTER_KEY     1
#define DOWN_KEY       2
#define RIGHT_KEY      3
#define UP_KEY         4

#define LEFT_K_A    741
#define CENTER_K_A  143
#define DOWN_K_A      0
#define RIGHT_K_A   328
#define UP_K_A      502
#define K_RELEAS_A 1022

#define NBCHAR_X      14
#define OFFSET_Y       2
#define NB_PIX_X       6

#define TIMEBACKLIGHT 5000
#define TIMELONGPRESS 2500
#define TIMEINFCT     8000


#define CENTER(ELEMENT) ((NBCHAR_X - strlen(ELEMENT)) / 2) * NB_PIX_X


typedef void (*FONCTION)();


class LCD4884
{
    public:

        LCD4884();

        void init(void);
        void turnBacklightOn(bool switchOn);
        void writeByte(unsigned char data, unsigned char dataType);
        void drawBmpPixel(unsigned char x, unsigned char y, unsigned char *map, unsigned char pixelX, unsigned char pixelY);
        void writeString(unsigned char x, unsigned char y, const char *s, char mode);
        void writeStringBig(unsigned char x, unsigned char y, const char *string, char mode);
        void writeCharBig(unsigned char x, unsigned char y, unsigned char ch, char mode);
        void writeChar(unsigned char c, char mode);
        void setCursorPosition(unsigned char x, unsigned char y);
        void clear(void);


        void waitOKkey(void);
        void updateButtonStatus(void);
        char getKey(unsigned int input);

        bool getLongPress(void);

        void initClass(const char *name, int maxElements, int minElements = 0);
        void showMenu(const char menuList[][NBCHAR_X]);
        void browseMenu(const char menuList[][NBCHAR_X], FONCTION *menuFunction);

        ~LCD4884();


    private:

        bool longPress;
        short int button_count[NUM_KEYS];
        short int button_status[NUM_KEYS];
        short int button_flag[NUM_KEYS];

        int nameSize;
        char menuName[NBCHAR_X];
        short int current_menu_item;
        short int max_menu_item;
        short int min_menu_item;


    protected:
};


extern LCD4884 lcd;


#include <avr/pgmspace.h>

#endif
