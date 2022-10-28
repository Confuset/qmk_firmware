#include QMK_KEYBOARD_H
#include "keymap_german.h"

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
#define _PUQ 0
#define _SYMB 1
#define _NAV 2
#define _SYMFK 3

#define SYM_L   MO(_SYMB)
#define ESC_NAV_L   LT(_NAV, KC_ESCAPE)
#define ENT_FK_L LT(_SYMFK, KC_ENTER)
#define SPC_MEH MEH_T(KC_SPACE)
#define SKC_RWIN S(KC_RWIN)
#define KC_RTAB S(KC_TAB)
#define A_LGUI LGUI_T(KC_A)
#define T_RGUI RGUI_T(KC_T)
#define E_LALT LALT_T(KC_E)
#define R_RALT RALT_T(KC_R)

enum custom_keycodes
{
  ELLIPSIS = SAFE_RANGE,
  DIAKRIT,
  OS_MODE
};

enum os_mode
{
  WINDOWS,
  LINUX
};

enum os_mode mode = WINDOWS;
uint16_t last_key_pressed = KC_NO;

void tap_key(uint16_t keycode)
{
	switch(mode)
	{
		case WINDOWS:
			tap_code_delay(keycode, 20);
			break;
		case LINUX:
			tap_code(keycode);
			break;
	}
}

uint16_t diakrit_key(uint16_t oldKey)
{
	switch(oldKey)
	{
		case DE_A: return DE_ADIA;
		case DE_U: return DE_UDIA;
		case DE_O: return DE_ODIA;
		case DE_S: return DE_SS;
		default: return KC_NO;
	}
}

enum os_mode switch_mode(enum os_mode mode)
{
	switch (mode)
	{
		default:
		case WINDOWS: return LINUX;
		case LINUX: return WINDOWS;
	}
}

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
	switch (keycode)
	{
		case ELLIPSIS:
			if(record->event.pressed)
			{
				switch (mode)
				{
					case WINDOWS:
						register_code(KC_LALT);
						tap_key(KC_P0);
						tap_key(KC_P1);
						tap_key(KC_P3);
						tap_key(KC_P3);
						unregister_code(KC_LALT);
						return false;

					case LINUX:
						register_code(KC_RALT);
						tap_code(DE_DOT);
						unregister_code(KC_RALT);
						return false;
				}
			}
			break;

		case DIAKRIT:
			if(record->event.pressed)
			{
				uint16_t kc = diakrit_key(last_key_pressed);

				if(kc == KC_NO) return false;

				tap_key(KC_BSPC);
				tap_key(kc);

				last_key_pressed = kc;

				return false;
			}
			return true;

		case OS_MODE:
			if(record->event.pressed)
				mode = switch_mode(mode);
			return false;

		case KC_RSFT:
			return true;

	}

	//we only react to pressed/down events. otherwise releasing
	//the key would override the 'last_key_pressed'
	if(record->event.pressed)
		last_key_pressed = keycode;

	return true;
};

