/////////////////////////////////////////////////////////////////////////////
// Name:        calcPanel.cpp
// Purpose:     panel to implement the calculator
// Author:      Richard Abbott
// Modified by:
// Created:     2003-06-03
// RCS-ID:      $Id: calcPanel.cpp,v 1.7 2005/03/17 16:34:27 rich Exp $
// Copyright:   (c) Richard Abbott
// Licence:     All rights reserved
/////////////////////////////////////////////////////////////////////////////

#include<math.h>
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include"calcPanel.h"

BEGIN_EVENT_TABLE(CalcPanel, wxPanel)
  EVT_CHAR(CalcPanel::OnChar)
  EVT_PAINT(CalcPanel::OnPaint)
  //  EVT_LEFT_DCLICK(GraphPanel::OnDClick)
  //  EVT_LEFT_UP(GraphPanel::OnLeftButtonUp)
  //  EVT_LEFT_DOWN(GraphPanel::OnLeftButtonDown)
  //  EVT_RIGHT_UP(GraphPanel::OnRightButtonUp)
  //  EVT_MOTION(GraphPanel::OnMouseMotion)
  //  EVT_TEXT_ENTER(ID_INNUMWINDOW, GraphPanel::OnLeftButtonUp)
END_EVENT_TABLE()
;

CalcPanel::CalcPanel(wxWindow *parent, wxWindowID id, const wxPoint pos,
							  const wxSize size)
  : wxPanel(parent, id, pos, size)
{
  int ii;
  for(ii=0;ii<STACKSIZE;ii++) theStack[ii]=0;
  currentLine=_T("+");
  currentValue=0;
  edited=EDITNEVER;
  marked=-1;
}

