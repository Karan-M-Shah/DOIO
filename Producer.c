//Karan Shah
//HW3
//CGS3763

//Creating semaphores
semaphore full;
semaphore full2;
semaphore full3;
semaphore empty;
binarysem mutex;
binarysem mutex2;
binarysem mutex3;
semaphore to;
semaphore ioreq;
semaphore operation_complete;
semaphore request_served[5];
semaphore reqpending;
semaphore PhysicalIO;

//Constants and Global Variables
const int length = 5;
const int RANGE1 = 1;
const int RANGE2 = 20;
int buffer[length];
int in = 0;
int out = 0;
int addr = 0;
int bufid = 1;
int bufdev = 1;
int bufaddr = 1;
int iorqid_0 = 5;
int iorqaddr_0 = 5;
int iorqid_1 = 5;
int iorqaddr_1 = 5;
int pio = 1;

//Insert into buffer
void add(int num)
{
  buffer[in] = num;
  in = (in + 1) % length;
}

//Remove from buffer
int remove()
{
  int temp;
  temp = buffer[out];
  out = (out + 1) % length;
  return temp;
}

//Produce item
int produce()
{
  int pcount = 0;
  if(pcount == 20)
  {
    return;
  }
  p(to);
  cout << "Item" << pcount << "produced" << endl;
  v(to);
  pcount = pcount + 1;
  return pcount;
}

//Consume item
void consume(int i)
{
  int ccount = 0;
  if(ccount == 20)
  {
    return;
  }
  p(to);
  cout << "Item" << ccount << "consumed" << endl;
  v(to);
  ccount = ccount + 1;
}

void producer()
{
  int i;
  for(;;)
  {
    i = produce();
    p(full);
    p(mutex);
    add(i);
    cout << "Item" << empty << "produced"  << endl;
    v(mutex);
    v(empty);
  }
}

void consumer() 
{
  int i;
  for (;;)
  {
    wait(empty);  
    wait(mutex);  
    i = remove(); 
    cout << "Item" << full << "consumed" << endl;
    signal(mutex);
    signal(full); 
    consume(i);
  } 
}

void user(int id)
{
  int i;
  int dev = RANGE1;
  int addr = RANGE2;

  for(i = 0; i < 5; i++)
  {
    p(full);
    p(mutex);
    bufid = id;
    bufdev = dev;
    bufaddr = addr;
    v(mutex);
    v(ioreq);
    p(request_served[id]);
  }
}

void DOIO()
{
  int id;
  int dev;
  int addr;
  int i;

  for(i = 0; i < 5; i++)
  {
    p(ioreq);
    p(mutex);
    id = bufid;
    dev = bufdev;
    addr = bufaddr;
    v(mutex);
    v(full);
    if(dev == 0)
    {
      iorqid_0 = id;
      iorqaddr_0 = addr; 
    }
    else
    {
      iorqid_1 = id;
      iorqaddr_1 = addr;
    }
    v(mutex2);
    v(reqpending);
  }
}

void Driver_1()
{
  int i;
  int id;
  int addr;

  for(i = 0; i < 5; i++)
  {
    p(reqpending);
    p(mutex2);
    id = iorqid_0;
    addr = iorqaddr_0;
    v(mutex2);
    v(full2);
    p(full3);
    p(mutex3);
    pio = addr;
    v(mutex);
    v(PhysicalIO);
    p(operation_complete);
    v(request_served[id]);
  }
}

void Driver_2()
{
  int i;
  int id;
  int addr;

  for(i = 0; i < 5; i++)
  {
    p(reqpending);
    p(mutex2);
    id = iorqid_1;
    addr = iorqaddr_1;
    v(mutex2);
    v(full2);
    p(full3);
    p(mutex3);
    pio = addr;
    v(mutex);
    v(PhysicalIO);
    p(operation_complete);
    v(request_served[id]);
  }
}


void Disk_1()
{
  int i;
  int seek;

  for(i = 0; i < 5; i++)
  {
    p(PhysicalIO);
    p(mutex3);
    seek = pio;
    v(mutex);
    for(i = seek; i < 10; i++)
    {
      ;
    }
    v(operation_complete);
    v(full3);
  }
}

void Disk_2()
  {
  int i;
  int seek;

  for(i = 0; i < 5; i++)
  {
    p(PhysicalIO);
    p(mutex3);
    seek = pio;
    v(mutex);
    for(i = seek; i < 10; i++)
    {
      ;
    }
    v(operation_complete);
    v(full3);
  }
}

main()
{
initialsem(full, 1);
initialsem(full2, 1);
initialsem(full3, 5);
initialsem(empty, 0);
initialsem(mutex, 1);
initialsem(mutex2, 1);
initialsem(mutex3, 1);
initialsem(to, 1);
initialsem(ioreq, 1);
initialsem( request_served[0], 0);
initialsem( request_served[1], 0);
initialsem( request_served[2], 0);
initialsem( request_served[3], 0);
initialsem( request_served[4], 0);
initialsem(operation_complete, 1);
initialsem(reqpending, 1);
initialsem(PhysicalIO, 1);
    cobegin {
        user(0); user(1); user(2); user(3); user(4); DOIO(); Driver_1(); Driver_2(); Disk_1(); Disk_2();
    }
}
