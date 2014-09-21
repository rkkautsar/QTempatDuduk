QTempatDuduk
============

A Qt4 app to randomize seating. Created for learning purposes.

## Features
- Configurable color, size, font, and animation
- Custom seating layout
- Custom seating rule

## Dependencies
- C++ >= 4.3.2
- Qt4

## Compiling & Running
```sh
$ git clone https://github.com/rkkautsar/QTempatDuduk
$ cd QTempatDuduk
$ qmake
$ make
$ ./QTempatDuduk
```
If you get an error, try using `qmake-qt4` instead.

## Configuration Files
### `male.txt` and `female.txt`
They are the names for the seating layout. Here's the template:
```
[number of name]
Alpha
Beta
Gamma
.
.
.
```

### `custom.seating.txt`
This is the custom rule and layout configuration, it is really not complicated to edit it or make a new layout and rule entirely. This is the template:
```
[number of male] [number of female]
[number of row] [number of column]
[The layout and rule]
```
A little example before we go further:
```
18 17
9 11
mm.ff.mm.mm
...........
ff.mm.ff.mm
...........
mm.ff.mm.ff
...........
ff.mm.ff.mm
...........
....fff....
```
The layout and rule section is a mapping or matrix `[number of row] x [number of column]` of characters. The accepted characters are `{'.','m','f'}`. `'.'` represents a space, `'m'` represents a seat that should be seated by a male, and `'f'` represents a seat that should be seated by a female. You should at dots between rows too so there is a space between seating rows.

