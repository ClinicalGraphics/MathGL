/***************************************************************************
 * vect.cpp is part of Math Graphic Library
 * Copyright (C) 2007-2012 Alexey Balakin <mathgl.abalakin@gmail.ru>       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "mgl2/vect.h"
#include "mgl2/eval.h"
#include "mgl2/data.h"
//-----------------------------------------------------------------------------
//
//	Traj series
//
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_traj_xyz(HMGL gr, HCDT x, HCDT y, HCDT z, HCDT ax, HCDT ay, HCDT az, const char *sch, const char *opt)
{
	long m,mx,my,mz,nx,ny,nz,n=ax->GetNx(),pal;
	if(mgl_check_dim1(gr,x,z,y,ax,"Traj"))	return;
	if(mgl_check_dim1(gr,ax,az,ay,0,"Traj"))	return;

	mreal len=gr->SaveState(opt);	if(mgl_isnan(len))	len = 0;
	static int cgid=1;	gr->StartGroup("Traj",cgid++);

	register long i, j;
	// find maximum
	i = ax->GetNy()>ay->GetNy() ? ax->GetNy():ay->GetNy();	j = z->GetNy()>az->GetNy() ? z->GetNy():az->GetNy();
	m = x->GetNy()>y->GetNy() ? x->GetNy():y->GetNy();		if(i>m)	m=i;	if(j>m)	m=j;
	gr->SetPenPal(sch,&pal);	gr->Reserve(4*n*m);

	mreal dx,dy,dz,dd,da;
	mglPoint p1,p2;
/*	for(j=0;j<m;j++)	for(i=0;i<n;i++)	// find maximal amplitude of vector field
	{
		mx = j<ax->GetNy() ? j:0;	my = j<ay->GetNy() ? j:0;	mz = j<az->GetNy() ? j:0;
		da = sqrt(ax->v(i,mx)*ax->v(i,mx)+ay->v(i,my)*ay->v(i,my)+az->v(i,mz)*az->v(i,mz));
		xm = xm>da ? xm : da;
	}
	xm = 1./(xm ? sqrt(xm):1);*/
	for(j=0;j<m;j++) // start prepare arrows
	{
		gr->NextColor(pal);
		for(i=0;i<n;i++)
		{
			if(gr->Stop)	return;
			nx = j<x->GetNy() ? j:0;	ny = j<y->GetNy() ? j:0;	nz = j<z->GetNy() ? j:0;
			mx = j<ax->GetNy() ? j:0;	my = j<ay->GetNy() ? j:0;	mz = j<az->GetNy() ? j:0;
			p1 = mglPoint(x->v(i,nx), y->v(i,ny), z->v(i,nz));
			p2 = mglPoint(ax->v(i,mx),ay->v(i,my),az->v(i,mz));
			da = p2.norm();
			if(len==0)
			{
				if(i<n-1)
				{	dx=x->v(i+1,nx)-p1.x;	dy=y->v(i+1,ny)-p1.y;	dz=z->v(i+1,nz)-p1.z;	}
				else
				{	dx=p1.x-x->v(i-1,nx);	dy=p1.y-y->v(i-1,ny);	dz=p1.z-z->v(i-1,nz);	}
				dd = da ? 1/da : 0;		dd *= sqrt(dx*dx+dy*dy+dz*dz);
			}
			else dd = len;

			nx = gr->AddPnt(p1);	ny = gr->AddPnt(p1+dd*p2,-1,mglPoint(NAN),-1,2);
			gr->vect_plot(nx,ny);
		}
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_traj_xy(HMGL gr, HCDT x, HCDT y, HCDT ax, HCDT ay, const char *sch, const char *opt)
{
	gr->SaveState(opt);
	mglData z(x->GetNx()), az(x->GetNx());	z.Fill(gr->Min.z,gr->Min.z);
	mgl_traj_xyz(gr,x,y,&z,ax,ay,&az,sch,0);
	gr->LoadState();
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_traj_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *ax, uintptr_t *ay, const char *sch, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_traj_xy(_GR_, _DA_(x), _DA_(y), _DA_(ax), _DA_(ay), s, o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_traj_xyz_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *ax, uintptr_t *ay, uintptr_t *az, const char *sch, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_traj_xyz(_GR_, _DA_(x), _DA_(y), _DA_(z), _DA_(ax), _DA_(ay), _DA_(az), s, o);
	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
//
//	Vect series
//
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_vect_xy(HMGL gr, HCDT x, HCDT y, HCDT ax, HCDT ay, const char *sch, const char *opt)
{
	long i,j,n=ax->GetNx(),m=ax->GetNy(),l=ax->GetNz(),k;
	if(mgl_check_dim2(gr,x,y,ax,ay,"Vect"))	return;

	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("Vect",cgid++);
	bool dot = sch && mglchr(sch,'.');
	bool fix = mglchr(sch,'f');
	bool end = mglchr(sch,'>');
	bool beg = mglchr(sch,'<');
	bool grd = mglchr(sch,'=');

	long ss = gr->AddTexture(sch);
	gr->Reserve(4*n*m);
	mreal	zVal = gr->Min.z;

	long tx=1,ty=1;
	if(gr->MeshNum>1)	{	tx=(n-1)/(gr->MeshNum-1);	ty=(m-1)/(gr->MeshNum-1);	}
	if(tx<1)	tx=1;	if(ty<1)	ty=1;

	mreal xm=0,cm=0,ca=0;
	mreal dd,dm=(fabs(gr->Max.c)+fabs(gr->Min.c))*1e-5;
	// use whole array for determining maximal vectors length
	mglPoint p1,p2, v, d;
	mreal c1,c2,xx;
	
	for(k=0;k<l;k++)	for(i=0;i<n;i+=tx)	for(j=0;j<m;j+=ty)
	{
		d = mglPoint(GetX(x,i,j,k).x, GetY(y,i,j,k).x);
		v = mglPoint(ax->v(i,j,k),ay->v(i,j,k));
		c1 = v.norm();	xm = xm<c1 ? c1:xm;	// handle NAN values
		p1 = i<n-1 ? mglPoint(GetX(x,i+tx,j,k).x, GetY(y,i+tx,j,k).x)-d : d-mglPoint(GetX(x,i-tx,j,k).x, GetY(y,i-tx,j,k).x);
		c1 = fabs(v*p1);	xx = p1.norm();	c1 *= xx?1/(xx*xx):0;
		p1 = j<m-1 ? mglPoint(GetX(x,i,j+ty,k).x, GetY(y,i,j+ty,k).x)-d : d-mglPoint(GetX(x,i,j-ty,k).x, GetY(y,i,j-ty,k).x);
		c2 = fabs(v*p1);	xx = p1.norm();	c2 *= xx?1/(xx*xx):0;
		c1 = c1<c2 ? c2:c1;	ca+=c1;	cm = cm<c1 ? c1:cm;
	}
	ca /= (n*m*l)/(tx*ty);
	xm = xm?1./xm:0;	cm = cm?1./cm:0;


	long n1,n2;
	for(k=0;k<l;k++)
	{
		if(ax->GetNz()>1)	zVal = gr->Min.z+(gr->Max.z-gr->Min.z)*mreal(k)/(ax->GetNz()-1);
		for(i=0;i<n;i+=tx)	for(j=0;j<m;j+=ty)
		{
			if(gr->Stop)	return;
			d = mglPoint(GetX(x,i,j,k).x, GetY(y,i,j,k).x, zVal);
			v = mglPoint(ax->v(i,j,k),ay->v(i,j,k));
			dd = v.norm();	v *= cm*(fix?(dd>dm ? 1./dd : 0) : xm);
			
			if(end)		{	p1 = d-v;	p2 = d;	}
			else if(beg)	{	p1 = d;	p2 = d+v;	}
			else		{	p1=d-v/2.;	p2=d+v/2.;	}
			if(grd)	{	c1=gr->GetC(ss,dd*xm-0.5,false);	c2=gr->GetC(ss,dd*xm,false);}
			else	c1 = c2 = gr->GetC(ss,dd*xm,false);
			n1=gr->AddPnt(p1,c1);	n2=gr->AddPnt(p2,c2);
			// allow vectors outside bounding box
			if(n1<0 && n2>=0)	n1=gr->AddPnt(p1,c1,mglPoint(NAN),-1,2);
			if(n2<0 && n1>=0)	n2=gr->AddPnt(p2,c2,mglPoint(NAN),-1,2);
			if(dot)	{	gr->line_plot(n1,n2);	gr->mark_plot(n1,'.');	}
			else	gr->vect_plot(n1,n2);
		}
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_vect_2d(HMGL gr, HCDT ax, HCDT ay, const char *sch, const char *opt)
{
	gr->SaveState(opt);
	mglData x(ax->GetNx()), y(ax->GetNy());
	x.Fill(gr->Min.x,gr->Max.x);
	y.Fill(gr->Min.y,gr->Max.y);
	mgl_vect_xy(gr,&x,&y,ax,ay,sch,0);
	gr->LoadState();
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_vect_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *ax, uintptr_t *ay, const char *sch, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_vect_xy(_GR_, _DA_(x), _DA_(y), _DA_(ax), _DA_(ay), s, o);
	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_vect_2d_(uintptr_t *gr, uintptr_t *ax, uintptr_t *ay, const char *sch, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_vect_2d(_GR_, _DA_(ax), _DA_(ay), s, o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
//
//	Vect3 series
//
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_vect_xyz(HMGL gr, HCDT x, HCDT y, HCDT z, HCDT ax, HCDT ay, HCDT az, const char *sch, const char *opt)
{
	register long i,j,n=ax->GetNx(),m=ax->GetNy(),l=ax->GetNz(),k;
	if(mgl_check_vec3(gr,x,y,z,ax,ay,az,"Vect_3d"))	return;

	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("Vect_3d",cgid++);
	bool dot = mglchr(sch,'.');
	bool fix = mglchr(sch,'f');
	bool end = mglchr(sch,'>');
	bool beg = mglchr(sch,'<');
	bool grd = mglchr(sch,'=');

	long ss = gr->AddTexture(sch);
	gr->Reserve(2*n*m*l);
	long tx=1,ty=1,tz=1;
	if(gr->MeshNum>1)
	{	tx=(n-1)/(gr->MeshNum-1);	ty=(m-1)/(gr->MeshNum-1);	tz=(l-1)/(gr->MeshNum-1);}
	if(tx<1)	tx=1;	if(ty<1)	ty=1;	if(tz<1)	tz=1;

	mreal xm=0,cm=0,ca=0;
	mreal dd,dm=(fabs(gr->Max.c)+fabs(gr->Min.c))*1e-5;
	// use whole array for determining maximal vectors length
	mglPoint p1,p2,p3, v, d;
	mreal c1,c2,c3,xx;
	
	for(k=0;k<l;k+=tz)	for(i=0;i<n;i+=tx)	for(j=0;j<m;j+=ty)
	{
		d = mglPoint(GetX(x,i,j,k).x, GetY(y,i,j,k).x, GetZ(z,i,j,k).x);
		v = mglPoint(ax->v(i,j,k),ay->v(i,j,k),az->v(i,j,k));
		c1 = v.norm();	xm = xm<c1 ? c1:xm;	// handle NAN values
		p1 = i<n-1 ? mglPoint(GetX(x,i+tx,j,k).x, GetY(y,i+tx,j,k).x, GetZ(z,i+tx,j,k).x)-d : d-mglPoint(GetX(x,i-tx,j,k).x, GetY(y,i-tx,j,k).x, GetZ(z,i-tx,j,k).x);
		c1 = fabs(v*p1);	xx = p1.norm();	c1 *= xx?1/(xx*xx):0;
		p1 = j<m-1 ? mglPoint(GetX(x,i,j+ty,k).x, GetY(y,i,j+ty,k).x, GetZ(z,i,j+ty,k).x)-d : d-mglPoint(GetX(x,i,j-ty,k).x, GetY(y,i,j-ty,k).x, GetZ(z,i,j-ty,k).x);
		c2 = fabs(v*p1);	xx = p1.norm();	c2 *= xx?1/(xx*xx):0;
		p1 = k<l-1 ? mglPoint(GetX(x,i,j,k+tz).x, GetY(y,i,j,k+tz).x, GetZ(z,i,j,k+tz).x)-d : d-mglPoint(GetX(x,i,j,k-tz).x, GetY(y,i,j,k-tz).x, GetZ(z,i,j,k-tz).x);
		c3 = fabs(v*p1);	xx = p1.norm();	c3 *= xx?1/(xx*xx):0;
		c1 = c1<c2 ? c2:c1;	c1 = c1<c3 ? c3:c1;
		ca+=c1;	cm = cm<c1 ? c1:cm;
	}
	ca /= (n*m*l)/(tx*ty*tz);
	xm = xm?1./xm:0;	cm = cm?1./cm:0;

	long n1,n2;
	for(k=0;k<l;k+=tz)	for(i=0;i<n;i+=tx)	for(j=0;j<m;j+=ty)
	{
		if(gr->Stop)	return;
		d=mglPoint(GetX(x,i,j,k).x, GetY(y,i,j,k).x, GetZ(z,i,j,k).x);
		v = mglPoint(ax->v(i,j,k),ay->v(i,j,k),az->v(i,j,k));
		dd = v.norm();	v *= cm*(fix?(dd>dm ? 1./dd : 0) : xm);
		
		if(end)		{	p1 = d-v;	p2 = d;	}
		else if(beg)	{	p1 = d;	p2 = d+v;	}
		else		{	p1=d-v/2.;	p2=d+v/2.;	}
		if(grd)	{	c1=gr->GetC(ss,dd*xm-0.5,false);	c2=gr->GetC(ss,dd*xm,false);	}
		else	c1 = c2 = gr->GetC(ss,dd*xm,false);
		n1=gr->AddPnt(p1,c1);	n2=gr->AddPnt(p2,c2);
		// allow vectors outside bounding box
		if(n1<0 && n2>=0)	n1=gr->AddPnt(p1,c1,mglPoint(NAN),-1,2);
		if(n2<0 && n1>=0)	n2=gr->AddPnt(p2,c2,mglPoint(NAN),-1,2);
		if(dot)	{	gr->line_plot(n1,n2);	gr->mark_plot(n1,'.');	}
		else	gr->vect_plot(n1,n2);
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_vect_3d(HMGL gr, HCDT ax, HCDT ay, HCDT az, const char *sch, const char *opt)
{
	gr->SaveState(opt);
	mglData x(ax->GetNx()), y(ax->GetNy()), z(ax->GetNz());
	x.Fill(gr->Min.x,gr->Max.x);
	y.Fill(gr->Min.y,gr->Max.y);
	z.Fill(gr->Min.z,gr->Max.z);
	mgl_vect_xyz(gr,&x,&y,&z,ax,ay,az,sch,0);
	gr->LoadState();
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_vect_xyz_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *ax, uintptr_t *ay, uintptr_t *az, const char *sch, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_vect_xyz(_GR_, _DA_(x), _DA_(y), _DA_(z), _DA_(ax), _DA_(ay), _DA_(az), s, o);
	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_vect_3d_(uintptr_t *gr, uintptr_t *ax, uintptr_t *ay, uintptr_t *az, const char *sch, const char *opt, int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_vect_3d(_GR_, _DA_(ax), _DA_(ay), _DA_(az), s, o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
//
//	Vect3 series
//
//-----------------------------------------------------------------------------
struct _mgl_vec_slice	{	mglData x,y,z,ax,ay,az;	};
//-----------------------------------------------------------------------------
void MGL_NO_EXPORT mgl_get_slice(_mgl_vec_slice &s, HCDT x, HCDT y, HCDT z, HCDT ax, HCDT ay, HCDT az, char dir, mreal d, bool both)
{
	register long i,j,i0,n=ax->GetNx(),m=ax->GetNy(),l=ax->GetNz(), nx=1,ny=1,p;

	if(dir=='x')	{	nx = m;	ny = l;	if(d<0)	d = n/2.;	}
	if(dir=='y')	{	nx = n;	ny = l;	if(d<0)	d = m/2.;	}
	if(dir=='z')	{	nx = n;	ny = m;	if(d<0)	d = l/2.;	}
	s.x.Create(nx,ny);	s.y.Create(nx,ny);	s.z.Create(nx,ny);
	s.ax.Create(nx,ny);	s.ay.Create(nx,ny);	s.az.Create(nx,ny);
	p = long(d);	d -= p;
	if(dir=='x' && p>=n-1)	{	d+=p-n+2;	p=n-2;	}
	if(dir=='y' && p>=m-1)	{	d+=p-m+2.;	p=m-2;	}
	if(dir=='z' && p>=l-1)	{	d+=p-l+2;	p=l-2;	}
	mreal v;

	if(both)
	{
		if(dir=='x')	for(j=0;j<ny;j++)	for(i=0;i<nx;i++)
		{
			i0 = i+nx*j;
			s.x.a[i0] = x->v(p,i,j)*(1-d) + x->v(p+1,i,j)*d;
			s.y.a[i0] = y->v(p,i,j)*(1-d) + y->v(p+1,i,j)*d;
			s.z.a[i0] = z->v(p,i,j)*(1-d) + z->v(p+1,i,j)*d;
			s.ax.a[i0] = ax->v(p,i,j)*(1-d) + ax->v(p+1,i,j)*d;
			s.ay.a[i0] = ay->v(p,i,j)*(1-d) + ay->v(p+1,i,j)*d;
			s.az.a[i0] = az->v(p,i,j)*(1-d) + az->v(p+1,i,j)*d;
		}
		if(dir=='y')	for(j=0;j<ny;j++)	for(i=0;i<nx;i++)
		{
			i0 = i+nx*j;
			s.x.a[i0] = x->v(i,p,j)*(1-d) + x->v(i,p+1,j)*d;
			s.y.a[i0] = y->v(i,p,j)*(1-d) + y->v(i,p+1,j)*d;
			s.z.a[i0] = z->v(i,p,j)*(1-d) + z->v(i,p+1,j)*d;
			s.ax.a[i0] = ax->v(i,p,j)*(1-d) + ax->v(i,p+1,j)*d;
			s.ay.a[i0] = ay->v(i,p,j)*(1-d) + ay->v(i,p+1,j)*d;
			s.az.a[i0] = az->v(i,p,j)*(1-d) + az->v(i,p+1,j)*d;
		}
		if(dir=='z')	for(j=0;j<ny;j++)	for(i=0;i<nx;i++)
		{
			i0 = i+nx*j;
			s.x.a[i0] = x->v(i,j,p)*(1-d) + x->v(i,j,p+1)*d;
			s.y.a[i0] = y->v(i,j,p)*(1-d) + y->v(i,j,p+1)*d;
			s.z.a[i0] = z->v(i,j,p)*(1-d) + z->v(i,j,p+1)*d;
			s.ax.a[i0] = ax->v(i,j,p)*(1-d) + ax->v(i,j,p+1)*d;
			s.ay.a[i0] = ay->v(i,j,p)*(1-d) + ay->v(i,j,p+1)*d;
			s.az.a[i0] = az->v(i,j,p)*(1-d) + az->v(i,j,p+1)*d;
		}
	}
	else	// x, y, z -- vectors
	{
		if(dir=='x')
		{
			v = x->v(p)*(1-d)+x->v(p+1)*d;
			for(j=0;j<ny;j++)	for(i=0;i<nx;i++)
			{
				i0 = i+nx*j;	s.x.a[i0] = v;
				s.y.a[i0] = y->v(i);	s.z.a[i0] = z->v(j);
				s.ax.a[i0] = ax->v(p,i,j)*(1-d) + ax->v(p+1,i,j)*d;
				s.ay.a[i0] = ay->v(p,i,j)*(1-d) + ay->v(p+1,i,j)*d;
				s.az.a[i0] = az->v(p,i,j)*(1-d) + az->v(p+1,i,j)*d;
			}
		}
		if(dir=='y')
		{
			v = y->v(p)*(1-d)+y->v(p+1)*d;
			for(j=0;j<ny;j++)	for(i=0;i<nx;i++)
			{
				i0 = i+nx*j;	s.y.a[i0] = v;
				s.x.a[i0] = x->v(i);	s.z.a[i0] = z->v(j);
				s.ax.a[i0] = ax->v(i,p,j)*(1-d) + ax->v(i,p+1,j)*d;
				s.ay.a[i0] = ay->v(i,p,j)*(1-d) + ay->v(i,p+1,j)*d;
				s.az.a[i0] = az->v(i,p,j)*(1-d) + az->v(i,p+1,j)*d;
			}
		}
		if(dir=='z')
		{
			v = z->v(p)*(1-d)+z->v(p+1)*d;
			for(j=0;j<ny;j++)	for(i=0;i<nx;i++)
			{
				i0 = i+nx*j;	s.z.a[i0] = v;
				s.x.a[i0] = x->v(i);	s.y.a[i0] = y->v(j);
				s.ax.a[i0] = ax->v(i,j,p)*(1-d) + ax->v(i,j,p+1)*d;
				s.ay.a[i0] = ay->v(i,j,p)*(1-d) + ay->v(i,j,p+1)*d;
				s.az.a[i0] = az->v(i,j,p)*(1-d) + az->v(i,j,p+1)*d;
			}
		}
	}
}
//-----------------------------------------------------------------------------
void MGL_NO_EXPORT mgl_get_slice_md(_mgl_vec_slice &s, const mglData *x, const mglData *y, const mglData *z, const mglData *ax, const mglData *ay, const mglData *az, char dir, mreal d, bool both)
{
	register long i,j,i0,i1,n=ax->nx,m=ax->ny,l=ax->nz, nx=1,ny=1,p;

	if(dir=='x')	{	nx = m;	ny = l;	if(d<0)	d = n/2.;	}
	if(dir=='y')	{	nx = n;	ny = l;	if(d<0)	d = m/2.;	}
	if(dir=='z')	{	nx = n;	ny = m;	if(d<0)	d = l/2.;	}
	s.x.Create(nx,ny);	s.y.Create(nx,ny);	s.z.Create(nx,ny);
	s.ax.Create(nx,ny);	s.ay.Create(nx,ny);	s.az.Create(nx,ny);
	p = long(d);	d -= p;
	if(dir=='x' && p>=n-1)	{	d+=p-n+2;	p=n-2;	}
	if(dir=='y' && p>=m-1)	{	d+=p-m+2.;	p=m-2;	}
	if(dir=='z' && p>=l-1)	{	d+=p-l+2;	p=l-2;	}
	mreal v;

	if(both)
	{
		if(dir=='x')	for(j=0;j<ny;j++)	for(i=0;i<nx;i++)
		{
			i0 = i+nx*j;	i1 = p+n*(i+m*j);
			s.x.a[i0] = x->a[i1]*(1-d) + x->a[i1+1]*d;
			s.y.a[i0] = y->a[i1]*(1-d) + y->a[i1+1]*d;
			s.z.a[i0] = z->a[i1]*(1-d) + z->a[i1+1]*d;
			s.ax.a[i0] = ax->a[i1]*(1-d) + ax->a[i1+1]*d;
			s.ay.a[i0] = ay->a[i1]*(1-d) + ay->a[i1+1]*d;
			s.az.a[i0] = az->a[i1]*(1-d) + az->a[i1+1]*d;
		}
		if(dir=='y')	for(j=0;j<ny;j++)	for(i=0;i<nx;i++)
		{
			i0 = i+nx*j;	i1 = i+n*(p+m*j);
			s.x.a[i0] = x->a[i1]*(1-d) + x->a[i1+n]*d;
			s.y.a[i0] = y->a[i1]*(1-d) + y->a[i1+n]*d;
			s.z.a[i0] = z->a[i1]*(1-d) + z->a[i1+n]*d;
			s.ax.a[i0] = ax->a[i1]*(1-d) + ax->a[i1+n]*d;
			s.ay.a[i0] = ay->a[i1]*(1-d) + ay->a[i1+n]*d;
			s.az.a[i0] = az->a[i1]*(1-d) + az->a[i1+n]*d;
		}
		if(dir=='z')	for(j=0;j<ny;j++)	for(i=0;i<nx;i++)
		{
			i0 = i+nx*j;	i1 = i+n*(j+m*p);
			s.x.a[i0] = x->a[i1]*(1-d) + x->a[i1+n*m]*d;
			s.y.a[i0] = y->a[i1]*(1-d) + y->a[i1+n*m]*d;
			s.z.a[i0] = z->a[i1]*(1-d) + z->a[i1+n*m]*d;
			s.ax.a[i0] = ax->a[i1]*(1-d) + ax->a[i1+n*m]*d;
			s.ay.a[i0] = ay->a[i1]*(1-d) + ay->a[i1+n*m]*d;
			s.az.a[i0] = az->a[i1]*(1-d) + az->a[i1+n*m]*d;
		}
	}
	else	// x, y, z -- vectors
	{
		if(dir=='x')
		{
			v = x->a[p]*(1-d)+x->a[p+1]*d;
			for(j=0;j<ny;j++)	for(i=0;i<nx;i++)
			{
				i0 = i+nx*j;	s.x.a[i0] = v;	i1 = p+n*(i+m*j);
				s.y.a[i0] = y->a[i];	s.z.a[i0] = z->a[j];
				s.ax.a[i0] = ax->a[i1]*(1-d) + ax->a[i1+1]*d;
				s.ay.a[i0] = ay->a[i1]*(1-d) + ay->a[i1+1]*d;
				s.az.a[i0] = az->a[i1]*(1-d) + az->a[i1+1]*d;
			}
		}
		if(dir=='y')
		{
			v = y->a[p]*(1-d)+y->a[p+1]*d;
			for(j=0;j<ny;j++)	for(i=0;i<nx;i++)
			{
				i0 = i+nx*j;	s.y.a[i0] = v;	i1 = i+n*(p+m*j);
				s.x.a[i0] = x->a[i];	s.z.a[i0] = z->a[j];
				s.ax.a[i0] = ax->a[i1]*(1-d) + ax->a[i1+n]*d;
				s.ay.a[i0] = ay->a[i1]*(1-d) + ay->a[i1+n]*d;
				s.az.a[i0] = az->a[i1]*(1-d) + az->a[i1+n]*d;
			}
		}
		if(dir=='z')
		{
			v = z->a[p]*(1-d)+z->a[p+1]*d;
			for(j=0;j<ny;j++)	for(i=0;i<nx;i++)
			{
				i0 = i+nx*j;	s.z.a[i0] = v;	i1 = i+n*(j+m*p);
				s.x.a[i0] = x->a[i];	s.y.a[i0] = y->a[j];
				s.ax.a[i0] = ax->a[i1]*(1-d) + ax->a[i1+n*m]*d;
				s.ay.a[i0] = ay->a[i1]*(1-d) + ay->a[i1+n*m]*d;
				s.az.a[i0] = az->a[i1]*(1-d) + az->a[i1+n*m]*d;
			}
		}
	}
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_vect3_xyz(HMGL gr, HCDT x, HCDT y, HCDT z, HCDT ax, HCDT ay, HCDT az, const char *sch, double sVal, const char *opt)
{
	bool both = mgl_isboth(x,y,z,ax);
	if(mgl_check_vec3(gr,x,y,z,ax,ay,az,"Vect3"))	return;

	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("Vect3",cgid++);
	char dir='y';
	if(mglchr(sch,'x'))	dir='x';
	if(mglchr(sch,'z'))	dir='z';

	bool dot = mglchr(sch,'.');
	bool fix = mglchr(sch,'f');
	bool end = mglchr(sch,'>');
	bool beg = mglchr(sch,'<');
	bool grd = mglchr(sch,'=');
	long ss = gr->AddTexture(sch);

	_mgl_vec_slice s;
	const mglData *mx = dynamic_cast<const mglData *>(x);
	const mglData *my = dynamic_cast<const mglData *>(y);
	const mglData *mz = dynamic_cast<const mglData *>(z);
	const mglData *max = dynamic_cast<const mglData *>(ax);
	const mglData *may = dynamic_cast<const mglData *>(ay);
	const mglData *maz = dynamic_cast<const mglData *>(az);
	if(mx&&my&&mz&&max&&may&&maz)
		mgl_get_slice_md(s,mx,my,mz,max,may,maz,dir,sVal,both);
	else
		mgl_get_slice(s,x,y,z,ax,ay,az,dir,sVal,both);

	long i,j,n=s.ax.nx,m=s.ax.ny;
	long tx=1,ty=1;
	if(gr->MeshNum>1)	{	tx=(n-1)/(gr->MeshNum-1);	ty=(m-1)/(gr->MeshNum-1);	}
	if(tx<1)	tx=1;	if(ty<1)	ty=1;
	mreal xm=0,cm=0,ca=0;
	mreal dd,dm=(fabs(gr->Max.c)+fabs(gr->Min.c))*1e-5;
	// use whole array for determining maximal vectors length 
	mglPoint p1,p2, v, d=(gr->Max-gr->Min)/mglPoint(1./ax->GetNx(),1./ax->GetNy(),1./ax->GetNz());
	mreal c1,c2, xx,yy,zz;

	register long i0, tn=ty*n;
	for(i=0;i<n;i+=tx)	for(j=0;j<m;j+=ty)
	{
		i0 = i+n*j;	xx = s.x.a[i0];	yy = s.y.a[i0];	zz = s.z.a[i0];
		p1 = i<n-1 ? mglPoint(s.x.a[i0+tx]-xx, s.y.a[i0+tx]-yy, s.z.a[i0+tx]-zz) : mglPoint(xx-s.x.a[i0-tx], yy-s.y.a[i0-tx], zz-s.z.a[i0-tx]);
		p2 = j<m-1 ? mglPoint(s.x.a[i0+tn]-xx, s.y.a[i0+tn]-yy, s.z.a[i0+tn]-zz) : mglPoint(xx-s.x.a[i0-tn], yy-s.y.a[i0-tn], zz-s.z.a[i0-tn]);
		v = mglPoint(s.ax.a[i0], s.ay.a[i0], s.az.a[i0]);
		c1 = v.norm();	xm = xm<c1 ? c1:xm;	// handle NAN values
		yy = fabs(v*d);	xx = d.norm();	yy *= xx?1/(xx*xx):0;
		c1 = fabs(v*p1);	xx = p1.norm();	c1 *= xx?1/(xx*xx):0;
		c2 = fabs(v*p2);	xx = p2.norm();	c2 *= xx?1/(xx*xx):0;
		c1 = c1<c2 ? c2:c1;	c1 = c1<yy ? yy:c1;
		ca+=c1;	cm = cm<c1 ? c1:cm;
	}
	ca /= (n*m)/(tx*ty);
	xm = xm?1./xm:0;	cm = cm?1./cm:0;

	long n1,n2;
	for(i=0;i<n;i+=tx)	for(j=0;j<m;j+=ty)
	{
		if(gr->Stop)	return;
		i0 = i+n*j;
		d = mglPoint(s.x.a[i0], s.y.a[i0], s.z.a[i0]);
		v = mglPoint(s.ax.a[i0], s.ay.a[i0], s.az.a[i0]);
		dd = v.norm();	v *= cm*(fix?(dd>dm ? 1./dd : 0) : xm);

		if(end)		{	p1 = d-v;	p2 = d;	}
		else if(beg)	{	p1 = d;	p2 = d+v;	}
		else		{	p1=d-v/2.;	p2=d+v/2.;	}
		if(grd)	{	c1=gr->GetC(ss,dd*xm-0.5,false);	c2=gr->GetC(ss,dd*xm,false);}
		else	c1 = c2 = gr->GetC(ss,dd*xm,false);
		n1=gr->AddPnt(p1,c1);	n2=gr->AddPnt(p2,c2);
		// allow vectors outside bounding box
		if(n1<0 && n2>=0)	n1=gr->AddPnt(p1,c1,mglPoint(NAN),-1,2);
		if(n2<0 && n1>=0)	n2=gr->AddPnt(p2,c2,mglPoint(NAN),-1,2);
		if(dot)	{	gr->line_plot(n1,n2);	gr->mark_plot(n1,'.');	}
		else	gr->vect_plot(n1,n2);
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_vect3(HMGL gr, HCDT ax, HCDT ay, HCDT az, const char *sch, double sVal, const char *opt)
{
	gr->SaveState(opt);
	mglData x(ax->GetNx()), y(ax->GetNy()),z(ax->GetNz());
	x.Fill(gr->Min.x,gr->Max.x);
	y.Fill(gr->Min.y,gr->Max.y);
	z.Fill(gr->Min.z,gr->Max.z);
	mgl_vect3_xyz(gr,&x,&y,&z,ax,ay,az,sch,sVal,0);
	gr->LoadState();
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_vect3_xyz_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *ax, uintptr_t *ay, uintptr_t *az, const char *sch, mreal *sVal, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_vect3_xyz(_GR_, _DA_(x), _DA_(y), _DA_(z), _DA_(ax), _DA_(ay), _DA_(az), s, *sVal, o);
	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_vect3_(uintptr_t *gr, uintptr_t *ax, uintptr_t *ay, uintptr_t *az, const char *sch, mreal *sVal, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_vect3(_GR_, _DA_(ax), _DA_(ay), _DA_(az), s, *sVal, o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
//
//	Flow 2d series
//
//-----------------------------------------------------------------------------
void MGL_NO_EXPORT flow(mglBase *gr, double zVal, double u, double v, const mglData &x, const mglData &y, const mglData &ax, const mglData &ay, long ss, bool vv)
{
	long n=10*(ax.nx+ax.ny);
	bool both = x.nx==ax.nx && y.nx==ax.nx && x.ny==ax.ny && y.ny==ax.ny;

	mglPoint *pp = new mglPoint[n], dp;
	mreal *cc = new mreal[n];
	mglPoint dx(1/fabs(gr->Max.x-gr->Min.x),1/fabs(gr->Max.y-gr->Min.y),1/fabs(gr->Max.z-gr->Min.z));

	mreal dt = 0.5/(ax.nx > ax.ny ? ax.nx : ax.ny),e,f,g,ff[4],gg[4],h,s=1;
	if(u<0 || v<0)	{	dt = -dt;	u = -u;		v = -v;		s = -1;}
	register long k=0,m;
	bool end = false;
	do{
		if(gr->Stop)	{	delete []pp;	delete []cc;	return;	}
		pp[k].x = both ? x.Spline1(u,v,0):x.Spline1(u,0,0);
		pp[k].y = both ? y.Spline1(u,v,0):y.Spline1(v,0,0);
		pp[k].z = zVal;
		for(m=0;m<k-1;m++)	// determines encircle
			if(mgl_norm((pp[k]-pp[m])/dx)<dt/10.)	{	end = true;	break;	}
		f = ax.Linear1(u,v,0);	g = ay.Linear1(u,v,0);
		h = hypot(f,g);	cc[k] = gr->GetC(ss,s*h);
		if(h<1e-5)	break;	// stationary point
		k++;
		// find next point by midpoint method
		h+=1;	ff[0]=f*dt/h;	gg[0]=g*dt/h;
		e = u+ff[0]/2;	h = v+gg[0]/2;
		f = ax.Linear1(e,h,0);	g = ay.Linear1(e,h,0);
		h = 1+hypot(f,g);	ff[1]=f*dt/h;	gg[1]=g*dt/h;
		e = u+ff[1]/2;	h = v+gg[1]/2;
		f = ax.Linear1(e,h,0);	g = ay.Linear1(e,h,0);
		h = 1+hypot(f,g);	ff[2]=f*dt/h;	gg[2]=g*dt/h;
		e = u+ff[2];	h = v+gg[2];
		f = ax.Linear1(e,h,0);	g = ay.Linear1(e,h,0);
		h = 1+hypot(f,g);	ff[3]=f*dt/h;	gg[3]=g*dt/h;
		u += ff[0]/6+ff[1]/3+ff[2]/3+ff[3]/6;
		v += gg[0]/6+gg[1]/3+gg[2]/3+gg[3]/6;
		// condition of end
		end = end || k>=n || u<0 || v<0 || u>1 || v>1;
	} while(!end);
	if(k>1)
	{
		long i,j,jj,a=long(1./fabs(dt));
		gr->Reserve(k);		j = gr->AddPnt(pp[0],cc[0]);
		for(i=1;i<k;i++)
		{
			jj=j;	j = gr->AddPnt(pp[i],cc[i]);
			if(vv && i%a==0)
			{
				if(dt<0)	gr->vect_plot(j,jj,a/5);
				else		gr->vect_plot(jj,j,a/5);
			}
			else	gr->line_plot(jj,j);
		}
	}
	delete []pp;	delete []cc;
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_flow_xy(HMGL gr, HCDT x, HCDT y, HCDT ax, HCDT ay, const char *sch, const char *opt)
{
	mreal u,v;
	if(mgl_check_dim2(gr,x,y,ax,ay,"Flow"))	return;

	mreal r = gr->SaveState(opt);
	long num = mgl_isnan(r)?5:long(r+0.5);
	static int cgid=1;	gr->StartGroup("Flow",cgid++);

	long ss = gr->AddTexture(sch);
	bool vv = mglchr(sch,'v');
	// allocate memory
	mreal zVal = gr->Min.z;
	bool cnt=!mglchr(sch,'#');
	mglData xx(x), yy(y), bx(ax), by(ay);

	for(long k=0;k<ax->GetNz();k++)
	{
		if(ax->GetNz()>1)	zVal = gr->Min.z+(gr->Max.z-gr->Min.z)*mreal(k)/(ax->GetNz()-1);
		for(long i=0;i<num;i++)
		{
			if(gr->Stop)	return;
			u = 0;	v = (i+1.)/(num+1.);
			flow(gr, zVal, u, v, xx, yy, bx, by,ss,vv);
			flow(gr, zVal, -u, -v, xx, yy, bx, by,ss,vv);
			u = 1;	v = (i+1.)/(num+1.);
			flow(gr, zVal, u, v, xx, yy, bx, by,ss,vv);
			flow(gr, zVal, -u, -v, xx, yy, bx, by,ss,vv);
			u = (i+1.)/(num+1.);	v = 0;
			flow(gr, zVal, u, v, xx, yy, bx, by,ss,vv);
			flow(gr, zVal, -u, -v, xx, yy, bx, by,ss,vv);
			u = (i+1.)/(num+1.);	v = 1;
			flow(gr, zVal, u, v, xx, yy, bx, by,ss,vv);
			flow(gr, zVal, -u, -v, xx, yy, bx, by,ss,vv);
			if(cnt)
			{
				u = 0.5;	v = (i+1.)/(num+1.);
				flow(gr, zVal, u, v, xx, yy, bx, by,ss,vv);
				flow(gr, zVal, -u, -v, xx, yy, bx, by,ss,vv);
				u = (i+1.)/(num+1.);	v = 0.5;
				flow(gr, zVal, u, v, xx, yy, bx, by,ss,vv);
				flow(gr, zVal, -u, -v, xx, yy, bx, by,ss,vv);
			}
		}
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_flow_2d(HMGL gr, HCDT ax, HCDT ay, const char *sch, const char *opt)
{
	gr->SaveState(opt);
	mglData x(ax->GetNx()), y(ax->GetNy());
	x.Fill(gr->Min.x,gr->Max.x);
	y.Fill(gr->Min.y,gr->Max.y);
	mgl_flow_xy(gr,&x,&y,ax,ay,sch,0);
	gr->LoadState();
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_flow_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *ax, uintptr_t *ay, const char *sch, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_flow_xy(_GR_, _DA_(x), _DA_(y), _DA_(ax), _DA_(ay), s, o);	delete []o;	delete []s;	}
void MGL_EXPORT mgl_flow_2d_(uintptr_t *gr, uintptr_t *ax, uintptr_t *ay, const char *sch, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_flow_2d(_GR_, _DA_(ax), _DA_(ay), s, o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_flowp_xy(HMGL gr, double x0, double y0, double z0, HCDT x, HCDT y, HCDT ax, HCDT ay, const char *sch, const char *opt)
{
	mglPoint p(x0,y0,z0);
	mreal u,v;
	long n=ax->GetNx(), m=ax->GetNy();
	bool both = x->GetNx()==n && y->GetNx()==n && x->GetNy()==m && y->GetNy()==m;
	if(mgl_check_dim2(gr,x,y,ax,ay,"FlowP"))	return;

	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("FlowP",cgid++);

	long ss = gr->AddTexture(sch);
	bool vv = mglchr(sch,'v');
	// find coordinates u, v
	register long i,j;
	register mreal d, dm=1e7;
	long i0=0,j0=0;
	for(i=0;i<n;i++)	for(j=0;j<m;j++)	// first find closest
	{
		d = both ? hypot(x->v(i,j)-p.x,y->v(i,j)-p.y) : hypot(x->v(i)-p.x,y->v(j)-p.y);
		if(d<dm)	{	i0=i;	j0=j;	dm=d;	}
	}
	if(dm==0)	{	u = i0/mreal(n);	v = j0/mreal(m);	}	// we find it
	else
	{
		mreal dxu,dxv,dyu,dyv, dx, dy;
		if(both)
		{
			dx = x->v(i0,j0)-p.x;	dy = y->v(i0,j0)-p.y;
			dxu= x->dvx(i0,j0);		dyu= y->dvx(i0,j0);
			dxv= x->dvy(i0,j0);		dyv= y->dvy(i0,j0);
			d = dxv*dyu-dxu*dyv;
			u = (i0+(dxv*dy-dx*dyv)/d)/n;
			v = (j0-(dxu*dy-dx*dyu)/d)/m;
		}
		else
		{
			dx = x->v(i0)-p.x;	dy = y->v(j0)-p.y;
			dxu= x->dvx(i0);	dyv= y->dvx(j0);
			u = (i0+dx/dxu)/n;	v = (j0+dy/dyv)/m;
		}
	}
	mglData xx(x), yy(y), bx(ax), by(ay);
	flow(gr, p.z, u, v, xx, yy, bx, by,ss,vv);
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_flowp_2d(HMGL gr, double x0, double y0, double z0, HCDT ax, HCDT ay, const char *sch, const char *opt)
{
	gr->SaveState(opt);
	mglData x(ax->GetNx()), y(ax->GetNy());
	x.Fill(gr->Min.x,gr->Max.x);
	y.Fill(gr->Min.y,gr->Max.y);
	mgl_flowp_xy(gr,x0,y0,z0,&x,&y,ax,ay,sch,0);
	gr->LoadState();
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_flowp_xy_(uintptr_t *gr, mreal *x0, mreal *y0, mreal *z0, uintptr_t *x, uintptr_t *y, uintptr_t *ax, uintptr_t *ay, const char *sch, const char *opt, int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_flowp_xy(_GR_, *x0,*y0,*z0, _DA_(x), _DA_(y), _DA_(ax), _DA_(ay), s, o);	delete []o;	delete []s;
}
void MGL_EXPORT mgl_flowp_2d_(uintptr_t *gr, mreal *x0, mreal *y0, mreal *z0, uintptr_t *ax, uintptr_t *ay, const char *sch, const char *opt, int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_flowp_2d(_GR_, *x0,*y0,*z0, _DA_(ax), _DA_(ay), s, o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
//
//	Flow 3d series
//
//-----------------------------------------------------------------------------
void flow(mglBase *gr, double u, double v, double w, const mglData &x, const mglData &y, const mglData &z, const mglData &ax, const mglData &ay, const mglData &az,long ss,bool vv, bool xo, bool zo)
{
	static long n=10*(ax.nx+ax.ny);
	long nn = ax.nx*ax.ny*ax.nz;
	bool both = x.nx*x.ny*x.nz==nn && y.nx*y.ny*y.nz==nn && z.nx*z.ny*z.nz==nn;
	mglPoint *pp = new mglPoint[n], dp;
	mreal *cc = new mreal[n];
	mglPoint dx(1/fabs(gr->Max.x-gr->Min.x),1/fabs(gr->Max.y-gr->Min.y),1/fabs(gr->Max.z-gr->Min.z));

	nn = (ax.nx > ax.ny ? ax.nx : ax.ny);
	nn = (nn > ax.nz ? nn : ax.nz);
	mreal dt = 0.2/nn, e,f,g,ee[4],ff[4],gg[4],h,s=1,u1,v1,w1;
	if(u<0 || v<0 || w<0)
	{	dt = -dt;	u = -u;		v = -v;		w = -w;		s = -1;}
	register long k=0,m;
	bool end = false;
	do{
		if(gr->Stop)	{	delete []pp;	delete []cc;	return;	}
		pp[k].x = both ? x.Spline1(u,v,w):x.Spline1(u,0,0);
		pp[k].y = both ? y.Spline1(u,v,w):y.Spline1(v,0,0);
		pp[k].z = both ? z.Spline1(u,v,w):z.Spline1(w,0,0);
		for(m=0;m<k-1;m++)	// determines encircle
			if(mgl_norm((pp[k]-pp[m])/dx)<dt/10.)	{	end = true;	break;	}
		e = ax.Linear1(u,v,w);	f = ay.Linear1(u,v,w);	g = az.Linear1(u,v,w);
		h = sqrt(e*e+f*f+g*g);	cc[k] = gr->GetC(ss,s*h);
		if(h<1e-5)	break;	// stationary point
		k++;
		// find next point by midpoint method
		h+=1;	ee[0]=e*dt/h;	ff[0]=f*dt/h;	gg[0]=g*dt/h;
		u1 = u+ee[0]/2;	v1 = v+ff[0]/2;	w1 = w+gg[0]/2;
		e = ax.Linear1(u1,v1,w1);	f = ay.Linear1(u1,v1,w1);
		g = az.Linear1(u1,v1,w1);	h = 1+sqrt(e*e+f*f+g*g);
		ee[1]=e*dt/h;	ff[1]=f*dt/h;	gg[1]=g*dt/h;
		u1 = u+ee[1]/2;	v1 = v+ff[1]/2;	w1 = w+gg[1]/2;
		e = ax.Linear1(u1,v1,w1);	f = ay.Linear1(u1,v1,w1);
		g = az.Linear1(u1,v1,w1);	h = 1+sqrt(e*e+f*f+g*g);
		ee[2]=e*dt/h;	ff[2]=f*dt/h;	gg[2]=g*dt/h;
		u1 = u+ee[2];	v1 = v+ff[2];	w1 = w+gg[2];
		e = ax.Linear1(u1,v1,w1);	f = ay.Linear1(u1,v1,w1);
		g = az.Linear1(u1,v1,w1);	h = 1+sqrt(e*e+f*f+g*g);
		ee[3]=e*dt/h;	ff[3]=f*dt/h;	gg[3]=g*dt/h;
		u += ee[0]/6+ee[1]/3+ee[2]/3+ee[3]/6;
		v += ff[0]/6+ff[1]/3+ff[2]/3+ff[3]/6;
		w += gg[0]/6+gg[1]/3+gg[2]/3+gg[3]/6;
		// condition of end
		end = end || k>=n || u<0 || v<0 || u>1 || v>1 || w<0 || w>1;
	} while(!end);
	if(k>1)
	{
		long i,j,jj,a=long(1./fabs(dt));
		mreal rr = mgl_norm(gr->Max-gr->Min)*gr->BarWidth/25, ll;
		mglPoint q1,q2,l;
		long n1=-1,n2=-1,n3=-1,n4=-1, m1=-1,m2=-1,m3=-1,m4=-1;

		gr->Reserve(4*k);	j = gr->AddPnt(pp[0],cc[0]);
		l = pp[1] - pp[0];	l /= mgl_norm(l);
		q1 = mglPoint(l.y,-l.x,0);	ll = mgl_norm(q1);
		if(ll)	q1 /= ll;	else	q1 = mglPoint(0,1,0);
		q2 = q1^l;
		if(xo)	{	n1 = gr->AddPnt(pp[0],-1,q2);	n2 = gr->AddPnt(pp[0]+rr*q1,-1,q2);	}
		if(zo)	{	n3 = gr->AddPnt(pp[0],-1,q1);	n4 = gr->AddPnt(pp[0]+rr*q2,-1,q1);	}
		for(i=1;i<k;i++)
		{
			jj=j;	j = gr->AddPnt(pp[i],cc[i]);
			if(vv && i%a==0)
			{
				if(dt<0)	gr->vect_plot(j,jj,a/5);
				else		gr->vect_plot(jj,j,a/5);
			}
			else	gr->line_plot(jj,j);
			l = pp[i]-pp[i-1];		l /= mgl_norm(l);
			q1 -= l*(l*q1);	q1/= mgl_norm(q1);	q2 = q1^l;
			m1 = n1;	m2 = n2;	m3 = n3;	m4 = n4;
			if(xo)
			{	n1 = gr->AddPnt(pp[i],-1,q2);	n2 = gr->AddPnt(pp[i]+rr*q1,-1,q2);	gr->quad_plot(n1,n2,m1,m2);	}
			if(zo)
			{	n3 = gr->AddPnt(pp[i],-1,q1);	n4 = gr->AddPnt(pp[i]+rr*q2,-1,q1);	gr->quad_plot(n3,n4,m3,m4);	}
		}
	}
	delete []pp;	delete []cc;
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_flow_xyz(HMGL gr, HCDT x, HCDT y, HCDT z, HCDT ax, HCDT ay, HCDT az, const char *sch, const char *opt)
{
	mreal u,v,w;
	long i,j;
	if(mgl_check_vec3(gr,x,y,z,ax,ay,az,"Flow3"))	return;

	mreal r = gr->SaveState(opt);
	long num = mgl_isnan(r)?3:long(r+0.5);
	static int cgid=1;	gr->StartGroup("Flow3",cgid++);
	bool cnt=!mglchr(sch,'#');
	long ss = gr->AddTexture(sch);
	bool vv = mglchr(sch,'v'), xo = mglchr(sch,'x'), zo = mglchr(sch,'z');

	mglData xx(x), yy(y), zz(z), bx(ax), by(ay), bz(az);
	for(i=0;i<num;i++)	for(j=0;j<num;j++)
	{
		if(gr->Stop)	return;
		u = (i+1.)/(num+1.);	v = (j+1.)/(num+1.);	w = 0;
		flow(gr, u, v, w, xx, yy, zz, bx, by, bz,ss,vv,xo,zo);
		flow(gr,-u,-v,-w, xx, yy, zz, bx, by, bz,ss,vv,xo,zo);
		u = (i+1.)/(num+1.);	v = (j+1.)/(num+1.);	w = 1;
		flow(gr, u, v, w, xx, yy, zz, bx, by, bz,ss,vv,xo,zo);
		flow(gr,-u,-v,-w, xx, yy, zz, bx, by, bz,ss,vv,xo,zo);
		u = 0;	v = (j+1.)/(num+1.);	w = (i+1.)/(num+1.);
		flow(gr, u, v, w, xx, yy, zz, bx, by, bz,ss,vv,xo,zo);
		flow(gr,-u,-v,-w, xx, yy, zz, bx, by, bz,ss,vv,xo,zo);
		u = 1;	v = (j+1.)/(num+1.);	w = (i+1.)/(num+1.);
		flow(gr, u, v, w, xx, yy, zz, bx, by, bz,ss,vv,xo,zo);
		flow(gr,-u,-v,-w, xx, yy, zz, bx, by, bz,ss,vv,xo,zo);
		u = (i+1.)/(num+1.);	v = 0;	w = (j+1.)/(num+1.);
		flow(gr, u, v, w, xx, yy, zz, bx, by, bz,ss,vv,xo,zo);
		flow(gr,-u,-v,-w, xx, yy, zz, bx, by, bz,ss,vv,xo,zo);
		u = (i+1.)/(num+1.);	v = 1;	w = (j+1.)/(num+1.);
		flow(gr, u, v, w, xx, yy, zz, bx, by, bz,ss,vv,xo,zo);
		flow(gr,-u,-v,-w, xx, yy, zz, bx, by, bz,ss,vv,xo,zo);
		if(cnt)
		{
			u = (i+1.)/(num+1.);	v = (j+1.)/(num+1.);	w = 0.5;
			flow(gr, u, v, w, xx, yy, zz, bx, by, bz,ss,vv,xo,zo);
			flow(gr,-u,-v,-w, xx, yy, zz, bx, by, bz,ss,vv,xo,zo);
			u = 0.5;	v = (j+1.)/(num+1.);	w = (i+1.)/(num+1.);
			flow(gr, u, v, w, xx, yy, zz, bx, by, bz,ss,vv,xo,zo);
			flow(gr,-u,-v,-w, xx, yy, zz, bx, by, bz,ss,vv,xo,zo);
			u = (i+1.)/(num+1.);	v = 0.5;	w = (j+1.)/(num+1.);
			flow(gr, u, v, w, xx, yy, zz, bx, by, bz,ss,vv,xo,zo);
			flow(gr,-u,-v,-w, xx, yy, zz, bx, by, bz,ss,vv,xo,zo);
		}
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_flow_3d(HMGL gr, HCDT ax, HCDT ay, HCDT az, const char *sch, const char *opt)
{
	gr->SaveState(opt);
	mglData x(ax->GetNx()), y(ax->GetNy()), z(ax->GetNz());
	x.Fill(gr->Min.x,gr->Max.x);
	y.Fill(gr->Min.y,gr->Max.y);
	z.Fill(gr->Min.z,gr->Max.z);
	mgl_flow_xyz(gr,&x,&y,&z,ax,ay,az,sch,0);
	gr->LoadState();
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_flow_xyz_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *ax, uintptr_t *ay, uintptr_t *az, const char *sch, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_flow_xyz(_GR_, _DA_(x), _DA_(y), _DA_(z), _DA_(ax), _DA_(ay), _DA_(az), s, o);	delete []o;	delete []s;	}
void MGL_EXPORT mgl_flow_3d_(uintptr_t *gr, uintptr_t *ax, uintptr_t *ay, uintptr_t *az, const char *sch, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_flow_3d(_GR_, _DA_(ax), _DA_(ay), _DA_(az), s, o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_flowp_xyz(HMGL gr, double x0, double y0, double z0, HCDT x, HCDT y, HCDT z, HCDT ax, HCDT ay, HCDT az, const char *sch, const char *opt)
{
	mglPoint p(x0,y0,z0);
	mreal u,v,w;
	long n=ax->GetNx(),m=ax->GetNy(),l=ax->GetNz();
	bool both = x->GetNx()*x->GetNy()*x->GetNz()==n*m*l && y->GetNx()*y->GetNy()*y->GetNz()==n*m*l && z->GetNx()*z->GetNy()*z->GetNz()==n*m*l;
	if(mgl_check_vec3(gr,x,y,z,ax,ay,az,"FlowP3"))	return;

	gr->SaveState(opt);
	static int cgid=1;	gr->StartGroup("FlowP3",cgid++);
	long ss = gr->AddTexture(sch);
	bool vv = mglchr(sch,'v'), xo = mglchr(sch,'x'), zo = mglchr(sch,'z');

	// find coordinates u, v, w
	register long i,j,k;
	register mreal d, dm=1e7;
	long i0=0,j0=0,k0=0;
	mreal dx,dy,dz;
	for(i=0;i<n;i++)	for(j=0;j<m;j++)	for(k=0;k<l;k++)	// first find closest
	{
		if(both)
		{	dx = x->v(i,j,k)-p.x;	dy = y->v(i,j,k)-p.y;	dz = x->v(i,j,k)-p.z;	}
		else
		{	dx = x->v(i)-p.x;	dy = y->v(j)-p.y;	dz = x->v(k)-p.z;	}
		d = sqrt(dx*dx+dy*dy+dz*dz);
		if(d<dm)	{	i0=i;	j0=j;	k0=k;	dm=d;	}
	}
	if(dm==0)	// we find it
	{	u=i0/mreal(n);	v=j0/mreal(m);	w=k0/mreal(l);	}
	else
	{
		mreal dxu,dxv,dxw,dyu,dyv,dyw,dzu,dzv,dzw;
		if(both)
		{
			dx = x->v(i0,j0,k0)-p.x;	dy = y->v(i0,j0,k0)-p.y;	dz = z->v(i0,j0,k0)-p.z;
			dxu= x->dvx(i0,j0,k0);		dyu= y->dvx(i0,j0,k0);		dzu= z->dvx(i0,j0,k0);
			dxv= x->dvy(i0,j0,k0);		dyv= y->dvy(i0,j0,k0);		dzv= z->dvy(i0,j0,k0);
			dxw= x->dvz(i0,j0,k0);		dyw= y->dvz(i0,j0,k0);		dzw= z->dvz(i0,j0,k0);
			d = dxu*(dyw*dzv-dyv*dzw)+dxv*(dyu*dzw-dyw*dzu)+dxw*(dyv*dzu-dyu*dzv);
			u = (i0+(dx*(dyw*dzv-dyv*dzw)+dxv*(dy*dzw-dyw*dz)+dxw*(dyv*dz-dy*dzv))/d)/n;
			v = (j0-(dx*(dyw*dzu-dyu*dzw)+dxu*(dy*dzw-dyw*dz)+dxw*(dyu*dz-dy*dzu))/d)/m;
			w = (i0+(dx*(dyv*dzu-dyu*dzv)+dxu*(dy*dzv-dyv*dz)+dxv*(dyu*dz-dy*dzu))/d)/l;
		}
		else
		{
			dx = x->v(i0)-p.x;	dy = y->v(j0)-p.y;	dz = z->v(k0)-p.z;
			dxu= x->dvx(i0);	dyv= y->dvx(j0);	dzw= z->dvx(k0);
			u = (i0+dx/dxu)/n;	v = (j0+dy/dyv)/m;	w = (k0+dz/dzw)/m;
		}
	}
	mglData xx(x), yy(y), zz(z), bx(ax), by(ay), bz(az);
	flow(gr, u, v, w, xx, yy, zz, bx, by, bz,ss,vv,xo,zo);
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_flowp_3d(HMGL gr, double x0, double y0, double z0, HCDT ax, HCDT ay, HCDT az, const char *sch, const char *opt)
{
	gr->SaveState(opt);
	mglData x(ax->GetNx()), y(ax->GetNy()), z(ax->GetNz());
	x.Fill(gr->Min.x,gr->Max.x);
	y.Fill(gr->Min.y,gr->Max.y);
	z.Fill(gr->Min.z,gr->Max.z);
	mgl_flowp_xyz(gr, x0,y0,z0, &x,&y,&z,ax,ay,az,sch,0);
	gr->LoadState();
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_flowp_xyz_(uintptr_t *gr, mreal *x0, mreal *y0, mreal *z0, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *ax, uintptr_t *ay, uintptr_t *az, const char *sch, const char *opt, int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_flowp_xyz(_GR_, *x0,*y0,*z0, _DA_(x), _DA_(y), _DA_(z), _DA_(ax), _DA_(ay), _DA_(az), s, o);
	delete []o;	delete []s;	}
void MGL_EXPORT mgl_flowp_3d_(uintptr_t *gr, mreal *x0, mreal *y0, mreal *z0, uintptr_t *ax, uintptr_t *ay, uintptr_t *az, const char *sch, const char *opt, int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_flowp_3d(_GR_, *x0,*y0,*z0, _DA_(ax), _DA_(ay), _DA_(az), s, o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
//
//	Grad series
//
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_grad_xyz(HMGL gr, HCDT x, HCDT y, HCDT z, HCDT phi, const char *sch, const char *opt)
{
	mglData ax(phi), ay,az,xx,yy,zz;
	ay.Set(ax);	az.Set(ax);	xx.Set(ax);	yy.Set(ax);	zz.Set(ax);
	long n=xx.nx, m=xx.ny, l=xx.nz, nn = n*m*l;
	if(x->GetNx()*x->GetNy()*x->GetNz()==nn && y->GetNx()*y->GetNy()*y->GetNz()==nn && x->GetNx()*x->GetNy()*x->GetNz()==nn)
	{	xx.Set(x);	yy.Set(y);	zz.Set(z);	}	// nothing to do
	else if(x->GetNx()==n && y->GetNx()==m && z->GetNx()==l)
	{	// prepare data
		register long i,j,k,i0;
		for(i=0;i<n;i++)	for(j=0;j<m;j++)	for(k=0;k<l;k++)
		{	i0 = i+n*(j+m*k);	xx.a[i0] = x->v(i);
			yy.a[i0] = y->v(j);	zz.a[i0] = z->v(k);	}
	}
	else	{	gr->SetWarn(mglWarnDim,"Grad");	return;	}
	ax.Diff(xx,yy,zz);	ay.Diff(yy,xx,zz);	az.Diff(zz,xx,yy);
	mgl_flow_xyz(gr,&xx,&yy,&zz,&ax,&ay,&az,sch,opt);
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_grad_xy(HMGL gr, HCDT x, HCDT y, HCDT phi, const char *sch, const char *opt)
{
	mglData ax(phi), ay,xx,yy;
	ay.Set(ax);	xx.Set(ax);	yy.Set(ax);
	long n = phi->GetNx(), m=phi->GetNy(), nn=n*m;
	if(x->GetNx()*x->GetNy()==nn && y->GetNx()*y->GetNy()==nn)	{	xx.Set(x);	yy.Set(y);	}
	else if(x->GetNx()==n && y->GetNx()==m)
	{
		register long i,j,i0;
		for(i=0;i<n;i++)	for(j=0;j<m;j++)
		{	i0 = i+n*j;	xx.a[i0] = x->v(i);	yy.a[i0] = y->v(j);	}
	}
	else	{	gr->SetWarn(mglWarnDim,"Grad");	return;	}
	ax.Diff(xx,yy);	ay.Diff(yy,xx);
	mgl_flow_xy(gr,&xx,&yy,&ax,&ay,sch,opt);
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_grad(HMGL gr, HCDT phi, const char *sch, const char *opt)
{
	mglData x(phi->GetNx()), y(phi->GetNy()), z(phi->GetNz());
	gr->SaveState(opt);
	x.Fill(gr->Min.x,gr->Max.x);	y.Fill(gr->Min.y,gr->Max.y);	z.Fill(gr->Min.z,gr->Max.z);
	if(phi->GetNz()==1)	mgl_grad_xy(gr,&x,&y,phi,sch,0);
	else				mgl_grad_xyz(gr,&x,&y,&z,phi,sch,0);
	gr->LoadState();
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_grad_xyz_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *ph, const char *sch, const char *opt, int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_grad_xyz(_GR_, _DA_(x), _DA_(y), _DA_(z), _DA_(ph), s, o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_grad_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *ph, const char *sch, const char *opt, int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_grad_xy(_GR_, _DA_(x), _DA_(y), _DA_(ph), s, o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_grad_(uintptr_t *gr, uintptr_t *ph, const char *sch, const char *opt, int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_grad(_GR_, _DA_(ph), s, o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
//
//	Pipe 2d series
//
//-----------------------------------------------------------------------------
void MGL_NO_EXPORT flowr(mglBase *gr, double zVal, double u, double v, const mglData &x, const mglData &y, const mglData &ax, const mglData &ay, double r0,long sc)
{
	long n=10*(ax.nx+ax.ny);
	bool both = x.nx==ax.nx && y.nx==ax.nx && x.ny==ax.ny && y.ny==ax.ny;

	mglPoint *pp = new mglPoint[n], dp;
	mreal *cc = new mreal[n];
	mglPoint dx(1/fabs(gr->Max.x-gr->Min.x),1/fabs(gr->Max.y-gr->Min.y),1/fabs(gr->Max.z-gr->Min.z));

	mreal dt = 0.5/(ax.nx > ax.ny ? ax.nx : ax.ny),e,f,g,ff[4],gg[4],h,s=1;
	mreal ss = 	4./mgl_ipow(gr->Max.c - gr->Min.c,2);
	if(u<0 || v<0)	{	dt = -dt;	u = -u;		v = -v;		s = -1;}
	register long k=0,m;
	bool end = false;
	do{
		if(gr->Stop)	{	delete []pp;	delete []cc;	return;	}
		pp[k].x = both ? x.Spline1(u,v,0):x.Spline1(u,0,0);
		pp[k].y = both ? y.Spline1(u,v,0):y.Spline1(v,0,0);
		pp[k].z = zVal;
		for(m=0;m<k-1;m++)	// determines encircle
			if(mgl_norm((pp[k]-pp[m])/dx)<dt/10.)	{	end = true;	break;	}
		f = ax.Linear1(u,v,0);	g = ay.Linear1(u,v,0);
		h = hypot(f,g);	cc[k] = gr->GetC(sc,s*h);
		pp[k].c = r0>0 ? r0*sqrt(1e-2+ss*h*h)/2 : -r0/sqrt(1e-2+ss*h*h)/5;
		if(h<1e-5)	break;	// stationary point
		k++;
		// find next point by midpoint method
		h+=1;	ff[0]=f*dt/h;	gg[0]=g*dt/h;
		e = u+ff[0]/2;	h = v+gg[0]/2;
		f = ax.Linear1(e,h,0);	g = ay.Linear1(e,h,0);	h = 1+hypot(f,g);
		ff[1]=f*dt/h;	gg[1]=g*dt/h;
		e = u+ff[1]/2;	h = v+gg[1]/2;
		f = ax.Linear1(e,h,0);	g = ay.Linear1(e,h,0);	h = 1+hypot(f,g);
		ff[2]=f*dt/h;	gg[2]=g*dt/h;
		e = u+ff[2];	h = v+gg[2];
		f = ax.Linear1(e,h,0);	g = ay.Linear1(e,h,0);	h = 1+hypot(f,g);
		ff[3]=f*dt/h;	gg[3]=g*dt/h;
		u += ff[0]/6+ff[1]/3+ff[2]/3+ff[3]/6;
		v += gg[0]/6+gg[1]/3+gg[2]/3+gg[3]/6;
		// condition of end
		end = end || k>=n || u<0 || v<0 || u>1 || v>1;
	} while(!end);
	if(k>1)
	{
		const int num=24;
		long i,j,*id=new long[2*num];
		mglPoint p,l=pp[1]-pp[0],t,q,d;
		t = !l;	t.Normalize();	q = t^l;	q.Normalize();
		mreal si,co,fi, rr=pp[0].c,dr=l.c;
		gr->Reserve(num*k);

		for(j=0;j<num;j++)
		{
			fi = j*2*M_PI/(num-1);	co = cos(fi);	si = sin(fi);
			p = pp[0] + t*(rr*co) + q*(rr*si);
			d = (t*si - q*co)^(l + t*(dr*co) + q*(dr*si));
			id[j] = gr->AddPnt(p,cc[0],d);
		}
		for(i=1;i<k;i++)
		{
			if(i<k-1)	l = pp[i+1]-pp[i-1];
			else	l = pp[i]-pp[i-1];
			t = !l;	t.Normalize();	q = t^l;	q.Normalize();
			rr=pp[i].c;	dr=l.c;
			memcpy(id+num,id,num*sizeof(long));
			for(j=0;j<num;j++)
			{
				fi = j*2*M_PI/(num-1);	co = cos(fi);	si = sin(fi);
				p = pp[i] + t*(rr*co) + q*(rr*si);
				d = (t*si - q*co)^(l + t*(dr*co) + q*(dr*si));
				id[j] = gr->AddPnt(p,cc[i],d);
				if(j>0)	gr->quad_plot(id[j-1],id[j],id[j+num-1],id[j+num]);
			}
		}
		delete []id;
	}
	delete []pp;	delete []cc;
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_pipe_xy(HMGL gr, HCDT x, HCDT y, HCDT ax, HCDT ay, const char *sch, double r0, const char *opt)
{
	mreal u,v;
	if(mgl_check_dim2(gr,x,y,ax,ay,"Pipe"))	return;

	mreal r = gr->SaveState(opt);
	long num = mgl_isnan(r)?5:long(r+0.5);
	static int cgid=1;	gr->StartGroup("Pipe",cgid++);

	long ss = gr->AddTexture(sch);
	// allocate memory
	mreal zVal = gr->Min.z;
	bool cnt=!mglchr(sch,'#');
	if(mglchr(sch,'i'))	r0 = -fabs(r0);

	mglData xx(x), yy(y), bx(ax), by(ay);
	for(long k=0;k<ax->GetNz();k++)
	{
		if(ax->GetNz()>1)	zVal = gr->Min.z+(gr->Max.z-gr->Min.z)*mreal(k)/(ax->GetNz()-1);
		for(long i=0;i<num;i++)
		{
			if(gr->Stop)	return;
			u = 0;	v = (i+1.)/(num+1.);
			flowr(gr, zVal, u, v, xx, yy, bx, by,r0,ss);
			flowr(gr, zVal, -u, -v, xx, yy, bx, by,r0,ss);
			u = 1;	v = (i+1.)/(num+1.);
			flowr(gr, zVal, u, v, xx, yy, bx, by,r0,ss);
			flowr(gr, zVal, -u, -v, xx, yy, bx, by,r0,ss);
			u = (i+1.)/(num+1.);	v = 0;
			flowr(gr, zVal, u, v, xx, yy, bx, by,r0,ss);
			flowr(gr, zVal, -u, -v, xx, yy, bx, by,r0,ss);
			u = (i+1.)/(num+1.);	v = 1;
			flowr(gr, zVal, u, v, xx, yy, bx, by,r0,ss);
			flowr(gr, zVal, -u, -v, xx, yy, bx, by,r0,ss);
			if(cnt)
			{
				u = 0.5;	v = (i+1.)/(num+1.);
				flowr(gr, zVal, u, v, xx, yy, bx, by,r0,ss);
				flowr(gr, zVal, -u, -v, xx, yy, bx, by,r0,ss);
				u = (i+1.)/(num+1.);	v = 0.5;
				flowr(gr, zVal, u, v, xx, yy, bx, by,r0,ss);
				flowr(gr, zVal, -u, -v, xx, yy, bx, by,r0,ss);
			}
		}
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_pipe_2d(HMGL gr, HCDT ax, HCDT ay, const char *sch, double r0, const char *opt)
{
	gr->SaveState(opt);
	mglData x(ax->GetNx()), y(ax->GetNy());
	x.Fill(gr->Min.x,gr->Max.x);
	y.Fill(gr->Min.y,gr->Max.y);
	mgl_pipe_xy(gr,&x,&y,ax,ay,sch,r0,0);
	gr->LoadState();
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_pipe_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *ax, uintptr_t *ay, const char *sch, mreal *r0, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_pipe_xy(_GR_, _DA_(x), _DA_(y), _DA_(ax), _DA_(ay), s, *r0, o);	delete []o;	delete []s;	}
void MGL_EXPORT mgl_pipe_2d_(uintptr_t *gr, uintptr_t *ax, uintptr_t *ay, const char *sch, mreal *r0, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];	memcpy(o,opt,lo);	o[lo]=0;
	mgl_pipe_2d(_GR_, _DA_(ax), _DA_(ay), s, *r0, o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
//
//	Pipe 3d series
//
//-----------------------------------------------------------------------------
void flowr(mglBase *gr, double u, double v, double w, const mglData &x, const mglData &y, const mglData &z, const mglData &ax, const mglData &ay, const mglData &az, double r0,long sc)
{
	static long n=10*(ax.nx+ax.ny);
	long nn = ax.nx*ax.ny*ax.nz;
	bool both = x.nx*x.ny*x.nz==nn && y.nx*y.ny*y.nz==nn && z.nx*z.ny*z.nz==nn;
	mglPoint *pp = new mglPoint[n], dp;
	mreal *cc = new mreal[n];
	mglPoint dx(1/fabs(gr->Max.x-gr->Min.x),1/fabs(gr->Max.y-gr->Min.y),1/fabs(gr->Max.z-gr->Min.z));

	nn = (ax.nx > ax.ny ? ax.nx : ax.ny);
	nn = (nn > ax.nz ? nn : ax.nz);
	mreal dt = 0.2/nn, e,f,g,ee[4],ff[4],gg[4],h,s=1,u1,v1,w1;
	mreal ss = 	4./mgl_ipow(gr->Max.c - gr->Min.c,2);

	if(u<0 || v<0 || w<0)
	{	dt = -dt;	u = -u;		v = -v;		w = -w;		s = -1;}
	register long k=0,m;
	bool end = false;
	do{
		if(gr->Stop)	{	delete []pp;	delete []cc;	return;	}
		pp[k].x = both ? x.Spline1(u,v,w):x.Spline1(u,0,0);
		pp[k].y = both ? y.Spline1(u,v,w):y.Spline1(v,0,0);
		pp[k].z = both ? z.Spline1(u,v,w):z.Spline1(w,0,0);
		for(m=0;m<k-1;m++)	// determines encircle
			if(mgl_norm((pp[k]-pp[m])/dx)<dt/10.)	{	end = true;	break;	}
		e = ax.Linear1(u,v,w);	f = ay.Linear1(u,v,w);	g = az.Linear1(u,v,w);
		h = sqrt(e*e+f*f+g*g);	cc[k] = gr->GetC(sc,s*h);
		pp[k].c = r0>0 ? r0*sqrt(1e-2+ss*h*h)/2 : -r0/sqrt(1e-2+ss*h*h)/5;
		if(h<1e-5)	break;	// stationary point
		k++;
		// find next point by midpoint method
		h+=1;	ee[0]=e*dt/h;	ff[0]=f*dt/h;	gg[0]=g*dt/h;
		u1 = u+ee[0]/2;	v1 = v+ff[0]/2;	w1 = w+gg[0]/2;
		e = ax.Linear1(u1,v1,w1);	f = ay.Linear1(u1,v1,w1);
		g = az.Linear1(u1,v1,w1);	h = 1+sqrt(e*e+f*f+g*g);
		ee[1]=e*dt/h;	ff[1]=f*dt/h;	gg[1]=g*dt/h;
		u1 = u+ee[1]/2;	v1 = v+ff[1]/2;	w1 = w+gg[1]/2;
		e = ax.Linear1(u1,v1,w1);	f = ay.Linear1(u1,v1,w1);
		g = az.Linear1(u1,v1,w1);	h = 1+sqrt(e*e+f*f+g*g);
		ee[2]=e*dt/h;	ff[2]=f*dt/h;	gg[2]=g*dt/h;
		u1 = u+ee[2];	v1 = v+ff[2];	w1 = w+gg[2];
		e = ax.Linear1(u1,v1,w1);	f = ay.Linear1(u1,v1,w1);
		g = az.Linear1(u1,v1,w1);	h = 1+sqrt(e*e+f*f+g*g);
		ee[3]=e*dt/h;	ff[3]=f*dt/h;	gg[3]=g*dt/h;
		u += ee[0]/6+ee[1]/3+ee[2]/3+ee[3]/6;
		v += ff[0]/6+ff[1]/3+ff[2]/3+ff[3]/6;
		w += gg[0]/6+gg[1]/3+gg[2]/3+gg[3]/6;
		// condition of end
		end = end || k>=n || u<0 || v<0 || u>1 || v>1 || w<0 || w>1;
	} while(!end);
	if(k>1)
	{
		const int num=24;
		long i,j,*id=new long[2*num];
		mglPoint p,l=pp[1]-pp[0],t,q,d;
		t = !l;	t.Normalize();	q = t^l;	q.Normalize();
		mreal si,co,fi, rr=pp[0].c,dr=l.c;
		gr->Reserve(num*k);

		for(j=0;j<num;j++)
		{
			fi = j*2*M_PI/(num-1);	co = cos(fi);	si = sin(fi);
			p = pp[0] + t*(rr*co) + q*(rr*si);
			d = (t*si - q*co)^(l + t*(dr*co) + q*(dr*si));
			id[j] = gr->AddPnt(p,cc[0],d);
		}
		for(i=1;i<k;i++)
		{
			if(i<k-1)	l = pp[i+1]-pp[i-1];
			else	l = pp[i]-pp[i-1];
			t = !l;	t.Normalize();	q = t^l;	q.Normalize();
			rr=pp[i].c;	dr=l.c;
			memcpy(id+num,id,num*sizeof(long));
			for(j=0;j<num;j++)
			{
				fi = j*2*M_PI/(num-1);	co = cos(fi);	si = sin(fi);
				p = pp[i] + t*(rr*co) + q*(rr*si);
				d = (t*si - q*co)^(l + t*(dr*co) + q*(dr*si));
				id[j] = gr->AddPnt(p,cc[i],d);
				if(j>0)	gr->quad_plot(id[j-1],id[j],id[j+num-1],id[j+num]);
			}
		}
		delete []id;
	}
	delete []pp;	delete []cc;
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_pipe_xyz(HMGL gr, HCDT x, HCDT y, HCDT z, HCDT ax, HCDT ay, HCDT az, const char *sch, double r0, const char *opt)
{
	mreal u,v,w;
	long i,j;
	if(mgl_check_vec3(gr,x,y,z,ax,ay,az,"Vect"))	return;

	mreal r = gr->SaveState(opt);
	long num = mgl_isnan(r)?3:long(r+0.5);
	static int cgid=1;	gr->StartGroup("Pipe3",cgid++);
	if(mglchr(sch,'i'))	r0 = -fabs(r0);

	long ss = gr->AddTexture(sch);
	bool cnt=!mglchr(sch,'#');

	mglData xx(x), yy(y), zz(z), bx(ax), by(ay), bz(az);
	for(i=0;i<num;i++)	for(j=0;j<num;j++)
	{
		if(gr->Stop)	return;
		u = (i+1.)/(num+1.);	v = (j+1.)/(num+1.);	w = 0;
		flowr(gr, u, v, w, xx, yy, zz, bx, by, bz,r0,ss);
		flowr(gr,-u,-v,-w, xx, yy, zz, bx, by, bz,r0,ss);
		u = (i+1.)/(num+1.);	v = (j+1.)/(num+1.);	w = 1;
		flowr(gr, u, v, w, xx, yy, zz, bx, by, bz,r0,ss);
		flowr(gr,-u,-v,-w, xx, yy, zz, bx, by, bz,r0,ss);
		u = 0;	v = (j+1.)/(num+1.);	w = (i+1.)/(num+1.);
		flowr(gr, u, v, w, xx, yy, zz, bx, by, bz,r0,ss);
		flowr(gr,-u,-v,-w, xx, yy, zz, bx, by, bz,r0,ss);
		u = 1;	v = (j+1.)/(num+1.);	w = (i+1.)/(num+1.);
		flowr(gr, u, v, w, xx, yy, zz, bx, by, bz,r0,ss);
		flowr(gr,-u,-v,-w, xx, yy, zz, bx, by, bz,r0,ss);
		u = (i+1.)/(num+1.);	v = 0;	w = (j+1.)/(num+1.);
		flowr(gr, u, v, w, xx, yy, zz, bx, by, bz,r0,ss);
		flowr(gr,-u,-v,-w, xx, yy, zz, bx, by, bz,r0,ss);
		u = (i+1.)/(num+1.);	v = 1;	w = (j+1.)/(num+1.);
		flowr(gr, u, v, w, xx, yy, zz, bx, by, bz,r0,ss);
		flowr(gr,-u,-v,-w, xx, yy, zz, bx, by, bz,r0,ss);
		if(cnt)
		{
			u = (i+1.)/(num+1.);	v = (j+1.)/(num+1.);	w = 0.5;
			flowr(gr, u, v, w, xx, yy, zz, bx, by, bz,r0,ss);
			flowr(gr,-u,-v,-w, xx, yy, zz, bx, by, bz,r0,ss);
			u = 0.5;	v = (j+1.)/(num+1.);	w = (i+1.)/(num+1.);
			flowr(gr, u, v, w, xx, yy, zz, bx, by, bz,r0,ss);
			flowr(gr,-u,-v,-w, xx, yy, zz, bx, by, bz,r0,ss);
			u = (i+1.)/(num+1.);	v = 0.5;	w = (j+1.)/(num+1.);
			flowr(gr, u, v, w, xx, yy, zz, bx, by, bz,r0,ss);
			flowr(gr,-u,-v,-w, xx, yy, zz, bx, by, bz,r0,ss);
		}
	}
	gr->EndGroup();
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_pipe_3d(HMGL gr, HCDT ax, HCDT ay, HCDT az, const char *sch, double r0, const char *opt)
{
	gr->SaveState(opt);
	mglData x(ax->GetNx()), y(ax->GetNy()), z(ax->GetNz());
	x.Fill(gr->Min.x,gr->Max.x);
	y.Fill(gr->Min.y,gr->Max.y);
	z.Fill(gr->Min.z,gr->Max.z);
	mgl_pipe_xyz(gr,&x,&y,&z,ax,ay,az,sch,r0,0);
	gr->LoadState();
}
//-----------------------------------------------------------------------------
void MGL_EXPORT mgl_pipe_xyz_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *ax, uintptr_t *ay, uintptr_t *az, const char *sch, mreal *r0, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];		memcpy(o,opt,lo);	o[lo]=0;
	mgl_pipe_xyz(_GR_, _DA_(x), _DA_(y), _DA_(z), _DA_(ax), _DA_(ay), _DA_(az), s, *r0, o);
	delete []o;	delete []s;	}
void MGL_EXPORT mgl_pipe_3d_(uintptr_t *gr, uintptr_t *ax, uintptr_t *ay, uintptr_t *az, const char *sch, mreal *r0, const char *opt,int l,int lo)
{	char *s=new char[l+1];	memcpy(s,sch,l);	s[l]=0;
	char *o=new char[lo+1];		memcpy(o,opt,lo);	o[lo]=0;
	mgl_pipe_3d(_GR_, _DA_(ax), _DA_(ay), _DA_(az), s, *r0, o);	delete []o;	delete []s;	}
//-----------------------------------------------------------------------------
