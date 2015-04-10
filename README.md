# LCD4884
Library for LCD4884 shield - Arduino

======

### How to use it


Please consider starting write down your information in the header of the example.


To initialize the Class:

```lcd.initClass(projectName, NUM_MENU_ITEM) ```

To show the menu on the screen:

```lcd.showMenu(menuList) ```

To browse the menu with the button:

```void browseMenu(char menuList[][NBCHAR_X], FONCTION *menuFunction) ```


======
### Access to Methods

You can use also these methods:

`turnBacklightOn(true / false)`

`writeString(posX, posY, string, MENU_NORMAL / MENU_HIGHLIGHT)`

`writeStringBig(posX, posY, string[digit], MENU_NORMAL / MENU_HIGHLIGHT)`

`writeChar(posX, posY, char, MENU_NORMAL / MENU_HIGHLIGHT)`

`writeCharBig(posX, posY, char[digit], MENU_NORMAL / MENU_HIGHLIGHT)`

`clear(void)`


======
======

## Notes
* Wanna configure a long press on CENTER_KEY ? Go to Line **413** of *.cpp*.
* With this configuration, you have the Name of your project written centered at the top of the screen, and access to 4 submenus.
