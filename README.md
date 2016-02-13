Assignment 2 - Huber Ochoa & Diana Wang

--- BRIEF SUMMARY OF ASSIGNMENT 2 ---
Our command shell (rshell) does the following:
  - prints a command prompt (e.g. $)
  - reads in commands on one line
  
Command form:
  cmd = executable [argumentList] [connector cmd]
  connector = || or && or ;
  
Example:
  $ ls -a
  $echo hello
  $mkdir test
  
  is equivalent to
  
  $ ls -a; echo hello; mkdir hello
  
To execute the commands, we use fork, execvp, and waitpid.
Also, anything that appears after a '#' in the user's input is considered to be a comment.

--- LIMITATIONS ---
- syntax for comments must be "#_" where the _ represents a space. (so basically, "# ")
- also, the connector following a comment must also have a space (e.g. "# blah blah blah ;")

--- NOTES ---
Equal effort was put in from both of us, we met up in person to work on the assignment (which explains why Huber has more GitHub commits than Diana.)
