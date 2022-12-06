#include "shard.h"
#include "shardApp.h"
#include "shardWelcomePane.h"
#include <wx/notebook.h>
#include "toolbar.h"

bool shardApp::OnInit()
{
   wxFrame* frame = new shardMainFrame(nullptr);
   frame->Show(true);

   return true;
}



// shardMenuBar IMPLEMENTATION
// PRIVATE

// PUBLIC

shardMenuBar::shardMenuBar(eMode mode) : wxMenuBar(wxBORDER_NONE)
{
   m_menuFile = new wxMenu;
   m_menuEdit = new wxMenu;
   m_menuView = new wxMenu;
   m_menuSettings = new wxMenu;
   m_menuHelp = new wxMenu;

   m_menuFile->Append(shardMENU_NEW);
   m_menuFile->Append(shardMENU_OPEN);
   m_itemSave     = new wxMenuItem(m_menuFile, shardMENU_SAVE);
   m_itemSaveAs   = new wxMenuItem(m_menuFile, shardMENU_SAVEAS);
   m_itemSaveAll  = new wxMenuItem(m_menuFile, shardMENU_SAVEALL, wxT("Save All"));
   m_itemClose    = new wxMenuItem(m_menuFile, shardMENU_CLOSE);
   m_itemCloseAll = new wxMenuItem(m_menuFile, shardMENU_CLOSE_ALL, wxT("Close All"));
   m_itemRun      = new wxMenuItem(m_menuFile, shardMENU_RUN_SIM, wxT("Run Sim"));
   m_menuFile->AppendSeparator();
   m_menuFile->Append(shardMENU_EXIT);

   m_menuEdit->Append(shardMENU_UNDO);
   m_menuEdit->Append(shardMENU_REDO);
   m_itemAddProjEvent = new wxMenuItem(m_menuEdit, shardMENU_ADDPROJ,
      wxT("Add Projectile Event..."));

   m_menuView->Append(shardMENU_TOGTOOLBAR, wxT("Toggle Toolbar"));
   m_menuView->Append(shardMENU_TOGSTATUSBAR, wxT("Toggle Status Bar"));
   m_menuView->Append(shardMENU_ZOOMIN);
   m_menuView->Append(shardMENU_ZOOMOUT);
   m_menuView->Append(shardMENU_ROTATE, wxT("Rotate..."));
   m_menuView->Append(shardMENU_TRANSLATE, wxT("Translate..."));
   m_menuView->Append(shardMENU_ROTANDTRANS, wxT("Rotate and Translate"));

   m_menuSettings->Append(shardMENU_KEYBINDINGS, wxT("Keybindings..."));

   m_menuHelp->Append(shardMENU_ABOUT);

   this->Append(m_menuFile, wxT("File"));
   this->Append(m_menuSettings, wxT("Settings"));
   this->Append(m_menuHelp, wxT("Help"));

   //Set(mode);
}

shardMenuBar::~shardMenuBar()
{
   Set(eMode::SIMULATION); // set mode to mode where all menus are being used for automatic deletion via wxwidgets
}

void shardMenuBar::Set(eMode mode)
{
   if (m_currentMode == mode)
   {
      return;
   }

   switch (mode)
   {
      case eMode::DEFAULT:
      {
         m_menuFile->Remove(shardMENU_SAVE);
         m_menuFile->Remove(shardMENU_SAVEAS);
         m_menuFile->Remove(shardMENU_SAVEALL);
         m_menuFile->Remove(shardMENU_CLOSE);
         m_menuFile->Remove(shardMENU_CLOSE_ALL);
         m_menuFile->Remove(shardMENU_RUN_SIM);

         this->Remove(size_t(1)); // remove edit menu
         this->Remove(size_t(1)); // remove view menu

         if (m_currentMode == eMode::SIMULATION)
         {
            m_menuEdit->Remove(shardMENU_ADDPROJ);
         }
      }
      break;
      case eMode::SIMULATION:
      {
         m_menuEdit->Append(m_itemAddProjEvent);
         if (m_currentMode == eMode::PHYSICAL)
         {
            break;
         }
      }
      case eMode::PHYSICAL:
      {

         if (m_currentMode == eMode::SIMULATION)
         {
            m_menuEdit->Remove(shardMENU_ADDPROJ);
         }
         else
         {
            m_menuFile->Insert(2, m_itemSave);
            m_menuFile->Insert(3, m_itemSaveAs);
            m_menuFile->Insert(4, m_itemSaveAll);
            m_menuFile->Insert(5, m_itemClose);
            m_menuFile->Insert(6, m_itemCloseAll);
            m_menuFile->Insert(7, m_itemRun);
            this->Insert(1, m_menuEdit, "Edit");
            this->Insert(2, m_menuView, "View");
         }

      }
      break;
   }
   m_currentMode = mode;
}


