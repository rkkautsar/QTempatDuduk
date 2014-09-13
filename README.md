QTempatDuduk
============

A QT4 app to randomize seating. Created for learning purposes.

## Features
- Randomize seating (with regards to gender)
- Randomize seating (with no regards of gender) (NEW)
- Hard-coded seating layout

## Configuration
Just edit `male.txt` and `female.txt` to change names.
For seating layout, you have to change it manually in the code, as I'm still learning QT and can't implement this feature yet. Sorry :'(

## Instalation (Linux)
	$ git clone https://github.com/rkkautsar/QTempatDuduk
	$ cd QTempatDuduk
	$ qmake
	$ make
	$ ./QTempatDuduk
If you get an error, try using `qmake-qt4` instead.
