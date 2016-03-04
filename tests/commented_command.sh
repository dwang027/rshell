#!/bin/sh
#tests comments within commands

echo "Test one"
echo "Testing 'ls name' with '#' before 'name'"
ls # name
echo "Successfully caught #"

echo "Test two"
echo "Testing ls -l with # before -l"
ls # -l
echo "Successfully caught #"

echo "Test three"
echo "Testing ls -a with # after -a"
ls -a #
echo "# did not affect ls!"
