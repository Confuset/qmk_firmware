#include QMK_KEYBOARD_H
#include "keymap_german.h"

#define MK_KINETIC_SPEED

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
#define _PUQ 0
#define _SYM 1
#define _NAV 2
#define _FK 3
#define _QWZ 4

#define KC_RTAB S(KC_TAB)

#define CTL_ALT OSM(MOD_LCTL | MOD_LALT)
#define OS_LSFT OSM(MOD_LSFT)
#define OS_LCTL OSM(MOD_LCTL)
#define OS_RALT OSM(MOD_RALT)
#define OS_LALT OSM(MOD_LALT)

enum custom_keycodes
{
  ELLIPSIS = SAFE_RANGE,
  DIAKRIT,
  OFF_FK,
  CIRCU,
  OS_MODE,
  D_20,
  D_30,
  D_40,
  D_50,
  HL_R,   //reinforce
  HL_ECB, //eagle cluster bomb
  HL_EAT, //expandble anti tank
};

enum os_mode
{
  WINDOWS,
  LINUX
};

enum os_mode mode = WINDOWS;
uint16_t delay = 0;
uint16_t last_key_pressed = KC_NO;

void keyboard_post_init_user(void)
{
	rgblight_setrgb(RGB_PINK);
}

void tap_key(uint16_t keycode)
{
	switch(mode)
	{
		case WINDOWS:
			tap_code_delay(keycode, delay);
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
		case LINUX:
			rgblight_setrgb(RGB_BLUE);
			delay = 20;
			return WINDOWS;
		case WINDOWS:
		default:
			rgblight_setrgb(RGB_RED);
			delay = 0;
			return LINUX;
	}
}

bool handle_OFF_FK(keyrecord_t* record)
{
	if(record->event.pressed)
	{
		layer_off(_FK);
		layer_on(_NAV);
	}
	else
	{
		layer_off(_NAV);
	}
	return false;
}

