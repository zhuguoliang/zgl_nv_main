import os
import sys
import numpy as np
import matplotlib.mlab as mlab
#import matplotlib.pyplot as plt
import pylab as pl
import os, fnmatch

import linecache
def all_files(root, patterns='*', single_level=False,yield_folders=False):
    patterns = patterns.split(';')
    for path,subdirs,files in os.walk(root):
            if yield_folders:
                files.extend(subdirs)
            files.sort()
            for name in files:
                for pattern in patterns:
                    if fnmatch.fnmatch(name,pattern):
                        yield os.path.join(path,name)
                        break
            
            if single_level:
                break

 #analysis a file in format <a_b> in which a is cycle b is data 
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

#analysis the memory consumed by app in each cycle epoch
#default incre=1000
#total 1000*100=100000 runs
def mult_run(incre,tracename):
    os.system("rm -rf row_count")
    cycle=0
    for i in range(100):
        cycle = cycle + incre
        out_fname = "test.out.normal" 
        cmd = "./nvmain.debug Config/ZGL_Standard_DRAM_channel.config " + tracename + " " + str(cycle) + " > " + "/dev/null"
        os.system("rm -rf *.out")
        os.system(cmd)
        tot_row_count=0
        for path in all_files('.','*.out'):
            count = len(open(path,'rU').readlines())
            tot_row_count +=count
            print path + " line_count is " + str(count)
            os.system("rm -rf " + path)
        open("row_count",'a').write(str(cycle)+"_")
        open("row_count",'a').write(str(tot_row_count))
        open("row_count",'a').write("\n")

            


 
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
    fig = pl.figure()

    ax1 = fig.add_subplot(111)

    ax1.set_title("Row count accumulate with time")    
    ax1.set_xlabel('cycle')
    ax1.set_ylabel('row count')

    ax1.plot(x,y, c='r', label='row access count')

    leg = ax1.legend()

    ax1.grid(True)
    pl.show()

    # Save figure using 72 dots per inch
    pl.savefig("401_row_count.png",dpi=72)

def plot_multi(file_name_list):
#    delete_last_line(stat_file_name)
    fig = pl.figure(1)    
    for fname in file_name_list:
        with open(fname) as f:
            data = f.read()
        data = data.split()
        x = [row.split("_")[0] for row in data]
        y = [row.split("_")[1] for row in data]
        
        ax1 = fig.add_subplot(111)
        ax1.set_title("Row count accumulate with time")    
        ax1.set_xlabel('cycle')
        ax1.set_ylabel('row count')
        ax1.grid(True)   
        #pl.plot(x,y, c='r', label=fname)
        pl.plot(x,y, label=fname.split(".")[1])
    leg=pl.legend(loc=2)
    pl.show()
    # Save figure using 72 dots per inch
    pl.savefig("All_row_count.png",dpi=72)


#mult_run(1000, "../../../traces/429_ref.trace.trc_nvmain")
#mult_run(10000, "../../../HMTT_1GB/401_ref.trace0.trc_nvmain")

#1. run with minus
#minus_two_consecutive_line("row_count")
#plot("row_count.minus")


#2 run without minus
#plot("row_count")

fname_list=["row_count.401","row_count.403","row_count.429","row_count.437",
"row_count.450","row_count.453","row_count.462","row_count.464","row_count.470","row_count.471"]
plot_multi(fname_list)

