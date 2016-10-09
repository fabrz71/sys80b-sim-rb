/* *** Gottileb System 80/B SIM PRB ***
* (SIMulation Pinball Replacement control Board)
* software for Teensy 3.x board developed with Arduino IDE
* under GNU General Public Livence v3
* ---
* SYSTEM/80B MENU USER INTERFACE
* ---
* Pinball menu user interface implementation.
*/

// User Interface Menu

#include <string.h>

// Note: each menu can't contain more than 15 items !

#define MAX_MENU_DEPTH 4
#define LAMP_TEST_BLINK_P 333
#define SOL_TEST_ON_TIME 500

#define _M_END {NULL, NULL, 0xff, false}

enum menuEvent { MENU_BACK, MENU_NEXT, MENU_ENTER };

PROGMEM const byte credits_ratio[25] = { 1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2,1,3,1 };
PROGMEM const byte coins_ratio[25] =   { 1,1,1,1,1,1,1,1,1, 1,2,2,2,2,2,2,2,2,2, 2,3,3,4,4,5 };
PROGMEM const byte max_credits[] = {8, 10, 15, 20};

const __FlashStringHelper *mainMenuName = F("MAIN MENU");

struct menuItem {
  const __FlashStringHelper *text; // item name
  const menuItem *subMenu; // optional sub menu (NULL when void)
  const byte id; // optional id (for settings retrieval)
  const bool isSetting;
};

char mbuf[30];
menuItem *currentMenu;
char *menuName, *itemName;
byte mdepth, mitem, itemId, menuId;
byte mpath[MAX_MENU_DEPTH];
bool _itemIsSubMenu;
bool _menuIsDynamic;
byte settVal;
bool _menuTaskExecution;
extern Light light[];

void initMenu();
void execMenuCmd(menuEvent me);
void _nextItem();
void _home();
void __updItemVars();
void _menuForward(bool rememberPath);
void _menuBack();
void _updMenuDisplay();
byte _getSettingsItem(byte iid);
void _setSettingsItem(byte iid, byte val);
void _doMenuItem(byte iid, byte val);
char *_getSpecialItemName(byte &mid, byte &n);
extern bool isSpecialLamp(byte lamp);

// MENU ITEMS

const menuItem _dynamicItems[] = { _M_END }; // special tag for dynamic items

const menuItem noYesItems[] = {
      { F("NO (DISABLED)"), NULL, 0, false },
      { F("YES (ENABLED)"), NULL, 0, false },
      _M_END };

const menuItem specialItems[] = {
      { F("SPECIAL"), NULL, 0, false },
      { F("EXTRA BALL"), NULL, 0, false },
      _M_END };

const menuItem ballsItems[] = {
      { F("3 BALLS"), NULL, 0, false },
      { F("5 BALLS"), NULL, 0, false },
      _M_END };

const menuItem captBallItems[] = {
      { F("RETURNED AFTER EACH BALL"), NULL, 0, false },
      { F("RETAINED AFTER EACH BALL"), NULL, 0, false },
      _M_END };

const menuItem replayItems[] = {
      { F("UNLIMITED"), NULL, 0, false },
      { F("ONCE ONLY"), NULL, 0, false },
      _M_END };

const menuItem gameModeItems[] = {
      { F("REPLAY"), NULL, 0, false },
      { F("EXTRA BALL"), NULL, 0, false },
      _M_END };

const menuItem _3rdCoinCtrlItems[] = {
      { F("NO EFFECT"), NULL, 0, false },
      { F("ADD 9"), NULL, 0, false },
      _M_END };

const menuItem hsRewardItems[] = {
      { F("NONE"), NULL, 0, false },
      { F("1 REPLAY"), NULL, 0, false },
      { F("2 REPLAY"), NULL, 0, false },
      { F("3 REPLAY"), NULL, 0, false },
      _M_END };

// TOP LEVEL MENUS

const menuItem pinTestsMenu[] = { // pinball tests menu
      { F("LAMPS TEST"), _dynamicItems, 0x21, false },
      { F("SPECIAL LAMPS TEST"), _dynamicItems, 0x22, false },
      { F("SOLENOID TEST"), _dynamicItems, 0x23, false },
      { F("SOUND TEST"), _dynamicItems, 0x24, false },
      { F("SWITCHES TEST"), NULL, 0x25, false },
      { F("DISPLAY TEST"), NULL, 0x26, false },
      _M_END };

