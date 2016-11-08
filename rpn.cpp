/////////////////////////////////////////////////////////////////////////////
// Name:        minimal.cpp
// Purpose:     graphPanel sample
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id: rpn.cpp,v 1.3 2005/12/29 18:56:49 rich Exp $
// Copyright:   (c) Richard Abbott
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx/wx.h".
#include<math.h>
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers)
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------

// the application icon (under Windows and OS/2 it is in resources)
#if defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXMAC__) || defined(__WXMGL__) || defined(__WXX11__)
#include "calc.xpm"
#endif

// ----------------------------------------------------------------------
// Other header files
// ----------------------------------------------------------------------
#include"calcPanel.h"

class MyApp : public wxApp
{
public:
  virtual bool OnInit();
};

// `MyFrame
class MyFrame : public wxFrame
{
  wxString m_filename;
  wxString m_defDir;
  CalcPanel *calcPanel;
public:
  MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size,
			 long style = wxDEFAULT_FRAME_STYLE);

  void OnSaveAs(wxCommandEvent& event);
  void OnQuit(wxCommandEvent& event);
  void OnAbout(wxCommandEvent& event);
  void OnKeys(wxCommandEvent& WXUNUSED(event));
  
private:
  DECLARE_EVENT_TABLE()
	 ;
};

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
  // menu items
  Minimal_Quit = 1,
  Minimal_FileOpen,
  Minimal_Test,
  KEYSMENU,
  SAVEASMENU,
  Minimal_About = wxID_ABOUT
};

// ----------------------------------------------------------------------------
// event tables and other macros for wxWindows
// ----------------------------------------------------------------------------

// the event tables connect the wxWindows events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
  EVT_MENU(SAVEASMENU,    MyFrame::OnSaveAs)
  EVT_MENU(Minimal_Quit,  MyFrame::OnQuit)
  EVT_MENU(KEYSMENU, MyFrame::OnKeys)
  EVT_MENU(Minimal_About, MyFrame::OnAbout)
  END_EVENT_TABLE()

  // Create a new application object: this macro will allow wxWindows to create
  // the application object during program execution (it's better than using a
  // static object for many reasons) and also declares the accessor function
  // wxGetApp() which will return the reference of the right type (i.e. MyApp and
  // not wxApp)
  IMPLEMENT_APP(MyApp)

  // ============================================================================
  // implementation
  // ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// 'Main program' equivalent: the program execution "starts" here
  bool MyApp::OnInit()
{
  // create the main application window
  MyFrame *frame = new MyFrame(_T("RPN"),
										 wxPoint(50, 50), wxSize(450, 340),
										 wxDEFAULT_FRAME_STYLE);

  // and show it (the frames, unlike simple controls, are not shown when
  // created initially)
  frame->Show(TRUE);

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned FALSE here, the
    // application would exit immediately.
  return TRUE;
}

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style)
  : wxFrame(NULL, -1, title, pos, size, style)
{
  // set the frame icon
  SetIcon(wxICON(mondrian));

#if wxUSE_MENUS
  // create a menu bar
  wxMenu *menuFile = new wxMenu;

  // the "About" item should be in the help menu
  wxMenu *helpMenu = new wxMenu;
  menuFile->Append(SAVEASMENU,   _T("Save &As\tAlt-A"), _T("Save the program as..."));
  menuFile->Append(Minimal_Test, _T("&Test\tAlt-T"), _T("Test the program"));
  helpMenu->Append(KEYSMENU,     _T("&Shortcuts\tAlt-S"), _T("Keyboard shortcuts"));
  helpMenu->Append(Minimal_About, _T("&About...\tF1"), _T("Show about dialog"));

  menuFile->Append(Minimal_Quit, _T("E&xit\tCtrl-W"), _T("Quit this program"));

  // now append the freshly created menu to the menu bar...
  wxMenuBar *menuBar = new wxMenuBar();
  menuBar->Append(menuFile, _T("&File"));
  menuBar->Append(helpMenu, _T("&Help"));

  // ... and attach this menu bar to the frame
  SetMenuBar(menuBar);
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
  // create a status bar just for fun (by default with 1 pane only)
  CreateStatusBar(2);
  SetStatusText(_T("Welcome to wxWindows!"));
#endif // wxUSE_STATUSBAR

  calcPanel=new CalcPanel(this, -1, wxDefaultPosition, wxDefaultSize);

  wxBoxSizer *mainSizer=new wxBoxSizer(wxHORIZONTAL);
  mainSizer->Add(calcPanel, 1, wxGROW, 1);
  SetSizer(mainSizer);
  calcPanel->SetFocus();
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
  // TRUE is to force the frame to close
  Close(TRUE);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
  wxString msg;
  msg.Printf( _T("RPN.\n")
				  _T("Compiled with %s"), wxVERSION_STRING);

  wxMessageBox(msg, _T("About Minimal"), wxOK | wxICON_INFORMATION, this);
}

void MyFrame::OnKeys(wxCommandEvent& WXUNUSED(event))
{
  wxString msg;
  msg.Printf( _T("_ - toggle sign\n")
				  _T("i - 1/x\n")
				  _T("p - pi\n")
				  _T("s/S - sin/asin\n")
				  _T("c/C - cos/acos\n")
				  _T("t/T - tan/atan\n")

				  _T("DELETE - clear line\n")
				  _T("SHIFT-DEL - clear stack\n")
				  _T("ENTER - copy to stack")
				  
				  );

  wxMessageBox(msg, _T("Shortcuts"), wxOK | wxICON_INFORMATION, this);
}

void MyFrame::OnSaveAs(wxCommandEvent& WXUNUSED(event))
{
  wxFileDialog fd(this, _T("Choose a file"), m_defDir/*default dir*/,
					m_filename,
                  _T("csv file (*.csv)|*.csv|All files (*.*)|*.*"),
					wxFD_SAVE);
  if(fd.ShowModal()==wxID_CANCEL) return;
  m_defDir=fd.GetDirectory();
  m_filename=fd.GetPath();
  if(calcPanel->Save(m_filename)==false){
    wxMessageBox(_T("Could not save file"), _T("Error"), wxOK);
  }
}

