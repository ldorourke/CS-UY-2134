import random
import time

def insertionSort(alist):


   t1 = time.time()
   print(t1)
   
   for index in range(1,len(alist)):

     currentvalue = alist[index]
     position = index

     while position>0 and alist[position-1]>currentvalue:
         alist[position]=alist[position-1]
         position = position-1

     alist[position]=currentvalue

   t2 = time.time()
   print(t2)
   
   print("Time to run 100000 inputs:", t2-t1, "seconds")

alist = []


def main():
   for x in range(100000):
      integer = random.randint(0, 100)
      alist.append(integer)
   
   insertionSort(alist)

main()

