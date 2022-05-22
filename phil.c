#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>

#define N 4

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Anas Khan, Aamir khan, Abdullah Naeem");
MODULE_DESCRIPTION("Dining Philosopher Solution!");
MODULE_VERSION("0.001");

int pid[5] = {0,1,2,3,4};
struct mutex forks[5];
struct mutex lock;
struct task_struct *diner1, *diner2, *diner3, *diner4, *diner5;

int philosopher_eating(int phil_no)
{
	if (phil_no != N)
	{
	    mutex_lock(&forks[((phil_no + 4) % N)]);     // checking availability of left fork
	    mutex_lock(&forks[phil_no + 1]);     	   // checking availability of right fork
	    printk(KERN_INFO "Philosopher %d took fork %d and fork %d\n", phil_no , ((phil_no + 4) % 5), phil_no);
	    mutex_lock(&lock);
	    printk(KERN_INFO "Philosopher %d is eating...\n", phil_no);    // critical section
	    mutex_unlock(&lock);
	    printk(KERN_INFO "Philosopher %d put fork %d and fork %d\n\n", phil_no, ((phil_no + 4) % 5), phil_no);
	    mutex_unlock(&forks[((phil_no + 4) % N)]);   // putting left fork
	    mutex_unlock(&forks[phil_no + 1]);   	   // putting right fork
	}	
	else if (phil_no == N)
	{
	    mutex_lock(&forks[phil_no]);     		   // checking availability of right fork
	    mutex_lock(&forks[((phil_no + 4) % N)]);     // checking availability of left fork
	    printk(KERN_INFO "Philosopher %d took fork %d and fork %d\n", phil_no, phil_no, ((phil_no + 4) % 5));
	    mutex_lock(&lock);
	    printk(KERN_INFO "Philosopher %d is eating...\n", phil_no);   // critical section
	    mutex_unlock(&lock);
	    printk(KERN_INFO "Philosopher %d put fork %d and fork %d\n\n", phil_no, phil_no, ((phil_no + 4) % 5));
	    mutex_unlock(&forks[phil_no]);      	   // putting right fork
	    mutex_unlock(&forks[((phil_no + 4) % N)]);   // putting left fork	   
	}
	return 0;
}


int philosopher_thinking(void *data)
{
	while(1)
	{
		int phil_no = *(int*)data;     // data indicates philosopher number
		fsleep(1200);    // critical section - thinking
		printk(KERN_INFO "Philosopher %d thinking...\n", phil_no);
		philosopher_eating(phil_no);
	}
	return 0;
}


int philosopher_init(void)
{
    // thread creation and wakeup 
    
     diner1 = kthread_create(&philosopher_thinking, (void *)&pid[0], "thread1");
     diner2 = kthread_create(&philosopher_thinking, (void *)&pid[1], "thread2");        
     diner3 = kthread_create(&philosopher_thinking, (void *)&pid[2], "thread3");      
     diner4 = kthread_create(&philosopher_thinking, (void *)&pid[3], "thread4");    
     diner5 = kthread_create(&philosopher_thinking, (void *)&pid[4], "thread5");  
     
     // waking up threads
     
     wake_up_process(diner1);
     wake_up_process(diner5);
     wake_up_process(diner2);
     wake_up_process(diner4); 
     wake_up_process(diner3);
                      
     return 0;
}

// kernel init function
int init_module(void)
{
    philosopher_init();
    return 0;
}

// kernel exit function
void cleanup_module(void)
{
    return;
}
