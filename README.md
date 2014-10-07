QTempatDuduk
============

A Qt4 app to randomize seating. Created for learning purposes.

## Features
- Configurable color, size, font, and animation
- Custom seating layout
- Custom seating rule
- Open database from file, in-program

## Dependencies
- C++ >= 4.3.2
- Qt4

Tested on Linux 3.11-2-686-pae #1 SMP Debian 3.11.8-1 (2013-11-13) i686 GNU/Linux (LMDE)

## Compiling & Running
```sh
$ git clone https://github.com/rkkautsar/QTempatDuduk	# ignore this if you download the zip/tar
$ cd QTempatDuduk					# go to the project directory
$ qmake
$ make
$ ./QTempatDuduk
```
If you get an error, try using `qmake-qt4` instead.

## Configuration Files
### people.txt
They are the names for the seating layout. Here's the template:
```
[number of male]
Alpha
Beta
Gamma
.
.

[number of female]
Alpha
Beta
Gamma
.
.

```

### seating.txt
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

