# LCD4884
Library for LCD4884 shield - Arduino

[Visit the Article on my Blog !](http://horloge-skynet.weebly.com/arduino/lcd4884-shield)

======

### How to use it


Please consider starting write down your information in the header like in the example.  
Now, complete the body of functions declared at the bottom, with the content you wish.  
When you'll choose the entry in the menu, this content will be executed.  

To initialize the Class:  
```lcd.initClass(projectName, NUM_MENU_ITEM) ```

To show the menu on the screen:  
```lcd.showMenu(menuList) ```

To browse menu with the buttons:  
```lcd.browseMenu(menuList, menuFunction) ```


======
### Access to Methods

You can use also these methods:

```turnBacklightOn(true / false)```  
```writeString(posX, posY, const char*, MENU_NORMAL / MENU_HIGHLIGHT)```  
```writeStringBig(posX, posY, const char*, MENU_NORMAL / MENU_HIGHLIGHT)```  
```writeChar(posX, posY, char, MENU_NORMAL / MENU_HIGHLIGHT)```  
```writeCharBig(posX, posY, char, MENU_NORMAL / MENU_HIGHLIGHT)```  
```clear(void) ```  
```init(void) ``` (to initialize the screen without menu or buttons operation)


======
======

## Notes
* With this configuration, you have the Name of your project written centered at the top of the screen, and access to 4 submenus.
* Wanna use a long press on *CENTER_KEY* ? Use the return value (_bool_) of ```getLongPress() ``` to reach the status of last pression.
* Wanna use the status of joystick's buttons in your own code ? Compare the return value ```int(analogRead(0))) ``` with ```LEFT_K_A ```, ```CENTER_K_A ```, ```DOWN_K_A ```, ```RIGHT_K_A ```, ```UP_K_A ```, ```K_RELEAS_A ```, the analogic values returned by the joystick. But careful ! The analogic value read for a position can be a bit different than the one behind the MACRO...
