//
//  operateur_mg.cpp
//  Fada
//
//  Created by Roland Becker on 02/05/2020.
//  Copyright © 2020 Roland Becker. All rights reserved.
//

#include  <stdio.h>
#include  <math.h>
#include  "operator.hpp"
#include  <armadillo>

/*-------------------------------------------------*/
int Operator::solve(bool print)
{
  VectorMG& u = _mgmem(0);
  VectorMG& f = _mgmem(1);
  VectorMG& d = _mgmem(2);
  VectorMG& w = _mgmem(3);

  int maxlevel = _mggrid.nlevels()-1;
  double res, tol;
//  std::cerr << "smoother " << smoother << std::endl;
//  std::cerr << "maxlevel " << maxlevel << std::endl;
  for(int iter=0; iter<this->maxiter+1; iter++)
  {
//    for(int l=0; l <= maxlevel; l++) w(l).fill(0.0);
//    std::cerr <<d(maxlevel).norm()<<" "<<f(maxlevel).norm()<<" "<<u(maxlevel).norm() << "\n";
    _timer.start("residual");
    residual(maxlevel, d(maxlevel), u(maxlevel), f(maxlevel));
    _timer.stop("residual");
//    std::cerr <<d(maxlevel).norm()<<" "<<f(maxlevel).norm()<<" "<<u(maxlevel).norm() << "\n";

    res = d(maxlevel).norm();
    if(iter==0)
    {
      tol = fmax(this->tol_abs, this->tol_rel*res);
      if(print) printf("--- %10.3e ---\n", tol);
    }
    if(print) printf("%3d %10.3e\n", iter, res);
    if(res <= tol)
    {
      return iter;
    }
    mgstep(maxlevel, u, f, d, w, tol);
  }
  return -1;
}
/*-------------------------------------------------*/
void Operator::solve_coarse(int l, Vector& u, const Vector& f, Vector& d, Vector& w) const
{
  _spmat.solve(u, f);
  return;
  u.fill(0);
  for(int iter=0;iter<100;iter++)
  {
    d = f;
    _spmat.getSparseMatrix().dot(d, u, -1.0);
//    residual(l, d, u, f);
    double res = d.norm();
    printf("-- %3d %10.2e\n",iter, res);
    if(res<1e-14) break;
    _mgmatrix(l)->jacobi(w,d);
    u.add(1.0, w);
//    _mgupdatesmooth(l)->addUpdate(w, u, d);
  }
  _spmat.solve(w, f);
  w.add(-1.0,u);
  assert(w.norm()<1e-12);
}

/*-------------------------------------------------*/
void Operator::mgstep(int l, VectorMG& u, VectorMG& f, VectorMG& d, VectorMG& w, double tol)
{
  if(l==_mggrid.minlevel())
  {
      _timer.start("solvecoarse");
      solve_coarse(l, u(l), f(l), d(l), w(l));
      _timer.stop("solvecoarse");
  }
  else
  {
    _timer.start("smooth");
    smoothpre(l, w(l), d(l));
    _timer.stop("smooth");
    _timer.start("update");
   _mgupdatesmooth(l)->addUpdate(w(l), u(l), d(l));
    _timer.stop("update");

//    restrict(l-1, d(l-1), d(l));
    _timer.start("transfer");
    _mgtransfer(l-1)->restrict(d(l-1), d(l));
    _timer.stop("transfer");

    f(l-1) = d(l-1);
    u(l-1).fill(0.0);
    mgstep(l-1, u, f, d, w, tol);
    
//    prolongate(l-1, w(l), u(l-1));
    _timer.start("transfer");
    _mgtransfer(l-1)->prolongate(w(l), u(l-1));
    _timer.stop("transfer");
    _timer.start("residual");
    residual(l, d(l), u(l), f(l));
    _timer.stop("residual");
    _timer.start("update");
    _mgupdate(l)->addUpdate(w(l), u(l), d(l));
    _timer.stop("update");
    _timer.start("smooth");
    smoothpost(l, w(l), d(l));
    _timer.stop("smooth");
    _timer.start("update");
    _mgupdatesmooth(l)->addUpdate(w(l), u(l), d(l));
    _timer.stop("update");
  }
}
