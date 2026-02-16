# Contributors
    1. Arnav Sreenivasan <a.sreenivasan@wustl.edu>
    2. Jonathan Rodriguez Gomez <j.rodriguezgomez@wustl.edu>

# Module Design
We started with an implementation of the simple module example from LKD shown on pages 338 and 339, and then added the definitions of the variables for log_sec and log_nsec. We set their default log_sec value to 1 so that the timer would expire once every second as per lab requirements.

## Timer Design and Evaluation

### System Logs
We created a ktime_t object using the module parameters, initialized the hrtimer structure, and assigned it a timer callback that restarted the timer on expiration. Lastly, we start the hrtimer structure which triggers on every interval until the module is unloaded and the timer is canceled.

Timer called every 1 millisecond (1000000 ns):
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

## Thread Design and Evaluation

To achieve a threaded logging mechanism, we had to keep track of the thread task struct and be mindful of the order of our function calls. First, we make sure all our thread initialization code takes place before the timer is created. This is because the timer has a dependency on thread creation by way of calls to wake_up_process() since it assumes an initialized thread task struct. Second, is the thread function itself, in this case we went for a do while loop so that our thread executes outputs at least one log even if the module is quickly loaded and unloaded. Lastly, there's the more sensitive module exit function, in this case we had to make sure that all timers had triggered and been cleaned up before destroying the thread, as once again, the timer callback has a dependency on thread task struct for wake_up_process.

### System Logs

Timer called every 1 millisecond (1000000 ns):
```
[ 1051.943448] monitor module initialized. log_sec=0, log_nsec=1000000

[...]

[ 1077.530293] Loop reached!
               Number of Voluntary Context Switches: 12518
               Number of InVoluntary Context Switches: 1982
[ 1077.530298] timer restarted.
[ 1077.531207] timer restarted.
[ 1077.531330] Loop reached!
               Number of Voluntary Context Switches: 12519
               Number of InVoluntary Context Switches: 1982
[ 1077.532674] timer restarted.
[ 1077.533247] timer restarted.
[ 1077.533998] timer restarted.
[ 1077.534987] timer restarted.
[ 1077.535986] timer restarted.
[ 1077.537000] timer restarted.
[ 1077.537040] Loop reached!
               Number of Voluntary Context Switches: 12520
               Number of InVoluntary Context Switches: 1982
[ 1077.538013] timer restarted.
[ 1077.538172] Loop reached!
               Number of Voluntary Context Switches: 12521
               Number of InVoluntary Context Switches: 1982
[ 1077.538989] timer restarted.
[ 1077.539048] Loop reached!
               Number of Voluntary Context Switches: 12522
               Number of InVoluntary Context Switches: 1982
[ 1077.539985] timer restarted.
[ 1077.540032] Loop reached!
               Number of Voluntary Context Switches: 12523
               Number of InVoluntary Context Switches: 1982
[ 1077.540992] timer restarted.
[ 1077.541044] Loop reached!
               Number of Voluntary Context Switches: 12524
               Number of InVoluntary Context Switches: 1982
[ 1077.541986] timer restarted.
[ 1077.542031] Loop reached!
               Number of Voluntary Context Switches: 12525
               Number of InVoluntary Context Switches: 1982
[ 1077.542321] Loop terminated, exiting thread!
[ 1077.542408] monitor module is being unloaded
```

Timer called every 1 seconds:
```
[ 1209.934463] monitor module initialized. log_sec=1, log_nsec=0
[ 1209.934484] Loop reached!
               Number of Voluntary Context Switches: 1
               Number of InVoluntary Context Switches: 0
[ 1210.935443] timer restarted.
[ 1210.935506] Loop reached!
               Number of Voluntary Context Switches: 2
               Number of InVoluntary Context Switches: 0
[ 1211.936404] timer restarted.
[ 1211.936443] Loop reached!
               Number of Voluntary Context Switches: 3
               Number of InVoluntary Context Switches: 0
[ 1212.937354] timer restarted.
[ 1212.937405] Loop reached!
               Number of Voluntary Context Switches: 4
               Number of InVoluntary Context Switches: 0
[ 1213.938292] timer restarted.
[ 1213.938335] Loop reached!
               Number of Voluntary Context Switches: 5
               Number of InVoluntary Context Switches: 0
[ 1214.939219] timer restarted.
[ 1214.939265] Loop reached!
               Number of Voluntary Context Switches: 6
               Number of InVoluntary Context Switches: 0
[ 1214.965973] Loop terminated, exiting thread!
[ 1214.966135] monitor module is being unloaded
```
