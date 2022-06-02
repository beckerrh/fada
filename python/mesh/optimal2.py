import math, os, sys, datetime

k = 8
l = 17
r = [0.054, 0.062, 0.07]

meshfile = open("optimal2.quad","w")
meshfile.write('# created by %s on %s user %s\n#\n' %(sys.argv[0], datetime.date.today(), os.getlogin()))
meshfile.write('%6d ascii\n' %(3*l*k + 2*k) )

# sommets
d = 2.0*math.pi/(l*k)
for j in range(3):
  rr = float(r[j])
  for j in range(l*k):
    xj = rr* math.cos((j-0.5)*d)
    yj = rr* math.sin((j-0.5)*d)
    meshfile.write('%22.16f %22.16f %22.16f\n' %(xj,yj,0))
step = 1.0*(r[1]-r[0])
for j in range(k):
  j1 = l*j
  xp = r[2]* math.cos((j1-0.5)*d)
  yp = r[2]* math.sin((j1-0.5)*d)
  xn = r[2]* math.cos((j1+0.5)*d)
  yn = r[2]* math.sin((j1+0.5)*d)
  dy = xp-xn
  dx = yn-yp
  rr = math.sqrt(dx*dx+dy*dy)
  dx = step*dx/rr
  dy = step*dy/rr 
  meshfile.write('%22.16f %22.16f %22.16f\n' %(xp+dx,yp+dy,0))
  meshfile.write('%22.16f %22.16f %22.16f\n' %(xn+dx,yn+dy,0))
  
# quads
meshfile.write('\n%6d ascii\n' %( 2*l*k + k) )
for j in range(l*k):
  meshfile.write('%6d %6d %6d %6d\n' %( (j+1)%(l*k), j, l*k+j, l*k+(j+1)%(l*k)))
  meshfile.write('%6d %6d %6d %6d\n' %( l*k+(j+1)%(l*k), l*k+j, 2*l*k+j,  2*l*k+(j+1)%(l*k) ))
for j in range(k):
  meshfile.write('%6d %6d %6d %6d\n' %(2*l*k+l*j+1, 2*l*k+l*j, 3*l*k+2*j, 3*l*k+2*j+1))

# lines
meshfile.write('\n%6d ascii\n' %(2*l*k + 2*k))
for j in range(l*k):
  meshfile.write('%6d %6d %8d\n' %(j,(j+1)%(l*k), 300))
for j in range(l*k):                            
  if j % l != 0 :
    meshfile.write('%6d %6d %8d\n' %(2*l*k+j,2*l*k+(j+1)%(l*k), 100))
for j in range(k):
  meshfile.write('%6d %6d %8d\n' %(3*l*k+2*j ,3*l*k+2*j+1, 10+j))
for j in range(k):
  meshfile.write('%6d %6d %8d\n' %(2*l*k+l*j, 3*l*k+2*j, 200))
  meshfile.write('%6d %6d %8d\n' %(2*l*k+l*j+1, 3*l*k+2*j+1, 200))


# curved boundaries
meshfile.write('\n2\n')
meshfile.write('100 CurvedBoundaryDescriptionQuadratic2d\n')
meshfile.write('300 CurvedBoundaryDescriptionQuadratic2d\n')
meshfile.write('6 ascii\n%22.16f 0.0 0.0 1.0 1.0 0.0\n' %(-r[2]*r[2]))
meshfile.write('6 ascii\n%22.16f 0.0 0.0 1.0 1.0 0.0\n' %(-r[0]*r[0]))
  
meshfile.close()