# Keyboard

Spec | Details
--- | ---
Voltage | 5V, 3.43V
Pullup | 1kOhm
Protocol | [Joybus](./Joybus)

This device connects with two plugs, one functions like a normal GameCube controller, the other as the keyboard.

## Pinout

JP6 is the columns. JP7 is the rows.

JPX | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8
--- | --- | --- | --- | --- | --- | --- | --- | ---
1 | Fn |||| B || OS | RCtrl
2 ||| RShift || V | X | Z
3 || LCtrl || F2 | 5 | F1 | `~
4 ||| LShift | F3 | T | Caps | Tab
5 |||| 3 | 4 | 2 | 1
6 |||| E | R | W | Q
7 |||| D | F | S | A
8 |||| F4 | G | LShift | Esc | LAlt

JPX | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16
--- | --- | --- | --- | --- | --- | --- | --- | ---
1 | Up | N | /? | (JIS) | Space || Right | Down
2 || M | (JIS) | ,< | Enter | .>
3 || 6 | -_() | =+ | F9 | F8 | Ins | Del
4 || Y | [{ | ]} | Back | F7
5 || 7 | 0 | 8 | F10 | 9 | F12 | F11
6 || U | P | I | \ | O
7 || J | ;: | K | Enter | L
8 | Left | H | " | F6 | F5 | RAlt

## Scancodes

Row + column = scancode.

Hex | 0x0 | 0x1 | 0x2 | 0x3 | 0x4 | 0x5 | 0x6 | 0X7
--- | --- | --- | --- | --- | --- | --- | --- | ---
**0x00** ||||||| Home | End
**0x10** | A | B | C | D | E | F | G | H
**0x20** | Q | R | S | T | U | V | W | X
**0x30** | 7 | 8 | 9 | 0 | -_() | =+ | PrtSc | Pause
**0x40** | F1 | F2 | F3 | F4 | F5 | F6 | F7 | F8
**0x50** | Back | Tab || Caps | LShift || LCtrl | LAlt
**0x60** || Enter

Hex | 0x8 | 0x9 | 0xA | 0xB | 0xC | 0xD | 0xE | 0xF
--- | --- | --- | --- | --- | --- | --- | --- | ---
**0x00** | PgUp | PgDn | ScrLk
**0x10** | I | J | K | L | M | N | O | P
**0x20** | Y | Z | 1 | 2 | 3 | 4 | 5 | 6
**0x30** | [{ | ;: | " | ]} | ,< | .> | /?
**0x40** | F9 | F10 | F11 | F12 | Esc | Ins | Del | `~
**0x50** | L OS | Space | R OS | Menu | Left | Down | Up | Right

## References

1. <http://hitmen.c02.at/files/yagcd/yagcd/chap9.html#sec9>