void CalcPanel::OnChar(wxKeyEvent& event)
{
  int ii;
  long tLong;
  double dTmp;

  int ltr=event.GetKeyCode();
  switch(ltr){
  case 'e':
	 if(currentLine.Find('e')>=0) break;
  case 'A':
  case 'B':
  case 'C':
  case 'D':
  case 'E':
  case 'F':
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
  case '.':
    if((edited==EDITSTACKED || edited==EDITENTERED)  && currentLine!=_T("+")){			  // this is a new number
		if(edited==EDITSTACKED){
		  for(ii=STACKSIZE-1;ii>0;ii--)
			 theStack[ii]=theStack[ii-1];
		  //		  currentLine.ToDouble(&theStack[0]);
		  theStack[0]=currentValue; // already set currentValue?
		}
		currentLine=_T("+");
		currentValue=0;
	 }
	 edited=EDITNOW;
	 if(currentLine==_T("+")){
		currentLine.Empty(); // don't carry the starting char around
		currentValue=0;
	 }
	 currentLine+=ltr;
	 marked=-1;
	 Refresh();
	 break;
  case WXK_UP:						  // move cursor up
	 if(++marked==STACKSIZE) marked--;
	 Refresh();
	 break;
  case WXK_DOWN:
	 if(--marked<-1) marked=0;
	 Refresh();
	 break;
  case WXK_LEFT:					  // swap with stack[0]
	 dTmp=(edited==EDITNEVER)?currentLine.ToDouble(&dTmp):currentValue;
	 currentLine=wxString::Format(_T("%lf"), theStack[0]);
	 currentValue=theStack[0];
	 theStack[0]=dTmp;
	 edited=EDITSTACKED;
	 Refresh();
	 break;
  case WXK_DELETE:				  // erase current line
	 if(event.m_shiftDown){		  // clear entire stack
		for(ii=0;ii<STACKSIZE;ii++)
		  theStack[ii]=0;
		currentLine=_T("+");
		currentValue=0;
		edited=EDITNEVER;
		Refresh();
		break;
	 }
	 if(theStack[0]==0){
     currentLine=_T("+");
		currentValue=0;
		edited=EDITNEVER;
	 }else{
     currentLine=wxString::Format(_T("%lf"), theStack[0]);
		currentValue=theStack[0];
		edited=EDITSTACKED;
	 }
	 for(ii=0; ii< STACKSIZE;ii++){
		theStack[ii]=theStack[ii+1];
	 }
	 marked=-1;
	 Refresh();
	 break;
  case WXK_BACK:
	 currentLine=currentLine.Left(currentLine.Length()-1);
	 if(currentLine.IsEmpty()){
     currentLine=_T("+");
		currentValue=0;
		edited=EDITNEVER;
	 }
	 marked=-1;
	 Refresh();
	 break;
  case WXK_RETURN:
	 if(marked>=0){				  // doing a swap
		if(edited==EDITNOW) currentLine.ToDouble(&dTmp); else dTmp=currentValue;
		currentLine=wxString::Format(_T("%lf"), theStack[marked]);
		currentValue=theStack[marked];
		theStack[marked]=dTmp;
		edited=EDITSTACKED;
	 }else{							  // copy the number up
		for(ii=STACKSIZE-1;ii>0;ii--)
		  theStack[ii]=theStack[ii-1];
		if(edited==EDITNOW){
		  currentLine.ToDouble(&theStack[0]);
		  currentValue=theStack[0];
		}else theStack[0]=currentValue;
		edited=EDITENTERED;
	 }
	 Refresh();
	 break;
  case '-':
    if((int)currentLine.Find(_T("e"))==(int)currentLine.Length()-1){
		currentLine+='-';
		break;
	 }
  case '^':
  case '%':
  case '/':
  case '*':
  case '+':							  // add
    if(currentLine==_T("-") && ltr=='+'){		  // just a sign change
      currentLine=_T("+");
    }else if(currentLine==_T("+") && ltr=='-'){
      currentLine=_T("-");
	 }else{
		if(edited==EDITNOW) currentLine.ToDouble(&dTmp); else dTmp=currentValue;
		if(ltr=='/') dTmp=theStack[0]/dTmp;
		if(ltr=='*') dTmp=theStack[0]*dTmp;
		if(ltr=='-') dTmp=theStack[0]-dTmp;
		if(ltr=='+') dTmp=theStack[0]+dTmp;
		if(ltr=='%') dTmp=(long)theStack[0]%(long)dTmp;
		if(ltr=='^') dTmp=pow(theStack[0], dTmp);
		currentLine=wxString::Format(_T("%lf"), dTmp);
		currentValue=dTmp;
		for(ii=1;ii<STACKSIZE;ii++)
		  theStack[ii-1]=theStack[ii];
		edited=EDITSTACKED;
	 }
	 marked=-1;
	 Refresh();
	 break;
  case '_':							  // change sign
	 if(edited==EDITNOW){
     if(currentLine.Left(1)==_T("-")) currentLine=currentLine.Mid(1);
     else if(currentLine.Left(1)==_T('+')) currentLine=_T('-')+currentLine.Mid(1);
     else currentLine=_T('-')+currentLine;
	 }else{
		currentValue=-currentValue;
		currentLine=wxString::Format(_T("%lf"), currentValue);
	 }
	 marked=-1;
	 Refresh();
	 break;
  case 'i':
	 if(edited==EDITNOW) currentLine.ToDouble(&dTmp); else dTmp=currentValue;
	 currentValue=1.0/dTmp;
	 currentLine=wxString::Format(_T("%lf"), currentValue);
	 edited=EDITSTACKED;
	 marked=-1;
	 Refresh();
	 break;
  case 's':
	 if(edited==EDITNOW) currentLine.ToDouble(&dTmp); else dTmp=currentValue;
	 currentValue=sin(dTmp);
	 currentLine=wxString::Format(_T("%lf"), currentValue);
	 edited=EDITSTACKED;
	 marked=-1;
	 Refresh();
	 break;
  case 'c':
	 if(edited==EDITNOW) currentLine.ToDouble(&dTmp); else dTmp=currentValue;
	 currentValue=cos(dTmp);
	 currentLine=wxString::Format(_T("%lf"), currentValue);
	 edited=EDITSTACKED;
	 marked=-1;
	 Refresh();
	 break;
  case 't':
	 if(edited==EDITNOW) currentLine.ToDouble(&dTmp); else dTmp=currentValue;
	 currentValue=tan(dTmp);
	 currentLine=wxString::Format(_T("%lf"), currentValue);
	 edited=EDITSTACKED;
	 marked=-1;
	 Refresh();
	 break;
  case 'S':
	 if(edited==EDITNOW) currentLine.ToDouble(&dTmp); else dTmp=currentValue;
	 currentValue=asin(dTmp);
	 currentLine=wxString::Format(_T("%lf"), currentValue);
	 edited=EDITSTACKED;
	 marked=-1;
	 Refresh();
	 break;
/*
  case 'C':
	 if(edited==EDITNOW) currentLine.ToDouble(&dTmp); else dTmp=currentValue;
	 currentValue=acos(dTmp);
	 currentLine=wxString::Format("%lf", currentValue);
	 edited=EDITSTACKED;
	 marked=-1;
	 Refresh();
	 break;
  */case 'T':
	 if(edited==EDITNOW) currentLine.ToDouble(&dTmp); else dTmp=currentValue;
	 currentValue=atan(dTmp);
	 currentLine=wxString::Format(_T("%lf"), currentValue);
	 edited=EDITSTACKED;
	 marked=-1;
	 Refresh();
	 break;
  case 'p':							  // pi
	 if(edited!=EDITNEVER && edited!=EDITENTERED){
		for(ii=STACKSIZE-1;ii>0;ii--)
		  theStack[ii]=theStack[ii-1];
		currentLine.ToDouble(&theStack[0]);
		currentValue=theStack[0];
	 }
	 currentLine=_T("3.141592654");
	 currentValue=3.141592654;
	 marked=-1;
	 edited=EDITSTACKED;
	 Refresh();
	 break;
  case 'h':
	 if(edited==EDITNOW) currentLine.ToDouble(&dTmp); else dTmp=currentValue;
	 wxMessageBox(wxString::Format(_T("0x%x"), (unsigned long)dTmp),
                _T("Hex Display"), wxOK);
	 break;
  case 'o':
	 if(edited==EDITNOW) currentLine.ToDouble(&dTmp); else dTmp=currentValue;
	 wxMessageBox(wxString::Format(_T("o%o"), (unsigned long)dTmp),
                _T("Octal Display"), wxOK);
	 break;
  case 'd':							  // convert hex to decimal
	 for(ii=STACKSIZE-1;ii>0;ii--)
		theStack[ii]=theStack[ii-1];
	 if(edited==EDITNOW){
		currentLine.ToLong(&tLong, 16);
		theStack[0]=(double)tLong;
	 }
	 else theStack[0]=currentValue;
	 edited=EDITENTERED;
	Refresh();
	 break;
  case 'b':
	 {
		long ii, jj, kk;
		wxString dString;
		  
		if(edited==EDITNOW) currentLine.ToDouble(&dTmp); else dTmp=currentValue;
		jj=(unsigned long)dTmp;
		ii=1;
		kk=-1;
		while(ii<=jj){
		  kk++;
		  if(kk==4){
			 kk=0;
			 dString=_T(" ")+dString;
		  }
		  if((jj&ii)==0){
        dString=_T("0")+dString;
		  }else{
        dString=_T("1")+dString;
		  }
		  ii*=2;
		}
		wxMessageBox(wxString::Format(_T("b%s"), dString.GetData()),
                 _T("Binary Display"), wxOK);
	 }
	 break;

  default:
	 event.Skip();
  };
}

