/***************************************************************************
 * wx.h.cpp is part of Math Graphic Library
 * Copyright (C) 2007 Alexey Balakin <balakin@appl.sci-nnov.ru>            *
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
#ifndef MGL_WX_H
#define MGL_WX_H
//-----------------------------------------------------------------------------
#include <wx/window.h>
#include <wx/image.h>
#include <wx/timer.h>
#include <wx/bitmap.h>
#include <mgl2/canvas_wnd.h>
//-----------------------------------------------------------------------------
/// Class is Wx widget which display MathGL graphics
class wxMathGL : public wxWindow
{
public:
	wxString appName;	///< Application name for message boxes
	bool AutoResize; 	///< Allow auto resizing (default is false)

	wxMathGL(wxWindow *parent, wxWindowID id=-1, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=0, const wxString& name=wxPanelNameStr);
	virtual ~wxMathGL();
	double GetRatio()	{	return double(gr->GetWidth())/gr->GetHeight();	};
	void SetPopup(wxMenu *p)	{	popup = p;	};	///< Set popup menu pointer
	void SetSize(int w, int h);		///< Set window/picture sizes
	void SetGraph(mglCanvas *gr);	///< Set grapher object
	inline void SetGraph(mglGraph *GR)
	{	SetGraph(dynamic_cast<mglCanvas *>(GR->Self()));	}
	inline HMGL GetGraph()	{	return gr;	}

	/// Set drawing functions and its parameter
	inline void SetDraw(int (*func)(mglBase *gr, void *par), void *par=0)
	{	draw_func = func;	draw_par = par;	}
	inline void SetDraw(mglDraw *dr)
	{	draw_cl = dr;	}
	inline void SetDraw(int (*draw)(mglGraph *gr))
	{	SetDraw(mgl_draw_graph,(void*)draw);	}
	inline void ZoomRegion(mreal xx1,mreal xx2,mreal yy1, mreal yy2)
	{	x1=xx1;	y1=yy1;	x2=xx2;	y2=yy2;	}

	int GetPer() 	{return per;};		///< Get perspective value
	int GetPhi() 	{return phi;};		///< Get Phi-angle value
	int GetTet() 	{return tet;};		///< Get Theta-angle value
	bool GetAlpha()	{return alpha;};	///< Get transparency state
	bool GetLight()	{return light;};	///< Get lightning state
	bool GetZoom()	{return zoom;};		///< Get mouse zooming state
	bool GetRotate()	{return rotate;};	///< Get mouse rotation state

	void Repaint();
	void Update();			///< Update picture
	void Copy(); 			///< copy graphics to clipboard
	void Print();			///< Print plot
//	void Stop();			///< Stop execution
	void SetPer(int p);		///< Set perspective value
	void SetPhi(int p);		///< Set Phi-angle value
	void SetTet(int t);		///< Set Theta-angle value
	void SetAlpha(bool a);	///< Switch on/off transparency
	void SetLight(bool l);	///< Switch on/off lightning
	void SetZoom(bool z);	///< Switch on/off mouse zooming
	void SetRotate(bool r);	///< Switch on/off mouse rotation
	void ZoomIn();			///< Zoom in graphics
	void ZoomOut();			///< Zoom out graphics
	void Restore();			///< Restore zoom and rotation to default values
//	void Reload();			///< Reload data and execute script
	void ShiftLeft();		///< Shift graphics to left direction
	void ShiftRight();		///< Shift graphics to right direction
	void ShiftUp();			///< Shift graphics to up direction
	void ShiftDown();		///< Shift graphics to down direction
	void ExportPNG(wxString fname=L"");	///< export to PNG file
	void ExportPNGs(wxString fname=L"");	///< export to PNG file (no transparency)
	void ExportJPG(wxString fname=L"");	///< export to JPEG file
	void ExportBPS(wxString fname=L"");	///< export to bitmap EPS file
	void ExportEPS(wxString fname=L"");	///< export to vector EPS file
	void ExportSVG(wxString fname=L"");	///< export to SVG file

	void Adjust();		///< Adjust plot size to fill entire window
	void NextSlide();	///< Show next slide
	void PrevSlide();	///< Show previous slide
	void Animation(bool st=true);	///< Start animation

	void About();		///< Show about information
	void AboutQt();		///< Show information about Qt version

protected:
	void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnNextSlide(wxTimerEvent& evt);	///< Show next slide
	void OnMouseLeftDown(wxMouseEvent &ev);
	void OnMouseDown(wxMouseEvent &ev);
	void OnMouseLeftUp(wxMouseEvent &ev);
	void OnMouseRightUp(wxMouseEvent &ev);
	void OnMouseMove(wxMouseEvent &ev);
//	void MousePressEvent(QMouseEvent *);
//	void MouseReleaseEvent(QMouseEvent *);
//	void MouseMoveEvent(QMouseEvent *);

	mglCanvas *gr;		///< pointer to grapher
	void *draw_par;		///< Parameters for drawing function mglCanvasWnd::DrawFunc.
	/// Drawing function for window procedure. It should return the number of frames.
	int (*draw_func)(mglBase *gr, void *par);
	mglDraw *draw_cl;

	wxString MousePos;	///< Last mouse position
	wxBitmap pic;		///< Pixmap for drawing (changed by update)
	double tet, phi;	///< Rotation angles
	double per;			///< Value of perspective ( must be in [0,1) )
	bool alpha;			///< Transparency state
	bool light;			///< Lightning state
	bool zoom;			///< Mouse zoom state
	bool rotate;		///< Mouse rotation state
	mreal x1,x2,y1,y2;	///< Zoom in region
	bool showMessage;	///< Flag for showing messages (enabled by each execute())
	wxMenu *popup;		///< Pointer to pop-up menu
	wxTimer *timer;		///< Timer for animation
	DECLARE_EVENT_TABLE()
private:
	int x0, y0, xe, ye;		///< Temporary variables for mouse
	unsigned char *grBuf;
};
//-----------------------------------------------------------------------------
#include <wx/scrolwin.h>
class wxSpinCtrl;
class wxMenu;
int mglWxRun();
//-----------------------------------------------------------------------------
class mglCanvasWX : public mglCanvasWnd
{
friend class wxMathGL;
public:
	int sshow;			///< Current state of animation switch (toggle button)
	wxMathGL *WMGL;		///< Control which draw graphics
	wxWindow *Wnd;		///< Pointer to window

	mglCanvasWX();
	virtual ~mglCanvasWX();

	/// Create a window for plotting. Now implemeted only for GLUT.
	void Window(int argc, char **argv, int (*draw)(mglBase *gr, void *p), const char *title,
				void *par=NULL, void (*reload)(void *p)=NULL, bool maximize=false);
	/// Switch on/off transparency (do not overwrite switches in user drawing function)
	void ToggleAlpha();
	/// Switch on/off lighting (do not overwrite switches in user drawing function)
	void ToggleLight();
	void ToggleRotate();	///< Switch on/off rotation by mouse
	void ToggleZoom();		///< Switch on/off zooming by mouse
	void ToggleNo();		///< Switch off all zooming and rotation
	void Update();			///< Update picture by calling user drawing function
	void Adjust();			///< Adjust size of bitmap to window size
	void GotoFrame(int d);	///< Show arbitrary frame (use relative step)
	void Animation();	///< Run animation (I'm too lasy to change it)
	
protected:
	wxScrolledWindow *scroll;	///< Scrolling area
	wxMenu *popup;			///< Popup menu
	wxSpinCtrl *tet, *phi;	///< Spin box for angles
//	QAction *anim;

	void MakeMenu();		///< Create menu, toolbar and popup menu
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------