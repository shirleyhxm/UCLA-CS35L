The major problems I ran into were:
(1) how to extract and modify the commands in main to be a separate function
(2) how to store all the arguments passed to t_function so that they can be
    accessed by a void pointer

For (1), I extracted everything in the double for loop in main to be a separate
function. Then I changed the range for the for loop to be the range of specific
thread.

For (2), I defined a new struct called t_args that stores the range of that
thread and a scene pointer. When &t_args are passed to t_function, I casted it
to a void pointer and recovered it insided the t_function.

I run make clean check for the first time and got the following error:

==================================================
time ./srt 1-test.ppm >1-test.ppm.tmp
Thread creation failed.
real   0m0.002s
user   0m0.000s
sys    0m0.001s
make: *** [Makefile:36: 1-test.ppm] Error 1
==================================================

I realized pthread_create and pthread_join evaluate to be 1(true)
when there is an error, so I fixed the if conditions.

Then I run again and got hundreds of lines from diff output. I
carefully checked my code where there was a printf, and I found out
that scaled_color[i] was printed based on the order of threads
instead of from (0,0) to (width-1,height-1).

So I changed the float array scaled_color to be a global variable and
from 1D to 3D so that the colors are stored in the corresponding
position. After all calculations are done, the colors can be printed
in order.

Then I successfully ran make clean check and obtained the following
performance comparision:

==================================================
time ./srt 1-test.ppm >1-test.ppm.tmp

real 0m48.044s
user 0m48.050s
sys  0m0.001s
mv 1-test.ppm.tmp 1-test.ppm
time ./srt 2-test.ppm >2-test.ppm.tmp

real 0m24.361s
user 0m48.598s
sys  0m0.002s
mv 2-test.ppm.tmp 2-test.ppm
time ./srt 4-test.ppm >4-test.ppm.tmp

real 0m16.118s
user 0m48.461s
sys  0m0.005s
mv 4-test.ppm.tmp 4-test.ppm
time ./srt 8-test.ppm >8-test.ppm.tmp

real 0m11.382s
user 0m50.511s
sys  0m0.002s
==================================================

The reduction in real time is significant as the number of threads
increases. Based on the reasults of 1, 2, 4, 8 threads, we can conclude
that the real time for executing with 2^N threads is about T/(N+1)
where T equals the real time for executing in single thread.

There is no noticeable change in user time and sys time as the number
of threads increases.
