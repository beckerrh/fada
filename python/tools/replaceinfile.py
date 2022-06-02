import sys, os, subprocess, shutil

def replaceinfile(infilename, outfilename, wordstochange):
    file = open(infilename, 'r')
    oldlines = file.readlines()
    newlines = []
    for line in oldlines:
        for ow, nw in wordstochange.items():
            # print 'ow, nw', ow, nw
            line = line.replace(ow,nw)
            # print 'line', line
        newlines.append(line)
    file.close()
    file = open(outfilename, 'w')
    for line in newlines:
        file.write(line)
    file.close()
    return



def is_binary(filename):
    fin = open(filename, 'rb')
    try:
        CHUNKSIZE = 1024
        while 1:
            chunk = fin.read(CHUNKSIZE)
            if '\0' in chunk: # found null byte
                return True
            if len(chunk) < CHUNKSIZE:
                break # done
    finally:
        fin.close()
    return False


def myreplace(toto):
    return toto.replace("Fada","CONCHA2").replace("Fada","Concha2").replace("fadalight","concha2")

if len(sys.argv) != 2:
    raise ValueError("usage: changenampe.py <DIRNAME>")
newdir = sys.argv[1]
startdir = newdir + '.Fada'

if(os.path.isdir(newdir)) :
  raise ValueError("directory exists " + newdir)

for root, dirs, files in os.walk(startdir):
  if root.find(".svn") !=-1:
    raise NameError("@@@ ERROR in toto: subfolders must not contain .svn "+ root)
  # rootnew = root.replace(startdir,newdir).replace("Fada","Concha2").replace("fadalight","concha2")
  rootnew = myreplace(root.replace(startdir,newdir))
  print('rootnew', rootnew)
  os.mkdir(rootnew)
  for file in files:
    fileroot = root + '/' + file
    # print 'fileroot: ', fileroot
    # continue
    # filenew = fileroot.replace(startdir,newdir).replace("Fada","Concha2").replace("fadalight","concha2")
    filenew = myreplace(fileroot.replace(startdir,newdir))
    if file.find('.tgz') !=-1 or  file.find('.png') !=-1:
      shutil.copyfile(fileroot, filenew)
      continue
    print('filenew', filenew)
    if is_binary(fileroot):
      print ('file is binary', fileroot)
      continue
    infile = open(fileroot, 'r')
    outfile = open(filenew, 'w')
    toto = infile.read()
    # print 'toto', toto
    # totonew = toto.replace("Fada","Concha2").replace("fadalight","concha2")
    totonew = myreplace(toto)
    # print 'totonew', totonew
    outfile.write(totonew)
    infile.close()
    outfile.close()