/*qk_tap_dance_action_t tap_dance_actions[] = {};*/

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t* record)
{
	switch(keycode){
		case ENT_FK_L:
		case ESC_NAV_L:
		case SPC_MEH:
			return true;
		default:
			return false;
	}
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_PUQ] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                                           ┌────────┬────────┬────────┬────────┬────────┬────────┐
     RESET   ,OS_MODE ,XXXXXXX ,XXXXXXX ,XXXXXXX ,XXXXXXX ,                                            XXXXXXX ,XXXXXXX ,XXXXXXX ,XXXXXXX ,XXXXXXX ,XXXXXXX ,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐                         ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     XXXXXXX ,DE_P    ,DE_U    ,DIAKRIT ,DE_COMM ,DE_Q    ,XXXXXXX ,                          XXXXXXX ,DE_G    ,DE_C    ,DE_L    ,DE_M    ,DE_F    ,XXXXXXX ,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤                         ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     XXXXXXX ,DE_H    ,DE_I    ,KC_E    ,KC_A    ,DE_O    ,XXXXXXX ,                          XXXXXXX ,DE_D    ,KC_T  ,KC_R      ,DE_N    ,DE_S    ,XXXXXXX ,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐       ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     XXXXXXX ,DE_K    ,DE_Y    ,DE_DOT  ,DE_SCLN ,DE_X    ,XXXXXXX ,XXXXXXX ,        XXXXXXX ,XXXXXXX  ,DE_J    ,DE_V    ,DE_W    ,DE_B    ,DE_Z    ,XXXXXXX ,
  //├────────┼────────┼────────┼────────┼────┬───┴────┬───┼────────┼────────┤       ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
     XXXXXXX ,XXXXXXX ,XXXXXXX ,XXXXXXX ,     KC_LSFT ,   ESC_NAV_L,KC_LCTL ,        ENT_FK_L,SYM_L   ,    SPC_MEH ,     XXXXXXX ,XXXXXXX  ,XXXXXXX ,XXXXXXX
  //└────────┴────────┴────────┴────────┘    └────────┘   └────────┴────────┘       └────────┴────────┘   └────────┘    └────────┴────────┴────────┴────────┘
  ),

  [_SYMB] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                                           ┌────────┬────────┬────────┬────────┬────────┬────────┐
     _______ ,_______ ,_______ ,_______ ,_______ ,_______ ,                                            _______ ,_______ ,_______ ,_______ ,_______ ,_______ ,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐                         ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______ ,ELLIPSIS,DE_UNDS ,DE_LBRC ,DE_RBRC ,DE_CIRC ,_______ ,                          _______ ,DE_EXLM ,DE_LABK ,DE_RABK ,DE_EQL  ,DE_AMPR ,_______ ,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤                         ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______ ,DE_BSLS ,DE_SLSH ,DE_LCBR ,DE_RCBR ,DE_ASTR ,_______ ,                          _______ ,DE_QUES ,DE_LPRN ,DE_RPRN ,DE_MINS ,DE_COLN ,_______ ,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐       ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______ ,DE_HASH ,DE_DLR  ,DE_PIPE ,DE_TILD ,DE_GRV  ,_______ ,_______ ,        _______ ,_______ ,DE_PLUS ,DE_PERC ,DE_DQUO ,DE_QUOT ,DE_SCLN ,_______ ,
  //├────────┼────────┼────────┼────────┼────┬───┴────┬───┼────────┼────────┤       ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
     _______ ,_______ ,_______ ,_______ ,     _______ ,    _______ ,_______ ,        _______ ,_______ ,    _______ ,     _______ ,_______ ,_______ ,_______
  //└────────┴────────┴────────┴────────┘    └────────┘   └────────┴────────┘       └────────┴────────┘   └────────┘    └────────┴────────┴────────┴────────┘
  ),

  [_NAV] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                                           ┌────────┬────────┬────────┬────────┬────────┬────────┐
     _______ ,_______ ,_______ ,_______ ,_______ ,_______ ,                                            _______ ,_______ ,_______ ,_______ ,_______ ,_______ ,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐                         ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______ ,KC_PGUP ,KC_BSPC ,KC_UP   ,KC_DEL  ,KC_PGDN ,_______ ,                          _______ ,XXXXXXX ,KC_7    ,KC_8    ,KC_9    ,XXXXXXX ,_______ ,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤                         ├────────┼────────┼────────┼────────┼────────┼────────┼────────
     _______ ,KC_HOME ,KC_LEFT ,KC_DOWN ,KC_RIGHT,KC_END  ,_______ ,                          _______ ,KC_0    ,KC_4    ,KC_5    ,KC_6    ,XXXXXXX ,_______ ,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐       ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______ ,KC_ESC  ,KC_TAB  ,KC_INS  ,KC_ENT  ,KC_RTAB ,_______ ,_______ ,        _______ ,_______ ,XXXXXXX ,KC_1    ,KC_2    ,KC_3    ,XXXXXXX ,_______ ,
  //├────────┼────────┼──r─────┼────────┼────┬───┴────┬───┼────────┼────────┤       ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
     _______ ,_______ ,_______ ,_______ ,     _______ ,    _______ ,_______ ,        SKC_RWIN,KC_RWIN ,    KC_0    ,     _______ ,_______ ,_______ ,_______
  //└────────┴────────┴────────┴────────┘    └────────┘   └────────┴────────┘       └────────┴────────┘   └────────┘    └────────┴────────┴────────┴────────┘
  ),

  [_SYMFK] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                                           ┌────────┬────────┬────────┬────────┬────────┬────────┐
     _______ ,_______ ,_______ ,_______ ,_______ ,_______ ,                                            _______ ,_______ ,_______ ,_______ ,_______ ,_______ ,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐                         ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______ ,RGB_M_P ,RGB_TOG ,RGB_MOD ,RGB_HUD ,RGB_HUI ,_______ ,                          _______ ,KC_F12  ,KC_F7   ,KC_F8   ,KC_F9   ,XXXXXXX ,_______ ,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤                         ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______ ,XXXXXXX ,XXXXXXX ,XXXXXXX ,XXXXXXX ,XXXXXXX ,_______ ,                          _______ ,KC_F10  ,KC_F4   ,KC_F5   ,KC_F6   ,XXXXXXX ,_______ ,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐       ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______ ,XXXXXXX ,XXXXXXX ,XXXXXXX ,XXXXXXX ,XXXXXXX ,_______ ,_______ ,        _______ ,_______ ,KC_F11  ,KC_F1   ,KC_F2   ,KC_F3   ,XXXXXXX ,_______ ,
  //├────────┼────────┼────────┼────────┼────┬───┴────┬───┼────────┼────────┤       ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
     _______ ,_______ ,_______ ,_______ ,     _______ ,    _______ ,_______ ,        _______ ,_______ ,    _______ ,     _______ ,_______ ,_______ ,_______
  //└────────┴────────┴────────┴────────┘    └────────┘   └────────┴────────┘       └────────┴────────┘   └────────┘    └────────┴────────┴────────┴────────┘
  ),
};