const menuItem bookkMenu[] = { // bookkeeping menu
      { F("LEFT CHUTE COINS"), NULL, 0x31, false },
      { F("CENTER CHUTE COINS"), NULL, 0x32, false },
      { F("RIGHT CHUTE COINS"), NULL, 0x33, false },
      { F("TOTAL PLAYS"), NULL, 0x34, false },
      { F("TOTAL REPLAYS"), NULL, 0x35, false },
      { F("REPLAY PERCENTAGE"), NULL, 0x36, false },
      { F("EXTRA BALLS"), NULL, 0x37, false },
      { F("TOTAL TILTS"), NULL, 0x38, false },
      { F("SPECIALS"), NULL, 0x39, false },
      { F("TIMES HIGHEST SCORE AWARDED"), NULL, 0x3a, false },
      { F("FIRST HIGH SCORE LEVEL"), NULL, 0x3b, false },
      { F("SECOND HIGH SCORE LEVEL"), NULL, 0x3c, false },
      { F("THIRD HIGH SCORE LEVEL"), NULL, 0x3d, false },
      { F("HIGHEST SCORE"), NULL, 0x3e, false },
      { F("AVERAGE PLAYING TIME"), NULL, 0x3f, false },
      _M_END };

const menuItem prbTestsItems[] = { // PRB tests menu
      { F("RETURNS TEST"), NULL, 0x41, false },
      { F("DIGITAL OUTPUTS TEST"), NULL, 0x42, false },
      { F("DIGITAL INPUTS TEST"), NULL, 0x43, false },
      { F("LED GRID TEST"), NULL, 0x44, false },
      _M_END };

const menuItem prbSettItems[] = { // PRB settings menu
      { F("ACTIVITY OUTPUT"), NULL, 0x51, false },
      { F("GAME INFO"), NULL, 0x52, false },
      { F("PRB VERSION"), NULL, 0x53, false },
      _M_END };

const menuItem pinSettMenu[] = { // pinball settings menu
      { F("LEFT COINS/CREDIT"), _dynamicItems, 0x11, true },
      { F("CENTER COINS/CREDIT"), _dynamicItems, 0x12, true },
      { F("RIGHT COINS/CREDIT"), _dynamicItems, 0x13, true },
      { F("RESET HIGH SCORES #2-#5 AT POWER OFF"), noYesItems, 0x14, true },
      { F("ATTRACT MODE SOUND"), noYesItems, 0x15, true },
      { F("AUTO PERCENTAGE MODE"), noYesItems, 0x16, true },
      { F("CAPTURED BALL CTRL"), captBallItems, 0x17, true },
      { F("MAXIMUM CREDITS"), _dynamicItems, 0x18, true },
      { F("PLAYFILED SPECIAL"), specialItems, 0x19, true },
      { F("HIGHEST SCORE AWARD"), hsRewardItems, 0x1a, true },
      { F("BALLS PER GAME"), ballsItems, 0x1b, true },
      { F("MATCH MODE"), noYesItems, 0x1c, true },
      { F("REPLAY LIMIT"), replayItems, 0x1d, true },
      { F("GAME MODE"), gameModeItems, 0x1e, true },
      { F("3RD COIN CHUTE CTRL"), _3rdCoinCtrlItems, 0x1f, true },
      _M_END };

const menuItem mainMenu[] = { // main menu
    { F("PINBALL SETTINGS"), pinSettMenu, 1, false },
    { F("PINBALL TESTS"), pinTestsMenu, 2, false},
    { F("BOOKKEEPING"),  bookkMenu, 3, false},
    { F("PRB TESTS"), prbTestsItems, 4, false },
    { F("PRB SETTINGS"), prbSettItems, 5, false },
    _M_END };

void initMenu() {
  _home();
  _menuTaskExecution = false;
  setDisplayBlink(0, false);
  setDisplayBlink(1, false);
  _updMenuDisplay();
}

