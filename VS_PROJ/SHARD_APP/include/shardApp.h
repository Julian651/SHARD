#pragma once
#include <wx/wx.h>

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

   void OnNew(wxCommandEvent& evt);
   void OnOpen(wxCommandEvent& evt);
   void OnAbout(wxCommandEvent& evt);
   void OnExit(wxCommandEvent& evt);

public:
   shardMainFrame(wxWindow* parent);

   wxDECLARE_EVENT_TABLE();
};
