// unused
#include "ev3api.h"
#include "app.h"
#include <string.h>

#define min(a,b) (a<b)?a:b
#define max(a,b) (a>b)?a:b
#define chmin(a,b) a=min(a,b)
#define chmax(a,b) a=max(a,b)

const int8_t font = EV3_FONT_SMALL;

int32_t 
print_head_ver = 0, 
print_head_hor = 0, 
font_ver = 0, 
font_hor = 0,
chars_ver = 1,
chars_hor = 1;

void substr(const char* src, int32_t start, int32_t len, char* dest) {
    strncpy(dest, src+start, len);
    return;
}

void init_printscreen() {
    ev3_lcd_set_font(font);
    ev3_font_get_size(font, &font_hor, &font_ver);
    chars_hor = EV3_LCD_WIDTH / font_hor;
    chars_ver = EV3_LCD_HEIGHT / font_ver;
    return;
}

void print(const char* str) {
    int print_head = 0;
    while (print_head < sizeof(str)) {
        int remain = chars_hor - print_head_hor;
        chmin(remain, sizeof(str) - print_head);
        
        if (remain == 0) {
            print_head_hor = 0;
            print_head_ver++;
            continue;
        }

        print_head += remain;
        // get substr
        char msg[remain];
        substr(str, print_head, remain, msg);

        // display
        ev3_lcd_draw_string(msg, print_head_hor * font_hor, print_head_ver * font_ver);
    }
}

void println(const char* str) {
    if (print_head_hor != 0) {
        print_head_ver++;
        print_head_hor = 0;
    }
    print(str);
}