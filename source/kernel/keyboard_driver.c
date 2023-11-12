#include "keyboard_driver.h"
#include "io.h"
#include "terminal_driver.h"
#include <stdbool.h>

bool SHIFT_PRESSED;

void keyboard_init() { SHIFT_PRESSED = false; }

void keyboard_irq(registers_t *frame)
{
    uint8_t scan_code = inb(0x60);
    char output;
    switch (scan_code) {
    case ESCAPE_PRESSED:
        // TODO
        break;
    case ONE_PRESSED:
        output = '1';
        break;
    case TWO_PRESSED:
        output = '2';
        break;
    case THREE_PRESSED:
        output = '3';
        break;
    case FOUR_PRESSED:
        output = '4';
        break;
    case FIVE_PRESSED:
        output = '5';
        break;
    case SIX_PRESSED:
        output = '6';
        break;
    case SEVEN_PRESSED:
        output = '7';
        break;
    case EIGHT_PRESSED:
        output = '8';
        break;
    case NINE_PRESSED:
        output = '9';
        break;
    case ZERO_PRESSED:
        output = '0';
        break;
    case MINUS_PRESSED:
        output = '-';
        break;
    case EQUALS_PRESSED:
        output = '=';
        break;
    case BACKSPACE_PRESSED:
        output = '\b';
        break;
    case TAB_PRESSED:
        break;
    case Q_PRESSED:
        output = 'q';
        break;
    case W_PRESSED:
        output = 'w';
        break;
    case E_PRESSED:
        output = 'e';
        break;
    case R_PRESSED:
        output = 'r';
        break;
    case T_PRESSED:
        output = 't';
        break;
    case Y_PRESSED:
        output = 'y';
        break;
    case U_PRESSED:
        output = 'u';
        break;
    case I_PRESSED:
        output = 'i';
        break;
    case O_PRESSED:
        output = 'o';
        break;
    case P_PRESSED:
        output = 'p';
        break;
    case LEFT_BRACKET_PRESSED:
        output = '[';
        break;
    case RIGHT_BRACKET_PRESSED:
        output = ']';
        break;
    case ENTER_PRESSED:
        output = '\n';
        break;
    case LEFT_CTRL_PRESSED:
        // TODO
        break;
    case A_PRESSED:
        output = 'a';
        break;
    case S_PRESSED:
        output = 's';
        break;
    case D_PRESSED:
        output = 'd';
        break;
    case F_PRESSED:
        output = 'f';
        break;
    case G_PRESSED:
        output = 'g';
        break;
    case H_PRESSED:
        output = 'h';
        break;
    case J_PRESSED:
        output = 'j';
        break;
    case K_PRESSED:
        output = 'k';
        break;
    case L_PRESSED:
        output = 'l';
        break;
    case SEMICOLON_PRESSED:
        output = ';';
        break;
    case SINGLE_QUOTE_PRESSED:
        output = '\'';
        break;
    case BACK_TICK_PRESSED:
        output = '`';
        break;
    case LEFT_SHIFT_PRESSED:
        SHIFT_PRESSED = true;
        break;
    case BACKSLASH_PRESSED:
        output = '\\';
        break;
    case Z_PRESSED:
        output = 'z';
        break;
    case X_PRESSED:
        output = 'x';
        break;
    case C_PRESSED:
        output = 'c';
        break;
    case V_PRESSED:
        output = 'v';
        break;
    case B_PRESSED:
        output = 'b';
        break;
    case N_PRESSED:
        output = 'n';
        break;
    case M_PRESSED:
        output = 'm';
        break;
    case COMMA_PRESSED:
        output = ',';
        break;
    case PERIOD_PRESSED:
        output = '.';
        break;
    case SLASH_PRESSED:
        output = '/';
        break;
    case RIGHT_SHIFT_PRESSED:
        SHIFT_PRESSED = true;
        break;
    case KEYPAD_ASTERISK_PRESSED:
        output = '*';
        break;
    case LEFT_ALT_PRESSED:
        // TODO
        break;
    case SPACE_PRESSED:
        output = ' ';
        break;
    case CAPS_LOCK_PRESSED:
        break;
    case F1_PRESSED:
        break;
    case F2_PRESSED:
        break;
    case F3_PRESSED:
        break;
    case F4_PRESSED:
        break;
    case F5_PRESSED:
        break;
    case F6_PRESSED:
        break;
    case F7_PRESSED:
        break;
    case F8_PRESSED:
        break;
    case F9_PRESSED:
        break;
    case F10_PRESSED:
        break;
    case NUM_LOCK_PRESSED:
        break;
    case SCROLL_LOCK_PRESSED:
        break;
    case KEYPAD_7_PRESSED:
        break;
    case KEYPAD_8_PRESSED:
        break;
    case KEYPAD_9_PRESSED:
        break;
    case KEYPAD_MINUS_PRESSED:
        break;
    case KEYPAD_4_PRESSED:
        break;
    case KEYPAD_5_PRESSED:
        break;
    case KEYPAD_6_PRESSED:
        break;
    case KEYPAD_PLUS_PRESSED:
        break;
    case KEYPAD_1_PRESSED:
        break;
    case KEYPAD_2_PRESSED:
        break;
    case KEYPAD_3_PRESSED:
        break;
    case KEYPAD_0_PRESSED:
        break;
    case KEYPAD_PERIOD_PRESSED:
        break;
    case F11_PRESSED:
        break;
    case F12_PRESSED:
        break;
    case ESCAPE_RELEASED:
        break;
    case ONE_RELEASED:
        break;
    case TWO_RELEASED:
        break;
    case THREE_RELEASED:
        break;
    case FOUR_RELEASED:
        break;
    case FIVE_RELEASED:
        break;
    case SIX_RELEASED:
        break;
    case SEVEN_RELEASED:
        break;
    case EIGHT_RELEASED:
        break;
    case NINE_RELEASED:
        break;
    case ZERO_RELEASED:
        break;
    case MINUS_RELEASED:
        break;
    case EQUALS_RELEASED:
        break;
    case BACKSPACE_RELEASED:
        break;
    case TAB_RELEASED:
        break;
    case Q_RELEASED:
        break;
    case W_RELEASED:
        break;
    case E_RELEASED:
        break;
    case R_RELEASED:
        break;
    case T_RELEASED:
        break;
    case Y_RELEASED:
        break;
    case U_RELEASED:
        break;
    case I_RELEASED:
        break;
    case O_RELEASED:
        break;
    case P_RELEASED:
        break;
    case LEFT_BRACKET_RELEASED:
        break;
    case RIGHT_BRACKET_RELEASED:
        break;
    case ENTER_RELEASED:
        break;
    case LEFT_CTRL_RELEASED:
        break;
    case A_RELEASED:
        break;
    case S_RELEASED:
        break;
    case D_RELEASED:
        break;
    case F_RELEASED:
        break;
    case G_RELEASED:
        break;
    case H_RELEASED:
        break;
    case J_RELEASED:
        break;
    case K_RELEASED:
        break;
    case L_RELEASED:
        break;
    case SEMICOLON_RELEASED:
        break;
    case SINGLE_QUOTE_RELEASED:
        break;
    case BACK_TICK_RELEASED:
        break;
    case LEFT_SHIFT_RELEASED:
        SHIFT_PRESSED = false;
        break;
    case BACKSLASH_RELEASED:
        break;
    case Z_RELEASED:
        break;
    case X_RELEASED:
        break;
    case C_RELEASED:
        break;
    case V_RELEASED:
        break;
    case B_RELEASED:
        break;
    case N_RELEASED:
        break;
    case M_RELEASED:
        break;
    case COMMA_RELEASED:
        break;
    case PERIOD_RELEASED:
        break;
    case SLASH_RELEASED:
        break;
    case RIGHT_SHIFT_RELEASED:
        SHIFT_PRESSED = false;
        break;
    case KEYPAD_ASTERISK_RELEASED:
        break;
    case LEFT_ALT_RELEASED:
        break;
    case SPACE_RELEASED:
        break;
    case CAPS_LOCK_RELEASED:
        break;
    case F1_RELEASED:
        break;
    case F2_RELEASED:
        break;
    case F3_RELEASED:
        break;
    case F4_RELEASED:
        break;
    case F5_RELEASED:
        break;
    case F6_RELEASED:
        break;
    case F7_RELEASED:
        break;
    case F8_RELEASED:
        break;
    case F9_RELEASED:
        break;
    case F10_RELEASED:
        break;
    case NUM_LOCK_RELEASED:
        break;
    case SCROLL_LOCK_RELEASED:
        break;
    case KEYPAD_7_RELEASED:
        break;
    case KEYPAD_8_RELEASED:
        break;
    case KEYPAD_9_RELEASED:
        break;
    case KEYPAD_MINUS_RELEASED:
        break;
    case KEYPAD_4_RELEASED:
        break;
    case KEYPAD_5_RELEASED:
        break;
    case KEYPAD_6_RELEASED:
        break;
    case KEYPAD_PLUS_RELEASED:
        break;
    case KEYPAD_1_RELEASED:
        break;
    case KEYPAD_2_RELEASED:
        break;
    case KEYPAD_3_RELEASED:
        break;
    case KEYPAD_0_RELEASED:
        break;
    case KEYPAD_PERIOD_RELEASED:
        break;
    case F11_RELEASED:
        break;
    case F12_RELEASED:
        break;
    default:
        output = '\0';
    }
    // return if null
    if (output == 0)
        return;
    // convert to upper case
    if (output >= 'a' && output <= 'z' && SHIFT_PRESSED)
        output -= 32;
    printch(output);
}