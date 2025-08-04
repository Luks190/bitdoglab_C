#ifndef INC_BUTTONS_H
#define INC_BUTTONS_H

void initButton(int pin);

void initButtonWithPullUp(int pin);

bool readButtonWithDebounce();

#endif