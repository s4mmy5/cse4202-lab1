# Contributors
    1. Arnav Sreenivasan <a.sreenivasan@wustl.edu>
    2. Jonathan Rodriguez Gomez <j.rodriguezgomez@wustl.edu>

# Module Design
We started with an implementation of the simple module example from LKD shown on pages 338 and 339, and then added the definitions of the variables for log_sec and log_nsec. We set their default log_sec value to 1 so that the timer would expire once every second as per lab requirements.

# Timer Design and Evaluation

## System Logs

Timer called every 1000000 nanoseconds:
```
[ 3214.490442] simple module is being unloaded
[ 3231.357108] simple module initialized. log_sec=0, log_nsec=1000000
[ 3231.358105] timer restarted.
[ 3231.359105] timer restarted.
[ 3231.360102] timer restarted.
[ 3231.361103] timer restarted.
[ 3231.362102] timer restarted.
[ 3231.363105] timer restarted.
[ 3231.364100] timer restarted.
[ 3231.365103] timer restarted.
[ 3231.366103] timer restarted.
[ 3231.367102] timer restarted.
[ 3231.368104] timer restarted.
[ 3231.369105] timer restarted.
[ 3231.370104] timer restarted.
[ 3231.371098] timer restarted.
[ 3231.372097] timer restarted.
[ 3231.373102] timer restarted.
[ 3231.374101] timer restarted.
[ 3231.375101] timer restarted.
[ 3231.376104] timer restarted.
[ 3231.377105] timer restarted.
[ 3231.377321] simple module is being unloaded
```

Timer called every 2 seconds:
```
[ 3054.565479] simple module initialized. log_sec=2, log_nsec=0
[ 3056.565389] timer restarted.
[ 3058.565291] timer restarted.
[ 3060.565194] timer restarted.
[ 3062.565099] timer restarted.
[ 3064.565008] timer restarted.
[ 3066.564912] timer restarted.
[ 3068.564818] timer restarted.
[ 3070.564726] timer restarted.
[ 3072.564640] timer restarted.
[ 3072.956216] simple module is being unloaded
```

