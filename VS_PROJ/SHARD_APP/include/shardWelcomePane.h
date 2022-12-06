#pragma once
#include "shardRenderPane.h"

class shardWelcomePane : public shardRenderPane
{
private:

public:
   shardWelcomePane(wxWindow* parent);
   virtual void Render(wxPaintEvent& evt) override;
   void onResize(wxSizeEvent& evt);

   wxDECLARE_EVENT_TABLE();
};