void execMenuCmd(menuEvent me) {
  switch (me) {
    case MENU_BACK:
      _menuBack();
      break;
    case MENU_NEXT:
      _nextItem();
      break;
    case MENU_ENTER:
      if (_itemIsSubMenu) _menuForward(true);
      else _doMenuItem(menuId, mitem);
      break;
    default:
      return;
  }
  _updMenuDisplay();
}

// updates attributes: _dymanicMenu [, currentMenu, menuName]
// based on: currentMenu
void _home() {
  mdepth = 0;
  menuId = 0;
  currentMenu = (menuItem*)mainMenu;
  menuName = (char*)mainMenuName;
  _menuIsDynamic = false;
  mitem = 0;
  settVal = 0xff;
  __updItemVars();
}

// updates attributes: currentItem, itemName, _itemIsIsubMenu, _itemIsDynamic, itemId
// based on: currentMenu, mitem
void __updItemVars() {
   if (_menuIsDynamic) { // dynamic items
      itemName = _getSpecialItemName(menuId, mitem);
      _itemIsSubMenu = false;
      itemId = 0xff;
   }
   else { // standard items
      _itemIsSubMenu = (currentMenu[mitem].subMenu != NULL);
      itemName = (char*)currentMenu[mitem].text;
      itemId = (byte)currentMenu[mitem].id;
   }
}

void _nextItem() {
  ++mitem;
  if (!_menuIsDynamic && currentMenu[mitem].text == NULL) mitem = 0; // no more items
  __updItemVars();
}

void _menuForward(bool rememberPath) {
    if (mdepth >= MAX_MENU_DEPTH) return; // max depth reached
    if (rememberPath) mpath[mdepth++] = mitem;
    settVal = 0xff;
    if (currentMenu[mitem].isSetting)
       settVal = _getSettingsItem(currentMenu[mitem].id);
    menuName = itemName;
    menuId = itemId;
    _menuIsDynamic = (currentMenu[mitem].subMenu == _dynamicItems);
    currentMenu = (menuItem*)currentMenu[mitem].subMenu;
    if (settVal == 0xff) mitem = 0; else mitem = settVal;
    __updItemVars();
}

void _menuBack() {
  byte i;

  if (mdepth < 1) return;
  mdepth--;
  // restore upper menu
  menuId = 0;
  menuName = (char*)mainMenuName;
  _menuIsDynamic = false;
  currentMenu = (menuItem*)mainMenu;
  mitem = mpath[0];
  __updItemVars();
  for (i=0; i<mdepth; i++) {
     _menuForward(false);
     mitem = mpath[i+1];
  }
  __updItemVars();
}

void _updMenuDisplay() {
  setDisplayText(0, menuName);
  setDisplayText(1, itemName);
  setDisplayBlink(1, mitem == settVal);
}

byte _getSettingsItem(byte iid) {
  byte r;

  switch (iid) {
    case 0x11: // left coin chute
    case 0x12: // center coin chute
    case 0x13:// right coin chute
      r = getSettingByte(iid-11) & 0x1f;
      break;
    case 0x14: // reset high score ...
    case 0x15: // attract mode
    case 0x16: // auto percentage mode
      r = getSettingSwitch(iid-8);
      break;
    case 0x17: // captured ball control
      r = getSettingSwitch(14);
      break;
    case 0x18: // maximum credits
      r = getSettingSwitch(15) | ((getSettingSwitch(16)<<1));
      break;
    case 0x19: // playfield special
      r = getSettingSwitch(22);
      break;
    case 0x1a:
      r = getSettingSwitch(23) | ((getSettingSwitch(24)<<1));
      break;
    case 0x1b:
    case 0x1c:
    case 0x1d:
      r = getSettingSwitch(iid+13);
      break;
    case 0x1e:
    case 0x1f:
      r = getSettingSwitch(iid+15);
      break;
    default:
      r = 0xff; // null value
      break;
  }
  return r;
}

