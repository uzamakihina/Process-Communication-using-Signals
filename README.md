# CMPUT 379 Assignment 1
## Design Questions Responses:

For differentiating how many signals will be used to send and receive messages, we disable and enable certain code based on what flag is used.

The SINGLE flag defines code that handles 1 signals and disables the two signal code, while the DOUBLE flag defines code that handles 2 signals and disable the one signal code.

We scan the message we want to send and convert each character of the message into bits that represent its ascii value.

### 1.
For each character, we send two SIGUSR1 signals while waiting 6ms in between to represent a 0, we send two SIGUSR1 signals while waiting 1.5 ms in-between to represent a 1.

Depending on how long the other processes signal handler waits in between two signals it knows if it’s a 0 for longer wait time  or 1 for shorter wait time.

Every 16 signals the signal handler will have 8 bits, and converts the 8 bits into the ascii character and appends that character into the message list.

Then restarts the bit count for the next character.

### 2.
Using both signals, for each character, we send a SIGUSR1 if a bit is 0, and SIGUSR2 if a bit 1.

After every 8 signals the other processes signal handler knows a character is done and appends the ascii value of the 8 bits received into the message list.

Then restarts the bit count for the next character.

### 3/4.
For checking the error, we first take the message received and add all the characters in ascii value in that message including the newline and modulo it by 256.

Then we send this value to the other processes signal handler which saves this value, when the signal handler finishes receiving the message, it adds all the characters in its message list and modulo it by 256.

When a newline is appended we know the entire message is received (we begin the message with the number sent at the beginning).

If the initially received value and the value it calculated at the end are the same, then we declare there is no error in the message.

### 5.
We wont be able to catch errors that happen to keep the end modulo number consistent with the one received, such as if we send abcd, but the handler translates bbcc,
abcd%256 = bbcc%256, then the error check fails to detect the error.

Also if there is an error in the initial send of the check number, it might detect correct messages to be wrong.

### 6.
We print a newline at the beginning and end of a message sent by the signal handler, this way the user input and print strings are clearly separated and distinguishable.

### 7.
We have only used SIGUSR1 and SIGUSR2 signals for this project.

### 8.
When there is no message being sent or received the program waits at the scanf state waiting for either an i/o interrupt or system interrupt. This is better than an infinite loop which uses cpu cycles.

## Work balance:
Assignment was completed entirely while pair-programming.
## Test Environment:
Assignment was tested (and running) by ssh-ing into "dcones@ug20.cs.ualberta.ca".
