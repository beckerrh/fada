import math, os, sys, datetime

k=4
l = 2
r = [0.054, 0.07]

meshfile = open("bidule.quad","w")
meshfile.write('# created by %s on %s user %s\n#\n' %(sys.argv[0], datetime.date.today(), os.getlogin()))
meshfile.write('%6d ascii\n' %(2*l*k))

# sommets
d = 2.0*math.pi/(l*k)
for j in range(2):
  rr = float(r[j])
  for j in range(l*k):
    xj = rr* math.cos((j-0.5)*d)
    yj = rr* math.sin((j-0.5)*d)
    meshfile.write('%22.16f %22.16f %22.16f\n' %(xj,yj,0))
  
# quads
meshfile.write('\n%6d ascii\n' %(l*k))
for j in range(l*k):
  meshfile.write('%6d %6d %6d %6d\n' %( (j+1)%(l*k), j, l*k+j, l*k+(j+1)%(l*k)))

# lines
meshfile.write('\n%6d ascii\n' %(2*l*k))
for j in range(l*k):
  meshfile.write('%6d %6d %8d\n' %(j,(j+1)%(l*k), 300))
for j in range(l*k):
  color = 100
  if j%l == 0 : color = 10+j/l
  meshfile.write('%6d %6d %8d\n' %(l*k+j,l*k+(j+1)%(l*k), color))


# curved boundary
# meshfile.write('\n1\n')
# meshfile.write('300 CurvedBoundaryDescriptionQuadratic2d\n')
# meshfile.write('6 ascii\n%22.16f 0.0 0.0 1.0 1.0 0.0\n' %(-r[0]*r[0]))

meshfile.write('\n2\n')
meshfile.write('100 CurvedBoundaryDescriptionQuadratic2d\n')
meshfile.write('300 CurvedBoundaryDescriptionQuadratic2d\n')
meshfile.write('6 ascii\n%22.16f 0.0 0.0 1.0 1.0 0.0\n' %(-r[1]*r[1]))
meshfile.write('6 ascii\n%22.16f 0.0 0.0 1.0 1.0 0.0\n' %(-r[0]*r[0]))
  
meshfile.close()