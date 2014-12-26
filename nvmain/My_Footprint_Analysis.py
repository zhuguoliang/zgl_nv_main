import os
import fnmatch
import linecache

#thefile="defaultMemory.channel0.FRFCFS.channel0.rank0.bank0.row_access_counter.out"
#count = len(open(thefile,'rU').readlines())
#print thefile + " line_count is " + str(count)


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

tot_row_count=0
for path in all_files('.','*.out'):
#    print path
    count =  len(open(path,'rU').readlines())
    tot_row_count +=count
    print path + " line_count is " + str(count)
print "tot_row_count is " + str(tot_row_count)    
