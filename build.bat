@ECHO off

ECHO.
ECHO ------------------------------------
make clean
make
ECHO ------------------------------------

copy .\out\ks.exe .\ks.exe
