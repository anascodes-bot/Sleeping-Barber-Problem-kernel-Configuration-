#include<linux/semaphore.h>
#include<linux/kthread.h>
#include<linux/delay.h>
#include<linux/unistd.h>
#include<linux/module.h>
#include<linux/init.h>

#define MAX_CUSTOMERS 50

struct semaphore waitingRoom;
struct semaphore barberChair;
struct semaphore barberPillow;
struct semaphore seatBelt;
int flag = 0;
int temp=25;
void* CUSTOMER (void *customer) {
   
    int c= (int) customer;
    printk("Customer %d leaving for barber shop.\n", c);
    msleep(5);
    printk("Customer %d arrived at barber shop.\n", c);
    down(&waitingRoom); //sem_wait(&waitingRoom);
    printk("Customer %d entering waiting room.\n", c);
    down(&barberChair);//sem_wait(&barberChair);
    up(&waitingRoom);//sem_post(&waitingRoom);
    printk("\n\t\t\tCustomer %d waking the barber.\n", c);
    up(&barberPillow);//sem_post(&barberPillow);
    down(&seatBelt);//sem_wait(&seatBelt);
    up(&barberChair);//sem_post(&barberChair);
    printk("Customer %d leaving barber shop.\n", c);
    return NULL;
}

void* barber(void *val){

    while (!flag) {
        printk("\t\tThe barber is sleeping\n");
       
        down(&barberPillow);//sem_wait(&barberPillow);
        if (!flag){
            printk("\t\tThe barber is cutting hair\n");
            msleep(3);
            printk("\t\tThe barber has finished cutting hair.\n");
            temp--;
            up(&seatBelt); //sem_post(&seatBelt);
        }

        else {
            printk("The barber is going home for the day.\n");
        }
    }
    return NULL;
   
}



asmlinkage long sys_SleepingBarber(void){
    int numCustomers, numChairs;
    int i;
    // do{
    //     printk("Number of customer should be less than 50: ");
    //     scanf("%d",&numCustomers);
    //     printk("\nEnter number of customers and chairs: ");
    //     scanf("%d",&numChairs);
    //     if ((numChairs<0)&&(numCustomers<0)&&(numCustomers > MAX_CUSTOMERS)) {
    //         printk("\n\t\t\tINVALID ENTRY!!!!!!\n");
    //     }
       
    // }while((numChairs<0)&&(numCustomers<0)&&(numCustomers > MAX_CUSTOMERS));
    numCustomers=25;
    numChairs=6;
   
    int customer[MAX_CUSTOMERS];
    struct task_struct *customer_id[MAX_CUSTOMERS]; //kthread_t customer_id[numCustomers];
    struct task_struct *barber_id;  //kthread_t barber_id;
   
    printk("A solution to the sleeping barber problem using semaphores.\n");
    for (i = 0; i < numCustomers; i++) {
        customer[i] = i+1;
    }

    sema_init(&waitingRoom, numChairs);
    sema_init(&barberChair, 1);
    sema_init(&barberPillow, 0);
    sema_init(&seatBelt,0);
    int junk=0;
    //pthread_create(&barber_id, NULL, barber, NULL);
    barber_id= kthread_create((void*)barber,(void *)&junk,"Barber");
    if(barber_id){
        wake_up_process(barber_id);
    }
    else{
        kthread_stop(barber_id);
    }
       
    for (i = 0; i < numCustomers; i++) {
        customer_id[i] = kthread_create((void*)CUSTOMER, (void *)&customer[i],"customer");
        if(customer_id[i]){
            wake_up_process(customer_id[i]);
        }
        else{
            kthread_stop(customer_id[i]);
        }
    }
   
    if(temp==0){
        flag=1;
        up(&barberPillow);
    }
   
   
    return 0;
}
