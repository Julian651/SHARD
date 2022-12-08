#pragma once

#include <wx/app.h>
#include <wx/menu.h>
#include <wx/frame.h>
#include <wx/notebook.h>


// ====================================================================================================================

class sApp : public wxApp
{
public:

   bool OnInit() override;
};

// ====================================================================================================================

enum class eMode
{
   DEFAULT,
   SIMULATION,
   PHYSICAL
};


class sMenuBar : public wxMenuBar
{
private:

   wxMenu* m_menuFile;
   wxMenu* m_menuEdit;
   wxMenu* m_menuView;
   wxMenu* m_menuSettings;
   wxMenu* m_menuHelp;

   // Dynamic file menu items
                                 // new  - static: present in all modes
                                 // open - static: present in all modes
   wxMenuItem* m_itemSave;       // save
   wxMenuItem* m_itemSaveAs;     // save as
   wxMenuItem* m_itemSaveAll;    // save all
   wxMenuItem* m_itemClose;      // close
   wxMenuItem* m_itemCloseAll;   // close all
   wxMenuItem* m_itemRun;        // run sim

   // Dynamic edit menu items
                                    // redo - static: present in all modes
                                    // undo - static: present in all modes
   wxMenuItem* m_itemAddProjEvent;  // add projectile event (smdl)

   eMode m_currentMode = eMode::DEFAULT;

public:

   sMenuBar(eMode mode = eMode::DEFAULT);
   ~sMenuBar();

   void Set(eMode mode);
};

// ====================================================================================================================

class sFrame : public wxFrame
{
private:

    sMenuBar* m_menuBar;
    wxNotebook* m_tabbedPanes;

public:

   sFrame();
   ~sFrame();
};

// ====================================================================================================================

class sNotebook : public wxNotebook
{
private:

   wxDECLARE_EVENT_TABLE();

public:

   sNotebook(wxWindow* parent);
   void OnKeyboard(wxKeyEvent& event);
};