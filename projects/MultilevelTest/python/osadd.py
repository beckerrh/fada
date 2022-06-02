import os
import shutil
import glob


# ----------------------------------------------------------------
def localPath(*args):
    args2 = [a for a in list(args) if a != "."]
    return os.getcwd() + os.sep + os.sep.join(args2)

# ----------------------------------------------------------------
def copytree(src, dest):
    try:
        shutil.rmtree(dest)
    except:
        pass
    shutil.copytree(src, dest)
