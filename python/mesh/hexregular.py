import math, os, sys, datetime

meshfile = open("hexregular.hex","w")
meshfile.write('# created by %s on %s user %s\n#\n' %(sys.argv[0], datetime.date.today(), os.getlogin()))

nx = 3
ny = 4
nz = 5

dx = 0.1
dy = 0.1
dz = 0.1


# sommets
meshfile.write('%6d ascii\n' %(nx*ny*nz))
for iz in range(nz):
  for iy in range(ny):
    for ix in range(nx):
      meshfile.write('%22.16f %22.16f %22.16f\n' %(ix*dx,iy*dy,iz*dz))

def index(ix, iy, iz):
  return ix + iy*nx + iz*nx*ny
  
# quads
meshfile.write('\n%6d ascii\n' %((nx-1)*(ny-1)*(nz-1)) )
for iz in range(nz-1):
  for iy in range(ny-1):
    for ix in range(nx-1):
      meshfile.write('%6d %6d %6d %6d %6d %6d %6d %6d\n' %( index(ix,iy,iz), index(ix+1,iy,iz), index(ix+1,iy+1,iz), index(ix,iy+1,iz), index(ix,iy,iz+1), index(ix+1,iy,iz+1), index(ix+1,iy+1,iz+1), index(ix,iy+1,iz+1)  ) )

# sides
meshfile.write('\n%6d ascii\n' %(2*(nx-1)*(ny-1)+2*(ny-1)*(nz-1)+2*(nx-1)*(nz-1)  ))
for ix in range(nx-1):
  for iy in range(ny-1):
    meshfile.write('%6d %6d %6d %6d %8d\n' %(  index(ix,iy,0), index(ix+1,iy,0), index(ix+1,iy+1,0), index(ix,iy+1,0)  , 100))
    meshfile.write('%6d %6d %6d %6d %8d\n' %(  index(ix,iy,nz-1), index(ix+1,iy,nz-1), index(ix+1,iy+1,nz-1), index(ix,iy+1,nz-1)  , 200))

for ix in range(nx-1):
  for iz in range(nz-1):
    meshfile.write('%6d %6d %6d %6d %8d\n' %(  index(ix,0,iz), index(ix+1,0,iz), index(ix+1,0,iz+1), index(ix,0,iz+1)  , 300))
    meshfile.write('%6d %6d %6d %6d %8d\n' %(  index(ix,ny-1,iz), index(ix+1,ny-1,iz), index(ix+1,ny-1,iz+1), index(ix,ny-1,iz+1)  , 400))

for iy in range(ny-1):
  for iz in range(nz-1):
    meshfile.write('%6d %6d %6d %6d %8d\n' %(  index(0,iy,iz), index(0,iy+1,iz), index(0,iy+1,iz+1), index(0,iy,iz+1)  , 500))
    meshfile.write('%6d %6d %6d %6d %8d\n' %(  index(nx-1,iy,iz), index(nx-1,iy+1,iz), index(nx-1,iy+1,iz+1), index(nx-1,iy,iz+1)  , 600))
  
meshfile.close()