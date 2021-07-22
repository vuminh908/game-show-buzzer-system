# Game Show Buzzer System

The idea of this project is to create a buzzer system similar to those seen on game shows like *Jeopardy!* or quiz bowls. These are usually called "lockout" buzzer systems, probably since the first person to buzz in causes everyone else to be "locked out" from buzzing in, too.

## Sketches
There are two sketch versions:
1. An older version for Arduino Uno
   - This periodically samples the button input pins
   - In the case that more than one player button is read as pressed, a player will be picked randomly
1. A newer version for Teensy LC **(WIP)**
    - This will use hardware interrupts to handle button inputs

## To Do:
- [X] Detect button input
- [X] Implement debouncing
- [X] Implement "lockout" functionality described above (with an additional button to reset everything)
- [X] Add LEDs - one for each player - to light up with the corresponding button that was pressed first (add a buzzer, too)
- [X] Migrate to Teensy LC from Arduino Uno
- [ ] Finalize all hardware components and assemble everything on a perfboard, use cables to connect each button to the case (old telephone cable)
- [ ] 3D print a housing for each button and a case for the perfboard and Arduino
- [ ] Testing and debugging
- [ ] Start using it for trivia/Jeopardy nights!
