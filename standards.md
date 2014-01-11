Standards
=========

### Coding

* 2 spaces, no tabs
* Roughly following the One True Brace Style for all common files.
  * If you're adding a sufficiently encapsulated module, deviance is acceptable.
* Prefer using over typedef
* Prefer C++ streams over c-style IO (*yes, I'm looking at you `logger.cpp`*)
* Use struct instead of class
  * This doesn't mean you shouldn't use `private`, but group public data at the top.
* Use initialization lists
* Use `std::shared_ptr`s whenever appropriate

### Libraries

- Adding new libraries is fine as long as they
  - Have a license compatible with our TBA license
  - Have a good required features / total features ratio.
    - Do *not* add a dependancy on all of gnome just to convert UTF16 strings to UTF8 strings.
- Added libraries should be optional whenever possible
  - If the library is only used in SDL mode, then do not require it for X11 and others.

### Comments

+ Are wonderful. I'm terrible at adding them. Please be better than me.
