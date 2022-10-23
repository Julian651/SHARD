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


class shardMainFrame : public wxFrame
{
private:
   wxMenu* m_menuFile;
   wxMenu* m_menuHelp;
   wxMenu* m_menuEdit;
   wxMenu* m_menuView;
   wxMenu* m_menuSettings;

   // Dynamic file menu items
   wxMenuItem* m_menuItemSave;
   wxMenuItem* m_menuItemSaveAs;
   wxMenuItem* m_menuItemSaveAll;
   wxMenuItem* m_menuItemClose;
   wxMenuItem* m_menuItemCloseAll;

   // Dynamic edit menu items
   wxMenuItem* m_menuItemAddProj;

   // Dynamic view menu items
   wxMenuItem* m_menuItemZoomIn;
   wxMenuItem* m_menuItemZoomOut;
   wxMenuItem* m_menuItemRotate;
   wxMenuItem* m_menuItemTranslate;
   wxMenuItem* m_menuItemRotAndTrans;

   wxPanel* m_panel;
   wxNotebook* m_book;
   shardRenderPane* m_welcomePane;

   uint8_t mode = 0;

   void OnNew(wxCommandEvent& evt);
   void OnOpen(wxCommandEvent& evt);
   void OnAbout(wxCommandEvent& evt);
   void OnExit(wxCommandEvent& evt);

   void InitMenu();

   void SetWelcomePane();
   void SetNotebookControl(); // the tabbed window view
public:
   shardMainFrame(wxWindow* parent);
   ~shardMainFrame();

   wxDECLARE_EVENT_TABLE();
};