#ifndef __CALCPANEL__
#define __CALCPANEL__

// `CalcPanel
#define STACKSIZE 50
#define EDITSTACKED     2
#define EDITENTERED     1
#define EDITNOW         3
#define EDITNEVER       0
class CalcPanel : public wxPanel
{
private:
  double theStack[STACKSIZE];
  wxString currentLine;
  double currentValue;
  int edited;
  int marked;

public:
  bool Save(wxString fn);
  CalcPanel(wxWindow *parent, wxWindowID id, const wxPoint pos,
				 const wxSize size);

  void OnChar(wxKeyEvent& event);
  void OnPaint(wxPaintEvent& WXUNUSED(event));
  void DrawNum(wxDC *dc);
private:
  DECLARE_EVENT_TABLE();
};

#endif
