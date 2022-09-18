-I never thought philosophy would be so deadly.

*Philosophers is a C project where we should have a certain number of philosophers, each one of them have one fork, they share one huge plate of spaghetti, each one of them is either eating, thinking or sleeping, the problem is: a philosopher to be able to eat needs TWO forks, which means he takes the fork of the philo next to him. so two philos that are setting next to each other won’t be able to eat at the same time, this is a very known sharing resources problem that’s represented by philos and forks, but should be handled in our C program.*

we have no real humans or philos to use in our program, but we have processes that share resources instead of forks. so we should look up for what is a process, a thread, what is a mutex and how are we gonna use all this things to create and solve our dinning problem.

## definitions and meanings

- **process**:
    
    A process refers to the code and data in memory segments into which the operating system loads a program. Simply put, a process is a program that is loaded into the memory to be executed by the processor.
    
    as we saw earlier, we used the function fork to create a child process inside our program to do a certain job, this one just copies all the data from the main process and use it to do whatever it should do, which means there is no shared data between the two process we have, once the program will make it to the line where fork is called, the whole prg will get copied, we can check wether we are in the child or the parent to separate their functionality, but whatever is down there will be executed twice, once in each process
    
    - example:
        
        ![alt text](https://github.com/laila-chk/philos/blob/master/imges_for_readme/example.png)
        
        the child process will increment j by 3 and print that value, the parent will wait for child until it’s done, and work with the original value of j and increment it by 2.
        
- **thread**
    
    A *thread* is a single sequential flow of control within a program. Threads from the same process **share** with each other :code section, data section, opened files, signals and other OS resources.
    
    I believe that creating another thread is a bit similar to creating a process, but instead of copying the old one, we’re just  directly accessing the data of the main program and if needed, changing it directly.
    
    - example:
        
        ![alt text](https://github.com/laila-chk/philos/blob/master/imges_for_readme/one.png)
        
        here we created two threads that both access the global variable, and just use it to do what routine function is supposed to do, unlike creating a process that copies all vars and work with the copy.
        
        to create a thread, we also need a routine function, it’s the function that the thread runs once it is created, and we can run multiple threads to run the same routine function at the same time (it is exactly what we did in the exp above, each thread keeps incrementing the same var using the same func at the same time.), we can illustrate this using this image :
        
        - click to see image
            
            ![alt text](https://github.com/laila-chk/philos/blob/master/imges_for_readme/threads.gif)
            
            we have two threads in 1 prg, each is doing a chain of actions specified by a certain routine.
            
- **mutex**
    
    a mutex (mutual exclusion object) is a program object that is created to organize a multiple threaded program, so each thread will take turn sharing the same resource (as access to the same file or var ..)
    
    in other words, it’s like a lock with only one key, only one thread per time have that key and can access the locked data, once it’s done with it, the key is passed to another thread.
    
- **data race**
    
    a data race is something we should avoid since it could ruin our code, it occurs when two threads access the same var at the same time, they read the same val, one thread write to it and change it while the second hangs on there for a while, once the second wants to keep going, it writes the val it had earlier, and jumps where the first thread stopped..
    
    - example:
        
        ```c
        /*.   code    .*/
        void *routine(*arg)
        {
        		int i;
        		
        		i = (int)arg;
        		i++;
        }
        int main()
        {
        	int i = 0;
        	
        	pthread_t th1;
        	pthread_t th2;
        	pthread_create(&th1, NULL, &routine, &i);
        	pthread_create(&th2, NULL, &routine, &i);
        }
        //------expect :
        
        //-----what happens:
        ```
        
- **race condition**
    
    the term of “race condition” is often misused by people to refer to “data race”, some think that they are the same thing, but a race condition is the condition where data race can be achieved, a race condition is a condition where the program behavior’s depends on multithreading or relative timing.. so there’s a chance of having a data race.
    

## peek at functions we can use and examples:

> 
> 
> 
> ### creating a thread:
> 
> **pthread_create()**, this function creates threads, it takes three arguments that are :  a **thread** of type pthread_t, an attribute of customization that we currently don’t need, the routine function that our threads will call, and the last parameter is a parameter that the routine func takes.
> 
> ### waiting for a thread to finish
> 
> **pthread_join():**  waits for the thread specified by thread to terminate. bc sometimes it may take way too much time to terminate and the main process may end before it which will cause termination of process before it even it ends, it takes to arguments, the thread and a pointer in case our func returns smth and we want to store it.
> 
> ### pthread_detach()
> 
> this function doesn’t make the program wait for the the thread to finish, but it tells the system to automatically free the memory occupied by the thread once the thread finishes.
> 
> - hint:
>     
>     at first, it may appear as if pthread_join is what we need, but because we’ll have an infinite loop in the routine (eat, sleep and think until death) and the death of one philo causes the end of the stimulation, so waiting for other threads to finish needs another strategy..
>     
> 
> ### a prb that may occur: Data Race
> 
> there’s a definition above.
> 
> - **code:**
>     
>     ```c
>     #include <pthread.h>
>     #include <stdio.h>
>     
>     int	g_var;
>     
>     void*	routine()
>     {
>     	int i = 0;
>     	while(i++ <= 10000000)
>     			g_var++;
>     }
>     
>     int main()
>     {
>     	g_var = 0;
>     	pthread_t	th1;
>     	pthread_t	th2;
>     	pthread_create(&th1, NULL, &routine,NULL);
>     	pthread_create(&th2, NULL, &routine, NULL);
>     	pthread_join(th1, NULL);
>     	pthread_join(th2, NULL);
>     	printf("global bariabl's value is %d\n", g_var);
>     }
>     ```
>     
> - **result:**
>     
>     ```c
>     the result should be 20000000
>     but instead it gives a lower and different value for each test.
>     we know that all our threads can access the variable g_var at the same
>     time, so each thread counts 10000000 times while incrementing, this is
>     happening twice at same time using the var i to keep on track on how
>     much more we should increment, each thread uses its own i, but both of
>     them change of g_var's value, what heppens is sometime one thread will
>     be hanging on a value, let say g_var = 900 at some point, and let say
>     that th1 made 5 iterations, g_var = 905, but th2 still stuck at 900 and
>     thinks that 900 is the latest value of g_var, so after one iteration,
>     g_var = 901 instead of 906 th1 now will find 901, take it and increment
>     it, by the end of both threads we'll find that the final value is wrong
>     ```
>     
> 
> ### solving this prb: avoiding Data Race :
> 
> as we saw earlier in the definitions, the mutex is the one responsible for organizing threads, so we will need one to keep a clean code and right result.
> 
> declaring and using a mutex is as follows:
> 
> ```c
> pthread_mutex_t mutex;
> pthread_mutex_lock(&mutex);
> pthread_mutex_unlock(&mutex);
> ```
> 
> the first line is a declaration, and ofc the var we declared should be accessed from all function or at least both main and routine.The second is passing the address of mutex to the function so it will change it value, and each thread should check up on that value to know wether it is possible to access the locked var/file.. or not, after locking we should unlock so the next thread will find its way to use the previously locked data. While one thread is able to access all data, other threads will stop at the line pthread_mutex_lock(&mutex), once it gets unlocked, only one other thread will be able to access and lock the way behind it, until it unlocks and let other thread go through the same cycle.
> 
> But another thing that we should do is initializing and destroying the mutex before and after using it and calling our threads.
> 
> ```c
> pthread_mutex_init(&mutex, NULL);
> pthread_mutex_destroy(&mutex);
> ```
> 
> ---
> 
> ### passing an argument to the routine function:
> 
> the routine function returns a void pointer and takes a void pointer as an argument, even tho we call pthread_create in that way:
> 
> ```c
> pthread_create(&th1, NULL, &rout, &arg_to_pass);
> ```
> 
> and arg_to_pass is already defined, we still should have a void arg in the routine function that we typecast as follows:
> 
> ```c
> #include <stdio.h>
> #include <pthread.h>
> 
> void*	routine(void *arg)
> {
> 	printf("we got the number %d \n", (*(int *)(arg)));
> 	return ((void *)1);
> }
> 
> int main()
> {
> 	int number = 5;
> 	pthread_t th1;
> 	pthread_create(&th1, NULL, &routine, &number);
> 	pthread_join(th1, NULL);
> }
> ```
> 
> - first parameter is the address of our thread
> - we’re passing NULL to **attr** argument points to a pthread_attr_t structure  whose  contents are  used  at  thread creation time to determine attributes for the new thread; this structure is initialized  using  pthread_attr_init().   If  attr is NULL, then the thread is created with default attributes.
> - **routine** is the function we’ll call and run after creating our thread
> - the last parameter is the arg we want to pass to strart_routine function

# how to code?

I believe that it’s a little bit clear from where we can start, we just need to pay attention to a few things. First of all we should check the args given while compiling, and make sure that they are numbers and positive.

our very step in any program is including the right header or library, in this program we will need to include pthread.h and also we should compile using the flag -pthread otherwise our program won’t work, this is a flag that allows things to link.

**storing our data:**

each time we run the program we will take some info from the standard input, to know how many philosopher will be there and how much they can survive with no food etc..

there are so many ways we can store and handle our vars, in my case global variables are forbidden, so working with structs is very efficient, everyone can have their own data structured the way they want, for me I created one struct in my header file, and included all the vars i need inside of it, there are people that found it more efficient to work with two structs and putting one inside the other, but that wan’t the case for me.

after declaring my struct I allocated as many as the number of philosophers, so each struct will be separated than the other, but we still can navigate through the cases of this one pointer and enter each struct individually using + 1 to increment the address for example.

```c
/*----------------------------in the header file---------------------------*/

typedef	struct	s_ph
{
	int					i;
	int					meals;
	pthread_mutex_t		fork;
	pthread_mutex_t		vals_mtx;
	pthread_t			th;
	...
	...
}	t_ph;

/*----------------------------  in the main()  ---------------------------*/
t_ph		*ph;

ph = malloc(atoi(av[1]) * sizeof(t_ph);
// we can easily access any of those structs using an index ph[i]
//and to access an element ph[2].i 

-/*in my code i didn't use the atoi(av[1]) because we first of all should
parse correctly and make sure that all args are positive numbers*/-
```

**creating the threads:**

we know that each philosopher should lock his fork and the fork of philosopher next to him, in order to not have each philo holding its own and waiting for a second until death, so letting some philos in before the others will guarantee that they will take two forks, so we should let each n+2 philo goes first, then the others, n is either 0 to have even philos go first or 1 to let odd philos in first .we should have a little time between them so even if a thread was a little late it will still be able to lock both its mutex and the next one’s mutex so we will need a sleep or usleep function between them. In my case sleeping 100 micro-secs worked fine, so I created even threads first (including the index 0), called usleep(100), then creating odd threads.

**routine function:**

the routine function takes one void argument that we can typecast to any type we want, so i passed the address of each struct each time. This way i wasn’t only able to access all the vars inside my struct yet access other structs and hence the fork of another philosopher.

in my routine i had three functions, eat, sleep and think, and a printing mutex getting locked and unlocked between printing the action that a philosopher does.

**the eat, sleep and think functions:**

starting from the most simple one which is **thinking**, we don’t need it to be a function, all we need to do is to print that the philo is thinking right after eating and sleeping. but we need to mutex before printing to prevent any data races, if using write(), an issue may appear which is having two sentences mashed up in the same line, the way how printf() works makes the chance of having this case very low, but as a precaution and as a good practice, we want to use the mutex on it.

**sleep** function: this may sound as simple as calling usleep and give it the time_to_sleep parameter * 1000 (to convert from ms to micro-sec), but this function isn’t as accurate and exact as it should be, so we should manually guarantee sleeping enough time, that will be possible by putting usleep with a short amount of time inside a loop and sleeping little by little until the required time passes, if that sounds very confusing a brief explanation of **gettimeofday** will make it clearer, you’ll find an explanation down below.

**eat function:** we are more likely to get data race in this one or because of it more than the others,  bc this is where we have to mutex forks, prints, update number of meals eaten and time between meals. in this one we have to make sure that each philo picks up their own fork and N+1’s fork, (N is the number of the current philo), except the last one that will take the first philosopher’s fork.

- my routine function:
    
    ```c
    void	*routine(void *arg)
    {
    	t_ph	*ph;    //typedef of my struct of philos
    
    	ph = (t_ph *)arg;
    	while (1)
    	{
    		eat(ph->i, ph);      //passing the index and 
    		lock_print("is sleeping", ph, ph->i);
    		my_sleep(ph->vals[3]);
    		lock_print("is thinking", ph, ph->i);
    	}
    	return ((void *)1);
    }
    ```
    

### gettimeofday:

the prototype of it is : int **gettimeofday(**struct timeval *tv, struct timezone *tz**)**; The tv argument is a struct timeval (as specified in <sys/time.h>)  and gives the number of seconds and microseconds since the  Epoch. The tz argument is a struct timezone:

```c
       struct timeval {
           time_t      tv_sec;     /* seconds */
           suseconds_t tv_usec;    /* microseconds */
       };
```

so we should declare a struct of type timeval and pass its address to this function  in order to have the tv_sec (time value in seconds) since the epoch, but what is the epoch you say? we’re getting the time in seconds from since: 1970-01-01   00:00:00   +0000 (UTC). 

it is quite obvious that we should keep on updating the tv_sec by calling gettimeofday(), we can write a func that does so.

we will need this function to calculate the difference between that current moment and and time when the stimulation started to **print stamps** of how many ms passed since the start of the stimulation, and to calculate if **time_to_eat** passed since the start of the stimulation or the last meal.

- example of using gettimeofday and func to get updated value:
    
    ```c
    /*++++++++using gettimeofday in a func to return updated time:++++++++*/
    long long	update_time(void)
    {
    	struct timeval	tv;
    	long long		time;
    
    	gettimeofday(&tv, NULL);
    	time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    	return (time);
    }
    //i wanted the result to be in ms so i multipied the seconds by 1000
    // and divided the micro-sec bt 1000 so time is now in milliseconds.
    //**we can now do var = update_time and get the current amount of sec
    //since 1970**
    ```
    

### the death of a philosopher:

it’s said in the subject that we should avoid the death of philosophers, in other words we should assure that they are going to start right (two philos next to each other should not try to eat at the same time), but if sleeping time is longer than time_to_die, obviously the philosopher will starve during their sleep. and that is the end of the stimulation, once a philosopher dies it ends, or when all philos eat the number of meals they should have, it stops there. 

### ending lives:

breaking the routine function isn’t the right way to stop the stimulation, because we should keep in mind that multiple threads enter the routine function and having a condition in our while loop might not give an exact time of death because we are working with ms and very fast threads,  we should make the main thread keep an eye on philos, so it will print that the philo died and end the stimulation, the thing is we don’t need to kill or stop a thread after the other, it’s enough to end the main thread.

once the main process had ended, other threads will keep running for a bit of time and we should make sure that no thread will print anything other than “philosopher X died”to perfectly picture the death of philosopher, or to not write anything at all in the case of finishing meals. please not that a segfault may occur as a result of trying to access a variable that got removed from the stack because the main process terminated, even though the program is still running, that’s because compile time memory allocation or static memory allocation of vars get deallocated automatically as the end of the scope of the vars, so if the main ended all statically declared vars are deallocated and accessing their memory causes a problem, instead we should use malloc, because then we either need to manually free the dynamically allocated memory or wait for the whole program to end so it will deallocate what was allocated in main, keep in mind that memory leaks can show up if we didn’t allocated in other functions that get called by main.

 This case of segfault is unlikely to take place in a single threaded process, because if we are back to the main function, that means all other functions had terminated, but in the multithreaded program, we use main to create threads and let them run the routine function while meantime we’re back to main to run the rest of our code, so if we are not using pthread_join to make the main thread wait for the threads to finish then the whole thing will end after the main so we should prevent the main from ending unless a certain condition was achieved which is the death of a philosopher or eating all meals.
