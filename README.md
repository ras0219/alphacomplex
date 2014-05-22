Alpha Complex
=============

### Platform Matrix

|            | Windows  | Linux | OSX  |
|------------|----------|-------|------|
| SDL        | Yes      | Yes   | Yes  |
| X11        | Cygwin   | Yes   | Yes  |
| Ncurses    | No       | Yes   | ???  |
| Websockets | ? (cyg?) | Yes   | Yes? |

### Compilation

A recent C++11 compiler is required.


Compile in a separate directory.

    $ mkdir build
    $ cd build
    $ cmake ..
    $ make

### Execution

Currently, cities are automagically seeded with a fractal city generator (Thanks Karthik!).

    $ ./alphacomplex

### Inspiration

Alpha complex is inspired by the roleplaying game "Paranoia" by Mongoose Publishing.

http://www.mongoosepublishing.com/rpgs/paranoia.html