void _setSettingsItem(byte iid, byte val) {
  switch (iid) {
    case 0x11: // left coin chute
    case 0x12: // center coin chute
    case 0x13:// right coin chute
      saveSettingByte((byte)(iid-0x11), (byte)0x00011111, val);
      break;
    case 0x14: // reset high score ...
    case 0x15: // attract mode
    case 0x16: // auto percentage mode
      setSettingSwitch(iid-8, val);
      break;
    case 0x17: // captured ball control
      setSettingSwitch(14, val);
      break;
    case 0x18: // maximum credits
      setSettingSwitch(15, val & 1);
      setSettingSwitch(16, val & 2);
      break;
    case 0x19: // playfield special
      setSettingSwitch(22, val);
      break;
    case 0x1a:
      setSettingSwitch(23, val & 1);
      setSettingSwitch(24, val & 2);
      break;
    case 0x1b:
    case 0x1c:
    case 0x1d:
      setSettingSwitch(iid+13, val);
      break;
    case 0x1e:
    case 0x1f:
      setSettingSwitch(iid+15, val);
      break;
  }
}

void _doMenuItem(byte iid, byte val) {
  uint16_t res;

  // SIMPLE SETTINGS
  if (iid >= 0x11 && iid <= 0x1f) {
    settVal = val;
    _setSettingsItem(iid, val);
    setDisplayBlink(1, true);
    return;
  }

  // SPECIAL FUNCTIONS
  if (iid >= 0x21 && iid <= 0x26) {
    switch (iid) {
      case 0x21: // lamps tests
        light[val].blink(LAMP_TEST_BLINK_P, 3);
        break;
      case 0x22: // solenoid tests
        setSolenoid(val , true, SOL_TEST_ON_TIME);
        break;
      case 0x23: // special lamps test
        light[val].blink(SOL_TEST_ON_TIME, 1);
        break;
      case 0x24: // sounds test
        setSound(val);
        break;
    }
    return;
  }

  // BOOKKEEPING INFOS
  if (iid >= 0x31 && iid <= 0x3f) {
    switch (iid) {
      case 0x31: // left chute coins
      case 0x32: // center chute coins
      case 0x33: // right chute coins
      case 0x34: // total plays
      case 0x35: // total replays
      case 0x36: // replay percentage
      case 0x37: // extra balls
      case 0x38: // total tilts
      case 0x39: // specials
        res = getStat16(2 + (itemId-0x31)*2);
        break;
      case 0x3a: // times highest game to date awarded
        res = getStat8(20);
        break;
      case 0x3b: // 1st high score level
      case 0x3c: // 2nd high score level
      case 0x3d: // 3rd high score level
        res = getStat32(22 + (itemId-0x3b)*4);
        break;
      case 0x3e: // top score
        res = getStat32(36);
        break;
      case 0x3f: // average playing score
        res = getStat16(34);
        break;
      default:
        res = 0;
    }
    sprintf(mbuf, "%d", res);
    setDisplayText(0, mbuf);
    return;
  }
}

char *_getSpecialItemName(byte &mid, byte &n) {
  mbuf[0] = 0;
  switch (mid) {
    case 0x11: // credits/coins settings
    case 0x12:
    case 0x13:
      if (n > 24) n = 0;
      sprintf(mbuf, "%d CREDITS / %d COINS", credits_ratio[n], coins_ratio[n]);
      break;
    case 0x18: // max credits
      if (n > 3) n = 0;
      sprintf(mbuf, "UP TO %d CREDITS", max_credits[n]);
      break;
    case 0x21: // lamps
      if (n > 47) n = 0;
      while (isSpecialLamp(n)) n++;
      sprintf(mbuf, "LAMP #%d", n);
      light[n].blink(LAMP_TEST_BLINK_P, 3);
      break;
    case 0x22: // special lamps
      if (n > 47) n = 0;
      while (!isSpecialLamp(n)) if (++n > 47) n = 0;
      sprintf(mbuf, "SPECIAL LAMP #%d", n);
      break;
    case 0x23:// solenoids
      if (n > 8) n = 0;
      sprintf(mbuf, "SOLENOID #%d", n+1);
      break;
    case 0x24: // sounds
      if (n > 31) n = 0;
      sprintf(mbuf, "SOUND #%d", n);
      break;
    default:
      sprintf(mbuf, "---");
      break;
  }
  return mbuf;
}
