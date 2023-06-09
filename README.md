# Sleeping-Barber-Problem-kernel-Configuration-

Implemented the Sleeping Barber problem using semaphores in the Linux kernel. It creates multiple customer threads and a barber thread, simulating customers visiting a barber shop.
Customers arrive, check for available chairs in the waiting room, and occupy a chair if available.
They then wait for the barber chair to be free, wake the sleeping barber, get their hair cut, and leave. 
The barber thread sleeps until woken up by a customer, cuts their hair, and repeats the process until there are no more customers.
Semaphores are used to coordinate access to shared resources such as the waiting room and barber chair.