// shardToolBar IMPLEMENTATION
// PRIVATE FUNCTIONS

// PUBLIC FUNCTIONS

shardToolBar::shardToolBar(wxWindow* parent, eMode mode) : wxToolBar(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 4L | wxBORDER_NONE)
{
   wxBitmap bNew(wxT("new.png"), wxBITMAP_TYPE_PNG);
   this->AddTool(shardMENU_NEW, wxT("New"), bNew);
   this->Realize();
}



// SHARDMAINFRAME IMPLEMENTATION
// PRIVATE FUNCTIONS

// PUBLIC FUNCTIONS

shardMainFrame::shardMainFrame(wxWindow* parent) : wxFrame(parent, wxID_ANY, wxT("mainFrame"), wxPoint(50, 50), wxSize(1200, 720))
{
   wxImage::AddHandler(new wxPNGHandler);

   m_menuBar = new shardMenuBar;

   auto toolBar = new Toolbar(this);

   wxPanel* panel = new wxPanel(this);
   //wxFlexGridSizer* panelSizer = new wxFlexGridSizer(1, 1, 0, 0);
   //wxBitmap map(wxT("welcome.png"), wxBITMAP_TYPE_PNG);
   //wxStaticBitmap* smap = new wxStaticBitmap(panel, wxID_ANY, map);

   auto welcome = new shardWelcomePane(panel);
   //panelSizer->AddGrowableCol(0);
   //panelSizer->AddGrowableRow(0);
   //panelSizer->Add(welcome, wxSizerFlags(1).Expand());
   //panelSizer->Add(smap, wxSizerFlags(1).Expand());
   //panel->SetSizer(panelSizer);
   //panel->SetAutoLayout(true);
   wxBoxSizer* bs = new wxBoxSizer(wxHORIZONTAL);
   //bs->Add(smap, wxSizerFlags(1).Expand());
   bs->Add(welcome, wxSizerFlags(1).Expand());
   panel->SetSizer(bs);

   wxFlexGridSizer* frameSizer = new wxFlexGridSizer(2, 1, 1, 0);
   frameSizer->AddGrowableRow(1);
   frameSizer->AddGrowableCol(0);
   frameSizer->Add(toolBar, wxSizerFlags(1));
   frameSizer->Add(panel, wxSizerFlags(1).Expand());
   this->SetSizer(frameSizer);

   //this->SetToolBar(toolBar);
   this->SetMenuBar(m_menuBar);
   this->CreateStatusBar();

   auto menuMode = new wxMenu;
   menuMode->Append(10, wxT("DEF"));
   menuMode->Append(11, wxT("SIM"));
   menuMode->Append(12, wxT("PHY"));
   m_menuBar->Append(menuMode, wxT("Mode"));
   this->Bind(wxEVT_MENU, [this](wxCommandEvent& evt)
      {
         switch (evt.GetId())
         {
         case 10:
         {
            this->m_menuBar->Set(eMode::DEFAULT);
         }
         break;
         case 11:
         {
            this->m_menuBar->Set(eMode::SIMULATION);
         }
         break;
         case 12:
         {
            this->m_menuBar->Set(eMode::PHYSICAL);
         }
         break;
         case shardMENU_NEW:
         {
         }
         break;
         }
      });
}

shardMainFrame::~shardMainFrame()
{
}

// APP IMPLEMENTATION
IMPLEMENT_APP(shardApp);


// SHARDMAINFRAME EVENT TABLE
wxBEGIN_EVENT_TABLE(shardMainFrame, wxFrame)
wxEND_EVENT_TABLE()
