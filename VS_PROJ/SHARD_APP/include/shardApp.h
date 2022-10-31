#pragma once
#include <wx/wx.h>
#include <wx/notebook.h>
#include "shardRenderPane.h"


class shardApp : public wxApp
{
private:

public:
   virtual bool OnInit() override;
};


enum class eMode
{
   DEFAULT,
   SIMULATION,
   PHYSICAL
};


class shardMenuBar : public wxMenuBar
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
   shardMenuBar(eMode mode = eMode::DEFAULT);
   ~shardMenuBar();

   void Set(eMode mode);
};


class shardToolBar : public wxToolBar
{
private:
   wxToolBarToolBase* m_toolAddProjEvent;

public:
   shardToolBar(wxWindow* parent, eMode mode = eMode::DEFAULT);

   void Set(eMode mode);
};


class shardMainFrame : public wxFrame
{
private:
   shardMenuBar* m_menuBar;

public:
   shardMainFrame(wxWindow* parent);
   ~shardMainFrame();

   wxDECLARE_EVENT_TABLE();
};