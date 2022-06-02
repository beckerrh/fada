import os
import shutil

class pyfPathManager:
    def __init__(self, install_dir="none", resource_dir="none", execpath="none", name="Fada"):
        if execpath == "none" and resource_dir == "none":
            self.install_dir = install_dir
        elif resource_dir != "none":
            assert install_dir == "none"
            self.install_dir = resource_dir
            filehandle = open(self.install_dir + '/SOURCE_DIR_' + name, 'r')
            self.source_dir = filehandle.read()
            filehandle.close()
            filehandle = open(self.install_dir + '/BUILD_DIR_' + name, 'r')
            self.build_dir = filehandle.read()
            filehandle.close()
        else:
            assert install_dir == "none"
            self.install_dir = self.parentDirectory(2, execpath)
            self.fadalight_dir = self.parentDirectory(3, execpath)
            filehandle = open(self.install_dir + '/SOURCE_DIR_' + name, 'r')
            self.source_dir = filehandle.read()
            filehandle.close()
            filehandle = open(self.install_dir + '/BUILD_DIR_' + name, 'r')
            self.build_dir = filehandle.read()
            filehandle.close()

    def parentDirectory(self, n, path):
        if n > 0:
            return self.parentDirectory(n - 1, os.path.split(path)[0])
        else:
            return path


    def storePaths(self, source_dir, build_dir, name="Fada"):
        filehandle = open(self.install_dir + '/SOURCE_DIR_' + name, 'w')
        filehandle.write(source_dir)
        filehandle.close()
        filehandle = open(self.install_dir + '/BUILD_DIR_' + name, 'w')
        filehandle.write(build_dir)
        filehandle.close()
