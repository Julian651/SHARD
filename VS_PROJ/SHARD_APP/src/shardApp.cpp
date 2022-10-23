#include "shard.h"
#include "shardApp.h"
#include "shardWelcomePane.h"
#include <wx/notebook.h>

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
   wxDialog* dialog = new wxDialog(this, wxID_ANY, wxT("New file..."), wxDefaultPosition, wxSize(480, 640));

   wxArrayString choices;
   choices.Add(wxString("sim"));
   choices.Add(wxString("phy"));
   wxChoice* choice = new wxChoice(dialog, wxID_ANY);
   choice->Bind(wxEVT_CHOICE, [this](wxCommandEvent& evt)
      {
         if (evt.GetString().compare("sim") == 0)
         {
            // create a new tab panel with a sim model
         }
         else if (evt.GetString().compare("phy") == 0)
         {
            // create a new tab panel with a phy model
         }
      });
   choice->Append(choices);

   dialog->Show();
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
   Destroy();
}

void shardMainFrame::InitMenu()
{
   m_menuFile = new wxMenu;
   auto test = m_menuFile->Append(shardMENU_NEW);
   m_menuFile->Append(shardMENU_OPEN);
   m_menuItemSave     = new wxMenuItem(m_menuFile, shardMENU_SAVE);
   m_menuItemSaveAs   = new wxMenuItem(m_menuFile, shardMENU_SAVEAS);
   m_menuItemSaveAll  = new wxMenuItem(m_menuFile, shardMENU_SAVEALL, wxT("Save All"));
   m_menuItemClose    = new wxMenuItem(m_menuFile, shardMENU_CLOSE);
   m_menuItemCloseAll = new wxMenuItem(m_menuFile, shardMENU_CLOSE_ALL, wxT("Close All"));
   m_menuFile->AppendSeparator();
   m_menuFile->Append(shardMENU_EXIT);

   m_menuEdit = new wxMenu;
   m_menuEdit->Append(shardMENU_UNDO, wxT("Undo"));
   m_menuEdit->Append(shardMENU_REDO, wxT("Redo"));
   // only insert add projectile when we are looking at a simulation file
   m_menuItemAddProj = new wxMenuItem(m_menuEdit, shardMENU_ADDPROJ, wxT("Add Projecilte Event..."));

   m_menuView = new wxMenu;
   m_menuView->Append(shardMENU_TOGTOOLBAR, wxT("Toggle Toolbar"));
   m_menuView->Append(shardMENU_TOGSTATUSBAR, wxT("Toggle Statusbar"));
   m_menuItemZoomIn      = new wxMenuItem(m_menuView, shardMENU_ZOOMIN, wxT("Zoom In"));
   m_menuItemZoomOut     = new wxMenuItem(m_menuView, shardMENU_ZOOMOUT, wxT("Zoom Out"));
   m_menuItemRotate      = new wxMenuItem(m_menuView, shardMENU_ROTATE, wxT("Rotate..."));
   m_menuItemTranslate   = new wxMenuItem(m_menuView, shardMENU_TRANSLATE, wxT("Translate..."));
   m_menuItemRotAndTrans = new wxMenuItem(m_menuView, shardMENU_ROTANDTRANS, wxT("Rotate and Translate"));

   m_menuSettings = new wxMenu;
   m_menuSettings->Append(shardMENU_KEYBINDINGS, wxT("Set Keybindings..."));

   m_menuHelp = new wxMenu;
   m_menuHelp->Append(shardMENU_ABOUT);

   wxMenuBar* menuBar = new wxMenuBar;
   menuBar->Append(m_menuFile, wxT("File"));
   // only insert edit when we are looking at a particular file
   //menuBar->Append(m_menuEdit, wxT("Edit"));
   // only insert view when we are looking at a particular file
   //menuBar->Append(m_menuView, wxT("View"));
   menuBar->Append(m_menuSettings, wxT("Settings"));
   menuBar->Append(m_menuHelp, wxT("Help"));

   this->SetMenuBar(menuBar);
}

void shardMainFrame::SetWelcomePane()
{
   if (mode != 1)
   {
      m_welcomePane->Reparent(m_panel);
      mode = 1;
   }
}

void shardMainFrame::SetNotebookControl()
{
   if (mode != 2)
   {
      m_book->Reparent(m_panel);
      mode = 2;
   }
}

// PUBLIC FUNCTIONS
shardMainFrame::shardMainFrame(wxWindow* parent) : wxFrame(parent, wxID_ANY, wxT("mainFrame"), wxPoint(50, 50), wxSize(1200, 720))
{
   //wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
   //wxPanel* panel = new wxPanel(this);
   //shardRenderPane* pane = new shardWelcomePane(this);
   //sizer->Add(pane, 1, wxEXPAND);
   
   m_panel = new wxPanel(this);
   m_welcomePane = new shardWelcomePane(m_panel);
   m_welcomePane->SetSize(wxSize(500, 500));
   wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
   sizer->Add(m_welcomePane, 1, wxEXPAND);
   m_panel->SetSizer(sizer);
   m_panel->SetAutoLayout(true);
   //m_book = new wxNotebook(this, wxID_ANY);

   this->InitMenu();
   this->CreateStatusBar();
   //this->SetSizer(sizer);
   //this->SetAutoLayout(true);
}

shardMainFrame::~shardMainFrame()
{
   wxDELETE(m_menuItemSave       );
   wxDELETE(m_menuItemSaveAs     );
   wxDELETE(m_menuItemSaveAll    );
   wxDELETE(m_menuItemClose      );
   wxDELETE(m_menuItemCloseAll   );
   wxDELETE(m_menuItemAddProj    );
   wxDELETE(m_menuItemZoomIn     );
   wxDELETE(m_menuItemZoomOut    );
   wxDELETE(m_menuItemRotate     );
   wxDELETE(m_menuItemTranslate  );
   wxDELETE(m_menuItemRotAndTrans);
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