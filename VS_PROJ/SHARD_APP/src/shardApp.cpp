#include "shardApp.h"
#include "shardRenderPane.h"

bool shardApp::OnInit()
{
   wxFrame* frame = new shardMainFrame(nullptr);
   frame->Show(true);

   return true;
}


// SHARDMAINFRAME IMPLEMENTATION
// PRIVATE FUNCTIONS
void shardMainFrame::OnNew(wxCommandEvent& evt)
{
   wxMessageBox(wxT("New file..."));
}

void shardMainFrame::OnOpen(wxCommandEvent& evt)
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

void shardMainFrame::OnAbout(wxCommandEvent& evt)
{
   wxMessageBox(wxT("About..."));
}

void shardMainFrame::OnExit(wxCommandEvent& evt)
{
   Close(true);
}

// PUBLIC FUNCTIONS
shardMainFrame::shardMainFrame(wxWindow* parent) : wxFrame(parent, wxID_ANY, wxT("mainFrame"), wxPoint(50, 50), wxSize(1200, 720))
{
   wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
   shardRenderPane* pane = new shardRenderPane(this);
   sizer->Add(pane, 1, wxEXPAND);

   m_menuFile = new wxMenu;
   m_menuFile->Append(wxID_NEW, wxT("New...\tCtrl-H"), wxT("Create a new model..."));
   m_menuFile->Append(wxID_OPEN, wxT("Open..."), wxT("Open a file..."));
   m_menuFile->AppendSeparator();
   m_menuFile->Append(wxID_EXIT, wxT("Exit"), wxT("Exit application."));

   m_menuHelp = new wxMenu;
   m_menuHelp->Append(wxID_ABOUT);

   wxMenuBar* menuBar = new wxMenuBar;
   menuBar->Append(m_menuFile, wxT("File"));
   menuBar->Append(m_menuHelp, wxT("Help"));

   this->SetMenuBar(menuBar);
   this->CreateStatusBar();
   this->SetSizer(sizer);
   this->SetAutoLayout(true);
}

// APP IMPLEMENTATION
IMPLEMENT_APP(shardApp);


// SHARDMAINFRAME EVENT TABLE
wxBEGIN_EVENT_TABLE(shardMainFrame, wxFrame)
   EVT_MENU(wxID_NEW, shardMainFrame::OnNew)
   EVT_MENU(wxID_OPEN, shardMainFrame::OnOpen)
   EVT_MENU(wxID_ABOUT, shardMainFrame::OnAbout)
   EVT_MENU(wxID_EXIT, shardMainFrame::OnExit)
wxEND_EVENT_TABLE()