#include <wx/panel.h>
#include <wx/glcanvas.h>
#include <wx/sizer.h>

#include <sApp.h>
#include <enums.h>
#include <toolbar.h>
#include <Canvas.h>

// ====================================================================================================================

bool sApp::OnInit()
{
   wxFrame* fr = new sFrame;

   fr->Show(true);

   return true;
}

// ====================================================================================================================

sMenuBar::sMenuBar(eMode mode) : wxMenuBar(wxBORDER_NONE)
{
   m_menuFile = new wxMenu;
   m_menuEdit = new wxMenu;
   m_menuView = new wxMenu;
   m_menuSettings = new wxMenu;
   m_menuHelp = new wxMenu;

   m_menuFile->Append(sMENU_NEW);
   m_menuFile->Append(sMENU_OPEN);
   m_itemSave     = new wxMenuItem(m_menuFile, sMENU_SAVE);
   m_itemSaveAs   = new wxMenuItem(m_menuFile, sMENU_SAVEAS);
   m_itemSaveAll  = new wxMenuItem(m_menuFile, sMENU_SAVEALL, wxT("Save All"));
   m_itemClose    = new wxMenuItem(m_menuFile, sMENU_CLOSE);
   m_itemCloseAll = new wxMenuItem(m_menuFile, sMENU_CLOSE_ALL, wxT("Close All"));
   m_itemRun      = new wxMenuItem(m_menuFile, sMENU_RUN_SIM, wxT("Run Sim"));
   m_menuFile->AppendSeparator();
   m_menuFile->Append(sMENU_EXIT);

   m_menuEdit->Append(sMENU_UNDO);
   m_menuEdit->Append(sMENU_REDO);
   m_itemAddProjEvent = new wxMenuItem(m_menuEdit, sMENU_ADDPROJ,
      wxT("Add Projectile Event..."));

   m_menuView->Append(sMENU_TOGTOOLBAR, wxT("Toggle Toolbar"));
   m_menuView->Append(sMENU_TOGSTATUSBAR, wxT("Toggle Status Bar"));
   m_menuView->Append(sMENU_ZOOMIN);
   m_menuView->Append(sMENU_ZOOMOUT);
   m_menuView->Append(sMENU_ROTATE, wxT("Rotate..."));
   m_menuView->Append(sMENU_TRANSLATE, wxT("Translate..."));
   m_menuView->Append(sMENU_ROTANDTRANS, wxT("Rotate and Translate"));

   m_menuSettings->Append(sMENU_KEYBINDINGS, wxT("Keybindings..."));

   m_menuHelp->Append(sMENU_ABOUT);

   this->Append(m_menuFile, wxT("File"));
   this->Append(m_menuSettings, wxT("Settings"));
   this->Append(m_menuHelp, wxT("Help"));

   //Set(mode);
}

sMenuBar::~sMenuBar()
{
   Set(eMode::SIMULATION); // set mode to mode where all menus are being used for automatic deletion via wxwidgets
}

void sMenuBar::Set(eMode mode)
{
   if (m_currentMode == mode)
   {
      return;
   }

   switch (mode)
   {
      case eMode::DEFAULT:
      {
         m_menuFile->Remove(sMENU_SAVE);
         m_menuFile->Remove(sMENU_SAVEAS);
         m_menuFile->Remove(sMENU_SAVEALL);
         m_menuFile->Remove(sMENU_CLOSE);
         m_menuFile->Remove(sMENU_CLOSE_ALL);
         m_menuFile->Remove(sMENU_RUN_SIM);

         this->Remove(size_t(1)); // remove edit menu
         this->Remove(size_t(1)); // remove view menu

         if (m_currentMode == eMode::SIMULATION)
         {
            m_menuEdit->Remove(sMENU_ADDPROJ);
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
            m_menuEdit->Remove(sMENU_ADDPROJ);
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

// ====================================================================================================================

wxIMPLEMENT_APP(sApp);

sFrame::sFrame() : wxFrame(nullptr, wxID_ANY, _("OpenGL wxtest"), wxPoint(300, 150), wxSize(1200, 720))
{
   m_menuBar = new sMenuBar;

   auto toolBar = new Toolbar(this);

   wxPanel* panel = new wxPanel(this);

   m_tabbedPanes = new sNotebook(panel);

   wxGLAttributes attribs;
   attribs.PlatformDefaults().Defaults().EndList();
   wxGLCanvas* canvas = new sModelEditor(m_tabbedPanes, attribs);

   m_tabbedPanes->AddPage(canvas, _("Page 1"), true);

   wxBoxSizer* bs = new wxBoxSizer(wxHORIZONTAL);
   bs->Add(m_tabbedPanes, wxSizerFlags(1).Expand());
   panel->SetSizer(bs);

   wxFlexGridSizer* frameSizer = new wxFlexGridSizer(2, 1, 1, 0);
   frameSizer->AddGrowableRow(1);
   frameSizer->AddGrowableCol(0);
   frameSizer->Add(toolBar, wxSizerFlags(1));
   frameSizer->Add(panel, wxSizerFlags(1).Expand());
   this->SetSizer(frameSizer);

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
         case sMENU_NEW:
         {
            fprintf(stderr, "Testing new menu item functionality\n");

            wxGLAttributes attribs;
            attribs.PlatformDefaults().Defaults().EndList();
            wxGLCanvas* c = new sModelEditor(this->m_tabbedPanes, attribs);
            this->m_tabbedPanes->AddPage(c, _("Page"), true);
         }
         break;
         }
      });

   HexaScene::Initialize();
}

sFrame::~sFrame()
{
   HexaScene::Destroy();
   sGLCanvas::DestroyContext();
}

int main(int argc, char** argv)
{
   return WinMain(::GetModuleHandle(NULL), NULL, ::GetCommandLineA(), SW_SHOWNORMAL);
}

// ====================================================================================================================

wxBEGIN_EVENT_TABLE(sNotebook, wxNotebook)
EVT_KEY_DOWN(sNotebook::OnKeyboard)
wxEND_EVENT_TABLE()

sNotebook::sNotebook(wxWindow* parent) : wxNotebook(parent, wxID_ANY)
{
}

void sNotebook::OnKeyboard(wxKeyEvent& event)
{
   if (GetCurrentPage())
   {
      GetCurrentPage()->HandleKeyDown(event.GetRawKeyCode(), event.GetRawKeyFlags());
   }
}
