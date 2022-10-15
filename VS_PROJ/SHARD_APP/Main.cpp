// wxWidgets "Hello world" Program
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
class MyApp : public wxApp
{
public:
   virtual bool OnInit();
};
class MyFrame : public wxFrame
{
public:
   MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
private:
   void OnNew(wxCommandEvent& event);
   void OnOpen(wxCommandEvent& event);
   void OnExit(wxCommandEvent& event);
   void OnAbout(wxCommandEvent& event);
   wxDECLARE_EVENT_TABLE();
};
enum
{
   ID_NEW = 1,
   ID_OPEN
};
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(ID_NEW, MyFrame::OnNew)
EVT_MENU(ID_OPEN, MyFrame::OnOpen)
EVT_MENU(wxID_EXIT, MyFrame::OnExit)
EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
wxEND_EVENT_TABLE()
wxIMPLEMENT_APP(MyApp);
bool MyApp::OnInit()
{
   MyFrame* frame = new MyFrame("Main Frame", wxPoint(50, 50), wxSize(640, 480));
   frame->Show(true);
   return true;
}
MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
   : wxFrame(NULL, wxID_ANY, title, pos, size)
{
   wxMenu* menuFile = new wxMenu;
   menuFile->Append(ID_NEW, "&New...\tCtrl-H",
      "Create a new model...");
   menuFile->AppendSeparator();
   menuFile->Append(ID_OPEN, "&Open...",
      "Open a file...");
   menuFile->AppendSeparator();
   menuFile->Append(wxID_EXIT, "&Exit", "Exit application.");
   wxMenu* menuHelp = new wxMenu;
   menuHelp->Append(wxID_ABOUT);
   wxMenuBar* menuBar = new wxMenuBar;
   menuBar->Append(menuFile, "&File");
   menuBar->Append(menuHelp, "&Help");
   SetMenuBar(menuBar);
   CreateStatusBar();
   SetStatusText("Welcome to wxWidgets!");
}
void MyFrame::OnExit(wxCommandEvent& event)
{
   Close(true);
}
void MyFrame::OnAbout(wxCommandEvent& event)
{
   wxMessageBox("This is a wxWidgets' Hello world sample",
      "About Hello World", wxOK | wxICON_INFORMATION);
}
void MyFrame::OnNew(wxCommandEvent& event)
{
   wxLogMessage("New file...");
}

void MyFrame::OnOpen(wxCommandEvent& event)
{
   wxString filename = wxFileSelector("Choose a file to open");
   if (!filename.empty())
   {
      // work with the file
   }
   //else: cancelled by user
   else
   {

   }
}
