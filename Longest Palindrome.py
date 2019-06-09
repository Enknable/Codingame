import sys
import math
palin = []

s = input()
#print(s)
for i in range(1,len(s)-1):
    search = 1
    if s[i-1:i+2] == s[i-1:i+2][::-1]:
        pivot_l = i-1
        pivot_r = i+1
        
        while(search is 1):
            if pivot_l-1 >= 0 and pivot_r+1 <= len(s)+1:
                if s[pivot_l-1:pivot_r+2] == s[pivot_l-1:pivot_r+2][::-1]:
                    pivot_l -= 1
                    pivot_r += 1
                else:
                    pivot_l += 1
                    pivot_r -= 1
                    palin.append(s[pivot_l-1:pivot_r+2])
                    search = 0
            else:
                search = 0
                palin.append(s[pivot_l:pivot_r+1])
    search = 1            
    if s[i-1:i+1] == s[i-1:i+1][::-1]:
        pivot_l = i-1
        pivot_r = i+1
        
        while(search is 1):
            if pivot_l-1 >= 0 and pivot_r+1 <= len(s)+1:
                if s[pivot_l-1:pivot_r+1] == s[pivot_l-1:pivot_r+1][::-1]:
                    pivot_l -= 1
                    pivot_r += 1
                else:
                    pivot_l += 1
                    pivot_r -= 1
                    palin.append(s[pivot_l-1:pivot_r+1])
                    search = 0
            else:
                search = 0
                palin.append(s[pivot_l:pivot_r+1])

maxs = len(max(palin, key=len))
for i in palin:
    if len(i) == maxs:
        print(i)
    else:
        pass
        