bool handle_DIAKRIT(keyrecord_t* record, uint16_t* last_key_pressed)
{
	if(record->event.pressed)
	{
		uint16_t kc = diakrit_key(*last_key_pressed);

		if(kc == KC_NO) return false;

		tap_key(KC_BSPC);
		tap_key(kc);

		*last_key_pressed = kc;

		return false;
	}
	return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
	switch (keycode)
	{
		case OFF_FK:
			return handle_OFF_FK(record);
		case DIAKRIT:
			return handle_DIAKRIT(record, &last_key_pressed);
		case CIRCU:
			if(record->event.pressed)
			{
				tap_key(DE_CIRC);
				tap_key(KC_SPC);
				return false;
			}
			break;
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

		case OS_MODE:
			if(record->event.pressed)
				mode = switch_mode(mode);
			return false;

		case D_20:
			delay = 20;
			return false;
		case D_30:
			delay = 30;
			return false;
		case D_40:
			delay = 40;
			return false;
		case D_50:
			delay = 50;
			return false;

		case KC_RSFT:
			return true;

		case HL_R:
			register_code(KC_LCTL);
			tap_key(KC_UP);
			tap_key(KC_DOWN);
			tap_key(KC_RIGHT);
			tap_key(KC_LEFT);
			tap_key(KC_UP);
			unregister_code(KC_LCTL);

		case HL_ECB:
			register_code(KC_LCTL);
			tap_key(KC_UP);
			tap_key(KC_RIGHT);
			tap_key(KC_DOWN);
			tap_key(KC_DOWN);
			tap_key(KC_RIGHT);
			unregister_code(KC_LCTL);

		case HL_EAT:
			register_code(KC_LCTL);
			tap_key(KC_DOWN);
			tap_key(KC_DOWN);
			tap_key(KC_LEFT);
			tap_key(KC_UP);
			tap_key(KC_RIGHT);
			unregister_code(KC_LCTL);

	}

	//we only react to pressed/down events. otherwise releasing
	//the key would override the 'last_key_pressed'
	if(record->event.pressed)
		last_key_pressed = keycode;

	return true;
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_PUQ] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                                           ┌────────┬────────┬────────┬────────┬────────┬────────┐
     RESET   ,OS_MODE ,D_20    ,D_30    ,D_40    ,D_50    ,                                            XXXXXXX ,XXXXXXX ,XXXXXXX ,XXXXXXX ,XXXXXXX ,RESET   ,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐                         ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     XXXXXXX ,DE_P    ,DE_U    ,DIAKRIT ,DE_COMM ,DE_Q    ,XXXXXXX ,                          XXXXXXX ,DE_G    ,DE_C    ,DE_L    ,DE_M    ,DE_F    ,DF(_QWZ),
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤                         ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     XXXXXXX ,DE_H    ,DE_I    ,KC_E    ,KC_A    ,DE_O    ,XXXXXXX ,                          XXXXXXX ,DE_D    ,KC_T    ,KC_R    ,DE_N    ,DE_S    ,XXXXXXX ,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐       ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     XXXXXXX ,DE_K    ,DE_Y    ,DE_DOT  ,DE_SCLN ,DE_X    ,XXXXXXX ,XXXXXXX ,        XXXXXXX ,XXXXXXX ,DE_J    ,DE_V    ,DE_W    ,DE_B    ,DE_Z    ,XXXXXXX ,
  //├────────┼────────┼────────┼────────┼────┬───┴────┬───┼────────┼────────┤       ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
     XXXXXXX ,XXXXXXX ,XXXXXXX ,XXXXXXX ,     KC_LSFT ,    MO(_NAV),KC_LCTL ,        KC_LALT ,MO(_SYM),    KC_SPC  ,     XXXXXXX ,XXXXXXX ,XXXXXXX ,XXXXXXX
  //└────────┴────────┴────────┴────────┘    └────────┘   └────────┴────────┘       └────────┴────────┘   └────────┘    └────────┴────────┴────────┴────────┘
  ),

  [_SYM] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                                           ┌────────┬────────┬────────┬────────┬────────┬────────┐
     _______ ,_______ ,_______ ,_______ ,_______ ,_______ ,                                            _______ ,_______ ,_______ ,_______ ,_______ ,_______ ,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐                         ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______ ,ELLIPSIS,DE_UNDS ,DE_LBRC ,DE_RBRC ,CIRCU   ,_______ ,                          _______ ,DE_EXLM ,DE_LABK ,DE_RABK ,DE_EQL  ,DE_AMPR ,_______ ,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤                         ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______ ,DE_BSLS ,DE_SLSH ,DE_LCBR ,DE_RCBR ,DE_ASTR ,_______ ,                          _______ ,DE_QUES ,DE_LPRN ,DE_RPRN ,DE_MINS ,DE_COLN ,_______ ,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐       ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______ ,DE_HASH ,DE_DLR  ,DE_PIPE ,DE_TILD ,DE_GRV  ,_______ ,_______ ,        _______ ,_______ ,DE_PLUS ,DE_PERC ,DE_DQUO ,DE_QUOT ,DE_SCLN ,_______ ,
  //├────────┼────────┼────────┼────────┼────┬───┴────┬───┼────────┼────────┤       ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
     _______ ,_______ ,_______ ,_______ ,     KC_RALT ,    _______ ,CTL_ALT ,        _______ ,_______ ,    _______ ,     _______ ,_______ ,_______ ,_______
  //└────────┴────────┴────────┴────────┘    └────────┘   └────────┴────────┘       └────────┴────────┘   └────────┘    └────────┴────────┴────────┴────────┘
  ),

  [_NAV] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                                           ┌────────┬────────┬────────┬────────┬────────┬────────┐
     _______ ,_______ ,_______ ,_______ ,_______ ,_______ ,                                            _______ ,_______ ,_______ ,_______ ,_______ ,_______ ,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐                         ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______ ,KC_PGUP ,KC_BSPC ,KC_UP   ,KC_DEL  ,KC_PGDN ,_______ ,                          _______ ,XXXXXXX ,KC_7    ,KC_8    ,KC_9    ,OSL(_FK) ,_______ ,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤                         ├────────┼────────┼────────┼────────┼────────┼────────┼────────
     _______ ,KC_HOME ,KC_LEFT ,KC_DOWN ,KC_RIGHT,KC_END  ,_______ ,                          _______ ,KC_0    ,KC_4    ,KC_5    ,KC_6    ,TO(_FK) ,_______ ,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐       ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______ ,KC_ESC  ,KC_TAB  ,KC_INS  ,KC_ENT  ,KC_RTAB ,_______ ,_______ ,        _______ ,_______ ,XXXXXXX ,KC_1    ,KC_2    ,KC_3    ,KC_APP  ,_______ ,
  //├────────┼────────┼──r─────┼────────┼────┬───┴────┬───┼────────┼────────┤       ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
     _______ ,_______ ,_______ ,_______ ,     _______ ,    _______ ,_______ ,        _______ ,_______ ,    KC_RWIN ,     _______ ,_______ ,_______ ,_______
  //└────────┴────────┴────────┴────────┘    └────────┘   └────────┴────────┘       └────────┴────────┘   └────────┘    └────────┴────────┴────────┴────────┘
  ),

  [_FK] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                                           ┌────────┬────────┬────────┬────────┬────────┬────────┐
     _______ ,_______ ,_______ ,_______ ,_______ ,_______ ,                                            _______ ,_______ ,_______ ,_______ ,_______ ,_______ ,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐                         ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______ ,XXXXXXX ,KC_WH_U ,KC_MS_U ,KC_WH_D ,XXXXXXX ,_______ ,                          _______ ,KC_F12  ,KC_F7   ,KC_F8   ,KC_F9   ,XXXXXXX ,_______ ,
  //├────────┼────────┼───────e┼────────┼────────┼────────┼────────┤                         ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______ ,KC_BTN1 ,KC_MS_L ,KC_MS_D ,KC_MS_R ,XXXXXXX ,_______ ,                          _______ ,KC_F10  ,KC_F4   ,KC_F5   ,KC_F6   ,XXXXXXX ,_______ ,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐       ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______ ,KC_BTN2 ,XXXXXXX ,XXXXXXX ,XXXXXXX ,XXXXXXX ,_______ ,_______ ,        _______ ,_______ ,KC_F11  ,KC_F1   ,KC_F2   ,KC_F3   ,XXXXXXX ,_______ ,
  //├────────┼────────┼────────┼────────┼────┬───┴────┬───┼────────┼────────┤       ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
     _______ ,_______ ,_______ ,_______ ,      _______,    OFF_FK  ,_______ ,        _______ ,_______ ,    KC_MEH  ,     _______ ,_______ ,_______ ,_______
  //└────────┴────────┴────────┴────────┘    └────────┘   └────────┴────────┘       └────────┴────────┘   └────────┘    └────────┴────────┴────────┴────────┘
  ),

  [_QWZ] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                                           ┌────────┬────────┬────────┬────────┬────────┬────────┐
     RESET   ,KC_ESC  ,DE_1    ,DE_2    ,DE_3    ,DE_4    ,                                            XXXXXXX ,XXXXXXX ,XXXXXXX ,XXXXXXX ,XXXXXXX ,RESET   ,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐                         ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     XXXXXXX ,KC_TAB  ,DE_Q    ,DE_W    ,DE_E    ,DE_R    ,XXXXXXX ,                          XXXXXXX ,XXXXXXX ,DM_REC1 ,KC_UP   ,DM_REC2 ,XXXXXXX ,DF(_PUQ),
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤                         ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     XXXXXXX ,KC_LSFT ,DE_A    ,DE_S    ,DE_D    ,DE_F    ,XXXXXXX ,                          XXXXXXX ,XXXXXXX ,KC_LEFT ,KC_DOWN ,KC_RIGHT,DM_RSTP ,XXXXXXX ,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐       ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     XXXXXXX ,KC_LCTL ,DE_Y    ,DE_X    ,DE_C    ,DE_V    ,DM_PLY1 ,DM_PLY2 ,        XXXXXXX ,XXXXXXX ,XXXXXXX ,XXXXXXX ,XXXXXXX ,XXXXXXX ,XXXXXXX ,XXXXXXX ,
  //├────────┼────────┼────────┼────────┼────┬───┴────┬───┼────────┼────────┤       ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
     XXXXXXX ,XXXXXXX ,XXXXXXX ,XXXXXXX ,     HL_R    ,    HL_ECB  ,HL_EAT  ,        XXXXXXX ,XXXXXXX ,    XXXXXXX      ,XXXXXXX ,XXXXXXX ,XXXXXXX ,XXXXXXX
  //└────────┴────────┴────────┴────────┘    └────────┘   └────────┴────────┘       └────────┴────────┘   └────────┘    └────────┴────────┴────────┴────────┘
  ),
};
