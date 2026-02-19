# Contributors
    1. Arnav Sreenivasan <a.sreenivasan@wustl.edu>
    2. Jonathan Rodriguez Gomez <j.rodriguezgomez@wustl.edu>

# Module Design
We started with an implementation of the simple module example from LKD shown on pages 338 and 339, and then added the definitions of the variables for log_sec and log_nsec. We set their default log_sec value to 1 so that the timer would expire once every second as per lab requirements.

# Timer Design and Evaluation

## System Logs
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

# Thread Design and Evaluation

To achieve a threaded logging mechanism, we had to keep track of the thread task struct and be mindful of the order of our function calls. First, we make sure all our thread initialization code takes place before the timer is created. This is because the timer has a dependency on thread creation by way of calls to wake_up_process() since it assumes an initialized thread task struct. Second, is the thread function itself, in this case we went for a do while loop so that our thread executes outputs at least one log even if the module is quickly loaded and unloaded. Lastly, there's the more sensitive module exit function, in this case we had to make sure that all timers had triggered and been cleaned up before destroying the thread, as once again, the timer callback has a dependency on thread task struct for wake_up_process.

## System Logs

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
[ 1077.541986]
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

# Multi-threading Design and Evaluation

The main challenge of this new multi-threaded design was tracking issues in the system logs since they were often clobbered by all the output. To achieve the main requirements: First, we used an array to keep track of the thread structs, because typing thread struct* 3 times is just enough, typing it 4 times is too much. Second, we made use of fixed loops to do the initialization and binding process; we also quickly noticed how the wake up calls had to be separated from the creation and bind since the module exit cleanup functions are not called in this edge case. Lastly, we had a bit of refactoring to do to make sure our other functions were aware of the change to multi-threading.

## Systems Logs

Timer called every 1 seconds:

```
[   87.083215] monitor module initialized. log_sec=1, log_nsec=0
[   87.084100] Loop reached!
               CPU: 0
               Number of Voluntary Context Switches: 1
               Number of InVoluntary Context Switches: 0
[   87.084109] Loop reached!
               CPU: 1
               Number of Voluntary Context Switches: 1
               Number of InVoluntary Context Switches: 0
[   87.086057] Loop reached!
               CPU: 2
               Number of Voluntary Context Switches: 1
               Number of InVoluntary Context Switches: 0
[   87.086775] Loop reached!
               CPU: 3
               Number of Voluntary Context Switches: 1
               Number of InVoluntary Context Switches: 0
[   87.103920] Loop terminated, exiting thread!
[   87.104203] Loop terminated, exiting thread!
[   87.104304] Loop terminated, exiting thread!
[   87.104475] Loop terminated, exiting thread!
[   87.104557] monitor module is being unloaded
```

Timer called every 1 ms (1000000 ns):

```
[  173.488211] monitor module initialized. log_sec=0, log_nsec=1000000
[  173.489036] Loop reached!
               CPU: 0
               Number of Voluntary Context Switches: 1
               Number of InVoluntary Context Switches: 0
[  173.489056] Loop reached!
               CPU: 3
               Number of Voluntary Context Switches: 1
               Number of InVoluntary Context Switches: 0

[...]

[  173.516077] Loop reached!
               CPU: 0
               Number of Voluntary Context Switches: 25
               Number of InVoluntary Context Switches: 0
[  173.516116] Loop reached!
               CPU: 3
               Number of Voluntary Context Switches: 20

               Number of InVoluntary Context Switches: 3
[  173.516124] Loop reached!
               CPU: 1
               Number of Voluntary Context Switches: 11
               Number of InVoluntary Context Switches: 0
[  173.516870] Loop terminated, exiting thread!
[  173.516978] Loop terminated, exiting thread!
[  173.517140] Loop terminated, exiting thread!
[  173.517252] Loop terminated, exiting thread!
[  173.517331] monitor module is being unloaded

```

Timer called every 0.1 ms (100000 ns):

```
[...]

[  237.616726] Loop reached!
               CPU: 0
               Number of Voluntary Context Switches: 226
               Number of InVoluntary Context Switches: 5
[  237.616779] timer restarted.
[  237.616795] Loop reached!
               CPU: 1
               Number of Voluntary Context Switches: 146
               Number of InVoluntary Context Switches: 7
[  237.616881] timer restarted.
[  237.616979] timer restarted.
[  237.617036] Loop reached!
               CPU: 2
               Number of Voluntary Context Switches: 197
               Number of InVoluntary Context Switches: 10
[  237.617069] Loop reached!
               CPU: 3
               Number of Voluntary Context Switches: 25
               Number of InVoluntary Context Switches: 2
[  237.617079] timer restarted.
[  237.619269] Loop terminated, exiting thread!
[  237.620122] Loop terminated, exiting thread!
[  237.620236] Loop terminated, exiting thread!
[  237.620397] Loop terminated, exiting thread!
[  237.620479] monitor module is being unloaded
```

Observations:
1. In the multi-threaded case timer interrupts seem to be way more spread out. In comparison with the single-threaded case where interrupts happened consistently around the expiration time. I fear this is due to bottom half processing happening much later and so the print statements take quite a bit to reach the system logs.
2. The number of involuntary context switches for longer timer interrupts noticeably went up. This is likely due to more contention for a single core, than before when our program was single-threaded and not tied to specific core.
3. The number of involuntary context switches still seems to go up with shorter intervals, which makes sense as the bottom half processing happens more often and thus it must also be preempted more often.


# System Performance
Based on the Kernelshark trace, it appears that the threads are sometimes preempted. The amount of time each CPU spends executing the threads varies between schedule switches. We believe the schedule() call inside each of the threads, when it executes, is what might be able to preempt the threads.

Delta measurements
The three delta measurements we found were 0.000083 sec, 0.000095 sec, 0.000093 sec. See total_exec.png for a sample measurement.

Jitter measurements
The three jitter measurements we took were: 0.000040 sec, 0.000045 sec, 0.000045 sec. The minimum jitter was 0.000040 sec, the maximum jitter was 0.000045 sec, and the mean jitter was 0.0000433 sec.

Thread running time measurements
Our five measurements were: 0.000038 sec, 0.000046 sec, 0.000053 sec, 0.000038 sec, 0.000037 sec. The minimum time was 0.000037 sec, maximum time was 0.000053 sec, and the average was 0.0000424 sec.

# Development Effort

In total we spent about 5-6 hours working on this lab. I believe that the highest effort section was the first threaded version of the module however that might be due to a lack of familiarity with threads.
