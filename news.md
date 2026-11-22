# News

## 0.3.9 - 2026-11-22

### Improvements

  * Suppressed false positives of Valgrind on stack scan. It's enabled
    when `valgrind/memcheck.h` exists on build. Valgrind isn't needed
    at run time.
    * configure: `valgrind/memcheck.h` is detected automatically.
    * Meson: Use `-Dvalgrind=enabled` to require it.
    * You need to mark a copy of a value in stack as defined in your
      mark procedure that is passed to `GCROOTS_init()`. See "Valgrind"
      in `README.md`.

  * Renamed `README` to `README.md`. The installed file name is also
    changed.

  * Added how to use with AddressSanitizer and Valgrind to `README.md`.

## 0.3.8 - 2026-11-22

### Improvements

  * Added support for AddressSanitizer.
    * You need to add `__attribute__((no_sanitize_address))` to your
      mark procedure that is passed to `GCROOTS_init()` because it
      scans stack.

## 0.3.7 - 2026-11-22

### Improvements

  * Installed `COPYING` and `README` to `share/doc/libgcroots/` by
    default.
    * configure: Use `--docdir` to change the directory.
    * Meson: Use `-Dlicensedir` to change the directory of `COPYING`.

  * Meson: Required Meson 1.1.0 or later.

## 0.3.6 - 2026-10-11

### Improvements

  * Suppressed a `unused-but-set-variable` warning.

## 0.3.5 - 2026-10-06

### Improvements

  * Meson: Changed the default values to adjust Meson style.

## 0.3.4 - 2026-10-06

### Improvements

  * Added support for Meson.

## 0.3.3 - 2026-09-22

### Improvements

  * Added missing `autogen.sh` to archive.
    * [uim/uim#342](https://github.com/uim/uim/issues/342)
    * Reported by NOKUBI Takatsugu

### Thanks

  * NOKUBI Takatsugu

## 0.3.2 - 2020-08-23

### Improvements

  * Added support for RISC-V.
    [GitHub #3][Patch by Shea Levy]

  * Added support for GCC 10.
    [GitHub #4][Patch by Jeroen Roovers]

### Thanks

  * Shea Levy

  * Jeroen Roovers

## 0.3.1 - 2018-05-10

### Improvements

  * Added AArch64 support
    [GitHub #1][Patch by Andreas Schwab]

### Thanks

  * Andreas Schwab

## 0.3.0 - 2018-05-09

No feature changes. This is just a repackaged version of 0.2.3.

