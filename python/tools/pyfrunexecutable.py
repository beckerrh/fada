# -*- coding: utf-8 -*-

from __future__ import print_function
import sys
import subprocess
import time
import os
# ------------------------------------------------------------------------

modes = ['shell', 'pipe', 'file']
MODE = modes[0]


class pyfRunExecutable:
    def run(self, cmd, args=[], mode="none"):
        if mode != "none":
            global MODE
            MODE = mode
        args_list = []
        args_list.append(cmd)
        args_list.extend(args)
        self.run_subprocess(args_list)

    # def runloop(self, loopname, rundir, cpp_param, project_exec=None):
    #     if project_exec is None: project_exec = os.getcwd().split('/')[-1]
    #     paramfile = project_exec + ".fadalightparam"
    #     name = os.path.join(rundir, paramfile)
    #     cpp_param.writeParameters(name)
    #     args_list = ["./" + project_exec, loopname, rundir]
    #     self.run_subprocess(args_list)

    def run_subprocess(self, cmd):
        program = cmd[0]
        if not os.path.isfile(program):
            raise KeyError(
                "program '%s' does not exist (maybe you need to compiel...)" % program)
        global MODE
        if MODE == 'file':
            try:
                stdout_file = open('./fadalight.stdout', 'a+')
                stderr_file = open('./fadalight.stderr', 'a+')
            except:
                raise IOError('cannot open files')
        elif MODE == 'pipe':
            stdout_file = subprocess.PIPE
            stderr_file = subprocess.PIPE
        starttime = time.time()
        cmdline = ' '.join([str(x) for x in cmd])
        if not(len(cmdline)):
            raise KeyError('### ERROR in run_subprocess cmd=' +
                           cmd + '\ncmdline=' + cmdline)
        if MODE == 'shell':
            proc = subprocess.Popen(cmdline, shell=True)
        else:
            proc = subprocess.Popen(
                args=cmd, stdout=stdout_file, stderr=stderr_file, shell=False)
        proc.wait()
        self.elapsed = (time.time() - starttime)
        comm = proc.communicate()
        if MODE == 'file':
            stdout_file.seek(0)
            stdout = stdout_file.read()
            stdout_file.close()
            stderr_file.seek(0)
            stderr = stderr_file.read()
            stderr_file.close()
        else:
            stdout = comm[0]
            stderr = comm[1]
        if proc.returncode:
            # raise ValueError('@@@ ERROR: could not execute command'+ str(cmd)+'\n@@@ ERROR: returncode = '+ str(proc.returncode)+'\n@@@ ERROR subprocess says: \"'+ str(stderr)+ "\"")
            print('cmd = ')
            for c in cmd:
                print(c, end=' ')
            raise ValueError('@@@ ERROR: could not execute command\n' + ' '.join(str(e) for e in cmd) +
                             '\n@@@ ERROR: returncode = ' + str(proc.returncode) + '\n@@@ ERROR subprocess says: \"' + str(stderr) + "\"")
