import os
import sys
import numpy as np
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt
import os, fnmatch

import linecache



print "In My_Data_Analysis.py, please input the feature of memory to analyse"
#print "Usage: python My_Data_Analysis.py Parm incre trace"
#print "Example: python My_Data_Analysis.py i0.defaultMemory.totalReadRequests 1000 ../../../traces/429_ref.trace.trc_nvmain"
print sys.argv
print len(sys.argv)
length = len(sys.argv)
if( length < 4):
    print "Parm not enough or too much"
    print "Usage: python My_Data_Analysis.py Parm incre trace"
    print "Example: python My_Data_Analysis.py i0.defaultMemory.totalReadRequests 1000 ../../../traces/429_ref.trace.trc_nvmain"
    sys.exit(2)
key_name = sys.argv[1]
increment = int(sys.argv[2])
trace_name = sys.argv[3]
#cmd = ./nvmain.debug Config/ZGL_Standard_DRAM_channel.config ../../../traces/429_ref.trace.trc_nvmain 10000 > test.out.normal

#tracename = "../../../traces/429_ref.trace.trc_nvmain"

#1000 2000 3000 4000
#incre = 1000
#key = "i0.defaultMemory.totalReadRequests"
#cycle = 0
#output stat data of key in form : [cycle_data]

def Stat_key(key,incre,tracename):
    os.system("rm -rf "+ key + ".stat")
    cycle=0
    for i in range(100):
        cycle = cycle + incre
        out_fname = "test_" + str(cycle) 
        cmd = "./nvmain.debug Config/ZGL_Standard_DRAM_channel.config " + tracename + " " + str(cycle) + " > " + out_fname
        os.system(cmd)
        f_object = open(out_fname)
        for line in f_object:
            if key in line:
                open(key + ".stat",'a').write(str(cycle)+"_")
                open(key + ".stat",'a').write(line.split()[1])
                open(key + ".stat",'a').write("\n")
        f_object.close()
        os.system("rm -rf "+ out_fname)


def minus_two_consecutive_line(fname):

    count = len(open(fname).readlines()) 
    ll_data=0
    ll_cycle=0
    for i in range(count):
        curl_data=int(linecache.getline(fname,i+1).split("_")[1])
        curl_cycle=int(linecache.getline(fname,i+1).split("_")[0])
        
        minus_data=curl_data-ll_data
        minus_cycle=curl_cycle

        open(fname+ ".minus",'a').write(str(minus_cycle)+"_")
        open(fname+ ".minus",'a').write(str(minus_data))
        open(fname+ ".minus",'a').write("\n")
        
        ll_data=curl_data
        ll_cycle=curl_cycle
 

#Stat_key("i0.defaultMemory.totalReadRequests",1000, "../../../traces/429_ref.trace.trc_nvmain")
#Stat_key("i0.defaultMemory.channel0.FRFCFS.rb_hits",1000, "../../../traces/429_ref.trace.trc_nvmain")



def delete_last_line(fname):
    readFile = open(fname)
    lines = readFile.readlines()
    readFile.close()
    w = open(fname,'w')
    w.writelines([item for item in lines[:-1]])
    w.close()



#plot the data from stat file
#n_groups: the number of data groups
#stat_file_name: name of stat_file , etc:"i0.defaultMemory.totalReadRequests.stat"
#incre: the x axis incre by incre, take default value of 1000
def plot(stat_file_name):
#    delete_last_line(stat_file_name)
    with open(stat_file_name) as f:
        data=f.read()
#   data.rstrip()
    data = data.split()

#    for row in data:
    x = [row.split("_")[0] for row in data]
    y = [row.split("_")[1] for row in data]

    print x
    print y
    fig = plt.figure()

    ax1 = fig.add_subplot(111)

    ax1.set_title("Plot title...")    
    ax1.set_xlabel('cycle')
    ax1.set_ylabel('row buffer miss')

    ax1.plot(x,y, c='r', label='row buffer miss curve')

    leg = ax1.legend()

    plt.show()

#    rects1 = plt.bar(index, means_men, bar_width,alpha=opacity, color='b',label=    'Men')
    

def plot_bar(stat_file_name):
    with open(stat_file_name) as f:
        data=f.read()

    data = data.split()

    for row in data:
        x = [row.split("_")[0] for row in data]
        y = [row.split("_")[1] for row in data]
    
    y=map(int, y)
    n_groups = 10
#y[0] y[9] y[19] y[39] y[49] y[59] y[69] y[79] y[89] y[99]
#    print tuple(y[0:n_groups])
    height = y[0] ,y[9] ,y[19] ,y[39] ,y[49] ,y[59] ,y[69] ,y[79] ,y[89] ,y[99]
    print height
    fig = plt.subplots()
    index=np.arange(n_groups)
    bar_width = 0.2
    opacity=0.4
    rect = plt.bar(index,height,bar_width,alpha=opacity, color='b',label= 'miss rate')
    plt.xlabel('cycle')
    plt.ylabel('miss rate')
    plt.xticks(index + bar_width/2,  ('A', 'B', 'C', 'D', 'E'))
    plt.legend()
    plt.tight_layout()
    plt.show()
#    means_men = (20, 35, 30, 35, 27)  
#    means_women = (25, 32, 34, 20, 25)  
  
#    fig, ax = plt.subplots()  
#    index = np.arange(n_groups)  
#    bar_width = 0.35  
#  
#    opacity = 0.4  
#    rects1 = plt.bar(index, means_men, bar_width,alpha=opacity, color='b',label=    'Men')  
#    rects2 = plt.bar(index + bar_width, means_women, bar_width,alpha=opacity,col    or='r',label='Women')  
  
#plt.xlabel('Group')  
#plt.ylabel('Scores')  
#plt.title('Scores by group and gender')  
#plt.xticks(index + bar_width, ('A', 'B', 'C', 'D', 'E'))  
#plt.ylim(0,40)  
#plt.legend()  
#  
#plt.tight_layout()  
#plt.show()  


minus_two_consecutive_line("i0.defaultMemory.channel0.FRFCFS.rb_hits.stat")
#plot("i0.defaultMemory.totalReadRequests.stat")
#plot_bar("i0.defaultMemory.channel0.FRFCFS.rb_hits.stat")
plot("i0.defaultMemory.channel0.FRFCFS.rb_hits.stat.minus")

