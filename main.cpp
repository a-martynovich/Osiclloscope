#include "oscilloscope.h"

#include <QtGui>
#include <QApplication>
#include <vector>
#include <algorithm>
#include <cmath>
#include <valarray>
#include <iostream>
using namespace std;
/*#include <boost/circular_buffer.hpp>
#include <iostream>
using namespace std;
void print_buffer(const boost::circular_buffer<int>& b) {
	for(int i=0;i<b.size(); i++) {
		cout<<b[i]<<",";
	}
	cout<<endl;
}*/

extern "C" void curfit_(
		int* iopt,	//in[1]: =-1
		int* m,		//in[1]: number of data points; m>k
		double* x,	//in[m]: x
		double* y,	//in[m]: y
		double* w,	//in[m]: wieghts!!!
		double* xb,	//in[1]: xb<=x(1), xe>=x(m).
        double* xe, //in[1]: xb<=x(1), xe>=x(m).
        int* k,		//in[1]: =3
        double* s,	//in[1]: =0
        int* nest,	//in[1]: total number of knots of the spline, =m+k+1
        int* n,		//out[1]: total number of knots of the spline approximation
        double* t,	//out[nest]: the knots of the spline
        double* c,	//out[nest]: the coefficients in the b-spline representation
        double* fp, //out[1]: weighted sum of squared residuals of the spline
        double* wrk,//out[m*(k+1)+nest*(7+3*k)]: working space
        int* lwrk,	//out[1]: size of wrk
        int* iwrk,	//out[nest]:
        int* ier	//out[1]: error
        );
/*
c    ier=10 : error. on entry, the input data are controlled on validity
c             the following restrictions must be satisfied.
c             -1<=iopt<=1, 1<=k<=5, m>k, nest>2*k+2, w(i)>0,i=1,2,...,m
c             xb<=x(1)<x(2)<...<x(m)<=xe, lwrk>=(k+1)*m+nest*(7+3*k)
c             if iopt=-1: 2*k+2<=n<=min(nest,m+k+1)
c                         xb<t(k+2)<t(k+3)<...<t(n-k-1)<xe
c                       the schoenberg-whitney conditions, i.e. there
c                       must be a subset of data points xx(j) such that
c                         t(j) < xx(j) < t(j+k+1), j=1,2,...,n-k-1
 */
extern "C" void splev_(double*,int*,double*,int*,double*,double*,int*,int*,int*);

int main(int argc, char *argv[])
{
/*	int m=1000, iopt=0, k=3, nest=m+k+1, n, lwrk=m*(k+1)+nest*(7+3*k), ier=0, m1=m/2;
	valarray<double> x(m),y(m), w(m), t(nest), c(nest), wrk(lwrk), x1(m1);
	valarray<int> iwrk(nest);
	double xb,xe,s=0, fp;
	for(int i=0; i<m; i++) {
		x[i] = i;
		w[i] = 1;
	}
	for(int i=0; i<m1; i++) {
		x1[i] = 0.5+i;
	}
	xb = x[0]; xe=x[x.size()-1];
	y = sin((2.0*M_PI*0.1)*x);

for(int i=0; i<10000; i++) {
	curfit_(&iopt, &m, &x[0],&y[0], &w[0], &xb, &xe, &k, &s, &nest,
			&n, &t[0], &c[0], &fp, &wrk[0],&lwrk, &iwrk[0], &ier);
	int e=0;
	splev_(&t[0], &n, &c[0], &k, &x[0], &y[0], &m1, &e, &ier);
}
	cout<<ier<<endl;*/
    QApplication a(argc, argv);
    Oscilloscope w;
    w.show();
    return a.exec();
}
