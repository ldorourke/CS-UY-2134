import time
import random

def selectionSort(alist):
   for fillslot in range(len(alist)-1,0,-1):
       positionOfMax=0
       for location in range(1,fillslot+1):
           if alist[location]>alist[positionOfMax]:
               positionOfMax = location

       temp = alist[fillslot]
       alist[fillslot] = alist[positionOfMax]
       alist[positionOfMax] = temp

def main():

   alist = []
   for x in range(100000):
      integer = random.randint(0,100)
      alist.append(integer)
   
   selectionSort(alist)

t1 = time.time()
print("Starting time:", t1)
main()
t2 = time.time()
print("Ending time:", t2)
print("Time to run 100000 inputs:", t2 - t1, "seconds")

