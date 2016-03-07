#!/bin/bash
echo "go to root directory of project please"
echo "doing test on makefile \"test makefile\", should say \"(True)\""
test makefile
echo "doing test on test brackets [makefile], should say \"(True)\""
[makefile]
echo "doing test on test iCantWaitToGraduate, should say \"(False)\""
test iCantWaitToGraduate
echo "test -e makefile, should say \"(True)\""
test -e makefile
echo "now with test brackets [-e makefile], should say \"(True)\""
[-e makefile]
echo "now with test brackets [-d makefile], should say \"(False)\""
[-d makefile]
echo "now with test brackets [-f makefile], should say \"(True)\""
[-f makefile]
echo "doing test on test -d src, should say \"(True)\""
test -d src
