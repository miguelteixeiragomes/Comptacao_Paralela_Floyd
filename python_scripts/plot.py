import pylab as pl

'''
This script takes the data from the record_time script
and plots the graphs of the computing time for 
the different cluster settings
'''

f = open("DATA.txt", "r")
lst = [i.replace('\n', '') for i in f.readlines() if i != '\n']
f.close()
colors = ['b', 'r', 'g', 'o']

pl.figure("Execution time")
pl.title("Execution time")
pl.ylabel("t (s)")
pl.xlabel("N")
pl.xscale('log')
pl.yscale('log')
pl.grid(b = True, which = 'major', color = 'black', linestyle = ':')
#pl.grid(b = True, which = 'minor', color = 'black', linestyle = ':')

i = 0
label = lst[0]
labeling = True
for line in range(1, len(lst)):
    try:
        N  = int(lst[line].split(' ')[0])
        t  = float(lst[line].split(' ')[1])
        dt = float(lst[line].split(' ')[2])
        if labeling:
            pl.scatter([N], [t], color = colors[i], marker = 'o', label = label, alpha = .5)
        else:
            pl.scatter([N], [t], color = colors[i], marker = 'o', alpha = .5)
        pl.scatter([N], [t + dt], color = colors[i], marker = '_', alpha = .5)
        pl.scatter([N], [t - dt], color = colors[i], marker = '_', alpha = .5)
        pl.plot([N]*2, [t - dt, t + dt], color = colors[i], alpha = .5)
        labeling = False

    except:
        label = lst[line]
        i += 1
        labeling = True

pl.legend()
pl.show()
