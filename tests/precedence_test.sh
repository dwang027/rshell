#!/bin/bash
echo "doing test on (echo A && echo B) || (echo C && echo D), should say \"A\" and \"B\""
(echo A && echo B) || (echo C && echo D)
echo "doing test on (echo "hi" || echo "bye") && (echo "tea" || echo "coffee"), should say \"hi\" and \"tea\""
(echo "hi" || echo "bye") && (echo "tea" || echo "coffee")
echo "doing test on (echo "hello"; echo " world") || (echo A || echo B), should say \"hello\" and \"world\"
(echo "hello"; echo " world") || (echo A || echo B)
echo "doing test ([aFileThatDNE] && echo 1) || echo 2, should say \"(False)\" and \"2\"
([aFileThatDNE] && echo 1) || echo 2
echo "go to the root directory bc we're doing a test on (test -d src && echo B) || echo C, should say \"(True)\" and \"B\"
(test -d src && echo B) || echo C