void CalcPanel::OnPaint(wxPaintEvent& WXUNUSED(event))
{
  wxPaintDC dc(this);
  DrawNum(&dc);
}

void CalcPanel::DrawNum(wxDC *dc)
{
  wxSize clientSize;
  wxRect clientRect;
  wxCoord width,height;
  wxPoint orig;
  wxString outtxt;

  int ii;
  int jj;

  orig=GetClientAreaOrigin();

  clientSize=GetClientSize();
  clientRect.SetX(orig.x);		  // set up the clientRectangle that we paint in
  clientRect.SetY(orig.y);
  clientRect.SetHeight(clientSize.GetHeight());
  clientRect.SetWidth(clientSize.GetWidth());

  outtxt=currentLine;
  dc->GetTextExtent(outtxt, &width, &height);	// draw the x label
  dc->DrawText(outtxt, clientRect.GetRight()-width,
					clientRect.GetBottom()-height);
  dc->DrawLine(clientRect.GetLeft(), clientRect.GetBottom()-(3*height/2),
					clientRect.GetWidth(), clientRect.GetBottom()-(3*height/2));
  jj=0;
  for(ii=clientRect.GetBottom()-4*height; ii>0; ii-=height){ // draw the stack
    outtxt=wxString::Format(_T("%lf"), theStack[jj]);
    if(jj>=0 && jj==marked) outtxt=_T("->")+outtxt;	// draw the mark
	 dc->GetTextExtent(outtxt, &width, &height);	// draw the x label
	 dc->DrawText(outtxt, clientRect.GetRight()-width, ii);
	 jj++;
  }
}

bool CalcPanel::Save(wxString fn)
{
  int ii, jj;
  FILE *fp;

  jj=STACKSIZE-1;
  while(jj>0 && theStack[jj]==0) jj--;
  if(jj<0) return(false);				  // no numbers in there

  fp=fopen(fn.char_str(), "w");
  if(fp==NULL) return(false);
  for(ii=0;ii<=jj;ii++){
    fprintf(fp, "%lf\n", theStack[ii]);
  }
  fclose(fp);
  return(true);
}
