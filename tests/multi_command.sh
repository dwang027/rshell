
#!/bin/bash
echo "testing false && echo random || echo weather"
false && echo random || echo weather
echo "should say: weather"
